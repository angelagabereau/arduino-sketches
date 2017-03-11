/* 
  Copyright (c) 2013-2014, Fabule Fabrications Inc, All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, version 3.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library.
*/

/*  THIS CLASS REPLACES THE CLYDE CLASS FOR THE DEVELOPMENT OF THE CHATTERBOX SHIELD */

#include "Chattershield.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//#define CLYDE_DEBUG_CHATTER

/****************************************************************************/

CChattershield::CChattershield( uint8_t _cs_pin, uint8_t _dcs_pin, uint8_t _dreq_pin):
  cs_pin(_cs_pin), dcs_pin(_dcs_pin), dreq_pin(_dreq_pin)
{
}

/****************************************************************************/

void CChattershield::init(void)
{
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::init");
  #endif

  // The SCI and SDI will start deselected
  pinMode(cs_pin,OUTPUT);
  digitalWrite(cs_pin,HIGH);
  pinMode(dcs_pin,OUTPUT);
  digitalWrite(dcs_pin,HIGH);

  // DREQ is an input
  pinMode(dreq_pin,INPUT);

  SPI.begin();

  // Init SPI slow mode
  #ifdef SPI_HAS_TRANSACTION //Support previous versions of Arduino/SPI library.
    spiSettings = SPISettings(250000, MSBFIRST, SPI_MODE0); //Slow!
  #else
    SPI.setClockDivider(SPI_CLOCK_DIV64); // Slow!
  #endif

  // Boot VS1003
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("Booting VS1003...");
  #endif

  delay(1);

  await_data_request();
  delay(10);                            // need a longer delay that just waiting for DREQ??
  write_register(VS_CLOCKF, 0x9800);    // SET clock multiplier to 3x xtal clock , +1.5x max fw boost
  delay(10);                            // kill some time to let the PLL sync etc / increased time from 1000 to 5000 - not helping

  //Load EEPROM
  loadEEPROM();                         // Note: because this resets the DSP, this must be called before loading all other config registers
  delay(20);                            // Allow time to reboot and load new firmware from EEPROM

  await_data_request();
  write_register(VS_CLOCKF, 0x9800);    // SET clock multiplier again because loadEEPROM() includes soft reset which resets clocking
  delay(10);   

  //Volume off
  write_register(VS_VOL, 0x0000);       // set volume to mute (0xFFFF)
  delayMicroseconds(60);                // wait for cmd to complete - fixed timer
  await_data_request();                 // or this way

  //Pause to start
  write_register(VS_AICTRL3, VS_CTRL3_PAUSE_ON); // 0=normal, 1=pause ON

  write_register(VS_BASS, 0x0F);        // Enable BASS boost, at max (+15 dB, 150 Hz crossover)
  write_register(VS_AIADDR, 0x30);      // write 0x30 (0x50 forVS1053b) to VS_AIADDR to launch the newly uploaded VS1003 firmware

  // soft reset
  softReset();

  // Now you can set high speed SPI clock
  #ifdef SPI_HAS_TRANSACTION //Support previous versions of Arduino/SPI library.
    spiSettings = SPISettings(4000000, MSBFIRST, SPI_MODE0); // Fastest available
  #else
    SPI.setClockDivider(SPI_CLOCK_DIV4); // Fastest available
  #endif

  //Volume off
  write_register(VS_VOL, 0x0000);       // set volume to mute (0xFFFF)
}

void CChattershield::softReset() const 
{
  write_register(VS_SCI_MODE,( VS_SM_SDINEW | VS_SM_RESET));
}

/****************************************************************************/

void CChattershield::playLoop(void) const
{
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::playLoop");
  #endif

  write_register(VS_AICTRL3, VS_CTRL3_PLAY_LOOP); // Loop same track forever
}

void CChattershield::play(void) const
{
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::play");
  #endif

  //write_register(VS_AICTRL3, VS_CTRL3_PLAY_NORM); // Default, cycle trough all tracks and start over forever
  //write_register(VS_AICTRL3, VS_CTRL3_PLAY_LOOP); // Loop same track forever
  //write_register(VS_AICTRL3, VS_CTRL3_PLAY_PAUSE_1RST); // pause before playing track
  //write_register(VS_AICTRL3, VS_CTRL3_PLAY_PAUSE_AFTER); // pause after playing track
  //write_register(VS_AICTRL3, VS_CTRL3_RANDOM_PLAY); // Turn on random (shuffle) play mode

  write_register(VS_AICTRL3, VS_CTRL3_PLAY_PAUSE_AFTER);
}

void CChattershield::playTrack(uint16_t trackNumber, bool loop = false){

  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::playTrack");
  #endif

  // Write 0x8000 + song number to SCI AICTRL0 to jump to another track.
  // NOTE: in shuffle mode, track # is ignored, and a new random track is immediately selected.
 // uint16_t result = read_register(VS_AICTRL0);     // read currently playing track number
  write_register(VS_AICTRL0, ( trackNumber | 0x8000 )); // force restart of same track ( use same method to select any track to play)
  
  if(loop){
    playLoop();
  }else{
    play();
  }

}

void CChattershield::fadeOut(void) const
{
  for( int v = 0; v <= 0xff; v++ ) {
    setVolume(v);
    delay(5);
  }
}

void CChattershield::pause(void) const
{
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::pause");
  #endif

  write_register(VS_AICTRL3, VS_CTRL3_PAUSE_ON);
}

uint16_t CChattershield::getTrackCount(void) const
{
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::getTrackCount");
  #endif

  return read_register(VS_AICTRL1);     // read how many tracks on the test SD card, FRANTZ CARD HAS 58
}

 uint16_t CChattershield::getCurrentTrackNumber(void) const
 {
  #ifdef CLYDE_DEBUG_CHATTER
  Serial.println("CChattershield::getCurrentTrackNumber");
  #endif

  return read_register(VS_AICTRL0);     // read currently playing track number
 }


/****************************************************************************/

void CChattershield::setVolume(uint8_t vol) const
{
  uint16_t value = vol;
  value <<= 8;
  value |= vol;

  write_register(VS_VOL,value); // VOL
}

/****************  WRITE  ************************************************************/

void CChattershield::write_register(uint8_t _reg,uint16_t _value) const
{
  #ifdef SPI_HAS_TRANSACTION //Support previous versions of Arduino/SPI library.
    SPI.beginTransaction(spiSettings);  // gain control of SPI bus
  #endif

  control_mode_on();
  delayMicroseconds(1); // tXCSS

  SPI.transfer(VS_CMD_WRITE); // Write operation
  SPI.transfer(_reg); // Which register
  SPI.transfer(_value >> 8); // Send high byte
  SPI.transfer(_value & 0xff); // Send low byte

  delayMicroseconds(60); // tXCSH
  await_data_request();
  control_mode_off();

  #ifdef SPI_HAS_TRANSACTION
    SPI.endTransaction();          // release the SPI bus
  #endif
}


/***************  READ  *************************************************************/

uint16_t CChattershield::read_register(uint8_t _reg) const
{
  uint16_t result;

  #ifdef SPI_HAS_TRANSACTION //Support previous versions of Arduino/SPI library.
    SPI.beginTransaction(spiSettings);  // gain control of SPI bus
  #endif

  control_mode_on();

  delayMicroseconds(1); // tXCSS
  SPI.transfer(VS_CMD_READ); // Read operation
  SPI.transfer(_reg); // Which register
  result = SPI.transfer(0xff) << 8; // read high byte
  result |= SPI.transfer(0xff); // read low byte
  delayMicroseconds(60); // tXCSH
  await_data_request();

  control_mode_off();

  #ifdef SPI_HAS_TRANSACTION
    SPI.endTransaction();          // release the SPI bus
  #endif

  return result;
}

/*To start the application after uploading the code, write 0x30 (0x50 forVS1053b) to VS_AIADDR.
Before starting the code, you should initialize SCI CLOCKF and SCI VOL.*/

/*

"Easy" mode (load firmware for VS1003 from EEPROM)
Via SPI interface:
 - write GPIO0 register in VS1003 to make output & drive 1 ;
 - trigger soft reset; VS1003 will immediately load firmare
 - proceed to play mode as usual


* To start the SCI-controlled standalone player, write 0xC017 to SCI WRAMADDR,
* then 0x0001, 0x0000, and 0x0001 to SCI WRAM. This sets GPIO0 to output a ’1’.
* Then give a software reset.
* The chip now detects GPIO0 high, and performs boot from SPI EEPROM.
* To return to slave mode either give a hardware reset, or write 0xC017 to SCI WRAMADDR,
* then 0x0000 to SCI WRAM, and give a software reset.
 */
void CChattershield::loadEEPROM(void) const
{                           
  // To set GPIO0 to output a ’1’:
  write_register(VS_WRAMADD, 0xC017);
  write_register(VS_WRAM, 0x0001);
  write_register(VS_WRAM, 0x0000);
  write_register(VS_WRAM, 0x0001);
  softReset();  // Do soft reset !
}

