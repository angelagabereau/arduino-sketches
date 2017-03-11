################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/EPD.c \
../example/src/VS1003API_EX.c \
../example/src/cr_startup_lpc15xx.c \
../example/src/sysinit.c 

OBJS += \
./example/src/EPD.o \
./example/src/VS1003API_EX.o \
./example/src/cr_startup_lpc15xx.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/EPD.d \
./example/src/VS1003API_EX.d \
./example/src/cr_startup_lpc15xx.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\Design\code\LPCxpresso\LPC15\lpc_chip_15xx\inc" -I"C:\Design\code\LPCxpresso\LPC15\lpc_board_nxp_lpcxpresso_1549\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

example/src/VS1003API_EX.o: ../example/src/VS1003API_EX.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\Design\code\LPCxpresso\LPC15\lpc_chip_15xx\inc" -I"C:\Design\code\LPCxpresso\LPC15\lpc_board_nxp_lpcxpresso_1549\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"example/src/VS1003API_EX.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


