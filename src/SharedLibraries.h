/*
 * SharedLibraries.h
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#ifndef SHAREDLIBRARIES_H_
#define SHAREDLIBRARIES_H_

#include <iostream>
#include <cstring>
#include <cstdio>
#include <mutex>
#include <thread>

#include <stdio.h>
#include <dlfcn.h>


#include "../../app_includes/app_typedef.h"
#include "../../app_includes/app_errors.h"

/*
 * LED colors
 */
#define BLUE		0
#define	GREEN		1
#define RED			2
#define	PURPLE		3
#define	YELLOW		4
#define CYAN		5
#define	WHITE		6
#define	LED_OFF		7


/*
 * LEDs Tasks
 */
#define	LED_TEMP		0
#define	LED_PWR			1
#define	LED_ANDROID		2
#define	LED_SIM			3


class SharedLibraries {
public:
	SharedLibraries();
	virtual ~SharedLibraries();

	void* SearchLibrary(const char*, const char*);

	error_type LoadLibrary(uint32_t);

	error_type (*WSEN_TIDS_Initialize)(void);
	error_type (*WSEN_TIDS_getTemperature)(float *);

	error_type (*configGPIO)(uint8_t, char*);
	error_type (*freeGPIO)(uint8_t);
	error_type (*getGPIO_Value)(uint8_t, uint8_t*);
	error_type (*getGPIO_Direction)(uint8_t, char*);
	error_type (*setGPIO_Value)(uint8_t, uint8_t);

	error_type (*PAC1932_Initialize)(void);
	error_type (*PAC1932_GetAllValues)(PAC1932_struct*);

	error_type (*setLED_Color_Blink)(uint8_t, uint8_t, uint8_t);
	error_type (*PCA9532_Initialize)(void);

	error_type (*CAN_Initialize)(struct can_message *);
	error_type (*CAN_Configure)(struct can_message *);
	error_type (*CAN_Send)(struct can_message *);
	error_type (*CAN_SendFile)(struct can_message *, char *);
	error_type (*CAN_Receive)(struct can_message *);
};

#endif /* SHAREDLIBRARIES_H_ */
