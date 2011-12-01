#include <errno.h>
#include <signal.h>
#include "ttTimer.h"
#include "ttInfraLog.h"

extern "C" {
static void
signalHandler(int sig)
{
	
	tt::Timer::instance().callHandler();
	
}
};

namespace tt {

Timer* Timer::inst = NULL;


Timer::Timer()
  : savedTimer(NULL)
{
	
	
}

Timer::~Timer()
{
	
	delete savedTimer;
	
}

Timer&
Timer::instance()
{
	// TODO Mutex
        if (inst == NULL) inst = new Timer;
        return(*inst);
}


void
Timer::set(int secs, Callback* callback)
{
	
	savedTimer = new TimerContext(secs, callback);
	if (sigset(SIGALRM, signalHandler) == SIG_ERR) {
		throw TTEXCEPTIONERR("Error setting signal handler");
	}
	if (setitimer(ITIMER_REAL, savedTimer->getItimerval(), NULL) != 0) {
		throw TTEXCEPTIONERR("Error setting timer");
	}
	
}

void
Timer::callHandler()
{
	
	savedTimer->callbackMethod();
	
}

};
