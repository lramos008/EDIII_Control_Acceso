################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/subsystems/input_sequence_scan/input_sequence_scan.c 

OBJS += \
./Core/subsystems/input_sequence_scan/input_sequence_scan.o 

C_DEPS += \
./Core/subsystems/input_sequence_scan/input_sequence_scan.d 


# Each subdirectory must supply rules for building sources it contributes
Core/subsystems/input_sequence_scan/%.o Core/subsystems/input_sequence_scan/%.su Core/subsystems/input_sequence_scan/%.cyclo: ../Core/subsystems/input_sequence_scan/%.c Core/subsystems/input_sequence_scan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/subsystems/display_manager" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/subsystems/input_sequence_scan" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/subsystems/user_check_task" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/drivers/keypad" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/drivers/sd_card" -I"C:/Users/Juli-Leo/Documents/Facultad/Proyecto_digitales_3/EDIII_Control_Acceso/Core/drivers/display" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Juli-Leo/Documents/Facultad/stm32 workspace/EDIII_Control_Acceso/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-subsystems-2f-input_sequence_scan

clean-Core-2f-subsystems-2f-input_sequence_scan:
	-$(RM) ./Core/subsystems/input_sequence_scan/input_sequence_scan.cyclo ./Core/subsystems/input_sequence_scan/input_sequence_scan.d ./Core/subsystems/input_sequence_scan/input_sequence_scan.o ./Core/subsystems/input_sequence_scan/input_sequence_scan.su

.PHONY: clean-Core-2f-subsystems-2f-input_sequence_scan

