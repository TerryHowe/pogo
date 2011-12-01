#ifndef ttSelector_h
#define ttSelector_h

#include "ttException.h"
#include "ttSelectable.h"
#include "ttTimerContext.h"
#include <map>


namespace tt {

typedef std::map<int, Selectable*> Selection;

class Selector {
public:
	Selector();
	virtual ~Selector();
	void add(Selectable& entry);
	void remove(Selectable& entry);
	void select();
	void setTimer(TimerContext* val);
private:
	Selector(Selector& rhs);
	Selector& operator=(Selector& rhs);
	void doUpdate();
	Selection readers;
	Selection writers;
	Selection errors;
	int nfds;
	fd_set* readfds;
	fd_set* writefds;
	fd_set* errorfds;
	TimerContext* timerContext;
};

};

#endif
