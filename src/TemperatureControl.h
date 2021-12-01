/*
 * TemperatureControl.h
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#ifndef TEMPERATURECONTROL_H_
#define TEMPERATURECONTROL_H_

#include <iostream>

#include <stdint.h>

#include "PowerControl.h"


class TemperatureControl: public PowerControl {
public:
	TemperatureControl();
	virtual ~TemperatureControl();

	void  TempStateMachine();

//	float TempData;
//	float HumData;

	bool TempThread_ON;

	uint32_t StateTemp;
};

#endif /* TEMPERATURECONTROL_H_ */
