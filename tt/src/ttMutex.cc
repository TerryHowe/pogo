#include "ttMutex.h"
#include <memory.h>


namespace tt {


Mutex::Mutex()
{
#ifdef _REENTRANT
   pthread_mutex_t tmpMute = PTHREAD_MUTEX_INITIALIZER;
   (void)memcpy(&mute, &tmpMute, sizeof(pthread_mutex_t));
#endif
}

Mutex::~Mutex()
{
#ifdef _REENTRANT
#endif
}

};
