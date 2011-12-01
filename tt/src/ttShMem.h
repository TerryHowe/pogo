#ifndef ttShMem_h
#define ttShMem_h

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "ttException.h"

extern int errno;
namespace tt {

template<typename T>
class ShMem {
public:
	ShMem(key_t key, int shmflg=0666) {
		if ((id = shmget(key, sizeof(T), (IPC_CREAT|shmflg))) == -1) {
			std::string errMsg("shmat error id=<");
			errMsg += id;
			errMsg += "> shmflg=<";
			errMsg += shmflg;
			errMsg += "> errno=<";
			errMsg += errno;
			errMsg += ">";
			throw TTEXCEPTION(errMsg);
		}
		if ((_ptr = (T*)shmat(id, NULL, shmflg)) == NULL) {
			std::string errMsg("shmat error id=<");
			errMsg += id;
			errMsg += "> shmflg=<";
			errMsg += shmflg;
			errMsg += "> errno=<";
			errMsg += errno;
			errMsg += ">";
			throw TTEXCEPTION(errMsg);
		}
	}
	~ShMem() { shmdt((char*)_ptr); }
	T& get() const { return(_ptr); }
	T& operator->() const { get(); }
private:
	ShMem(ShMem& rhs);
	ShMem& operator=(ShMem& rhs);
	int id;
	T* _ptr;
};

};

#endif
