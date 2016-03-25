################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include stm32haldev/system/src/newlib/subdir.mk
-include stm32haldev/system/src/diag/subdir.mk
-include stm32haldev/system/src/cortexm/subdir.mk
-include stm32haldev/system/STM32F0xx_HAL_Driver/Src/subdir.mk
-include stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/system/subdir.mk
-include stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb/subdir.mk
-include stm32haldev/stm32dev/src/stm32f0/subdir.mk
-include stm32haldev/stm32dev/src/general/subdir.mk
-include stm32haldev/stm32dev/boards/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ESHF0-0004-F072-BluetoothPixel_D.hex \

SECONDARY_SIZE += \
ESHF0-0004-F072-BluetoothPixel_D.siz \


# All Target
all: ESHF0-0004-F072-BluetoothPixel_D.elf secondary-outputs

dependents:
	-cd /Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32f030x4 && $(MAKE) all

# Tool invocations
ESHF0-0004-F072-BluetoothPixel_D.elf: $(OBJS) $(USER_OBJS) /Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32f030x4/ESHXX000-stm32haldev.elf
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/ldscripts/stm32f072xB" -Wl,-Map,"ESHF0-0004-F072-BluetoothPixel_D.map" --specs=nano.specs -o "ESHF0-0004-F072-BluetoothPixel_D.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ESHF0-0004-F072-BluetoothPixel_D.hex: ESHF0-0004-F072-BluetoothPixel_D.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ESHF0-0004-F072-BluetoothPixel_D.elf"  "ESHF0-0004-F072-BluetoothPixel_D.hex"
	@echo 'Finished building: $@'
	@echo ' '

ESHF0-0004-F072-BluetoothPixel_D.siz: ESHF0-0004-F072-BluetoothPixel_D.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ESHF0-0004-F072-BluetoothPixel_D.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) ESHF0-0004-F072-BluetoothPixel_D.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:
/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32f030x4/ESHXX000-stm32haldev.elf:

-include ../makefile.targets
