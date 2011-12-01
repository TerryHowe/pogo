// Copyright (C) 1999-2002 Open Source Telecom Corporation.
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
// If you write modifications of your own for Common C++, it is your
// choice whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.


#include "ttAtomicCounter.h"
#include "ttMutexLock.h"


namespace tt {

#ifndef WIN32
#ifdef	HAVE_ATOMIC
AtomicCounter::AtomicCounter()
{
	atomic.counter = 0;
}

AtomicCounter::AtomicCounter(int value)
{
	atomic.counter = value;
}

int AtomicCounter::operator++(void)
{
	atomic_inc(&atomic);
	return atomic_read(&atomic);
}

int AtomicCounter::operator--(void)
{
	int chk = atomic_dec_and_test(&atomic);
	if(chk)
		return 0;
	chk = atomic_read(&atomic);
	if(!chk)
		++chk;
	return chk;
}

int AtomicCounter::operator+=(int change)
{
	atomic_add(change, &atomic);
	return atomic_read(&atomic);
}

int AtomicCounter::operator-=(int change)
{
	atomic_sub(change, &atomic);
	return atomic_read(&atomic);
}

int AtomicCounter::operator+(int change)
{
	return atomic_read(&atomic) + change;
}

int AtomicCounter::operator-(int change)
{
	return atomic_read(&atomic) - change;
}

int AtomicCounter::operator=(int value)
{
	atomic_set(&atomic, value);
	return atomic_read(&atomic);
}

bool AtomicCounter::operator!(void)
{
	int value = atomic_read(&atomic);
	if(value)
		return false;
	return true;
}

AtomicCounter::operator int()
{
	return atomic_read(&atomic);
}

#else // !HAVE_ATOMIC

AtomicCounter::AtomicCounter()
{
	counter = 0;
}

AtomicCounter::AtomicCounter(int value)
{
	counter = value;
}

int AtomicCounter::operator++(void)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		value = ++counter;
	} catch (...) {}
	return value;
}

int AtomicCounter::operator--(void)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		value = --counter;
	} catch (...) {}
	return value;
}

int AtomicCounter::operator+=(int change)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		counter += change;
		value = counter;
	} catch (...) {}
	return value;
}

int AtomicCounter::operator-=(int change)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		counter -= change;
	value = counter;
	} catch (...) {}
	return value;
}

int AtomicCounter::operator+(int change)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		value = counter + change;
	} catch (...) {}
	return value;
}

int AtomicCounter::operator-(int change)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		value = counter - change;
	} catch (...) {}
	return value;
}

AtomicCounter::operator int()
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		value = counter;
	} catch (...) {}
	return value;
}

int AtomicCounter::operator=(int value)
{
	int ret;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		ret = counter;
		counter = value;
	} catch (...) {}
	return ret;
}

bool AtomicCounter::operator!(void)
{
	int value;
	try {
		MutexLock locker(padlock, __FILE__, __LINE__);
		value = counter;
	} catch (...) {}
	if(value)
		return false;
	return true;
}
#endif // HAVE_ATOMIC
#else // WIN32
int AtomicCounter::operator+=(int change)
{
	// FIXME: enhance with InterlockExchangeAdd
	while(--change>=0)
		InterlockedIncrement(&atomic);
	
	return atomic;
}

int AtomicCounter::operator-=(int change)
{
	// FIXME: enhance with InterlockExchangeAdd
	while(--change>=0)
		InterlockedDecrement(&atomic);

	return atomic;
}
#endif // !WIN32

};
