/*
 * SharedLibraries.h
 *
 *  Created on: 18 nov. 2021
 *      Author: bernardo
 */

#ifndef SHAREDLIBRARIES_H_
#define SHAREDLIBRARIES_H_

#include <iostream>
#include <cstring>
#include <cstdio>

#include <stdio.h>
#include <dlfcn.h>

#include "../../app_includes/app_typedef.h"
#include "../../app_includes/app_errors.h"

class SharedLibraries {

public:

	SharedLibraries();
	virtual ~SharedLibraries();

	void* SearchLibrary(const char*, const char*);
	error_type LoadLibrary(uint32_t);

	error_type (*HTS221_Initialize)(void);
	error_type (*HTS221_getHumidity)(float *);
	error_type (*HTS221_getTemperature)(float *);

	error_type (*configGPIO)(uint8_t, char*);
	error_type (*freeGPIO)(uint8_t);
	error_type (*getGPIO_Value)(uint8_t, uint8_t*);
	error_type (*getGPIO_Direction)(uint8_t, char*);
	error_type (*setGPIO_Value)(uint8_t, uint8_t);

	error_type (*PAC1932_Initialize)(void);
	error_type (*PAC1932_GetAllValues)(PAC1932_struct*);

	error_type (*setLED_Value)(uint8_t, uint8_t);
	error_type (*PCA9532_Initialize)(void);

};

#endif /* SHAREDLIBRARIES_H_ */
