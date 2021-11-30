/*
 * PowerControl.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include "PowerControl.h"

extern std::mutex mutex_hardware;

PowerControl::PowerControl() {
	// TODO Auto-generated constructor stub

	std::cout << "PowerControl Constructor" << std::endl;

	this->PowerData		= NULL;
	this->PowerState	= SENSOR_IDDLE;

	this->PowerThread_ON = true;
}

void PowerControl::PwrStateMachine()
{

	while(this->PowerThread_ON){

		switch(this->PowerState){

			case SENSOR_IDDLE:

				this->LoadLibrary(PAC1932);
				this->LoadLibrary(PCA9532);

				mutex_hardware.lock();
				this->PAC1932_Initialize();
				this->PCA9532_Initialize();
				mutex_hardware.unlock();

				mutex_hardware.lock();
				this->setLED_Value(LED_PWR, BLUE);
				mutex_hardware.unlock();

				this->PowerState = SENSOR_READY;
				break;

			case SENSOR_READY:

				mutex_hardware.lock();
				this->PAC1932_GetAllValues(this->PowerData);
				mutex_hardware.unlock();

				mutex_hardware.lock();
				this->setLED_Value(LED_PWR, GREEN);
				mutex_hardware.unlock();

				break;

			case SENSOR_WARNING:


				break;

			case SENSOR_ERROR:

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

