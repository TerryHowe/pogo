#ifndef ttSelectable_h
#define ttSelectable_h

#include "ttException.h"


namespace tt {

class Selectable {
public:
	Selectable(int _fd=-1);
	virtual ~Selectable();
	int getReadFd() { return(fd); }
	int getWriteFd() { return(-1); }
	int getErrorFd() { return(-1); }
	virtual void doRead() {}
	virtual void doWrite() {}
	virtual void doError() {}
	void setFd(int _fd) { fd = _fd; }
protected:
private:
	Selectable(Selectable& rhs);
	Selectable& operator=(Selectable& rhs);
	int fd;
};

};

#endif
