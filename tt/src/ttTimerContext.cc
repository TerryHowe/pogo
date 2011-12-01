#include <errno.h>
#include <memory.h>
#include "ttTimerContext.h"
#include "ttInfraLog.h"

namespace tt {


TimerContext::TimerContext(const int val, Callback* cb)
  : timerInterval(val), cbptr(cb)
{
	
	setTimerVal(val);
	
}

TimerContext::~TimerContext()
{
	
	delete cbptr;
	
}

void
TimerContext::setTimerVal(const int val)
{
	
	nextTimeOut.setCurrentTime();
	nextTimeOut.tv_sec += val;
	timerInterval = val;
	
}

void
TimerContext::callbackMethod()
{
	
	if (cbptr) cbptr->callbackMethod();
	
}

void
TimerContext::incrementTimer()
{
	
	nextTimeOut.tv_sec += timerInterval;
	
}

struct timeval*
TimerContext::getTimeOut()
{
	
	if (timerInterval) {
		TimeVal currentTime;
		timeOut = nextTimeOut - currentTime;
		if (timeOut.tv_usec < 0)
			timeOut.tv_usec = 0;
		if (timeOut.tv_sec < 0)
			timeOut.tv_sec = 0;
		if ((timeOut.tv_usec == 0) && (timeOut.tv_sec == 0)) {
			timeOut.tv_usec = 1000;
		}
		
		
		return(&timeOut);
	}
	
	return(NULL);
}

struct itimerval*
TimerContext::getItimerval()
{
	
	(void)getTimeOut();
	(void)memset(&Itimerval, 0, sizeof(Itimerval));
	// No reset for now, perhaps add another method for reset
	// Itimerval.it_interval.tv_sec = timerInterval;
	// Itimerval.it_interval.tv_usec = 0;
	Itimerval.it_value.tv_sec = timeOut.tv_sec;
	Itimerval.it_value.tv_usec = timeOut.tv_usec;
	
	return(&Itimerval);
}

};
