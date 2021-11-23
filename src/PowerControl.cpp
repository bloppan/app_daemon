/*
 * PowerControl.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: bernardo
 */

#include "PowerControl.h"

PowerControl::PowerControl() {
	// TODO Auto-generated constructor stub

	this->PowerData	= NULL;
	this->State		= IDDLE;
}

void PowerControl::StateMachine()
{

	while(1){

		switch(PowerControl::State){

			case IDDLE:

				SharedLibraries::LoadLibrary(PAC1932);
				SharedLibraries::PAC1932_Initialize();

				this->State = SENSOR_READY;
				break;

			case SENSOR_READY:

				SharedLibraries::PAC1932_GetAllValues(PowerControl::PowerData);
				break;

			case WARNING:


				break;

			case ERROR:

				break;

			default:
				break;

		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


PowerControl::~PowerControl() {
	// TODO Auto-generated destructor stub
}

