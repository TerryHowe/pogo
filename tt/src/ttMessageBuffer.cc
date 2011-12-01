#include <stdio.h>
#include "ttMessageBuffer.h"
#define TT_NODEBUG 1
#include "ttInfraLog.h"

namespace tt {

MessageBuffer::MessageBuffer(unsigned int siz)
  : Lexer(siz)
{
	
	
}

MessageBuffer&
MessageBuffer::operator<<(short val)
{
	char tmpBuf[128];
	(void)sprintf(tmpBuf, "%d", (int)val);
	_buffer += tmpBuf;
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator<<(int val)
{
	char tmpBuf[128];
	(void)sprintf(tmpBuf, "%d", val);
	_buffer += tmpBuf;
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator<<(long val)
{
	char tmpBuf[128];
	(void)sprintf(tmpBuf, "%ld", val);
	_buffer += tmpBuf;
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator<<(unsigned short val)
{
	char tmpBuf[128];
	(void)sprintf(tmpBuf, "%u", (unsigned int)val);
	_buffer += tmpBuf;
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator<<(unsigned int val)
{
	char tmpBuf[128];
	(void)sprintf(tmpBuf, "%u", val);
	_buffer += tmpBuf;
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator<<(unsigned long val)
{
	char tmpBuf[128];
	(void)sprintf(tmpBuf, "%lu", val);
	_buffer += tmpBuf;
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator<<(const std::string& val) throw (Exception)
{
	writeData(val, val.length());
	_buffer += *_irs.data();
	return(*this);
}

MessageBuffer&
MessageBuffer::operator>>(std::string& val) throw (Exception)
{
	readData(val);
	
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(const char* rhs) throw(Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(const char rhs) throw(Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(short& rhs) throw (Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(int& rhs) throw (Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(long& rhs) throw (Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(unsigned short& rhs) throw (Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(unsigned int& rhs) throw (Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}
MessageBuffer&
MessageBuffer::operator>>(unsigned long& rhs) throw (Exception)
{
	Lexer::operator>>(rhs);
	return(*this);
}


MessageBuffer&
MessageBuffer::operator<<(const char* val)
{
	_buffer += val;
	_buffer += *_irs.data();
	return(*this);
}

};
