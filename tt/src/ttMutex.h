#ifndef ttMutex_h
#define ttMutex_h

#ifdef _REENTRANT
#include <pthread.h>
#endif

namespace tt {

class Mutex {
private:
#ifdef _REENTRANT
	pthread_mutex_t mute;
#endif
public:
	Mutex();
	~Mutex();
#ifdef _REENTRANT
	pthread_mutex_t* getMute() { return(&mute); }
#endif
};
	
};

#endif
