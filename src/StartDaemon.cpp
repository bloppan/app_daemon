/*
 * StartDaemon.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include <thread>
#include <unistd.h>

#include "StartDaemon.h"

/**
 * Constructor de la clase StartDaemon
 */
StartDaemon::StartDaemon() {
	// TODO Auto-generated constructor stub

	std::cout << "StartDaemon Constructor" << std::endl;
}

/**
 * Lanza los hilos del sistema de soporte
 */
void StartDaemon::LaunchThreads()
{

	std::cout << "\nDAEMON - Launching Threads... \n" << std::endl;

	std::thread TempThread(&TemperatureControl::TempStateMachine, *this);
	std::thread PowerThread(&PowerControl::PowerStateMachine, *this);
	std::thread AndroidThread(&AndroidControl::communication_state_machine, *this);
	std::thread CanThread(&CanControl::CanStateMachine, *this);


	while(1){

		// Do nothing
		sleep(1);
	}

}
/**
 * Destructor de la clase StartDaemon
 */
StartDaemon::~StartDaemon() {
	// TODO Auto-generated destructor stub
}

