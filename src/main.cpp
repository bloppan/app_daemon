//============================================================================
// Name        : main.cpp
// Author      : Bernardo
// Version     :
// Copyright   : Your copyright
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <mutex>
#include <thread>

#include <stdio.h>
#include <unistd.h>

#include "StartDaemon.h"
#include "TemperatureControl.h"

using namespace std;

std::mutex mutex_hardware;

uint32_t TempThread_ON, PowerThread_ON;
uint32_t TempState, PowerState;

float temperatura;

int main() {

	StartDaemon daemon;

	TempState 		= 0;
	PowerState 		= 0;

	TempThread_ON 	= 1;
	PowerThread_ON	= 1;

	temperatura = 0;

	daemon.LaunchThreads();

	cout << "Esperando enter ... " << std::endl;

	getchar();

	cout << "Entra al while(1) " << std::endl;

	while(1){}



	sleep(10);

	return 0;
}
