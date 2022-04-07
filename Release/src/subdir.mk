################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AndroidControl.cpp \
../src/CanControl.cpp \
../src/PowerControl.cpp \
../src/SharedLibraries.cpp \
../src/StartDaemon.cpp \
../src/TemperatureControl.cpp \
../src/app_daemon.cpp 

OBJS += \
./src/AndroidControl.o \
./src/CanControl.o \
./src/PowerControl.o \
./src/SharedLibraries.o \
./src/StartDaemon.o \
./src/TemperatureControl.o \
./src/app_daemon.o 

CPP_DEPS += \
./src/AndroidControl.d \
./src/CanControl.d \
./src/PowerControl.d \
./src/SharedLibraries.d \
./src/StartDaemon.d \
./src/TemperatureControl.d \
./src/app_daemon.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


