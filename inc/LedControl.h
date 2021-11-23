/*
 * LedControl.h
 *
 *  Created on: 19 nov. 2021
 *      Author: bernardo
 */

#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_


#include "../inc/TemperatureControl.h"
#include "../inc/PowerControl.h"

class LedControl: public TemperatureControl, public PowerControl {
public:
	LedControl();
	virtual ~LedControl();

	void StateMachine();

};

#endif /* LEDCONTROL_H_ */
