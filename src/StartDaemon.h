/*
 * StartDaemon.h
 *
 *  Created on: Nov 29, 2021
 *      Author: bernar
 */

#ifndef STARTDAEMON_H_
#define STARTDAEMON_H_

#include <iostream>

#include "Hardware.h"


class StartDaemon: public Hardware {
public:
	StartDaemon();
	virtual ~StartDaemon();

	void get_ThreadsState();
	void stopThreads();
	void LaunchThreads();
};

#endif /* STARTDAEMON_H_ */
