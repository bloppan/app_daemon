/*
 * CanControl.cpp
 *
 *  Created on: Mar 7, 2022
 *      Author: bernar
 */

#include "CanControl.h"

extern uint32_t CanThread_isActive;
extern uint32_t CanState;

#define	CAN_IDDLE		0
#define CAN_INITIALIZE	1
#define	CAN_CONFIGURE	2
#define CAN_LISTEN		3
#define	CAN_ERROR		4

//extern std::mutex ////log_mutex;
/*
 * Constructor de la clase CanControl
 */
CanControl::CanControl() {
	// TODO Auto-generated constructor stub

}
/*
 * Destructor de la clase CanControl
 */
CanControl::~CanControl() {
	// TODO Auto-generated destructor stub
}
/*
 * Maquina de estados de la comunicacion CAN
 */
void CanControl::CanStateMachine()
{
	struct can_message can_data;

	while(CanThread_isActive){

		switch(CanState){

			case CAN_IDDLE:
				//log_mutex.lock();
				std::cout << "\n\nCAN THREAD:\t" << std::endl;
				if(!this->LoadLibrary(CAN)){

					CanState = CAN_INITIALIZE;
				}
				//log_mutex.unlock();
				break;
			case CAN_INITIALIZE:
				//log_mutex.lock();
				std::cout << "\n\nCAN THREAD:\t" << std::endl;
				// Inicializa el bus CAN
				if(!this->CAN_Initialize(&can_data)){

					CanState = CAN_CONFIGURE;
				}
				//log_mutex.unlock();
				break;
			case CAN_CONFIGURE:
				//log_mutex.lock();
				std::cout << "\n\nCAN THREAD:\t" << std::endl;
				// Configura el bus CAN
				if(!this->CAN_Configure(&can_data)){

					CanState = CAN_LISTEN;
				}else{

					CanState = CAN_INITIALIZE;
				}
				//log_mutex.unlock();
				break;
			case CAN_LISTEN:
				// Escucha mensajes del bus CAN
				//log_mutex.lock();
				std::cout << "\n\nCAN THREAD:\t" << std::endl;
				if(this->CAN_SendFile(&can_data, (char *)"/home/user1/can_message.txt")){ // Si se detecta algun error, se reinicia el bus

					CanState = CAN_CONFIGURE;
				}
				//log_mutex.unlock();
				break;
			default:
					CanState = CAN_CONFIGURE;
				break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
