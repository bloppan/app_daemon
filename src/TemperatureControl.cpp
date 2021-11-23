/*
 * TemperatureControl.cpp
 *
 *  Created on: 16 nov. 2021
 *      Author: bernardo
 */

#include "TemperatureControl.h"

using namespace std;

TemperatureControl::TemperatureControl() {

	this->State = IDDLE;

	this->HumData	= NULL;
	this->TempData	= NULL;

}

void TemperatureControl::StateMachine(void)
{

	while(1){

		switch(TemperatureControl::State) {

			case IDDLE:

				SharedLibraries::LoadLibrary(HTS221);
				SharedLibraries::HTS221_Initialize();

				this->State = SENSOR_READY;

			break;

			case SENSOR_READY:

				SharedLibraries::HTS221_getTemperature(TemperatureControl::TempData);
				SharedLibraries::HTS221_getHumidity(TemperatureControl::HumData);

				std::cout << "Temperatura = ";
				std::cout << TemperatureControl::TempData << endl;
				//std::cout << "Humedad = ";
				//std::cout << TemperatureControl::HumData << std::endl;

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

TemperatureControl::~TemperatureControl() {



}

