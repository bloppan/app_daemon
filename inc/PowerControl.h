/*
 * PowerControl.h
 *
 *  Created on: 18 nov. 2021
 *      Author: bernardo
 */

#ifndef POWERCONTROL_H_
#define POWERCONTROL_H_

#include <iostream>
#include <thread>
#include <string.h>
#include <stdio.h>
#include "../inc/SharedLibraries.h"

class PowerControl: public SharedLibraries {
public:
	PowerControl();
	virtual ~PowerControl();

	void  StateMachine(void);

	PAC1932_struct* PowerData;

	enum States {

		IDDLE,
		SENSOR_READY,
		WARNING,
		ERROR

	}State;

};

#endif /* POWERCONTROL_H_ */
