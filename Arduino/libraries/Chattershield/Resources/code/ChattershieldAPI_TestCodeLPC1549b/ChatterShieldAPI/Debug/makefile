################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ChatterShieldAPI.axf

# Tool invocations
ChatterShieldAPI.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Design\code\LPCxpresso\LPC15\lpc_chip_15xx\Debug" -L"C:\Design\code\LPCxpresso\LPC15\lpc_board_nxp_lpcxpresso_1549\Debug" -Xlinker -Map="ChatterShieldAPI.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "periph_spi_rom_interrupt_Debug.ld" -o "ChatterShieldAPI.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ChatterShieldAPI.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "ChatterShieldAPI.axf"; # arm-none-eabi-objcopy -v -O binary "ChatterShieldAPI.axf" "ChatterShieldAPI.bin" ; # checksum -p LPC1549 -d "ChatterShieldAPI.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
