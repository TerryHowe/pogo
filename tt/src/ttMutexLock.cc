#include "ttMutexLock.h"


namespace tt {

MutexLock::MutexLock(tt::Mutex& _mutie, const char* file, unsigned int line
) throw(Exception)
#ifdef _REENTRANT
   : mutie(_mutie)
#endif
{
#ifdef _REENTRANT
   if (pthread_mutex_lock(mutie.getMute())) {
      std::string eStr("Error locking mutex <");
      eStr += file;
      eStr += ":";
      eStr += line;
      eStr += ">";
      throw  tt::Exception(__FILE__, __LINE__, eStr);
   }
#endif
}

MutexLock::~MutexLock() throw(Exception)
{
#ifdef _REENTRANT
   if (pthread_mutex_unlock(mutie.getMute()) != 0) {
      std::string eStr("Error unlocking mutex");
      throw  tt::Exception(__FILE__, __LINE__, eStr);
   }
#endif
}

};
