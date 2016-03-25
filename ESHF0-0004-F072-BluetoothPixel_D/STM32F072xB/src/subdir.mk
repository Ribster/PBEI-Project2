################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/application_adc.c \
../src/application_bluetooth.c \
../src/application_flash.c \
../src/application_general.c \
../src/application_ledPWM.c \
../src/application_lm75.c \
../src/application_printf.c \
../src/application_rng.c \
../src/application_rtc.c \
../src/application_usb.c \
../src/application_ws2812.c \
../src/main.c \
../src/stm32f0xx_hal_msp.c \
../src/stm32f0xx_it.c 

OBJS += \
./src/_write.o \
./src/application_adc.o \
./src/application_bluetooth.o \
./src/application_flash.o \
./src/application_general.o \
./src/application_ledPWM.o \
./src/application_lm75.o \
./src/application_printf.o \
./src/application_rng.o \
./src/application_rtc.o \
./src/application_usb.o \
./src/application_ws2812.o \
./src/main.o \
./src/stm32f0xx_hal_msp.o \
./src/stm32f0xx_it.o 

C_DEPS += \
./src/_write.d \
./src/application_adc.d \
./src/application_bluetooth.d \
./src/application_flash.d \
./src/application_general.d \
./src/application_ledPWM.d \
./src/application_lm75.d \
./src/application_printf.d \
./src/application_rng.d \
./src/application_rtc.d \
./src/application_usb.d \
./src/application_ws2812.d \
./src/main.d \
./src/stm32f0xx_hal_msp.d \
./src/stm32f0xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F072xB -I../include -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/STM32F0xx_HAL_Driver/Inc" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/boards" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/general" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/stm32f0" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


