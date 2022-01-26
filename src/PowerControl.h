/*
 * PowerControl.h
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#ifndef POWERCONTROL_H_
#define POWERCONTROL_H_

#include <iostream>

#include "SharedLibraries.h"

#include "../../app_includes/app_typedef.h"

class PowerControl: public SharedLibraries {
public:
	PowerControl();
	virtual ~PowerControl();

	void  PowerStateMachine();
	void PowerChangeState(uint32_t);
};

#endif /* POWERCONTROL_H_ */
