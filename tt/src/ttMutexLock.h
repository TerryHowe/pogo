#ifndef ttMutexLock_h
#define ttMutexLock_h

#include "ttMutex.h"
#include "ttException.h"

namespace tt {

class MutexLock {
private:
#ifdef _REENTRANT
	Mutex& mutie;
#endif
public:
	MutexLock(Mutex& _mutie, const char* file = "unknown", unsigned int line=0) throw(Exception);
	~MutexLock() throw(Exception);
};
	
};

#endif
