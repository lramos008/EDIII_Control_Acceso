################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/drivers/display/display_functions.c \
../Core/drivers/display/ssd1306.c \
../Core/drivers/display/ssd1306_fonts.c \
../Core/drivers/display/ssd1306_tests.c 

OBJS += \
./Core/drivers/display/display_functions.o \
./Core/drivers/display/ssd1306.o \
./Core/drivers/display/ssd1306_fonts.o \
./Core/drivers/display/ssd1306_tests.o 

C_DEPS += \
./Core/drivers/display/display_functions.d \
./Core/drivers/display/ssd1306.d \
./Core/drivers/display/ssd1306_fonts.d \
./Core/drivers/display/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Core/drivers/display/%.o Core/drivers/display/%.su Core/drivers/display/%.cyclo: ../Core/drivers/display/%.c Core/drivers/display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/subsystems/display_manager" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/subsystems/input_sequence_scan" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/subsystems/user_check_task" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/drivers/keypad" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/drivers/sd_card" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/drivers/display" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Juli-Leo/Documents/Facultad/stm32 workspace/EDIII_Control_Acceso/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-drivers-2f-display

clean-Core-2f-drivers-2f-display:
	-$(RM) ./Core/drivers/display/display_functions.cyclo ./Core/drivers/display/display_functions.d ./Core/drivers/display/display_functions.o ./Core/drivers/display/display_functions.su ./Core/drivers/display/ssd1306.cyclo ./Core/drivers/display/ssd1306.d ./Core/drivers/display/ssd1306.o ./Core/drivers/display/ssd1306.su ./Core/drivers/display/ssd1306_fonts.cyclo ./Core/drivers/display/ssd1306_fonts.d ./Core/drivers/display/ssd1306_fonts.o ./Core/drivers/display/ssd1306_fonts.su ./Core/drivers/display/ssd1306_tests.cyclo ./Core/drivers/display/ssd1306_tests.d ./Core/drivers/display/ssd1306_tests.o ./Core/drivers/display/ssd1306_tests.su

.PHONY: clean-Core-2f-drivers-2f-display

