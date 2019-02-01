################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/actuator.c \
../Src/communicationOUT.c \
../Src/communicationvsz.c \
../Src/controlSTEERING.c \
../Src/controlVELOCITY.c \
../Src/linetracking.c \
../Src/main.c \
../Src/navigation.c \
../Src/statemachine.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c \
../Src/timing.c \
../Src/tracking.c 

OBJS += \
./Src/actuator.o \
./Src/communicationOUT.o \
./Src/communicationvsz.o \
./Src/controlSTEERING.o \
./Src/controlVELOCITY.o \
./Src/linetracking.o \
./Src/main.o \
./Src/navigation.o \
./Src/statemachine.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o \
./Src/timing.o \
./Src/tracking.o 

C_DEPS += \
./Src/actuator.d \
./Src/communicationOUT.d \
./Src/communicationvsz.d \
./Src/controlSTEERING.d \
./Src/controlVELOCITY.d \
./Src/linetracking.d \
./Src/main.d \
./Src/navigation.d \
./Src/statemachine.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d \
./Src/timing.d \
./Src/tracking.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -I"C:/fokartyasz/fokartyas/Inc" -I"C:/fokartyasz/fokartyas/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/fokartyasz/fokartyas/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/fokartyasz/fokartyas/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/fokartyasz/fokartyas/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


