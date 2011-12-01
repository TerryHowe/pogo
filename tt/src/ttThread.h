#ifndef ttThread_h
#define ttThread_h

#include <pthread.h>
#include <list>
#include "ttException.h"

namespace tt {

class ThreadBase {
private:
	pthread_t tid;
protected:
public:
	ThreadBase();
	~ThreadBase();
	void start() throw(Exception);
	virtual void* run() = 0;
	pthread_t getTid() { return(tid); }
};

//
// Singleton thread joiner
//
class ThreadJoiner {
private:
	ThreadJoiner() {}
	~ThreadJoiner() {}
	std::list<pthread_t> tids;
	static ThreadJoiner* inst;
protected:
public:
	static ThreadJoiner& instance();
	void add(pthread_t tid) { tids.push_back(tid); }
	void wait();
};

};

#endif
