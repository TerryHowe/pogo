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
// The exception is that, if you link the Common C++ library with other
// files to produce an executable, this does not by itself cause the
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

/**
 * @file ttTime.h
 * @short Time manipulation.
 **/

#ifndef	ttTime_h
#define	ttTime_h

#include <ctime>

namespace tt {

/**
 * The Time class uses a integer representation of the current
 * time.  This is then manipulated in several forms
 * and may be exported as needed.
 *
 * @author Marcelo Dalmas <mad@brasmap.com.br>
 * @short Integer based time class.
 */

class Time
{
protected:
	long seconds;

protected:
	void toSeconds(int hour, int minute, int second);
	void fromSeconds(char *buf) const;
	virtual void Update(void)
		{return;};

public:
	Time(time_t tm);
	Time(tm *dt);
	Time(char *str, size_t size = 0);
	Time(int hour, int minute, int second);	
	Time();

	long getValue(void) const;
	int getHour(void) const;
	int getMinute(void) const;
	int getSecond(void) const;
	char *getTime(char *buffer) const;
	time_t getTime(void) const;
	tm *getTime(tm *buf) const;
	void setTime(char *str, size_t size = 0);
	bool isValid(void) const;

	friend Time operator+(Time &time1, Time &time2);
	friend Time operator-(Time &time1, Time &time2);
	friend Time operator+(Time &time, int val);
	friend Time operator-(Time &time, int val);
	friend Time operator+(int val, Time &time);
	friend Time operator-(int val, Time &time);

	operator long()
		{return getValue();};

	std::string operator()() const;
	Time& operator++();
	Time& operator--();
	Time& operator+=(int val);
	Time& operator-=(int val);
	int operator==(Time &time);
	int operator!=(Time &time);
	int operator<(Time &time);
	int operator<=(Time &time);
	int operator>(Time &time);
	int operator>=(Time &time);
	bool operator!()
		{return !isValid();};
};

};

#endif
