/*
 * SharedLibraries.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: bernardo
 */

#include "SharedLibraries.h"

SharedLibraries::SharedLibraries() {
	// TODO Auto-generated constructor stub

	this->HTS221_Initialize		= NULL;
	this->HTS221_getHumidity 	= NULL;
	this->HTS221_getTemperature	= NULL;

	this->configGPIO 		= NULL;
	this->freeGPIO 			= NULL;
	this->getGPIO_Value 	= NULL;
	this->getGPIO_Direction = NULL;
	this->setGPIO_Value 	= NULL;

	this->PAC1932_Initialize	= NULL;
	this->PAC1932_GetAllValues	= NULL;

	this->setLED_Value			= NULL;
	this->PCA9532_Initialize 	= NULL;
}

SharedLibraries::~SharedLibraries() {
	// TODO Auto-generated destructor stub
}

error_type SharedLibraries::LoadLibrary(uint32_t Library)
{
	error_type codeError = NO_ERROR;
	void* libHandlerHTS221 = NULL;
	void* libHandlerGPIO = NULL;
	void *libHandlerPAC1932 = NULL;
	void *libHandlerPCA9532 = NULL;

	switch(Library){

	case HTS221:

		// Load HTS221 library
		libHandlerHTS221 = SharedLibraries::SearchLibrary("libHTS221.so.1", "/usr/lib/");

		this->HTS221_Initialize		= (error_type ( *)(void)) dlsym(libHandlerHTS221, "HTS221_Initialize");
		this->HTS221_getHumidity 	= (error_type ( *)(float *)) dlsym(libHandlerHTS221, "HTS221_getHumidity");
		this->HTS221_getTemperature	= (error_type ( *)(float *)) dlsym(libHandlerHTS221, "HTS221_getTemperature");
		break;

	case GPIO:

		// Load GPIO shared library
		libHandlerGPIO = SharedLibraries::SearchLibrary("libGPIO.so.1", "/usr/lib/");

		this->configGPIO 		= (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "configGPIO");
		this->freeGPIO 			= (error_type ( *)(uint8_t)) dlsym(libHandlerGPIO, "freeGPIO");
		this->getGPIO_Value 	= (error_type ( *)(uint8_t, uint8_t*)) dlsym(libHandlerGPIO, "getGPIO_Value");
		this->getGPIO_Direction = (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "getGPIO_Direction");
		this->setGPIO_Value 	= (error_type ( *)(uint8_t, uint8_t)) dlsym(libHandlerGPIO, "setGPIO_Value");
		break;

	case PAC1932:

		// Load PAC1932 shared library
		libHandlerPAC1932 = SharedLibraries::SearchLibrary("libPAC1932.so.1", "/usr/lib/");

		this->PAC1932_Initialize	= (error_type ( *)(void)) dlsym(libHandlerPAC1932, "PAC1932_Initialize");
		this->PAC1932_GetAllValues	= (error_type ( *)(PAC1932_struct*)) dlsym(libHandlerPAC1932, "PAC1932_GetAllValues");
		break;

	case PCA9532:

		// Load PCA9532 shared library
		libHandlerPCA9532 = SharedLibraries::SearchLibrary("libPCA9532.so.1", "/usr/lib/");

		this->setLED_Value			= (error_type ( *)(uint8_t, uint8_t)) dlsym(libHandlerPCA9532, "setLED_Value");
		this->PCA9532_Initialize 	= (error_type ( *)(void)) dlsym(libHandlerPCA9532, "PCA9532_Initialize");

		break;

	default:
		break;

	}

	return codeError;
}

void* SharedLibraries::SearchLibrary(const char* name, const char* directory)
{
	void *libHandler = NULL;
	char route[256] = {0};

	std::strcat(route, directory);
	std::strcat(route, name);

	libHandler = dlopen(route, RTLD_LAZY);

	if(libHandler == NULL) printf("[ERROR]\tLoading %s \n", route);

	else printf("[OK]\tLibrary %s loaded \n", name);

	printf("Hola \n");

	return libHandler;
}
