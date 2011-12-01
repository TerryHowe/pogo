#include <stdio.h>
#include <iostream>
#include "ttException.h"
#include <errno.h>
#include <string.h>
extern int errno;


namespace tt {


Exception::Exception(const char* _file, unsigned int _line, std::string _mesg) :
  file(_file), line(_line), mesg(_mesg)
{
}

Exception::~Exception()
{
}

std::string
Exception::toString()
{
	std::string tmpstr = file;
	tmpstr += ":";
	char buf[128];
	(void)sprintf(buf, "%u", line);
	tmpstr += buf;
	tmpstr += " ";
	tmpstr += mesg;
	return(tmpstr);
}

ExceptionErrno::ExceptionErrno(const char* _file, unsigned int _line, std::string _mesg) :
   Exception(_file, _line, _mesg)
{
	mesg += " error string <";
	mesg += strerror(errno);
	mesg += ">";
}

ExceptionErrno::~ExceptionErrno()
{
}

};

std::ostream& operator<<(std::ostream& lhs, tt::Exception& rhs) {
	lhs << rhs.getFile() << ":" << rhs.getLine() << " " << rhs.getMesg();
	return(lhs);
}
