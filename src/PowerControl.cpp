/*
 * PowerControl.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include "PowerControl.h"

extern uint32_t PowerThread_isActive;
extern uint32_t PowerState;

/*

USB, BATERY, PERIPHERALS, SOM

OK	OK		OK				OK	- VERDE			// Todos los voltajes funcionan correctamente
OK	OK		OK				NOK	- YELLOW		// Falla la alimentacion de los perifericos
OK	OK		NOK				OK	- YELLOW		// Falla la alimentacion de los perifericos
OK	OK		NOK				NOK	- YELLOW		// Falla la alimentacion de los perifericos
OK 	NOK		OK				OK 	- PURPLE		// Falla la alimentacion del movil
OK	NOK		OK				NOK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
OK	NOK		NOK				OK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
OK	NOK		NOK				NOK - CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
NOK	OK		OK				OK	- PURPLE		// Falla la alimentacion del movil
NOK	OK		OK				NOK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
NOK	OK		NOK				OK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
NOK	OK		NOK				NOK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
NOK NOK		OK				OK	- PURPLE		// Falla la alimentacion del movil
NOK NOK		OK				NOK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
NOK NOK		NOK				OK	- CYAN			// Falla la alimentacion del MOVIL y los PERIFERICOS
NOK	NOK		NOK				NOK	- RED			// Falla todas las alimentaciones

Reset	-> LED_OFF		POWER_IDDLE
Loaded	-> BLUE			POWER_INITIALIZE

(00 00) -> VERDE		POWER_READY (Todo ceros)
(11 11) -> RED			POWER_FAULT_ALL_SOURCES (Todo unos)

(00 X1) -> PURPLE		POWER_FAULT_TERMINAL
(X1 00) -> YELLOW		POWER_FAULT_PERIPHERALS
(X1 X1) -> CYAN			POWER_FAULT_TERMINAL_AND_PERIPHERALS

Bad Read -> WHITE		POWER_ERROR_READ

*/

#define	POWER_IDDLE									0		// Librerias sin cargar
#define	POWER_INITIALIZE							1		// Sensor de voltaje inicializado
#define	POWER_READY									2		// Lectura correcta de todos los voltajes
#define	POWER_FAULT_ALL_SOURCES						3		// Fallo en todas las alimentaciones
#define	POWER_FAULT_PERIPHERALS						4		// Fallo en la alimentacion de los perifericos
#define	POWER_FAULT_TERMINAL						5		// Fallo en la alimentacion de terminal
#define	POWER_FAULT_TERMINAL_AND_PERIPHERALS		6		// Fallo en la alimentacion de terminal y perifericos
#define	POWER_ERROR_READ							7		// Fallo lectura del sensor

/**
 * Constructor de la clase PowerControl
 */
PowerControl::PowerControl() {
	// TODO Auto-generated constructor stub

	std::cout << "PowerControl Constructor" << std::endl;
}
/**
 * Cambio de estado en la alimentacion del sistema de soporte
 */
void PowerControl::PowerChangeState(uint32_t State)
{

	switch(State){

		case POWER_READY:	// Lectura correcta de todos los voltajes (0x0000)
			this->setLED_Color_Blink(LED_PWR, GREEN, NO_BLINK);
			break;
		case POWER_FAULT_ALL_SOURCES:	// 0x1111
			this->setLED_Color_Blink(LED_PWR, RED, NO_BLINK);
			break;
		case POWER_FAULT_PERIPHERALS: // 0x01
			this->setLED_Color_Blink(LED_PWR, PURPLE, NO_BLINK);
			break;
		case POWER_FAULT_TERMINAL:	  // 0x10
			this->setLED_Color_Blink(LED_PWR, YELLOW, NO_BLINK);
			break;
		case POWER_FAULT_TERMINAL_AND_PERIPHERALS:	// 0x11
			this->setLED_Color_Blink(LED_PWR, CYAN, NO_BLINK);
			break;
		case POWER_ERROR_READ:
			this->setLED_Color_Blink(LED_PWR, WHITE, NO_BLINK);
			break;
		default:
			break;
	}
}
/**
 * Maquina de estados de control de la alimentacion del sistema de soporte
 */
void PowerControl::PowerStateMachine()
{

	PAC1932_struct	PowerData = {0};
	uint32_t		power_error_code = 0;
	uint32_t 		old_state;

	old_state = PowerState;

	while(PowerThread_isActive){

		std::cout << "\n\nPOWER THREAD:\t" << std::endl;

		switch(PowerState){

			case POWER_IDDLE:

				if(!this->LoadLibrary(PAC1932) && !this->LoadLibrary(PCA9532) && !
						this->LoadLibrary(GPIO) && !this->PCA9532_Initialize() && !this->setLED_Color_Blink(LED_PWR, BLUE, NO_BLINK)){

					PowerState = POWER_INITIALIZE;
				}
				break;

			case POWER_INITIALIZE:

				if(!this->PAC1932_Initialize()){

					PowerState = POWER_READY;
				}
				break;

			default:

				if(!this->PAC1932_GetAllValues(&PowerData)){

					power_error_code = 0x0000;

					// Se comprueban si los voltajes de los perifericos y del terminal estan dentro del rango de lecturas correctas. En caso de que
					// alguno de ellos no este dentro del rango se marca 1 bandera de error
					if(PowerData.USB_Connector.Voltage < 4.8 || PowerData.USB_Connector.Voltage > 5.0){

						power_error_code |= 0x0001;
					}
					if(PowerData.Terminal_Battery.Voltage < 4.1 || PowerData.Terminal_Battery.Voltage > 4.3){

						power_error_code |= 0x0010;
					}
					if(PowerData.Peripherals.Voltage < 3.1 || PowerData.Peripherals.Voltage > 3.3){

						power_error_code |= 0x0100;
					}
					if(PowerData.SOM.Voltage < 3.1 || PowerData.SOM.Voltage > 3.3){

						power_error_code |= 0x1000;
					}

					// Si todo esta bien (0x0000) se ignora esta comprobacion
					// Si todo esta mal (0x1111) se ignora esta comprobacion
					// Si hay algo mal, se obtiene el codigo de error (0x01, 0x10 o 0x11)
					if(power_error_code != 0x0000 && power_error_code != 0x1111){

						power_error_code = (power_error_code & 0x0001) | ((power_error_code & 0x0010) >> 8) | ((power_error_code & 0x0100) >> 8) | ((power_error_code & 0x1000) >> 16);
					}

					// Se actualiza el estado del control de potencia en funcion del codigo de error obtenido
					if(power_error_code == 0x0000){

						PowerState = POWER_READY;

					}else if(power_error_code == 0x1111){

						PowerState = POWER_FAULT_ALL_SOURCES;

					}else if(power_error_code == 0x01){

						PowerState = POWER_FAULT_PERIPHERALS;

					}else if(power_error_code == 0x10){

						PowerState = POWER_FAULT_TERMINAL;

					}else if(power_error_code == 0x11){

						PowerState = POWER_FAULT_TERMINAL_AND_PERIPHERALS;

					}else{

						PowerState = POWER_ERROR_READ;
					}
				}else{
					std::cout << "\nERROR reading Power sensor \n" << std::endl;
					// Encender LED blanco
					this->setLED_Color_Blink(LED_PWR, WHITE, NO_BLINK);
				}

				break;
		}

		// Comprueba si se tiene que cambiar de estado
		if(PowerState != old_state){

			this->PowerChangeState(PowerState);
			old_state = PowerState;
		}

		// Se duerme el hilo durante 3 segundos
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
/**
 * Destructor de la clase PowerControl
 */
PowerControl::~PowerControl() {
	// TODO Auto-generated destructor stub
}

