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
 * @file ttNumber.h
 * @short Numbers manipulation.
 **/

#ifndef	ttNumber_h
#define	ttNumber_h

namespace tt {

/**
 * A number manipulation class.  This is used to extract, convert,
 * and manage simple numbers that are represented in C ascii strings
 * in a very quick and optimal way.  
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short number manipulation.
 */
class Number
{
protected:
	char *buffer;
	unsigned size;

public:
	/**
	 * Create an instance of a number.
 	 * @param buffer or NULL if created internally.
	 * @param size use - values for zero filled.
	 */
	Number(char *buffer, unsigned size);

	void setValue(long value);
	const char *getBuffer()
		{return buffer;};

	long getValue();

	long operator()()
		{return getValue();};

	operator long()
		{return getValue();};

	operator char*()
		{return buffer;};

	long operator=(long value);
	long operator+=(long value);
	long operator-=(long value);
	long operator--();
	long operator++();
	int operator==(Number &num);
	int operator!=(Number &num);
	int operator<(Number &num);
	int operator<=(Number &num);
	int operator>(Number &num);
	int operator>=(Number &num);

	friend long operator+(Number &num, long val);
	friend long operator+(long val, Number &num);
	friend long operator-(Number &num, long val);
	friend long operator-(long val, Number &num);
};

class ZNumber : public Number
{
public:
	ZNumber(char *buf, unsigned size);
	void setValue(long value);
	long operator=(long value);
};

};

#endif
