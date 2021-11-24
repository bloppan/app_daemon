/*
 * main.cpp
 *
 *  Created on: 16 nov. 2021
 *      Author: bernardo
 */

#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "inc/SharedLibraries.h"
#include "inc/TemperatureControl.h"
#include "inc/PowerControl.h"
//#include "inc/json.hpp"

//#include <nlohmann/json.hpp>

using namespace std;

//using namespace nlohmann;

PAC1932_struct PAC1932data = {0};

int main(void)
{

	TemperatureControl T;
	PowerControl P;
/*
	json j;

	// add a number that is stored as double (note the implicit conversion of j to an object)
	j["pi"] = 3.141;

	// add a Boolean that is stored as bool
	j["happy"] = true;

	// add a string that is stored as std::string
	j["name"] = "Niels";

	// add another null object by passing nullptr
	j["nothing"] = nullptr;

	// add an object inside the object
	j["answer"]["everything"] = 42;

	// add an array that is stored as std::vector (using an initializer list)
	j["list"] = { 1, 0, 2 };

	// add another object (using an initializer list of pairs)
	j["object"] = { {"currency", "USD"}, {"value", 42.99} };

	//j << cout;

	cout << j << endl;
*/
	std::thread TempThread(&TemperatureControl::StateMachine, TemperatureControl());
	std::thread PwrThread(&PowerControl::StateMachine, PowerControl());

	while(1){}

	//std::this_thread::sleep_for(std::chrono::seconds(1000));

	//sleep(10);

	return 0;
}

