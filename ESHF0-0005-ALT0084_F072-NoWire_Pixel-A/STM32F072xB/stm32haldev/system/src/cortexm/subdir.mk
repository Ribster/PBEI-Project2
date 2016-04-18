################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/src/cortexm/_initialize_hardware.c \
/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/src/cortexm/_reset_hardware.c \
/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/src/cortexm/exception_handlers.c 

OBJS += \
./stm32haldev/system/src/cortexm/_initialize_hardware.o \
./stm32haldev/system/src/cortexm/_reset_hardware.o \
./stm32haldev/system/src/cortexm/exception_handlers.o 

C_DEPS += \
./stm32haldev/system/src/cortexm/_initialize_hardware.d \
./stm32haldev/system/src/cortexm/_reset_hardware.d \
./stm32haldev/system/src/cortexm/exception_handlers.d 


# Each subdirectory must supply rules for building sources it contributes
stm32haldev/system/src/cortexm/_initialize_hardware.o: /Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/src/cortexm/_initialize_hardware.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F072xB -I../include -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/STM32F0xx_HAL_Driver/Inc" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/boards" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/general" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/stm32f0" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32haldev/system/src/cortexm/_reset_hardware.o: /Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/src/cortexm/_reset_hardware.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F072xB -I../include -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/STM32F0xx_HAL_Driver/Inc" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/boards" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/general" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/stm32f0" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32haldev/system/src/cortexm/exception_handlers.o: /Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/src/cortexm/exception_handlers.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F072xB -I../include -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/STM32F0xx_HAL_Driver/Inc" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/boards" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/general" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/stm32f0" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


