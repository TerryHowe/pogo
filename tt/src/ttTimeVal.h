#ifndef ttTimeVal_h
#define ttTimeVal_h

#include "ttException.h"
#include <sys/time.h>


namespace tt {

class TimeVal : public timeval {
public:
	TimeVal();
	TimeVal(int sec, int usec = 0);
	TimeVal(const TimeVal& rhs);
	virtual ~TimeVal();
	TimeVal& operator=(const TimeVal& rhs);
	TimeVal& operator+=(const TimeVal& rhs);
	TimeVal& operator-=(const TimeVal& rhs);
	TimeVal operator+(const TimeVal& rhs) const;
	TimeVal operator-(const TimeVal& rhs) const;
	bool operator<(const TimeVal& rhs) const;
	bool operator>(const TimeVal& rhs) const;
	bool operator==(const TimeVal& rhs) const;
	bool operator<=(const TimeVal& rhs) const;
	bool operator>=(const TimeVal& rhs) const;
	std::string toString() const;
	void setCurrentTime();
private:
};

};

#endif
