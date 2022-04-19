//============================================================================
// Name        : main.cpp
// Author      : Bernardo
// Version     :
// Copyright   : Your copyright
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "StartDaemon.h"

// Variables de activacion / desactivacion de los hilos
uint32_t TempThread_isActive, PowerThread_isActive, AndroidThread_isActive, CanThread_isActive;

// Variables de estado de las maquinas de estado de los hilos
uint32_t TempState, PowerState, AndroidState, CanState;

int main() {

	StartDaemon daemon;

	TempState 		= 0;
	PowerState 		= 0;
	AndroidState	= 0;
	CanState		= 0;

	TempThread_isActive		= 1;
	PowerThread_isActive	= 1;
	AndroidThread_isActive	= 1;
	CanThread_isActive		= 0;

	daemon.LaunchThreads();

	return 0;
}

