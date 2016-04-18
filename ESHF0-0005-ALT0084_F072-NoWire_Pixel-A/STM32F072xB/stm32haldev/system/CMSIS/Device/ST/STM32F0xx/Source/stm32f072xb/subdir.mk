################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb/startup_stm32f072xb.S 

OBJS += \
./stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb/startup_stm32f072xb.o 

S_UPPER_DEPS += \
./stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb/startup_stm32f072xb.d 


# Each subdirectory must supply rules for building sources it contributes
stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb/startup_stm32f072xb.o: /Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb/startup_stm32f072xb.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -x assembler-with-cpp -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F072xB -I../include -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/STM32F0xx_HAL_Driver/Inc" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/boards" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/general" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/stm32f0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


