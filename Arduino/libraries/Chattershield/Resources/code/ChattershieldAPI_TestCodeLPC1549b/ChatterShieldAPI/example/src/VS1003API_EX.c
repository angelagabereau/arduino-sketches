/*
 * @brief VS1003 API & DEMO, using NXP LPC1549 eval board as SPI bus master, with ROM SPI API in interrupt mode
 *
 * @note
 * Copyright(C) SOTA Design Inc, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
//#include "MP3data.h"
//#include "VS1003_FW_player_SCI.h"
#include "VS1003API.h"




/* ChatterShield
Revised "Easy" boot process

SDmux & BootMode/EE_CS are tied together, with 100k pulldown :

Normal "slave" mode (default):
board boots in normal "slave" mode, with SDmux selected for Arduino access to SD card.
To switch SDcard to VS1003 instead:
 - write GPIO0 register in VS1003 (via SPI) to make output & drive 1;

"Easy" mode (load firmware for VS1003 from EEPROM)
Via SPI interface:
 - write GPIO0 register in VS1003 to make output & drive 1 ;
 - trigger soft reset; VS1003 will immediately load firmare
 - proceed to play mode as usual

Thus, control of SD card (SPI) mux only needs SPI interface to VS1003, no GPIO

UART mode (untested): provides API to read/write control registers same as SPI,
 but a bit more complicated (4 bytes per CMD). * */

#define Version2			// moved several IO pin assignments:
							//   DRQ goes from IO.11 (P0.28, J1.7) -> IO.13 (P0.16, J1.5)
							//  D.SS goes from IO.13 (P0.16, J1.5) -> IO.12 (P0.12, j1.6)
							// SD.SS goes from IO.12 (P0.12, j1.6) -> IO.11 (P0.28, J1.7)


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define SUCCESS		0
#define FAILED		(SUCCESS-1)

#define TICKRATE_HZ1 		(1000)

// IO PIN declarations specific to the LPC1549 eval board (with Arduino shield headers)
//#define VS_RST_PIN		22			// not really needed

#define VS_SD_MUX_PIN		27
#ifdef Version2							// moved serveral IO pin assignments:
#define VS_DREQ_PIN			16								//   DRQ goes from IO.11 (P0.28, J1.7) -> IO.13 (P0.16, J1.5)
//#define AT_SD_SS_PIN		28								// SD.SS goes from IO.12 (P0.12, j1.6) -> IO.11 (P0.28, J1.7)
#else														// nD.SS goes from IO.13 (P0.16, J1.5) -> IO.12 (P0.12, j1.6)
#define VS_DREQ_PIN			28
//#define AT_SD_SS_PIN		12			// TODO: test this functionality later! not used at present!
#endif


// Declarations specific to the LPC15xx SPI peripheral ( chip select flags )
							//	This nybble selectrs the SS line to drive - negative logic: E = SS0, D= SS1, B= SS2, 7 =SS3
//								 v
#define VS_CMD_SS			0x0F0E0000		// SS0
#define VS_DAT_SS			0x0F0D0000		// SS1
#define AT_SS_SS			0x0F0B0000  	// SS2
#define NU_SS_SS			0x0F070000  	// SS3
#define NO_SS_SS			0x0F0F0000  	// all off


static SPI_HANDLE_T *spiHandleMaster;			/* SPI master handle and memory for ROM API */

/* Use a buffer size larger than the expected return value of spi_get_mem_size() for the static SPI handle type */
static uint32_t spiMasterHandleMEM[0x20];

static uint16_t xferArray[16] = {0};
//static uint16_t xferArray[] = {0x1111, 0x2222, 0x3333, 0x4444};

static uint16_t rx_buff[sizeof(xferArray) / sizeof(uint16_t)];

static volatile int intErrCode;					/* Interrupt error code (used as semaphore) */

static volatile uint32_t SysTickCount=0;

// For use in direct SD card access my Arduino processor (not used here)
/* To communicate with the SD card, first put the SD card into the SPI mode.
 * To do this, set the MOSI and CS lines to logic value 1 and toggle SD CLK for at least 74 cycles. ( 5x 16bit words of 1's )
 * After the 74 cycles (or more) have occurred,
 * your program should set the CS line to 0 and send the command CMD0 (SD_RESET_CMD) : */

const  uint16_t SD_RESET_CMD[3] = {0x4000, 0x0000, 0x0095} ;  // send with CS = 0; card will switch to SPI and respond with the SPI mode R1 response

/* response token is sent by the card after every command with the exception of SEND_STATUS commands.
 * It is one byte long, and the MSB is always set to zero.
 * The other bits are error indications, an error being signaled by a ‘1’. */


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	SysTickCount++;
}

/**
 * @brief	Handle SPI0 interrupt by calling SPI ROM handler
 * @return	Nothing
 */
void SPI0_IRQHandler(void)
{
	LPC_SPID_API->spi_isr(spiHandleMaster);					/* Call SPI ISR function in ROM with the SPI handle */
}


/* Initializes pin muxing for SPI interface - note that SystemInit() may already setup your pin muxing at system startup */
static void Init_SPI_PinMux(void)
{
#if (defined(BOARD_NXP_LPCXPRESSO_1549))

	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
	/*
	 * Initialize SPI0 pins connect
	 * SCK0: PINASSIGN3[15:8]: Select P0.0
	 * MOSI0: PINASSIGN3[23:16]: Select P0.16
	 * MISO0: PINASSIGN3[31:24] : Select P0.10
	 * SSEL0: PINASSIGN4[7:0]: Select P0.9
	 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0,  2, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));  // this was commented out before - see rev 2 IO changes
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 16, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));  // really need this ??
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 30, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));

#if 0
	Chip_SWM_MovablePinAssign(SWM_SPI0_SCK_IO, 0);			/* P0.0 */
	Chip_SWM_MovablePinAssign(SWM_SPI0_MOSI_IO, 16);		/* P0.16 */
	Chip_SWM_MovablePinAssign(SWM_SPI0_MISO_IO, 10);		/* P0.10 */
	Chip_SWM_MovablePinAssign(SWM_SPI0_SSELSN_0_IO, 9);		/* P0.9 */

#else		// ONE Shield SPI pin mapping test
	Chip_SWM_MovablePinAssign(SWM_SPI0_SCK_IO, 2);			/* P0.2 */
	Chip_SWM_MovablePinAssign(SWM_SPI0_MOSI_IO, 17);		/* P0.17 */
	Chip_SWM_MovablePinAssign(SWM_SPI0_MISO_IO, 30);		/* P0.30 */
	Chip_SWM_MovablePinAssign(SWM_SPI0_SSELSN_0_IO, 10);	/* P0.10 */	// nCSS

#ifdef Version2		// moved serveral IO pin assignments:	//   DRQ goes from IO.11 (P0.28, J1.7) -> IO.13 (P0.16, J1.5)
															// nD.SS goes from IO.13 (P0.16, J1.5) -> IO.12 (P0.12, j1.6)
															// SD.SS goes from IO.12 (P0.12, j1.6) -> IO.11 (P0.28, J1.7)

	Chip_SWM_MovablePinAssign(SWM_SPI0_SSELSN_1_IO, 12);	/* P0.12 */ // nDSS
//	Chip_SWM_MovablePinAssign(SWM_SPI0_SSELSN_2_IO, 12);	/* P0.12 */ // nSDSS
#else
	Chip_SWM_MovablePinAssign(SWM_SPI0_SSELSN_1_IO, 16);	/* P0.16 */ // nDSS
//	Chip_SWM_MovablePinAssign(SWM_SPI0_SSELSN_2_IO, 12);	/* P0.12 */ // nSDSS
#endif
#endif

	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);		/* Disable the clock to the Switch Matrix to save power */
#else

#warning "No SPI pin muxing defined"						/* Configure your own SPI pin muxing here if needed */
#endif
}

static void errorSPI(void)				/* Turn on LED to indicate an error */
{
	Board_LED_Set(0, true);
	while (1) {}
}

static void setupSpiMaster()			/* Setup SPI handle and parameters */
{
	SPI_CONFIG_T spiConfigRec;

	Chip_SPI_Init(LPC_SPI0);			/* Enable SPI clock and reset SPI peripheral - the boot ROM does not do this */

										/* Perform a sanity check on the storage allocation */
	if (LPC_SPID_API->spi_get_mem_size() > sizeof(spiMasterHandleMEM))
	{									/* Example only: this should never happen and probably isn't needed for  most SPI code. */
		errorSPI();
	}

										/* Setup the SPI0 handle */
	spiHandleMaster = LPC_SPID_API->spi_setup(LPC_SPI0_BASE, (uint8_t *) &spiMasterHandleMEM);
	if (spiHandleMaster == NULL)
	{
		errorSPI();
	}
										/* Setup SPI0 configuration record */
//	spiConfigRec.delay = 0x2222;
	spiConfigRec.delay = 0x1111;		// 1 clock in each phase
	/* SysClock divided is set to maximum */
//	spiConfigRec.divider = 0xFFFF;
	spiConfigRec.divider = 0x0047;		// 1 MHz
//	spiConfigRec.divider = 0x008F;		// 500 KHz
//	spiConfigRec.divider = 0x02CF;		// 100 KHz

										/* Loopback mode, master mode and SPI block enabled */
//	spiConfigRec.config = 0x85;
	spiConfigRec.config = 0x05;			// disable loopback
	spiConfigRec.error_en = 0;

	/* Init SPI0 */
	LPC_SPID_API->spi_init(spiHandleMaster, &spiConfigRec);
}

/* SPI interrupt callback, called on completion of SPI operation when in interrupt mode. Called in interrupt context. */
static void cbSpiComplete(uint32_t err_code, uint32_t n)
{
//	uint32_t i;
	if ((err_code == LPC_OK))  //  && (n == (sizeof(xferArray) / sizeof(xferArray[0]))))  /// TEST!! comment out size check for now since its hardcoded to a fixed 4-word size! doh!
	{
		/* Verify if received data is same as transmit
		for (i = 0; i < n; i++) {
			if (rx_buff[i] != xferArray[i]) {
				errorSPI();
			}
		}*/
		intErrCode = (int) err_code;
	}
	else
	{
		errorSPI();
	}
}


static void WriteSpiMssg(uint16_t *xferPtr, uint32_t xferSize,  uint32_t TXCTL)		/* Master SPI transmit in interrupt mode */
{
	SPI_PARAM_T paramRec;
	intErrCode = -1;								/* Init variable used as semaphore */
													/* Setup transfer record */
	paramRec.tx_buffer = xferPtr;					/* SPI TX buffer */
	paramRec.size = xferSize;						/* total number of SPI transfers */
	paramRec.rx_buffer = rx_buff;					/* SPI RX buffer */
	paramRec.fsize_sel = TXCTL;						// 0x0F0E0000;/* Set Tx Control for 16 bit transfer, SSEL0 asserted */
	paramRec.eof_flag = 1;							/* End of Frame enabled */
	paramRec.tx_rx_flag = 2;						/* transmit and receive */
	paramRec.driver_mode = 1;						/* interrupt mode */
	paramRec.dma_cfg = NULL;						/* DMA configuration */
	paramRec.cb = (SPI_CALLBK_T) cbSpiComplete;		/* SPI completion callback */
	paramRec.dma_cb = NULL;							/* DMA completion callback */

	if (LPC_SPID_API->spi_master_transfer(spiHandleMaster, &paramRec) != LPC_OK) 	// Transfer message as SPI master via interrupt
	{
		errorSPI();									// Signal SPI error
	}

	while (intErrCode == -1)						//  Sleep until transfer is complete, but allow IRQ to wake system to handle SPI IRQ
	{
		__WFI();
	}
}


/*	Hardware abstraction function for SPI interface - To be redefined for Arduino
 * 	Everything below this should be portable more or less as-is
 *
 * Note: all transfers are in multiples of 16-bits (2 Bytes)
 * most are 2 words (32-bits) but a few are more, up to about 8 words (16 Bytes)
 * rarely only 1 byte transfer is needed -
 * So we allocate 1 32-byte data buffer for transmission, and a ??  byte one for Rx
 */


void DelayXXuS(uint32_t MicroSeconds)
{
	uint32_t i;
	MicroSeconds<<= 5;								// multiply by 32; rough guess at about 1uS delay
	for (i =0; i<MicroSeconds;i++)
	{
		Board_LED_Toggle(0);
	}
}


void DelayXXmS(uint32_t MilliSeconds)
{
	SysTickCount=0;
	while (SysTickCount < MilliSeconds)
	{
		__WFI();
	}
}


void WaitForDREQ(void)		// only needed for fast data transfer when streaming audio data directly from CPU to VS1003
{
	while ( Chip_GPIO_ReadPortBit(LPC_GPIO, 0, VS_DREQ_PIN) == false)	// wait for DREQ IO pin to go high
	{
		Board_LED_Toggle(0);
		DelayXXmS(1);
	}
}

#if 0
void SPIsendByte(void)		// wrapper function - if needed
{
}

uint8_t	VS1003SPItransfer(void)
{
//	return FAILED;
	return SUCCESS;
}

#endif

/*
 * VS1003 transfers fall into 3 categories :
 * Write CMD registers
 * Read  CMD registers
 * Write Data (audio)	Note: readback of audio data is not possible
 *
 * Thus for simplicity, we implement 3 wrappers for these cases
 * Note : parameters and return values are 32-bits for efficiency on Cortex, but 8-bits are sufficient (ex: Arduino port)
 * Note : for portability, it might be useful to define data buffers like xferArray[] as unions of various unsigned data types, 8, 16, and 32-bit
 * Return values, if present, indicate success or failure of transfer (for whatever reason)
 */


void	VS1003SPIwriteCMDreg(uint32_t Register, uint32_t Value)			// See VS1003API.h for list of valid registers (#defines)
{																		// Value is always 16-bits; for 8-bit processors, might be better to split up into 2x 8bits
	xferArray[0] = (uint16_t)((VS_CMD_WRITE<<8) | (uint16_t)Register);	// Compose Write CMD & Register number into first 16-bit location
	xferArray[1] = (uint16_t) Value;									//
	WriteSpiMssg(xferArray, 2, VS_CMD_SS);								// (send 2 x 16 bit words) to default SCI bus
	DelayXXuS(2);														// for simplicity, include mandatory delay here - some commands still need longer,
																		// these must be added externally in user code (see  See VS1003API.h )
}

void	VS1003SPIreadCMDreg(uint32_t Register, uint32_t * ValuePtr)		//  see VS1003API.h for list of valid registers (#defines)
{																		// Value is always 16-bits; for 8-bit processors, might be better to split up into 2x 8bits
	xferArray[0] = (uint16_t)((VS_CMD_READ<<8) | (uint16_t)Register);	// Compose Write CMD & Register number into first 16-bit location
	xferArray[1] = 0;													// Dummy data
	WriteSpiMssg(xferArray, 2, VS_CMD_SS);								// (send 2 x 16 bit words) to default SCI bus
	*ValuePtr = rx_buff[1];												// store received data
}
#if 0
void	VS1003SPIwriteDATA(void)										// TODO (when needed)
{
}
#endif
/* When a file has been selected, the MSDOS short filename (8+3 characters) can be read from VS10xx memory.
 * The filename is in Y memory at addresses 0x780..0x785 (VS1011B,VS1011E, VS1002D) or 0x1800..0x1805 (VS1003B, VS1033C).
 * The first character is in the most-significant bits of the first word.  */
uint32_t	VS1003SPIgetTrackDOSname(uint8_t StringBuf[] )
{
	uint32_t i,ValRead;
	VS1003SPIreadCMDreg(VS_AICTRL3, & ValRead);

	if (ValRead & VS_CTRL3_FILE_READY)  				// file ready
	{													// fetch filename in Y memory at addresses 0x780..0x785
		VS1003SPIwriteCMDreg(VS_WRAMADD, 0x5800);		// write ( Y memory  addresses 0x780?) to SCI WRAMADDR,
		for (i=0; i < 12; i++)
		{
			VS1003SPIreadCMDreg(VS_WRAM, & ValRead);	// get first/next  2 chars of DOS 8.3 filename...
			StringBuf[i]= (ValRead >> 8);
			i++;
			StringBuf[i]= (ValRead & 0xFF);				// tested - this works!
		}
		return SUCCESS;
	}
	return FAILED;
}

#if 0
// Pseudocode reference example from VLSI appnote - for reference, above function is based on this

#define MKWORD(a,b) (((int)(unsigned char)(a)<<8)|(unsigned char)(b))
int song = 0;
WriteMp3Reg(SCI_AICTRL3, (2<<1)); /* pause before play mode */
WriteMp3Reg(SCI_AICTRL0, 0x8000+song); /* select song */


while(1)
{
	if (ReadMp3Reg(SCI_AICTRL3) & (1<<3))
	{ /* file ready */
		unsigned short ch[6], name[6] = {MKWORD('S','O'), MKWORD('N','G'), MKWORD(' ',' '),	MKWORD(' ',' '), MKWORD('M','P'), MKWORD('3','\0')};
		int i;
		WriteMp3Reg(SCI_WRAMADDR, 0x4780); /* 0x5800 for VS1003B/VS1033C */
		ReadMp3Reg(SCI_AICTRL2); /* dummy read - required for VS1002 only */
		for (i=0; i < 6; i++)
		{ /* read filename */
			ch[i] = ReadMp3Reg(SCI_WRAM); /* first 2 chars, AICTRL2 for VS1002 */
			printf("%c%c", ch[i]>>8, ch[i]);
		}
		ch[5] &= 0xff00; /* mask away unused bits */
		printf("\n");
		if (!memcmp(ch, name))
		{ /* compare filenames */
			break; /* filename matched, leave loop */
		}
		else
		{	/* the right file not found!! */
			if (++song == ReadMp3Reg(SCI_AICTRL1))
			{			/* The requested file was not on the card! */
			}
			else
			{			/* clear file ready, keep pause on, pause before play mode */
				WriteMp3Reg(SCI_AICTRL3, (1<<4)|(2<<1));
				WriteMp3Reg(SCI_AICTRL0, 0x8000+song); /* select next song */
			}
		}
	}
}
/* SONG.MP3 file number is now in the variable 'song' */
/* clear file ready and pause, select loop song mode */
WriteMp3Reg(SCI_AICTRL3, (1<<1));

#endif

/*To start the application after uploading the code, write 0x30 (0x50 forVS1053b) to VS_AIADDR.
Before starting the code, you should initialize SCI CLOCKF and SCI VOL.*/

/*
* To start the SCI-controlled standalone player, write 0xC017 to SCI WRAMADDR,
* then 0x0001, 0x0000, and 0x0001 to SCI WRAM. This sets GPIO0 to output a ’1’.
* Then give a software reset.
* The chip now detects GPIO0 high, and performs boot from SPI EEPROM.
* To return to slave mode either give a hardware reset, or write 0xC017 to SCI WRAMADDR,
* then 0x0000 to SCI WRAM, and give a software reset.
 */
void VS1003FWloadEEPROM(void)
{														// To set GPIO0 to output a ’1’:
	VS1003SPIwriteCMDreg(VS_WRAMADD, 0xC017);			// write 0xC017 to SCI WRAMADDR,
	VS1003SPIwriteCMDreg(VS_WRAM, 0x0001);
	VS1003SPIwriteCMDreg(VS_WRAM, 0x0000);
	VS1003SPIwriteCMDreg(VS_WRAM, 0x0001);
	VS1003SPIwriteCMDreg(VS_SCI_MODE,( VS_SM_SDINEW_bM | VS_SM_RESET_bM));	// Do soft reset !
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Main routine for SPI example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t sysTickRate;
	uint32_t ValRead;
	volatile uint16_t result = 0;
	uint8_t TrackFileName[16];			//

	SystemCoreClockUpdate();
	Board_Init();
	/* The sysTick counter only has 24 bits of precision, so it will overflow quickly with a fast core clock.
	 * You can alter the sysTick divider to generate slower sysTick clock rates. */
	Chip_Clock_SetSysTickClockDiv(1);

	/* A SysTick divider is present that scales the sysTick rate down from the core clock.
	 * Using the SystemCoreClock variable as a rate reference for the SysTick_Config() function won't work,
	 * so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / TICKRATE_HZ1);							/* Enable and setup SysTick Timer at a periodic rate */

// NOTE: remote control of VS1003 reset no longer required
//	Chip_GPIO_SetPinOutHigh(LPC_GPIO, 0, VS_RST_PIN);					// make Reset output active (high) (same as default pullup) inverted reset with mosfet
//	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, VS_RST_PIN);					// enable Reset control output
////	Chip_GPIO_SetPinOutLow(LPC_GPIO, 0, VS_RST_PIN);				// make Reset output active (old normal reset polarity)

// Disable DREQ buffer control for new VS1003 controlled mux mode TEST
//	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, VS_SD_MUX_PIN);				// enable DREQ buffer / SD mux control
//	Chip_GPIO_SetPinOutLow(LPC_GPIO, 0, VS_SD_MUX_PIN);					// turn on DREQ buffer (active low)
// NOTE: new setup automatically sets DREQ buffer on when SD card is not accessible to Arduino core (default)

	Board_LED_Set(0, false);							// Clear activity LED
	Init_SPI_PinMux();
	setupSpiMaster();									// Allocate SPI handle, setup rate, and initialize clocking
	NVIC_EnableIRQ(SPI0_IRQn);							// Enable SPI0 interrupt

	WaitForDREQ();
	DelayXXmS(10);										// need a longer delay that just waiting for DREQ??
	VS1003SPIwriteCMDreg(VS_CLOCKF, 0x9800);			// SET clock multiplier to 3x xtal clock , +1.5x max fw boost
	DelayXXmS(10);										// kill some time to let the PLL sync etc / increased time from 1000 to 5000 - not helping

	VS1003FWloadEEPROM();								// Note: because this resets the DSP, this must be called before loading all other config registers
	DelayXXmS(20);										// Allow time to reboot and load new firmware from EEPROM

	WaitForDREQ();
	VS1003SPIwriteCMDreg(VS_CLOCKF, 0x9800);			// Do it again because VS1003FWloadEEPROM() includes soft reset which resets clocking
	DelayXXmS(10);										// kill some time to let the PLL sync etc

	VS1003SPIreadCMDreg(VS_CLOCKF, & ValRead);			// Verify value - for debugging only
	result = (uint16_t)ValRead;							// normally 0x9800 // now reading 0 ?  Binary: 1001 1000 0000 0000

	VS1003SPIwriteCMDreg(VS_VOL, 0x0000);				// set volume to max (0); smaller number is louder. db scale. 0xFFFF = mute
	DelayXXuS(60);										// wait for cmd to complete - fixed timer
	WaitForDREQ();										// or this way

														// Standalone player "EASY" mode mp3 playback test

														// make sure SPI mux is set to give VS1003b exclusive access to the uSD connector
//	Chip_GPIO_SetPinOutHigh(LPC_GPIO, 0, VS_SD_MUX_PIN);// high for VS to connect with uSD card;

//TODO: 												// AICTRL3 should be set to the desired play mode by the user before starting the code.
//	VS1003SPIwriteCMDreg(VS_PLAY_MODE, VS_MODE_REPEAT_bM); // set autorepeat on

//	VS1003SPIwriteCMDreg(VS_AICTRL3, VS_CTRL3_RANDOM_PLAY);	// Turn on random (shuffle) play mode

														/* Name 		Bits 		Description
														 * ST_AMPLITUDE 15:12 	Treble Control in 1.5 dB steps (-8..7, 0 = off)
														 * ST_FREQLIMIT 11:8 	Lower limit frequency in 1000 Hz steps (0..15)
														 * SB_AMPLITUDE  7:4 	Bass Enhancement in 1 dB steps (0..15, 0 = off)
														 * SB_FREQLIMIT  3:0 	Lower limit frequency in 10 Hz steps (2..15) */
	VS1003SPIwriteCMDreg(VS_BASS, 0x0F);				// Enable BASS boost, at max (+15 dB, 150 Hz crossover)


	VS1003SPIwriteCMDreg(VS_AIADDR, 0x30);				// write 0x30 (0x50 forVS1053b) to VS_AIADDR to launch the newly uploaded VS1003 firmware
														// default behavior is to just play all tracks one after another is order they were written to sd card
	DelayXXmS(500);										// give it some time to read/ analyse SD card.... it takes a while

	VS1003SPIreadCMDreg(VS_AICTRL1, & ValRead);			// read how many tracks on the test SD card
	result = (uint16_t)ValRead;							// normally 58

	VS1003SPIreadCMDreg(VS_AICTRL0, & ValRead);			// read currently playing track number
	result = (uint16_t)ValRead;							//

	VS1003SPIgetTrackDOSname(TrackFileName);

	DelayXXmS(5000);									// let it keep playing another 5 seconds

	VS1003SPIwriteCMDreg(VS_AICTRL0, (result |0x8000));	// force restart of same track ( use same method to select any track to play)
														// Write 0x8000 + song number to SCI AICTRL0 to jump to another track.
														// NOTE: in shuffle mode, track # is ignored, and a new random track is immediately selected.

	DelayXXmS(5000);									// let it keep playing another 5 seconds



	while (1) 											// Loop forever
	{
		Board_LED_Toggle(0);							/* Toggle LED to show activity. */
		DelayXXmS(200);
	}
	return 0;											/* Code never reaches here. Only used to satisfy standard main() */
}







































