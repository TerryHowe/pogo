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
 * @file ttDate.h
 * @short Date manipulation.
 **/

#ifndef	ttDate_h
#define	ttDate_h

#include <ctime>

namespace tt {

/**
 * The Date class uses a julian date representation of the current
 * year, month, and day.  This is then manipulated in several forms
 * and may be exported as needed.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short julian number based date class.
 */
class Date
{
protected:
	long julian;

protected:
	void toJulian(long year, long month, long day);
	void fromJulian(char *buf) const;

	/**
	 * A method to use to "post" any changed values when shadowing
	 * a mixed object class.  This is used by DateNumber.
	 */
	virtual void update(void)
		{return;};

public:

	Date(time_t tm);
	Date(tm *dt);
	Date(char *str, size_t size = 0);
	Date(int year, unsigned month, unsigned day);
	Date();

	int getYear(void) const;
	unsigned getMonth(void) const;
	unsigned getDay(void) const;
	unsigned getDayOfWeek(void) const;
	char *getDate(char *buffer) const;
	time_t getDate(void) const;
	time_t getDate(tm *buf) const;
	long getValue(void) const;
	void setDate(const char *str, size_t size = 0);
	bool isValid(void) const;

	friend Date operator+(Date &date, long val);
	friend Date operator-(Date &date, long val);
	friend Date operator+(long val, Date &date);
	friend Date operator-(long val, Date &date);

	operator long() const
		{return getValue();};

	std::string operator()() const;
	Date& operator++();
	Date& operator--();
	Date& operator+=(long val);
	Date& operator-=(long val);
	int operator==(Date &date);
	int operator!=(Date &date);
	int operator<(Date &date);
	int operator<=(Date &date);
	int operator>(Date &date);
	int operator>=(Date &date);
	bool operator!() const
		{return !isValid();};
};

};

#endif
