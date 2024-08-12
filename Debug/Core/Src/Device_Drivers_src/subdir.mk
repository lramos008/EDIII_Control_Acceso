################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Device_Drivers_src/File_Handling_RTOS.c \
../Core/Src/Device_Drivers_src/adc_functions.c \
../Core/Src/Device_Drivers_src/fatfs_sd.c \
../Core/Src/Device_Drivers_src/keypad.c \
../Core/Src/Device_Drivers_src/lock.c \
../Core/Src/Device_Drivers_src/screen.c \
../Core/Src/Device_Drivers_src/sd.c \
../Core/Src/Device_Drivers_src/ssd1306.c \
../Core/Src/Device_Drivers_src/ssd1306_fonts.c \
../Core/Src/Device_Drivers_src/ssd1306_tests.c 

OBJS += \
./Core/Src/Device_Drivers_src/File_Handling_RTOS.o \
./Core/Src/Device_Drivers_src/adc_functions.o \
./Core/Src/Device_Drivers_src/fatfs_sd.o \
./Core/Src/Device_Drivers_src/keypad.o \
./Core/Src/Device_Drivers_src/lock.o \
./Core/Src/Device_Drivers_src/screen.o \
./Core/Src/Device_Drivers_src/sd.o \
./Core/Src/Device_Drivers_src/ssd1306.o \
./Core/Src/Device_Drivers_src/ssd1306_fonts.o \
./Core/Src/Device_Drivers_src/ssd1306_tests.o 

C_DEPS += \
./Core/Src/Device_Drivers_src/File_Handling_RTOS.d \
./Core/Src/Device_Drivers_src/adc_functions.d \
./Core/Src/Device_Drivers_src/fatfs_sd.d \
./Core/Src/Device_Drivers_src/keypad.d \
./Core/Src/Device_Drivers_src/lock.d \
./Core/Src/Device_Drivers_src/screen.d \
./Core/Src/Device_Drivers_src/sd.d \
./Core/Src/Device_Drivers_src/ssd1306.d \
./Core/Src/Device_Drivers_src/ssd1306_fonts.d \
./Core/Src/Device_Drivers_src/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Device_Drivers_src/%.o Core/Src/Device_Drivers_src/%.su Core/Src/Device_Drivers_src/%.cyclo: ../Core/Src/Device_Drivers_src/%.c Core/Src/Device_Drivers_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Juli-Leo/Documents/Facultad/stm32 workspace/EDIII_Control_Acceso/Core/Inc" -I"C:/Users/Juli-Leo/Documents/Facultad/stm32 workspace/EDIII_Control_Acceso/Core/Inc/Device_Drivers_inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Device_Drivers_src

clean-Core-2f-Src-2f-Device_Drivers_src:
	-$(RM) ./Core/Src/Device_Drivers_src/File_Handling_RTOS.cyclo ./Core/Src/Device_Drivers_src/File_Handling_RTOS.d ./Core/Src/Device_Drivers_src/File_Handling_RTOS.o ./Core/Src/Device_Drivers_src/File_Handling_RTOS.su ./Core/Src/Device_Drivers_src/adc_functions.cyclo ./Core/Src/Device_Drivers_src/adc_functions.d ./Core/Src/Device_Drivers_src/adc_functions.o ./Core/Src/Device_Drivers_src/adc_functions.su ./Core/Src/Device_Drivers_src/fatfs_sd.cyclo ./Core/Src/Device_Drivers_src/fatfs_sd.d ./Core/Src/Device_Drivers_src/fatfs_sd.o ./Core/Src/Device_Drivers_src/fatfs_sd.su ./Core/Src/Device_Drivers_src/keypad.cyclo ./Core/Src/Device_Drivers_src/keypad.d ./Core/Src/Device_Drivers_src/keypad.o ./Core/Src/Device_Drivers_src/keypad.su ./Core/Src/Device_Drivers_src/lock.cyclo ./Core/Src/Device_Drivers_src/lock.d ./Core/Src/Device_Drivers_src/lock.o ./Core/Src/Device_Drivers_src/lock.su ./Core/Src/Device_Drivers_src/screen.cyclo ./Core/Src/Device_Drivers_src/screen.d ./Core/Src/Device_Drivers_src/screen.o ./Core/Src/Device_Drivers_src/screen.su ./Core/Src/Device_Drivers_src/sd.cyclo ./Core/Src/Device_Drivers_src/sd.d ./Core/Src/Device_Drivers_src/sd.o ./Core/Src/Device_Drivers_src/sd.su ./Core/Src/Device_Drivers_src/ssd1306.cyclo ./Core/Src/Device_Drivers_src/ssd1306.d ./Core/Src/Device_Drivers_src/ssd1306.o ./Core/Src/Device_Drivers_src/ssd1306.su ./Core/Src/Device_Drivers_src/ssd1306_fonts.cyclo ./Core/Src/Device_Drivers_src/ssd1306_fonts.d ./Core/Src/Device_Drivers_src/ssd1306_fonts.o ./Core/Src/Device_Drivers_src/ssd1306_fonts.su ./Core/Src/Device_Drivers_src/ssd1306_tests.cyclo ./Core/Src/Device_Drivers_src/ssd1306_tests.d ./Core/Src/Device_Drivers_src/ssd1306_tests.o ./Core/Src/Device_Drivers_src/ssd1306_tests.su

.PHONY: clean-Core-2f-Src-2f-Device_Drivers_src

