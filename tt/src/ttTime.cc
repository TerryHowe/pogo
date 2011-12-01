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
#include <memory.h>
#include "ttTime.h"
#include "ttNumber.h"
#include "ttException.h"

namespace tt {
using namespace std;

Time::Time()
{
	time_t now;
	struct tm *dt;
	time(&now);
	dt = localtime(&now);

	toSeconds(dt->tm_hour, dt->tm_min, dt->tm_sec);
}

Time::Time(struct tm *dt)
{
	toSeconds(dt->tm_hour, dt->tm_min, dt->tm_sec);
}

Time::Time(time_t tm)
{
	struct tm *dt = localtime(&tm);
	toSeconds(dt->tm_hour, dt->tm_min, dt->tm_sec);
}

Time::Time(char *str, size_t size)
{
	setTime(str, size);
}

Time::Time(int hour, int minute, int second)
{
	toSeconds(hour, minute, second);
}

bool Time::isValid(void) const
{
	if(seconds == -1)
		return false;
	return true;
}

char *Time::getTime(char *buf) const
{
	fromSeconds(buf);
	return buf;
}

time_t Time::getTime(void) const
{
	char buf[7];
	struct tm dt;
	memset(&dt, 0, sizeof(dt));
	fromSeconds(buf);
	ZNumber nhour(buf, 2);
	ZNumber nminute(buf + 2, 2);
	ZNumber nsecond(buf + 4, 2);
	
	dt.tm_hour = nhour();
	dt.tm_min = nminute();
	dt.tm_sec = nsecond();
	return mktime(&dt);
}

int Time::getHour(void) const
{
	char buf[7];
	fromSeconds(buf);
	ZNumber num(buf, 2);
	return num();
}

int Time::getMinute(void) const
{
	char buf[7];
	fromSeconds(buf);
	ZNumber num(buf + 2, 2);
	return num();
}

int Time::getSecond(void) const
{
	char buf[7];
	fromSeconds(buf);
	ZNumber num(buf + 4, 2);
	return num();
}

void Time::setTime(char *str, size_t size)
{
	int tmpSeconds = 00;

	if(!size)
		size = strlen(str);

//00:00
	if (size == 5)
	{
		tmpSeconds = 00;
	}
//00:00:00	
	else if (size == 8)
	{
		ZNumber nsecond(str + 6, 2);
		tmpSeconds = nsecond();
	}
	else
	{
		throw TTEXCEPTIONERR("Invalid time.");
	}	    	

	ZNumber nhour(str, 2);
	ZNumber nminute(str+3, 2);
	toSeconds(nhour(), nminute(), tmpSeconds);
}

string Time::operator()() const
{
	char buf[7];

	fromSeconds(buf);
	string time(buf);

	return time;
}

long Time::getValue(void) const
{
	return seconds;
}

Time& Time::operator++()
{
	++seconds;
	Update();
	return *this;
}

Time& Time::operator--()
{
	--seconds;
	Update();
	return *this;
}

Time& Time::operator+=(int val)
{
	seconds += val;
	Update();
	return *this;
}

Time& Time::operator-=(int val)
{
	seconds -= val;
	Update();
	return *this;
}

int Time::operator==(Time &t)
{
	return seconds == t.seconds;
}

int Time::operator!=(Time &t)
{
	return seconds != t.seconds;
}

int Time::operator<(Time &t)
{
	return seconds < t.seconds;
}

int Time::operator<=(Time &t)
{
	return seconds <= t.seconds;
}

int Time::operator>(Time &t)
{
	return seconds > t.seconds;
}

int Time::operator>=(Time &t)
{
	return seconds >= t.seconds;
}

void Time::toSeconds(int hour, int minute, int second)
{
	seconds = -1;
	
	if (hour > 23) {
		throw TTEXCEPTIONERR("Invalid hour.");
	}
	if (minute > 59) {
		throw TTEXCEPTIONERR("Invalid minute.");
	}
	if (second > 59) {
		throw TTEXCEPTIONERR("Invalid second.");
	}

	seconds = 3600 * hour + 60 * minute + second;
}

void Time::fromSeconds(char *buffer) const
{
	ZNumber hour(buffer, 2);
	ZNumber minute(buffer + 2, 2);
	ZNumber second(buffer + 4, 2);

	hour = seconds / 3600;
	minute = (seconds - (3600 * hour())) / 60;
	second = seconds - (3600 * hour()) - (60 * minute());
	buffer[6] = '\0';
}

Time operator+(Time &time1, Time &time2)
{
	Time t;
	t.seconds = time1.seconds + time2.seconds;
	t.Update();
	return t;
}

Time operator-(Time &time1, Time &time2)
{
	Time t;
	t.seconds = time1.seconds - time2.seconds;
	t.Update();
	return t;
}

Time operator+(Time &time, int val)
{
	Time t = time;
	t.seconds += val;
	t.Update();
	return t;
}

Time operator+(int val, Time &time)
{
	Time t = time;
	t.seconds += val;
	t.Update();
	return t;
}

Time operator-(Time &time, int val)
{
	Time t = time;
	t.seconds -= val;
	t.Update();
	return t;
}

Time operator-(int val, Time &time)
{
	Time t = time;
	t.seconds -= val;
	t.Update();
	return t;
}

};
