#ifndef ttTimer_h
#define ttTimer_h

#include "ttTimerContext.h"


namespace tt {

class Timer {
public:
	virtual ~Timer();
	static Timer& instance();
	void set(int secs, Callback* callback);
	void callHandler();
private:
	Timer();
	Timer(Timer& rhs);
	Timer& operator=(Timer& rhs);
	TimerContext* savedTimer;
	static Timer* inst;
};

};

#endif
