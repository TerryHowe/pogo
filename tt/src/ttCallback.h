#ifndef ttCallback_h
#define ttCallback_h

#include "ttException.h"
#include "ttTimeVal.h"


namespace tt {

class Callback {
public:
	Callback();
	virtual ~Callback();
	virtual void callbackMethod() = 0;
private:
	Callback(const Callback& rhs);
	Callback& operator=(const Callback& rhs);
};

};

#endif
