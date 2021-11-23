################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LedControl.cpp \
../src/PowerControl.cpp \
../src/SharedLibraries.cpp \
../src/TemperatureControl.cpp \
../src/testjson.cpp 

OBJS += \
./src/LedControl.o \
./src/PowerControl.o \
./src/SharedLibraries.o \
./src/TemperatureControl.o \
./src/testjson.o 

CPP_DEPS += \
./src/LedControl.d \
./src/PowerControl.d \
./src/SharedLibraries.d \
./src/TemperatureControl.d \
./src/testjson.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I../inc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


