/*
 * TemperatureControl.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include "TemperatureControl.h"

extern uint32_t TempThread_isActive;
extern uint32_t TempState;

extern std::mutex log_mutex;

//extern float temperatura;

#define	TEMP_IDDLE			0		// Funciones del sensor sin cargar
#define	TEMP_INITIALIZE		1		// Inicializacion del sensor
#define	TEMP_NORMAL			2		// Sensor inicializado y temperatura normal
#define	TEMP_ALARM_1		3		// Temperatura alta, se activan los ventiladores
#define	TEMP_ALARM_2		4		// Temperatura alta, ventiladores activados y movil apagado
#define	TEMP_ERROR			5		// Error de lectura del sensor de temperatura

/**
 * Constructor de la clase TemperatureControl
 */
TemperatureControl::TemperatureControl() {
	// TODO Auto-generated constructor stub

	std::cout << "TemperatureControl Constructor" << std::endl;

}
/**
 * Cambio de estado del control de temperatura
 */
void TemperatureControl::TempChangeState(uint32_t State)
{

	switch(State) {

		case TEMP_IDDLE:
			// Temperatura normal, LED azul
			this->setLED_Value(LED_TEMP, BLUE);
			break;

		case TEMP_NORMAL:
			// Temperatura normal, LED verde
			this->setLED_Value(LED_TEMP, GREEN);

			// Apagar ventiladores
			this->setGPIO_Value(FANOUT_1, 0);
			this->setGPIO_Value(FANOUT_2, 0);

			// Encender el movil
			this->setGPIO_Value(EN_5V_USB_MOB, 1);
			this->setGPIO_Value(EN_4V2, 1);
			break;
		case TEMP_ALARM_1:
			// Encender LED amarillo
			this->setLED_Value(LED_TEMP, YELLOW);

			// Encender ventiladores
			this->setGPIO_Value(FANOUT_1, 1);
			this->setGPIO_Value(FANOUT_2, 1);

			// Encender el movil
			this->setGPIO_Value(EN_5V_USB_MOB, 0);
			this->setGPIO_Value(EN_4V2, 0);
			break;
		case TEMP_ALARM_2:
			// Encender LED rojo
			this->setLED_Value(LED_TEMP, RED);

			// Encender ventiladores
			this->setGPIO_Value(FANOUT_1, 1);
			this->setGPIO_Value(FANOUT_2, 1);

			// Apagar el movil
			this->setGPIO_Value(EN_5V_USB_MOB, 1);
			this->setGPIO_Value(EN_4V2, 1);

			break;

		case TEMP_ERROR:
			// Encender LED blanco
			this->setLED_Value(LED_TEMP, WHITE);

			break;

		default:
			break;
	}
}
/**
 * Maquina de estados del control de temperatura
 */
void TemperatureControl::TempStateMachine()
{

	float TempData = 0;

	char direction[4] = "out";

	uint32_t old_state;

	old_state = TempState;

	while(TempThread_isActive){

		switch(TempState){

			case TEMP_IDDLE:

				log_mutex.lock();
				if(!this->LoadLibrary(WSEN_TIDS) && !this->LoadLibrary(PCA9532) &&
						!this->LoadLibrary(GPIO) && !this->PCA9532_Initialize() && !this->setLED_Value(LED_TEMP, BLUE)){

					TempState = TEMP_INITIALIZE;
				}
				log_mutex.unlock();
				break;

			case TEMP_INITIALIZE:

				log_mutex.lock();
				std::cout << "\n\nTEMPERATURE THREAD:\t" << std::endl;

				//GPIOs de los ventiladores
				this->configGPIO(FANOUT_1, direction);
				this->configGPIO(FANOUT_2, direction);

				//GPIOs de la alimentacion del terminal
				this->configGPIO(EN_5V_USB_MOB, direction);
				this->configGPIO(EN_4V2, direction);

				this->WSEN_TIDS_Initialize();
				this->PCA9532_Initialize();

				log_mutex.unlock();
				TempState = TEMP_NORMAL;
				break;

			default:

				log_mutex.lock();
				std::cout << "\n\nTEMPERATURE THREAD:\t" << std::endl;

				// Lee el valor de temperatura del sensor
				if(this->WSEN_TIDS_getTemperature(&TempData) == NO_ERROR){

					//TempData = temperatura; // DEBUG

					if(TempData < 40){
						// Temperatura normal
						TempState = TEMP_NORMAL;
					}else if(TempData >= 40 && TempData < 50){
						// Temperatura elevada. Alarma nivel 1
						TempState = TEMP_ALARM_1;
					}else{
						// Temperatura muy elevada. Alarma nivel 2
						TempState = TEMP_ALARM_2;
					}
				}else{
					// Error de lectura del sensor. No se pueden leer datos del sensor
					TempState = TEMP_ERROR;
				}

				if(TempState == TEMP_ERROR){

					std::cout << "\nERROR reading Temperature sensor \n" << std::endl;
				}

				break;
		}

		// Comprueba si se tiene que cambiar de estado
		if(TempState != old_state){

			this->TempChangeState(TempState);
			old_state = TempState;
		}
		log_mutex.unlock();
		// El hilo se duerme durante 1 segundo
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
/**
 * Destructor de la clase TemperatureControl
 */
TemperatureControl::~TemperatureControl() {
	// TODO Auto-generated destructor stub
}

