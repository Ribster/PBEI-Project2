################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/application_adc.c \
../src/application_ble.c \
../src/application_general.c \
../src/application_gpio.c \
../src/application_leddim.c \
../src/application_lm75.c \
../src/application_rtc.c \
../src/application_shiftreg.c \
../src/application_spiflash.c \
../src/application_uart.c \
../src/application_ws2812b.c \
../src/main.c \
../src/stm32f0xx_hal_cb.c \
../src/stm32f0xx_hal_msp.c \
../src/stm32f0xx_it.c 

OBJS += \
./src/_write.o \
./src/application_adc.o \
./src/application_ble.o \
./src/application_general.o \
./src/application_gpio.o \
./src/application_leddim.o \
./src/application_lm75.o \
./src/application_rtc.o \
./src/application_shiftreg.o \
./src/application_spiflash.o \
./src/application_uart.o \
./src/application_ws2812b.o \
./src/main.o \
./src/stm32f0xx_hal_cb.o \
./src/stm32f0xx_hal_msp.o \
./src/stm32f0xx_it.o 

C_DEPS += \
./src/_write.d \
./src/application_adc.d \
./src/application_ble.d \
./src/application_general.d \
./src/application_gpio.d \
./src/application_leddim.d \
./src/application_lm75.d \
./src/application_rtc.d \
./src/application_shiftreg.d \
./src/application_spiflash.d \
./src/application_uart.d \
./src/application_ws2812b.d \
./src/main.d \
./src/stm32f0xx_hal_cb.d \
./src/stm32f0xx_hal_msp.d \
./src/stm32f0xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall  -g -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F072xB -I../include -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/STM32F0xx_HAL_Driver/Inc" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/system/CMSIS/Device/ST/STM32F0xx/Include" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/boards" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/general" -I"/Users/Robbe/GITHUB/ECLIPSE/STM32_HAL/ESHXX000-stm32haldev/stm32dev/include/stm32f0" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


