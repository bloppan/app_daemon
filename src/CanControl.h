/*
 * CanControl.h
 *
 *  Created on: Mar 7, 2022
 *      Author: bernar
 */

#ifndef CANCONTROL_H_
#define CANCONTROL_H_

#include "TemperatureControl.h"

class CanControl: public TemperatureControl {
public:
	CanControl();
	virtual ~CanControl();

	void CanStateMachine();
};

#endif /* CANCONTROL_H_ */
