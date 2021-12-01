/*
 * TemperatureControl.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include "TemperatureControl.h"
#include <thread>
#include <mutex>

extern std::mutex mutex_hardware;

TemperatureControl::TemperatureControl() {
	// TODO Auto-generated constructor stub

	this->StateTemp = SENSOR_IDDLE;

//	this->HumData	= NULL;
//	this->TempData	= NULL;

	this->TempThread_ON = true;

	std::cout << "TemperatureControl Constructor" << std::endl;


}


void TemperatureControl::TempStateMachine()
{

	float TempData = 0;
	float HumData = 0;

	while(this->TempThread_ON){

		switch(this->StateTemp) {

			case SENSOR_IDDLE:

				this->LoadLibrary(HTS221);
				this->LoadLibrary(PCA9532);

				mutex_hardware.lock();
				this->HTS221_Initialize();
				this->PCA9532_Initialize();
				mutex_hardware.unlock();

				mutex_hardware.lock();
				this->setLED_Value(LED_TEMP, BLUE);
				mutex_hardware.unlock();

				this->StateTemp = SENSOR_READY;

				std::cout << "\n-DAEMON Temperature state READY \n" << std::endl;
				break;

			case SENSOR_READY:

				std::cout << "\n-DAEMON Temperature Thread: \n" << std::endl;
				mutex_hardware.lock();

				this->HTS221_getTemperature(&TempData);
				this->HTS221_getHumidity(&HumData);
				mutex_hardware.unlock();

				mutex_hardware.lock();
				this->setLED_Value(LED_TEMP, GREEN);
				mutex_hardware.unlock();

				std::cout << "Temperatura = ";
				std::cout << TempData << std::endl;
				std::cout << "Humedad = ";
				std::cout << HumData << std::endl;

				std::cout << "Power state = " << this->PowerState << std::endl;

				std::cout << "\n" << std::endl;

				break;

			case SENSOR_WARNING:

				break;

			case SENSOR_ERROR:

			break;

			default:
				break;

		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
//	}
	}
}

TemperatureControl::~TemperatureControl() {
	// TODO Auto-generated destructor stub
}

