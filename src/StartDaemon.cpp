/*
 * StartDaemon.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include <thread>
#include <mutex>

#include "StartDaemon.h"
//#include "shared_libraries.h"

#include <unistd.h>

using namespace std;

extern uint32_t TempThread_isActive;
extern uint32_t TempState;

extern uint32_t PowerThread_isActive;
extern uint32_t PowerState;

extern uint32_t AndroidThread_isActive;

extern float temperatura;

extern uint8_t global_errors;
/**
 * Constructor de la clase StartDaemon
 */
StartDaemon::StartDaemon() {
	// TODO Auto-generated constructor stub

	std::cout << "StartDaemon Constructor" << std::endl;
}

void StartDaemon::get_ThreadsState()
{
/*
	uint32_t contador = 0;
	uint32_t DaemonThread = 1;


	getchar();

	while(DaemonThread){

		contador++;

		if(contador == 3){

			temperatura = 45;
		}

		if(contador == 7){

			temperatura = 60;
		}
		if(contador == 11){

			temperatura = 110;
		}

		if(contador == 15){

			TempThread_ON = 0;
			PowerThread_ON = 0;
			DaemonThread = 0;
		}

		sleep(1);
	}
*/
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
	//std::thread CanThread(&CanControl::CanStateMachine, *this);

	while(1){

		// DAEMON MAIN THREAD

		sleep(1);
	}
}
/**
 * Destructor de la clase StartDaemon
 */
StartDaemon::~StartDaemon() {
	// TODO Auto-generated destructor stub
}

