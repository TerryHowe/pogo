#ifndef ttTimerContext_h
#define ttTimerContext_h

#include "ttException.h"
#include "ttTimeVal.h"
#include "ttCallback.h"


namespace tt {

class TimerContext {
public:
	TimerContext(const int val, Callback* cb = NULL);
	virtual ~TimerContext();

	void callbackMethod();
	void setTimerVal(const int val);
	void incrementTimer();
	struct timeval* getTimeOut();
	struct itimerval* getItimerval();
private:
	TimerContext(const TimerContext& rhs);
	TimerContext& operator=(const TimerContext& rhs);
	int timerInterval;
	TimeVal nextTimeOut;
	TimeVal timeOut;
	struct itimerval Itimerval;
	Callback* cbptr;
};

};

#endif
