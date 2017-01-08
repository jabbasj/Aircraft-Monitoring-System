#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>   /// To get system Frequency

#include "AircraftMonitoringSystem.h"


//=============================================================================
//                                 M     A     I   N
//=============================================================================
int main(void)
{
	AircraftMonitoringSystem AMS;

	AMS.Start();

	std::cout << "All threads terminated. "<< std::endl;
	return 0;
}
//=============================================================================
