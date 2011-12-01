#include <iostream>
#include "ttThread.h"


namespace tt {

extern "C" {
  void*
  startThread(void* arg)
  {
	ThreadBase* theThread = static_cast<ThreadBase*>(arg);
	void* retVal = theThread->run();
	pthread_exit(retVal);
	return(retVal);
  }
}

ThreadBase::ThreadBase()
{
}

void
ThreadBase::start() throw(Exception)
{
   if (pthread_create(&tid, NULL, startThread, this) != 0) {
	std::string eStr("Error creating thread");
	throw tt::ExceptionErrno(__FILE__, __LINE__, eStr);
   }
   ThreadJoiner::instance().add(tid);
   return;
}

ThreadBase::~ThreadBase()
{
}

ThreadJoiner* ThreadJoiner::inst = NULL;

ThreadJoiner&
ThreadJoiner::instance()
{
	// TODO Mutex
	if (inst == NULL) inst = new ThreadJoiner;
	return(*inst);
}

void
ThreadJoiner::wait()
{
	std::list<pthread_t>::iterator itr;
	itr = tids.begin();
	while (itr != tids.end()) {
		(void)pthread_join((pthread_t)(*itr), (void **)NULL);
		tids.pop_front();
		itr = tids.begin();
	}
}

};
