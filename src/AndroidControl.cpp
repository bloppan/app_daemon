/*
 * AndroidControl.cpp
 *
 *  Created on: Mar 4, 2022
 *      Author: bernar
 */


#include "AndroidControl.h"

using namespace std;

extern uint32_t AndroidThread_isActive;

extern std::mutex log_mutex;
/**
 * Constructor de la clase AndroidControl
 */
AndroidControl::AndroidControl() {
	// TODO Auto-generated constructor stub

	std::cout << "AndroidControl Constructor" << std::endl;

}
/**
 * Destructor de la clase AndroidControl
 */
AndroidControl::~AndroidControl() {
	// TODO Auto-generated destructor stub

}
/**
 * Maquina de estados de la comunicacion con el movil Android
 */
void AndroidControl::communication_state_machine(void)
{
	int sockfd = 0;
	struct sockaddr_in servaddr;
	uint8_t portasim_value = 0, portasim_old_value = 0;

	char gpio_output[4] = "out";

	int communication_state = ADB_IDDLE;

    char command[100];

	while(AndroidThread_isActive){

		switch(communication_state){

		case ADB_IDDLE:

			this->android_data_initialize();

			this->update_server_file();

			log_mutex.lock();

			std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
			if(!this->LoadLibrary(PCA9532) && !this->LoadLibrary(GPIO) &&
					!this->PCA9532_Initialize() && !this->setLED_Value(LED_ANDROID, BLUE)){

				this->configGPIO(EN_4V2, gpio_output);
				this->configGPIO(EN_5V_USB_MOB, gpio_output);
				this->configGPIO(0, (char *) "in");

				communication_state = ADB_POWER_UP;
			}
			log_mutex.unlock();
			break;
		case ADB_POWER_UP:

			log_mutex.lock();
			std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
			if(!this->setGPIO_Value(EN_5V_USB_MOB, 1) && !this->setGPIO_Value(EN_4V2, 1)){

				communication_state = ADB_INITIALIZE;
			}
			log_mutex.unlock();
			break;
		case ADB_INITIALIZE:

			log_mutex.lock();
			std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		    /* command contiene la string del comando a ejecutar (a character array) */
		    bzero(command, sizeof(command));
		    sprintf(command, "adb devices");
		    // Ejecuta "adb devices"
		    popen(command,"r");
		    log_mutex.unlock();

		    sleep(3);

		    log_mutex.lock();
		    std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		    bzero(command, sizeof(command));
		    sprintf(command, "adb forward tcp:%d tcp:%d", PORT, PORT);
		    // Ejecuta el comando
		    popen(command,"r");
		    log_mutex.unlock();

		    sleep(1);

		    log_mutex.lock();

		    if(!this->create_socket(&sockfd)){

		    	this->setLED_Value(LED_ANDROID, WHITE);
				communication_state = ADB_CONNECTING;
		    }
		    log_mutex.unlock();
			break;
		case ADB_CONNECTING:
			log_mutex.lock();


			// Conexion al socket
			if(this->connect_socket(&servaddr, &sockfd) == NO_ERROR){
				this->setLED_Value(LED_ANDROID, GREEN);
				communication_state = ADB_ESTABLISHED;
				log_mutex.unlock();
			}else {
				communication_state = ADB_INITIALIZE;
				log_mutex.unlock();
				sleep(3);
			}

			break;
		case ADB_ESTABLISHED:
			log_mutex.lock();
			// Escucha el socket. Lo cierra en caso de perder la comunicacion
			if(this->listen_socket(&sockfd) != NO_ERROR){

				this->setLED_Value(LED_ANDROID, WHITE);
				communication_state = ADB_CLOSE_SOCKET;
			}
			log_mutex.unlock();
			break;
		case ADB_CLOSE_SOCKET:
			log_mutex.lock();
			// Cierra el socket

			if(this->close_socket(&sockfd) == NO_ERROR){

				communication_state = ADB_INITIALIZE;
				std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
				printf("[ANDROID]\t\tTrying reconnect to server...\n");
			}
			log_mutex.unlock();
			break;
		default:
			break;
		}

		log_mutex.lock();
		std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		// Comprueba si el portaSIM esta conectado
		this->getGPIO_Value(0, &portasim_value);

		if(portasim_value){
			// Si se lee un 1, el portaSIM esta conectado. Led verde.
			this->setLED_Value(LED_SIM, GREEN);
		}else{
			// Si se lee un 0, el portaSIM esta desconectado. Led blanco.
			this->setLED_Value(LED_SIM, WHITE);
		}

		log_mutex.unlock();

		usleep(200000);
	}
}

/**
 * Inicializacion de la estructura que contiene la informacion del movil Android
 */
void AndroidControl::android_data_initialize()
{

	this->android_data.type	 		= 0;
	this->android_data.message_id 	= 0;

	this->android_data.metrics_app.interfaces	= 0;
	this->android_data.metrics_app.instrumentation_version 	= "0.0";
	this->android_data.metrics_app.metrics_version			= "0.0";
	this->android_data.metrics_app.script_version			= "0.0";

	this->android_data.android_dev.id_sonda		= "S/N";
	this->android_data.android_dev.model		= "S/N";
	this->android_data.android_dev.serial_no	= "S/N";

	this->android_data.android_dev_update.md5		= "S/N";
	this->android_data.android_dev_update.path		= "S/N";
	this->android_data.android_dev_update.version	= "S/N";
}
/**
 * Cierre del socket
 */
int AndroidControl::close_socket(int * sockfd){

	if(close(*sockfd)){

		return APP_REPORT(APP_DAEMON, CLOSING_SOCKET);
	}else {

		return NO_ERROR;
	}
}
/**
 * Creacion del socket
 */
int AndroidControl::create_socket(int * sockfd){

	// socket create and verification
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sockfd == -1) {
		std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		printf("[ANDROID]\t\tSocket creation failed...\n");
		return APP_REPORT(APP_DAEMON, OPENING_SOCKET);
	}
	else{
		std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		printf("[ANDROID]\t\tSocket successfully created...\n");
		return NO_ERROR;
	}
}
/**
 * Conexion del socket
 */
int AndroidControl::connect_socket(struct sockaddr_in * servaddr, int * sockfd){

	bzero(servaddr, sizeof(*servaddr));
	// assign IP, PORT
	servaddr->sin_family = AF_INET;
	servaddr->sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr->sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(*sockfd, (SA*)servaddr, sizeof(*servaddr)) != 0) {
		std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		printf("[ANDROID]\t\tConnection with server failed...\n");
		return APP_REPORT(APP_DAEMON, CONNECTING_SOCKET);
	}else{
		std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		printf("[ANDROID]\t\tConnected to server... Waiting response...\n");
		return NO_ERROR;
	}
}
/**
 * Enviar a traves del socket
 */
int AndroidControl::send_socket(int * sockfd, const void * data , int lenght)
{
	// Escribe datos a traves del socket
	if (write(*sockfd, data, lenght) == lenght){
		printf("[ANDROID]\t\tWrite socket = %s \n", (char *) data);
		return NO_ERROR;
	} else {
		perror("[ANDROID]\t\tWrite error: ");
		return APP_REPORT(APP_DAEMON, WRITING_SOCKET);
	}
}
/**
 * Escucha los mensajes que llegan al socket
 */
int AndroidControl::listen_socket(int * sockfd)
{
    char buff[MAX];
    static int reintentos = 0;
    int nbytes = 0;

    fd_set rfds;
    struct timeval tv;

    tv.tv_sec = 3;
    tv.tv_usec = 0;

	bzero(buff, sizeof(buff));

	FD_ZERO(&rfds);
	FD_SET(*sockfd, &rfds);

	int retval = select(*sockfd + 1, &rfds, NULL, NULL, &tv);

	if(retval > 0){

		usleep(100000);
		nbytes = read(*sockfd, buff, sizeof(buff));

		if(nbytes == 0){

			if(reintentos++ > 3){

				return CONNECTION_LOST;
			}
			sleep(1);

		}else{

			reintentos = 0;

			try{

				this->android_data.json_message = nlohmann::json::parse(buff);
				this->save_message_info();

			}catch(const std::exception& e){
				std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
				printf("[ANDROID]\t\tJSON Parse error, received from server : %s", buff);
			}
		}
	}else if(retval == 0){
		//printf("Timeout! \n"); // do nothing
	}else{
		std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
		perror("[ANDROID]\t\tError select:  \n");
		return APP_REPORT(APP_DAEMON, SELECT_SOCKET);
	}

    return NO_ERROR;
}
/**
 * Guarda la informacion del mensaje JSON recibida
 */
int AndroidControl::save_message_info()
{

	char command[100];
	std::cout << "\n\nANDROID COMMUNICATION THREAD:\t" << std::endl;
	printf("[ANDROID]\t\tJSON Message received from server \n");

	this->android_data.type 		= this->android_data.json_message["header"]["type"];
	this->android_data.message_id 	= this->android_data.json_message["header"]["message_id"];

	switch(this->android_data.type){

		case 1:
			printf("Type 1: Android Info \n");

			// Guarda la informacion del movil Android
			this->android_data.android_dev.id_sonda 	= this->android_data.json_message["body"]["android_dev"]["id_sonda"];
			this->android_data.android_dev.serial_no 	= this->android_data.json_message["body"]["android_dev"]["serial_no"];
			this->android_data.android_dev.model	 	= this->android_data.json_message["body"]["android_dev"]["model"];

			this->android_data.metrics_app.interfaces 				= this->android_data.json_message["body"]["metrics_app"]["interfaces"];
			this->android_data.metrics_app.instrumentation_version 	= this->android_data.json_message["body"]["metrics_app"]["versions"]["instrumentation"];
			this->android_data.metrics_app.metrics_version 			= this->android_data.json_message["body"]["metrics_app"]["versions"]["metrics"];
			this->android_data.metrics_app.script_version 			= this->android_data.json_message["body"]["metrics_app"]["versions"]["script"];

			cout << "Android Device: " << endl <<
									"\t\tID Sonda: " << this->android_data.android_dev.id_sonda << endl <<
									"\t\tSerial number: " << this->android_data.android_dev.serial_no << endl <<
									"\t\tDevice model: " << this->android_data.android_dev.model << endl << endl;

			cout << "Metrics App: " << endl <<
									"\t\tInterfaces: " << this->android_data.metrics_app.interfaces << endl <<
									"\t\tInstrumentation version: " << this->android_data.metrics_app.instrumentation_version << endl <<
									"\t\tMetrics version: " << this->android_data.metrics_app.metrics_version << endl <<
									"\t\tScript version: " << this->android_data.metrics_app.script_version << endl << endl;

			this->update_server_file();
			break;

		case 2:
			printf("Type 2: Android update available \n");

			// Guarda la informacion de la actualizacion
			this->android_data.android_dev_update.path 		= this->android_data.json_message["body"]["path"];
			this->android_data.android_dev_update.md5		= this->android_data.json_message["body"]["md5"];
			this->android_data.android_dev_update.version	= this->android_data.json_message["body"]["version"];

			cout << "Android Update available: " << endl <<
									"\t\tpath: " << this->android_data.android_dev_update.path << endl <<
									"\t\tmd5: " << this->android_data.android_dev_update.md5 << endl <<
									"\t\tversion: " << this->android_data.android_dev_update.version << endl << endl;

			// Descarga el fichero de actualizacion del movil Android en el directorio /home
		    bzero(command, sizeof(command));
		    sprintf(command, "adb pull %s /home/", this->android_data.android_dev_update.path.c_str());
		    // Ejecuta el comando
		    popen(command,"r");
			// Lanza la actualizacion
		    system("/home/update_android/install.sh");

			break;

		case 3:
			printf("Type 3: Reboot Android device \n");
			// Reinicia el movil Android
			system("adb reboot");
			break;

		case 4:
			printf("Type 4: Reboot system support \n");
			// Reinicia el sistema de soporte
			system("reboot");
			break;

		default:
			printf("Another type \n");
			break;
	}

	return 0;
}

void AndroidControl::update_server_file(void)
{
	//Declaramos variables
	int fd;
	char web_content[2000];
	char content_aux[2000];

	bzero(web_content, 2000);
	bzero(content_aux, 2000);

	fd = open("/usr/share/apache2/default-site/htdocs/index.html",O_WRONLY|O_CREAT);

	sprintf(web_content, "ID Sonda: %s <br>", this->android_data.android_dev.id_sonda.c_str());

	sprintf(content_aux, "Serial Number: %s <br>", this->android_data.android_dev.serial_no.c_str());
	strcat(web_content, content_aux);
	bzero(content_aux, 2000);

	sprintf(content_aux, "Model: %s <br><br>", this->android_data.android_dev.model.c_str());
	strcat(web_content, content_aux);
	bzero(content_aux, 2000);

	sprintf(content_aux, "Interfaces: %d <br>", this->android_data.metrics_app.interfaces);
	strcat(web_content, content_aux);
	bzero(content_aux, 2000);

	sprintf(content_aux, "Instrumentation Version: %s <br>", this->android_data.metrics_app.instrumentation_version.c_str());
	strcat(web_content, content_aux);
	bzero(content_aux, 2000);

	sprintf(content_aux, "Metrics Version: %s <br>", this->android_data.metrics_app.metrics_version.c_str());
	strcat(web_content, content_aux);
	bzero(content_aux, 2000);

	sprintf(content_aux, "Script Version: %s <br>", this->android_data.metrics_app.script_version.c_str());
	strcat(web_content, content_aux);
	bzero(content_aux, 2000);

	//Guardamos en el archivo lo que ingresamos por teclado
	write(fd, web_content, sizeof(web_content));

	//CERRAR ARCHIVO
	close(fd);

}
