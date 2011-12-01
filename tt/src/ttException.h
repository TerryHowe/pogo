#ifndef ttException_h
#define ttException_h

#include <pthread.h>
#include <string>
#include <sstream>

namespace tt {

#define TTEXCEPTION(_x_) tt::Exception(__FILE__, __LINE__, _x_)
#define TTEXCEPTIONERR(_x_) tt::ExceptionErrno(__FILE__, __LINE__, _x_)


//
// Basic exception class
//
class Exception {
private:
	std::string file;
	unsigned int line;
protected:
	std::string mesg;
public:
	Exception(const char* _file, unsigned int _line, std::string _mesg);
	virtual ~Exception();
	std::string toString();
	std::string& getFile() { return(file); }
	unsigned int getLine() { return(line); }
	std::string& getMesg() { return(mesg); }
	// friend class ostream;
};

//
// Exception class that tags on strerror
//
class ExceptionErrno : public Exception {
private:
protected:
public:
	ExceptionErrno(const char* _file, unsigned int _line, std::string _mesg);
	virtual ~ExceptionErrno();
};

};

std::ostream& operator<<(std::ostream& lhs, tt::Exception& rhs);

#endif
