/*
 * StartDaemon.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include <thread>
#include <mutex>

#include "StartDaemon.h"

#include <unistd.h>

using namespace std;

StartDaemon::StartDaemon() {
	// TODO Auto-generated constructor stub

	std::cout << "StartDaemon Constructor" << std::endl;
}

void StartDaemon::get_ThreadsState()
{


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
	std::thread PowerThread(&PowerControl::PwrStateMachine, *this);

	TempThread.join();
	PowerThread.join();


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

