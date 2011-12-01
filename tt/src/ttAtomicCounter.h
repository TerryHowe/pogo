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
 * @file ttAtomicCounter.h
 * @short Atomic counter class
 **/

#ifndef	ttAtomicCounter_h
#define	ttAtomicCounter_h

#include "ttMutex.h"


namespace tt {


/**
 * The AtomicCounter class offers thread-safe manipulation of an integer
 * counter.  These are commonly used for building thread-safe "reference"
 * counters for C++ classes.  The AtomicCounter depends on the platforms
 * support for "atomic" integer operations, and can alternately substitute
 * a "mutex" if no atomic support exists.
 *
 * @author Sean Cavanaugh <sean@dimensionalrift.com>
 * @short atomic counter operation.
 */
class AtomicCounter
{
#ifndef CCXX_USE_WIN32_ATOMIC
private:
#ifdef	HAVE_ATOMIC
	atomic_t atomic;
#else
	int counter;
	Mutex padlock;
#endif

public:
	/**
	 * Initialize an atomic counter to 0.
	 */
	AtomicCounter();

	/**
	 * Initialize an atomic counter to a known value.
	 *
	 * @param value initial value.
	 */
	AtomicCounter(int value);

	int operator++(void);
	int operator--(void);
	int operator+=(int change);
	int operator-=(int change);
	int operator+(int change);
	int operator-(int change);
	int operator=(int value);
	bool operator!(void);
	operator int();
#else
private:
	long atomic;

public:
	inline AtomicCounter()
		{atomic = 0;};

	inline AtomicCounter(int value)
		{atomic = value;};

	inline int operator++(void)
		{return InterlockedIncrement(&atomic);};

	inline int operator--(void)
		{return InterlockedDecrement(&atomic);};

	int operator+=(int change);

	int operator-=(int change);

	inline int operator+(int change)
		{return atomic + change;};

	inline int operator-(int change)
		{return atomic - change;};
	
	inline int operator=(int value)
		{return InterlockedExchange(&atomic, value);};

	inline bool operator!(void)
		{return (atomic == 0) ? true : false;};

	inline operator int()
		{return atomic;};
#endif
};

};

#endif
