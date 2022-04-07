/*
 * AndroidControl.h
 *
 *  Created on: Mar 4, 2022
 *      Author: bernar
 */

#ifndef ANDROIDCONTROL_H_
#define ANDROIDCONTROL_H_


#include <iostream>
#include <mutex>
#include <thread>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "CanControl.h"

#include "json.hpp"

using namespace std;
using namespace nlohmann;

#define MAX 600
#define PORT 14000
#define SA struct sockaddr



#define ADB_IDDLE			0
#define	ADB_POWER_UP		1
#define	ADB_INITIALIZE		2
#define	ADB_CONNECTING		3
#define	ADB_ESTABLISHED		4
#define	ADB_CLOSE_SOCKET	5


struct support_system_info {

	string serial_no;
	string local_version;
};

struct android_app {

	int interfaces;
	string metrics_version;
	string instrumentation_version;
	string script_version;
};

struct android_update {

	string 	path;
	string 	md5;
	string 	version;
};

struct android_device {

	string id_sonda;
	string serial_no;
	string model;
};


struct android_message {

	int type;
	int message_id;

	json json_message;

	struct android_app metrics_app;
	struct android_update android_dev_update;
	struct android_device android_dev;
};

class AndroidControl: public CanControl {
public:

	struct android_message android_data;

	AndroidControl();
	virtual ~AndroidControl();

	void	android_data_initialize();
	void 	change_state_communication(uint32_t state);
	void	communication_state_machine();

	int 	create_socket(int *);
	int 	connect_socket(struct sockaddr_in *, int *);
	int 	listen_socket(int *);
	int 	send_socket(int * sockfd, const void * data, int lenght);
	int 	close_socket(int *);

	int 	save_message_info();
};


#endif /* ANDROIDCONTROL_H_ */
