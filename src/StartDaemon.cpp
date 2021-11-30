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

StartDaemon::StartDaemon() {
	// TODO Auto-generated constructor stub

	std::cout << "StartDaemon Constructor" << std::endl;
}

void StartDaemon::LaunchThreads()
{

	std::thread TempThread(&TemperatureControl::TempStateMachine, *this);
	std::thread PowerThread(&PowerControl::PwrStateMachine, *this);

	TempThread.join();
	PowerThread.join();

}

StartDaemon::~StartDaemon() {
	// TODO Auto-generated destructor stub
}

