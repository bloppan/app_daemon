//============================================================================
// Name        : main.cpp
// Author      : Bernardo
// Version     :
// Copyright   : Your copyright
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <mutex>
#include <thread>

#include <stdio.h>
#include <unistd.h>

#include "StartDaemon.h"

using namespace std;

std::mutex mutex_hardware;

int main() {

	StartDaemon daemon;

	daemon.LaunchThreads();

	cout << "Entra al while(1) " << std::endl;

	while(1){}



	sleep(10);

	return 0;
}
