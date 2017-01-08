/*
 * SystemOutput.h
 *
 *  Created on: 24-Nov-2016
 *      Author: j_abba
 */

#ifndef SYSTEMOUTPUT_H_
#define SYSTEMOUTPUT_H_

#include <string>
#include <iostream.h>
#include <sstream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <ctime>

#include "SystemInput.h"

//Forward declarations
class SystemInput;
class FuelSensor;
class EnginePressureSensor;
class EngineTemperatureSensor;
class SmokeDetector;
class Keyboard;

#ifndef _WARNING_
#define _WARNING_
struct WARNING {
	SystemInput * sender;
	std::string time;
	std::string message;
};
#endif

//=============================================================================
// SYSTEM OUTPUT
//=============================================================================
class SystemOutput {
	friend class AircraftMonitoringSystem;
	friend class SystemInput;
protected:
	std::string getTimeStamp();
	int MilliToMicroseconds(int msec) { return 1000 * msec; }
	template <typename T> std::string to_string(T value);
};

//Derived classes of SystemOutput below:

//=============================================================================
// DIAL
//=============================================================================
class Dial: public SystemOutput {
	friend class AircraftMonitoringSystem;
	friend class SystemInput;
public:

	float measurement;
	std::string unit_type;

	void setUnit(std::string unit);
	void setMeasurement(float measure);
	float getMeasurement();
	std::string printMeasurement();
};


//=============================================================================
// LAMP
//=============================================================================
class Lamp: public SystemOutput {
	friend class AircraftMonitoringSystem;
	friend class SystemInput;
public:
	Lamp();
protected:
	std::string color;

	void setGreen();
	void setRed();
	std::string getColor();
};

//=============================================================================
// CRT DISPLAY
//=============================================================================
class CRTDisplay: public SystemOutput {
	friend class AircraftMonitoringSystem;
	friend class SystemInput;
protected:
	std::vector<WARNING> warning_messages;
	void send_warning(SystemInput* sender, std::string msg);
	WARNING get_top_warning();
	void erase_top_warning();

	std::vector<std::string> readings_requested;
	void send_reading(std::string);
	void clear_readings();

	void print_screen(std::string fuelColor, std::string pressureColor, std::string temperatureColor,
		std::string smoke1Color, std::string smoke2Color, std::string fuelDialMeasurement,
		std::string pressureDialMeasurement, std::string temperatureDialMeasurement, bool READINGS_REQUESTED);

	void clear_screen();

private:
	pthread_mutex_t warnings_mutex;
	pthread_mutex_t readings_mutex;
};


//=============================================================================
// STORAGE
//=============================================================================
class Storage: public SystemOutput {
	friend class AircraftMonitoringSystem;
	friend class SystemInput;

protected:
	std::queue<WARNING> magnetic_medium;
	void save_warning(std::string msg);

private:
	pthread_mutex_t mutex;

};



#endif /* SYSTEMOUTPUT_H_ */
