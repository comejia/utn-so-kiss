################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gestionar_consola.c \
../src/kernel.c \
../src/kernel_config.c 

OBJS += \
./src/gestionar_consola.o \
./src/kernel.o \
./src/kernel_config.o 

C_DEPS += \
./src/gestionar_consola.d \
./src/kernel.d \
./src/kernel_config.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2022-1c-lo-importante-es-aprobar/utils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


