/*
 * timer.h
 *
 *  Created on: 12-Oct-2016
 *      Author: j_abba
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <sys/neutrino.h>

typedef struct
{
	int message;  //DATA_REQUEST 1 , DATA_READY 2
}_message;

typedef union
{
	_message msg;
	struct _pulse pulse;
}Message;

#include <iostream.h>
#include <time.h>
#include <errno.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>

#define TIMERCODE 1

class Timer
{
	private:
		int coid;                  //connection back to channel
		struct sigevent event;     //event to deliver
		struct itimerspec itime;   //timer's data struct
		timer_t timer_id;          //timer's ID for the timer

	public:
		Timer() {};

		Timer(int chid)
		{
				coid = ConnectAttach(0,0, chid,0,0);   //Attach timer to our channel
				if (coid == -1)
				{
					cerr << "Timer: ConnectAttach ERROR :(" << endl;
					//exit(EXIT_FAILURE);
				}
				//initialization macro of the sigevent:"event" structure
				SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, TIMERCODE, 0);
				cout << "TIMER pulse initiated" << endl;

				//create timer
				if (timer_create(CLOCK_REALTIME, &event, &timer_id) == -1)
					cout << " timer_create ERROR"<< endl;
		}
		~Timer()
		{
			  //cout << "TIMER Destructor called" << endl;
		}

		void setTimer(long seconds, long nanoseconds)
		{
			//Will receive  pulse initially (the itime.it_value) and every
			//(the itime.it_interval)seconds thereafter
			itime.it_value.tv_sec = seconds;
			itime.it_value.tv_nsec = nanoseconds;
			itime.it_interval.tv_sec = seconds;
			itime.it_interval.tv_nsec = nanoseconds;
			timer_settime(timer_id, 0, &itime, NULL);    //0 for relative time
		}
};

//=============================================================================

#endif /* TIMER_HPP_ */
