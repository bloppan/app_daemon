/*
 * TemperatureControl.cpp
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#include "TemperatureControl.h"


//#include "../../app_shared_libraries/libGPIO/GPIO_export.h"

#include <thread>
#include <mutex>

extern std::mutex mutex_hardware;
extern uint32_t TempThread_ON;
extern uint32_t TempState;

extern float temperatura;

#define	TEMP_IDDLE			0		// Funciones del sensor sin cargar
#define	TEMP_INITIALIZE		1		// Inicializacion del sensor
#define	TEMP_NORMAL			2		// Sensor inicializado y temperatura normal
#define	TEMP_ALARM_1		3		// Temperatura alta, se activan los ventiladores
#define	TEMP_ALARM_2		4		// Temperatura alta, ventiladores activados y movil apagado
#define	TEMP_ERROR			5		// Error de lectura del sensor de temperatura

#define EN_5V_USB_MOB	((((5)-1)*32)+((9)&31))
#define	EN_4V2			((((1)-1)*32)+((4)&31))

TemperatureControl::TemperatureControl() {
	// TODO Auto-generated constructor stub

	std::cout << "TemperatureControl Constructor" << std::endl;

}

void TemperatureControl::TempChangeState(uint32_t State)
{

	switch(State) {

		case TEMP_IDDLE:

			// Temperatura normal, LED azul
			mutex_hardware.lock();
			setLED_Value(LED_TEMP, BLUE);
			mutex_hardware.unlock();

			break;

		case TEMP_NORMAL:
			mutex_hardware.lock();
			// Temperatura normal, LED verde
			this->setLED_Value(LED_TEMP, GREEN);

			// Apagar ventiladores
			this->setGPIO_Value(5, 0);
			this->setGPIO_Value(9, 0);

			// Encender el movil
			this->setGPIO_Value(EN_5V_USB_MOB, 1);
			this->setGPIO_Value(EN_4V2, 1);
			mutex_hardware.unlock();
			break;

		case TEMP_ALARM_1:
			mutex_hardware.lock();
			// Encender LED amarillo
			this->setLED_Value(LED_TEMP, YELLOW);

			// Encender ventiladores
			this->setGPIO_Value(5, 1);
			this->setGPIO_Value(9, 1);

			// Encender el movil
			this->setGPIO_Value(EN_5V_USB_MOB, 0);
			this->setGPIO_Value(EN_4V2, 0);
			mutex_hardware.unlock();
			break;

		case TEMP_ALARM_2:
			mutex_hardware.lock();
			// Encender LED rojo
			this->setLED_Value(LED_TEMP, RED);

			// Encender ventiladores
			this->setGPIO_Value(5, 1);
			this->setGPIO_Value(9, 1);

			// Apagar el movil
			this->setGPIO_Value(EN_5V_USB_MOB, 1);
			this->setGPIO_Value(EN_4V2, 1);
			mutex_hardware.unlock();
			break;

		case TEMP_ERROR:
			mutex_hardware.lock();
			// Encender LED blanco
			this->setLED_Value(LED_TEMP, WHITE);
			mutex_hardware.unlock();
			break;

		default:
			break;
	}

}

void TemperatureControl::TempStateMachine()
{

	float TempData = 0;
	//float HumData = 0;

	char direction[] = "out";

	uint32_t old_state;

	old_state = TempState;

	while(TempThread_ON){

		switch(TempState){

			case TEMP_IDDLE:

				this->LoadLibrary(HTS221);
				this->LoadLibrary(PCA9532);
				this->LoadLibrary(GPIO);

				TempState = TEMP_INITIALIZE;
				break;

			case TEMP_INITIALIZE:

				mutex_hardware.lock();

				std::cout << "\n-DAEMON TempControl state READY \n" << std::endl;

				//GPIOs de los ventiladores
				this->configGPIO(5, direction);
				this->configGPIO(9, direction);

				//GPIOs de la alimentacion del terminal
				this->configGPIO(EN_5V_USB_MOB, direction);
				this->configGPIO(EN_4V2, direction);

				this->HTS221_Initialize();
				this->PCA9532_Initialize();

				this->setLED_Value(LED_TEMP, BLUE);

				mutex_hardware.unlock();

				TempState = TEMP_NORMAL;
				break;

			default:

				mutex_hardware.lock();
				std::cout << "\n-DAEMON TempControl thread \n" << std::endl;
				// Lee el valor de temperatura del sensor
				if(this->HTS221_getTemperature(&TempData) != NO_ERROR){

					TempData = temperatura; // DEBUG

					if(TempData < 40){
						// Temperatura normal
						TempState = TEMP_NORMAL;
					}else if(TempData >= 40 && TempData < 50){
						// Temperatura elevada. Alarma nivel 1
						TempState = TEMP_ALARM_1;
					}else if(TempData >= 50 && TempData <= 100){
						// Temperatura muy elevada. Alarma nivel 2
						TempState = TEMP_ALARM_2;
					}else{
						// Bad read. El sensor proporciona una lectura que no es correcta
						TempState = TEMP_ERROR;
					}
				}else{
					// Error de lectura del sensor. No se pueden leer datos del sensor
					TempState = TEMP_ERROR;
				}

				if(TempState == TEMP_ERROR){

					std::cout << "\nERROR reading Temperature sensor \n" << std::endl;
				}

				mutex_hardware.unlock();

				break;
		}

		// Comprueba si se tiene que cambiar de estado
		if(TempState != old_state){

			this->TempChangeState(TempState);
			old_state = TempState;
		}
		// El hilo se duerme durante 1 segundo
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

TemperatureControl::~TemperatureControl() {
	// TODO Auto-generated destructor stub
}

