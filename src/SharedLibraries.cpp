/*
 * SharedLibraries.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include "SharedLibraries.h"

/**
 * Constructor de la clase SharedLibraries
 */
SharedLibraries::SharedLibraries() {
	// TODO Auto-generated constructor stub

	std::cout << "SharedLibraries Constructor" << std::endl;

	this->WSEN_TIDS_Initialize		= NULL;
	this->WSEN_TIDS_getTemperature 	= NULL;

	this->configGPIO 		= NULL;
	this->freeGPIO 			= NULL;
	this->getGPIO_Value 	= NULL;
	this->getGPIO_Direction = NULL;
	this->setGPIO_Value 	= NULL;

	this->PAC1932_Initialize	= NULL;
	this->PAC1932_GetAllValues	= NULL;

	this->setLED_Value			= NULL;
	this->PCA9532_Initialize 	= NULL;

	this->CAN_Initialize		= NULL;
	this->CAN_Send				= NULL;
	this->CAN_SendFile			= NULL;
	this->CAN_Receive			= NULL;
}

SharedLibraries::~SharedLibraries() {
	// TODO Auto-generated destructor stub
}

/**
 * Carga las librerias compartidas
 */
error_type SharedLibraries::LoadLibrary(uint32_t Library)
{
	error_type codeError = NO_ERROR;
	void* libHandlerWSEN_TIDS = NULL;
	void* libHandlerGPIO = NULL;
	void *libHandlerPAC1932 = NULL;
	void *libHandlerPCA9532 = NULL;
	void *libHandlerCAN = NULL;

	switch(Library){

	case WSEN_TIDS:

		// Load WSEN-TIDS library
		libHandlerWSEN_TIDS = this->SearchLibrary("libWSEN_TIDS.so.1", "/usr/lib/");

		this->WSEN_TIDS_Initialize		= (error_type ( *)(void)) dlsym(libHandlerWSEN_TIDS, "WSEN_TIDS_Initialize");
		this->WSEN_TIDS_getTemperature	= (error_type ( *)(float *)) dlsym(libHandlerWSEN_TIDS, "WSEN_TIDS_getTemperature");
		break;

	case GPIO:

		// Load GPIO shared library
		libHandlerGPIO = this->SearchLibrary("libGPIO.so.1", "/usr/lib/");

		this->configGPIO 		= (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "configGPIO");
		this->freeGPIO 			= (error_type ( *)(uint8_t)) dlsym(libHandlerGPIO, "freeGPIO");
		this->getGPIO_Value 	= (error_type ( *)(uint8_t, uint8_t*)) dlsym(libHandlerGPIO, "getGPIO_Value");
		this->getGPIO_Direction = (error_type ( *)(uint8_t, char*)) dlsym(libHandlerGPIO, "getGPIO_Direction");
		this->setGPIO_Value 	= (error_type ( *)(uint8_t, uint8_t)) dlsym(libHandlerGPIO, "setGPIO_Value");
		break;

	case PAC1932:

		// Load PAC1932 shared library
		libHandlerPAC1932 = this->SearchLibrary("libPAC1932.so.1", "/usr/lib/");

		this->PAC1932_Initialize	= (error_type ( *)(void)) dlsym(libHandlerPAC1932, "PAC1932_Initialize");
		this->PAC1932_GetAllValues	= (error_type ( *)(PAC1932_struct*)) dlsym(libHandlerPAC1932, "PAC1932_GetAllValues");
		break;

	case PCA9532:

		// Load PCA9532 shared library
		libHandlerPCA9532 = this->SearchLibrary("libPCA9532.so.1", "/usr/lib/");

		this->setLED_Value			= (error_type ( *)(uint8_t, uint8_t)) dlsym(libHandlerPCA9532, "setLED_Value");
		this->PCA9532_Initialize 	= (error_type ( *)(void)) dlsym(libHandlerPCA9532, "PCA9532_Initialize");

		break;

	case CAN:

		// Load PCA9532 shared library
		libHandlerCAN = this->SearchLibrary("libCAN.so.1", "/usr/lib/");

		this->CAN_Initialize		= (error_type ( *)(struct can_message *)) dlsym(libHandlerCAN, "CAN_Initialize");
		this->CAN_Configure			= (error_type ( *)(struct can_message *)) dlsym(libHandlerCAN, "CAN_Configure");
		this->CAN_Send				= (error_type ( *)(struct can_message *)) dlsym(libHandlerCAN, "CAN_Send");
		this->CAN_SendFile			= (error_type ( *)(struct can_message *, char *)) dlsym(libHandlerCAN, "CAN_SendFile");
		this->CAN_Receive			= (error_type ( *)(struct can_message *)) dlsym(libHandlerCAN, "CAN_Receive");

		break;

	default:
		break;

	}

	return codeError;
}
/**
 * Busca la libreria compartida en el filesystem
 */
void* SharedLibraries::SearchLibrary(const char* name, const char* directory)
{
	void *libHandler = NULL;
	char route[256] = {0};

	std::strcat(route, directory);
	std::strcat(route, name);

	libHandler = dlopen(route, RTLD_LAZY);

	if(libHandler == NULL) printf("[ERROR]\tLoading %s \n", route);

	else printf("[OK]\tLibrary %s loaded \n", name);

	return libHandler;
}
