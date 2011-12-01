#ifndef ttProcess_h
#define ttProcess_h

#include <pthread.h>
#include <list>
#include "ttException.h"

namespace tt {

class ProcessBase {
private:
	int pid;
protected:
	virtual int run() = 0;
public:
	ProcessBase();
	~ProcessBase();
	void start() throw(Exception);
	int getPid() { return(pid); }
};

//
// Singleton thread joiner
//
class ProcessJoiner {
private:
	ProcessJoiner() {}
	~ProcessJoiner() {}
	std::list<pthread_t> tids;
	static ProcessJoiner* inst;
protected:
public:
	static ProcessJoiner& instance();
	void add(pthread_t tid) { tids.push_back(tid); }
	void wait();
};

};

#endif
