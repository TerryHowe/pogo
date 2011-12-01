#include <errno.h>
#include <sys/time.h>
#include <sstream>
#include "ttTimeVal.h"

#define TT_NODEBUG 1
#include "ttInfraLog.h"

#ifndef MICROSEC
#define MICROSEC 1000000
#endif

extern int errno;

namespace tt {


TimeVal::TimeVal()
{
	
	if (gettimeofday(this, NULL) == -1) {
		TtLogErr("Error in call of gettimeofday(3C) errno=" << errno);
		tv_sec = 0;
		tv_usec = 0;
	}
	
}

TimeVal::TimeVal(int sec, int usec)
{
	
	tv_sec = sec;
	tv_usec = usec;
	
}

TimeVal::TimeVal(const TimeVal& rhs)
{
	
	*this = rhs;
	
}

TimeVal::~TimeVal()
{
	
	
}

TimeVal&
TimeVal::operator=(const TimeVal& rhs)
{
	
	tv_sec = rhs.tv_sec;
	tv_usec = rhs.tv_usec;
	
	return(*this);
}

TimeVal&
TimeVal::operator+=(const TimeVal& rhs)
{
	
	tv_sec += rhs.tv_sec;
	tv_usec += rhs.tv_usec;
	if (tv_usec > MICROSEC) {
		++tv_sec;
		tv_usec -= MICROSEC;
	}
	
	return(*this);
}

TimeVal&
TimeVal::operator-=(const TimeVal& rhs)
{
	
	tv_sec -= rhs.tv_sec;
	tv_usec -= rhs.tv_usec;
	if (tv_usec < 0) {
		--tv_sec;
		tv_usec += MICROSEC;
	}
	
	return(*this);
}

TimeVal
TimeVal::operator+(const TimeVal& rhs) const
{
	
	TimeVal tmpVal(*this);
	tmpVal += rhs;
	
	return(tmpVal);
}

TimeVal
TimeVal::operator-(const TimeVal& rhs) const
{
	
	TimeVal tmpVal(*this);
	tmpVal -= rhs;
	
	return(tmpVal);
}

bool
TimeVal::operator<(const TimeVal& rhs) const
{
	
	if ((tv_sec < rhs.tv_sec) ||
	    ((tv_sec == rhs.tv_sec) && (tv_usec < rhs.tv_usec))) {
		return(true);
	}
	
	return(false);
}

bool
TimeVal::operator>(const TimeVal& rhs) const
{
	
	if ((tv_sec > rhs.tv_sec) ||
	    ((tv_sec == rhs.tv_sec) && (tv_usec > rhs.tv_usec))) {
		return(true);
	}
	
	return(false);
}

bool
TimeVal::operator==(const TimeVal& rhs) const
{
	
	if ((tv_sec == rhs.tv_sec) && (tv_usec == rhs.tv_usec)) {
		return(true);
	}
	
	return(false);
}

bool
TimeVal::operator<=(const TimeVal& rhs) const
{
	
	if (*this < rhs) return(true);
	if (*this == rhs) return(true);
	
	return(false);
}

bool
TimeVal::operator>=(const TimeVal& rhs) const
{
	
	if (*this > rhs) return(true);
	if (*this == rhs) return(true);
	
	return(false);
}

std::string
TimeVal::toString() const
{
	
	std::ostringstream tmpStr;
	tmpStr << "sec=" << tv_sec << " usec=" << tv_usec;
	
	return(tmpStr.str());
}

void
TimeVal::setCurrentTime()
{
	
	if (gettimeofday(this, NULL) == -1) {
		TtLogErr("Error in call of gettimeofday(3C) errno=" << errno);
		tv_sec = 0;
		tv_usec = 0;
	}
	
}

};
