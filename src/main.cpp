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

using namespace std;
using namespace nlohmann;


uint32_t TempThread_isActive, PowerThread_isActive, AndroidThread_isActive, CanThread_isActive;
uint32_t TempState, PowerState, AndroidState, CanState;

//float temperatura;

std::mutex log_mutex;

int main() {

	StartDaemon daemon;

	TempState 		= 0;
	PowerState 		= 0;
	AndroidState	= 0;
	CanState		= 0;

	TempThread_isActive		= 1;
	PowerThread_isActive	= 1;
	AndroidThread_isActive	= 1;
	CanThread_isActive		= 1;


	daemon.LaunchThreads();

	return 0;
}
