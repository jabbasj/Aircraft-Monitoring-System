/*
 * SystemOutput.cpp
 *
 *  Created on: 24-Nov-2016
 *      Author: j_abba
 */

#include "SystemOutput.h"

//=============================================================================
// SYSTEM OUTPUT
//=============================================================================
std::string SystemOutput::getTimeStamp() {
	char buffer[80];
	std::string time_stamp;

	time_t secs = time(0);		/* get seconds since the Epoch */
	struct tm * local;
	local = localtime(&secs);	/* convert to localtime */
	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", local);	/* and set the string */
	time_stamp = std::string(buffer);

	return time_stamp;
}


//=============================================================================
// DIAL
//=============================================================================
void Dial::setUnit(std::string unit) {
	unit_type = unit;
}

void Dial::setMeasurement(float measure) {
	measurement = measure;
}

float Dial::getMeasurement() {
	return measurement;
}

std::string Dial::printMeasurement() {
	//std::cout << measurement << " " << unit_type << std::endl;
	return to_string(measurement) + " " + unit_type;
}


//=============================================================================
// LAMP
//=============================================================================
Lamp::Lamp() {
	color = "green";
}

void Lamp::setGreen() {
	color = "green";
}
void Lamp::setRed() {
	Lamp::color = "red";
}

std::string Lamp::getColor() {
	//std::cout << color;
	return color;
}


//=============================================================================
// CRT DISPLAY
//=============================================================================
void CRTDisplay::send_warning(SystemInput* sender, std::string msg) {

	std::string time_stamp = getTimeStamp();
	msg += ", TIME: " + time_stamp;
	WARNING warning = { sender, time_stamp, msg };

	pthread_mutex_lock( &warnings_mutex );
	warning_messages.push_back(warning);
	pthread_mutex_unlock( &warnings_mutex );

	//std::cout << std::endl << msg << std::endl;
}

WARNING CRTDisplay::get_top_warning() {

	WARNING warning = {};

	pthread_mutex_lock( &warnings_mutex );

	if (warning_messages.size() > 0) {
		warning = warning_messages.front();
	}

	pthread_mutex_unlock( &warnings_mutex );

	return warning;
}


void CRTDisplay::erase_top_warning() {

	pthread_mutex_lock( &warnings_mutex );

	if (warning_messages.size() > 0) {
		warning_messages.erase(warning_messages.begin());
	}

	pthread_mutex_unlock( &warnings_mutex );
}

void CRTDisplay::send_reading(std::string reading) {
	pthread_mutex_lock( &readings_mutex );

	readings_requested.push_back(reading);

	pthread_mutex_unlock( &readings_mutex );
}

void CRTDisplay::clear_readings() {
	pthread_mutex_lock( &readings_mutex );

	readings_requested.clear();

	pthread_mutex_unlock( &readings_mutex );
}

void CRTDisplay::clear_screen() {
	//Portable solution
	std::cout << string( 100, '\n' );
}

void CRTDisplay::print_screen(std::string fuelColor, std::string pressureColor, std::string temperatureColor,
	std::string smoke1Color, std::string smoke2Color, std::string fuelDialMeasurement,
	std::string pressureDialMeasurement, std::string temperatureDialMeasurement, bool READINGS_REQUESTED) {

	clear_screen();

	std::cout << "===========================  LAMPS  =======================================\n";
	std::cout << "Fuel-> " << fuelColor << "\n";
	std::cout << "Engine Pressure-> " << pressureColor << "\n";
	std::cout << "Engine Temperature-> " << temperatureColor << "\n";
	std::cout << "Smoke Detector 1-> " << smoke1Color << "\n";
	std::cout << "Smoke Detector 2-> " << smoke2Color << "\n";
	std::cout << "\n===========================  DIALS  =======================================\n";
	std::cout << "Fuel-> " << fuelDialMeasurement << "\n";
	std::cout << "Engine Pressure-> " << pressureDialMeasurement << "\n";
	std::cout << "Engine Temperature-> " << temperatureDialMeasurement << "\n";
	std::cout << "\n===========================  WARNINGS  ====================================\n";

	pthread_mutex_lock( &warnings_mutex );
	for (unsigned int i = 0 ; i < warning_messages.size(); i++) {
		std::cout << warning_messages[i].message << "\n";
	}
	if (warning_messages.size() == 0) {
		std::cout << "NONE\n";
	}
	pthread_mutex_unlock( &warnings_mutex );

	std::cout << "\n===========================  READINGS  ====================================\n";

	pthread_mutex_lock( &readings_mutex );
	for (unsigned int i = 0; i < readings_requested.size(); i++) {
		std::cout << readings_requested[i] << "\n";
	}
	if (readings_requested.size() == 0) {
		std::cout << "NONE\n";
	}
	pthread_mutex_unlock( &readings_mutex );

	std::cout << "\n\n===========================================================================\n";
	std::cout << "Select OPTION: ACK Warning (Y), IGNORE Warning (N), REQUEST Reading (R),\n";
	std::cout << " Quit (K), SIMULATE Smoke1 (Z), SIMULATE Smoke2 (X), CLEAR Readings (C)\n";
	std::cout << "===========================================================================\n";

	if (READINGS_REQUESTED) {
		std::cout << "\n===========================================================================\n";
		std::cout << "Select READING: Fuel (F), Pressure (P), Temperature (T), Smoke1 (Z), Smoke2 (X)\n";
		std::cout << "===========================================================================\n";
	}
	fflush(stdout);
}


//=============================================================================
// STORAGE
//=============================================================================
void Storage::save_warning(std::string msg) {

	pthread_mutex_lock( &mutex );

	std::string time_stamp = getTimeStamp();

	WARNING to_save = {NULL, time_stamp, msg};

	magnetic_medium.push(to_save);
	usleep(MilliToMicroseconds(MAGNETIC_MEDIUM_RESPONSE_TIME));

	std::cout << "SAVED:: " << to_save.message  << ", TIME: " << to_save.time << std::endl;

	pthread_mutex_unlock( &mutex );
}



template <typename T> std::string SystemOutput::to_string(T value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << value ;

  //convert the string stream into a string and return
  return os.str() ;
}
