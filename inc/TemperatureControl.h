/*
 * TemperatureControl.h
 *
 *  Created on: 16 nov. 2021
 *      Author: bernardo
 */

#ifndef TEMPERATURECONTROL_H_
#define TEMPERATURECONTROL_H_

#include <iostream>
#include <thread>
#include <string.h>
#include <stdio.h>
#include "../inc/SharedLibraries.h"

class TemperatureControl: public SharedLibraries {
public:
	TemperatureControl();
	virtual ~TemperatureControl();

	void  StateMachine(void);

	float* TempData;
	float* HumData;

	enum States {

		IDDLE,
		SENSOR_READY,
		WARNING,
		ERROR

	}State;

};

#endif /* TEMPERATURECONTROL_H_ */
