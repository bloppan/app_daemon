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

extern uint32_t TempThread_ON;
extern uint32_t TempState;

extern uint32_t PowerThread_ON;
extern uint32_t PowerState;

extern float temperatura;

StartDaemon::StartDaemon() {
	// TODO Auto-generated constructor stub

	std::cout << "StartDaemon Constructor" << std::endl;
}

void StartDaemon::get_ThreadsState()
{

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

}

void StartDaemon::LaunchThreads()
{

	std::cout << "\nDAEMON - Launching Threads... \n" << std::endl;

	/*
		while(1){

			this->TempStateMachine();
			this->PwrStateMachine();
			sleep(1);
		}
	*/


	std::thread TempThread(&TemperatureControl::TempStateMachine, *this);
	std::thread PowerThread(&PowerControl::PowerStateMachine, *this);
	std::thread DaemonThread(&StartDaemon::get_ThreadsState, *this);

	TempThread.join();
	PowerThread.join();
	DaemonThread.join();


/*
	while(1){

	std::thread TempThread(&TemperatureControl::TempStateMachine, *this);
	std::thread PowerThread(&PowerControl::PwrStateMachine, *this);

	TempThread.join();
	PowerThread.join();

	cout << "Temp state = " << TemperatureControl::StateTemp << std::endl;
	cout << "Power state = " << PowerControl::PowerState << std::endl;

}

*/

}

StartDaemon::~StartDaemon() {
	// TODO Auto-generated destructor stub
}

