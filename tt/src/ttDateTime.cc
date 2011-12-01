// Copyright (C) 1999-2003 Open Source Telecom Corporation.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// As a special exception to the GNU General Public License, permission is
// granted for additional uses of the text contained in its release
// of Common C++.
//
// The exception is that, if you link the Common C++ library with other files
// to produce an executable, this does not by itself cause the
// resulting executable to be covered by the GNU General Public License.
// Your use of that executable is in no way restricted on account of
// linking the Common C++ library code into it.
//
// This exception does not however invalidate any other reasons why
// the executable file might be covered by the GNU General Public License.
//
// This exception applies only to the code released under the
// name Common C++.  If you copy code from other releases into a copy of
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
//
// If you write modifications of your own for Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.

#include <iostream>
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include <memory.h>
#include "ttDateTime.h"
#include "ttNumber.h"
#include "ttException.h"

namespace tt {
using namespace std;

Datetime::Datetime(time_t tm)
{
	struct tm *dt = localtime(&tm);
	toJulian(dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday);
	toSeconds(dt->tm_hour, dt->tm_min, dt->tm_sec);
}

Datetime::Datetime(tm *dt) :
Date(dt), Time(dt)
{}

Datetime::Datetime(char *str, size_t size)
{
	char *timestr;

	if (!size)
		size = strlen(str);

// 00/00 00:00
	if (size ==  11)
	{
		timestr = str + 6;
		setDate(str, 5);
		setTime(timestr, 5);
	}
// 00/00/00 00:00
	else if (size == 14)
	{
		timestr = str + 9;
		setDate(str, 8);
		setTime(timestr,5);
	}
// 00/00/00 00:00:00
	else if (size == 17)
	{
		timestr = str + 9;
		setDate(str, 8);
		setTime(timestr,8);
	}
// 0000/00/00 00:00:00
	else if (size == 19)
	{
		timestr = str + 11;
		setDate(str, 10);
		setTime(timestr,8);
	}
	else
	{
		throw TTEXCEPTIONERR("Invalid time.");
	}	    	
}

Datetime::Datetime(int year, unsigned month, unsigned day,
		   int hour, int minute, int second) :
  Date(year, month, day), Time(hour, minute, second)
{}

Datetime::Datetime() : Date(), Time()
{
	time_t now;
	struct tm *dt;
	time(&now);
	dt = localtime(&now);

	toSeconds(dt->tm_hour, dt->tm_min, dt->tm_sec);
	toJulian(dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday);
}

bool Datetime::isValid(void) const
{
	return Date::isValid() && Time::isValid();
}

char *Datetime::getDatetime(char *buf) const
{
	fromJulian(buf);
	fromSeconds(buf+8);
	return buf;
}

time_t Datetime::getDatetime(void) const
{
	char buf[9];
	struct tm dt;
	memset(&dt, 0, sizeof(dt));

	fromJulian(buf);
	ZNumber nyear(buf, 4);
	ZNumber nmonth(buf + 4, 2);
	ZNumber nday(buf + 6, 2);
	dt.tm_year = nyear() - 1900;
	dt.tm_mon = nmonth() - 1;
	dt.tm_mday = nday();

	fromSeconds(buf);
	ZNumber nhour(buf, 2);
	ZNumber nminute(buf + 2, 2);
	ZNumber nsecond(buf + 4, 2);
	dt.tm_hour = nhour();
	dt.tm_min = nminute();
	dt.tm_sec = nsecond();
	dt.tm_isdst = -1;
	
	return mktime(&dt);
}

Datetime& Datetime::operator=(Datetime datetime)
{
	julian = datetime.julian;
	seconds = datetime.seconds;
	
	return *this;	
}

Datetime& Datetime::operator+=(Datetime &datetime)
{
	seconds += datetime.seconds;
	julian += datetime.julian; 
	Update();
	return *this;
}

Datetime& Datetime::operator-=(Datetime &datetime)
{
	seconds -= datetime.seconds;
	julian -= datetime.julian; 
	Update();
	return *this;
}

Datetime& Datetime::operator+=(Time &time)
{
	seconds += time.getValue();
	Update();
	return *this;
}

Datetime& Datetime::operator-=(Time &time)
{
	seconds -= time.getValue();
	Update();
	return *this;
}


int Datetime::operator==(Datetime &d)
{
	return (julian == d.julian) && (seconds == d.seconds);
}

int Datetime::operator!=(Datetime &d)
{
	return (julian != d.julian) || (seconds != d.seconds);
}

int Datetime::operator<(Datetime &d)
{
	if (julian != d.julian) {
		return (julian < d.julian);
	}
	else {
		return (seconds < d.seconds);
	}
}

int Datetime::operator<=(Datetime &d)
{
	if (julian != d.julian) {
		return (julian < d.julian);
	}
	else {
		return (seconds <= d.seconds);
	}
}

int Datetime::operator>(Datetime &d)
{
	if (julian != d.julian) {
		return (julian > d.julian);
	}
	else {
		return (seconds > d.seconds);
	}
}

int Datetime::operator>=(Datetime &d)
{
	if (julian != d.julian) {
		return (julian > d.julian);
	}
	else {
		return (seconds >= d.seconds);
	}
}

bool Datetime::operator!()
{
	return !(Date::isValid() && Time::isValid());
}


string Datetime::strftime(const char *format) const
{
	char buffer[64];
	int last;
	time_t t;
	tm *tbp;
	string retval;

	t = getDatetime();
        tbp = localtime(&t);
#ifdef	WIN32
	last = ::strftime(buffer, 64, format, tbp);
#else
	last = std::strftime(buffer, 64, format, tbp);
#endif

	buffer[last] = '\0';
	retval = buffer;
	return retval;

}

};
