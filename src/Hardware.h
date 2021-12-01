/*
 * Hardware.h
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "TemperatureControl.h"

class Hardware: public TemperatureControl {
public:
	Hardware();
	virtual ~Hardware();
};

#endif /* HARDWARE_H_ */
