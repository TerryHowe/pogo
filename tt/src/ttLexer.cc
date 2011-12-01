
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <ctype.h>
#include "ttLexer.h"

#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {

Lexer::Lexer(const char* buffer)
	: _buffer((buffer==NULL)?"":buffer),
	  _ifs(" 	"),
	  _irs("\012\015"),
	  _tokens("~!@#$%^&*()+`-={}|:\"[]\\;'<>?,./"),
	  _ptr(_buffer.c_str()),
	  _line(1),
	  _linePos(1),
	  _position(0)
{
	
	contextSave();
	
}

Lexer::Lexer(unsigned int siz)
	: _buffer(siz, '\000'),
	  _ifs(" 	"),
	  _irs("\012\015"),
	  _tokens("~!@#$%^&*()+`-={}|:\"[]\\;'<>?,./"),
	  _ptr(_buffer.c_str()),
	  _line(1),
	  _linePos(1),
	  _position(0)
{
	
	contextSave();
	
}

void Lexer::set_Buffer(const char* buffer)
{
	_buffer = buffer;
	reset();
	return;
}

void Lexer::reset()
{
	_ptr = _buffer.c_str();
	_line = 1;
	_linePos = 1;
	_position = 0;
	contextSave();
	return;
}

std::string Lexer::toString()
{
	return _buffer;
}

void Lexer::contextRestore()
{
	_ptr = _rollbackPtr;
	_line = _rollbackLine;
	_linePos = _rollbackLinePos;
	_position = _rollbackPosition;
	return;
}

void Lexer::contextSave()
{
	_rollbackPtr = _ptr;
	_rollbackLine = _line;
	_rollbackLinePos = _linePos;
	_rollbackPosition = _position;
	return;
}

void Lexer::skipIrsIfs()
{
	//
	// Get to the start
	//
	const char* tmpPtr;
	for (; (*_ptr); ++_ptr,++_position, ++_linePos) {

		//
		// Check for IRS
		//
		for (tmpPtr=_irs.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) {
			++_line;
			_linePos = 0;
			continue;
		}

		//
		// Check for IFS
		//
		for (tmpPtr=_ifs.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) continue;
		break;
	}
	return;
}

Lexer& Lexer::operator>>(std::string& rhs) throw (Exception)
{
	

	//
	// Save context
	//
	contextSave();
	

	//
	// Get to the start
	//
	skipIrsIfs();
	unsigned int startPosition;
	startPosition = _position;
	

	//
	// Get to the end
	//
	const char* tmpPtr;
	for (; (*_ptr); ++_ptr,++_position, ++_linePos) {

		//
		// Check for IFS
		//
		for (tmpPtr=_ifs.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) break;

		//
		// Check for IRS
		//
		for (tmpPtr=_irs.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) break;

		//
		// Check for token
		//
		for (tmpPtr=_tokens.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) {
			//
			// First position is token
			//
			
			if (((int)startPosition) >= (((int)_position)-1)) {
				
				++_ptr;
				++_position;
				++_linePos;
			}
			break;
		}
	}
	

	//
	// Get value
	//
	if (startPosition < _position) {
		rhs = _buffer.substr(startPosition,(_position-startPosition));
	} else {
		rhs = "";
	}
	

	
	return *this;
}

Lexer& Lexer::operator>>(const char* rhs) throw (Exception)
{
	std::string tmpStr;
	
	*this >> tmpStr;
	if (tmpStr != rhs) {
		contextRestore();
		std::string eStr("Expected string <");
		eStr += rhs;
		eStr += "> does not match <";
		eStr += tmpStr;
		eStr += "> on line <";
		char tmpBuf[64];
		(void)sprintf(tmpBuf, "%u", _line);
		eStr += tmpBuf;
		eStr += "> position <";
		(void)sprintf(tmpBuf, "%u", _linePos);
		eStr += tmpBuf;
		eStr += ">";
		throw  tt::Exception(__FILE__, __LINE__, eStr);
	}
	
	return *this;
}

Lexer& Lexer::operator>>(const char rhs) throw (Exception)
{
	

	//
	// Save context
	//
	contextSave();
	

	//
	// Get to the start
	//
	skipIrsIfs();
	unsigned int startPosition;
	startPosition = _position;
	

	//
	// See if we have a match
	//
	if (*_ptr != rhs) {
		contextRestore();
		std::string eStr("Expected string <");
		eStr += rhs;
		eStr += "> does not match <";
		eStr += *_ptr;
		eStr += "> on line <";
		char tmpBuf[64];
		(void)sprintf(tmpBuf, "%u", _line);
		eStr += tmpBuf;
		eStr += "> position <";
		(void)sprintf(tmpBuf, "%u", _linePos);
		eStr += tmpBuf;
		eStr += ">";
		throw  tt::Exception(__FILE__, __LINE__, eStr);
	}
	++_ptr;
	++_position;
	++_linePos;

	
	return *this;
}

std::string Lexer::getUntil(const char rhs) throw (Exception)
{
	

	//
	// Save context
	//
	contextSave();
	

	//
	// Get to the start
	//
	skipIrsIfs();
	unsigned int startPosition;
	startPosition = _position;
	

	//
	// See if we have a match
	//
	while ((*_ptr) && (*_ptr != rhs)) {
		//
		// Check for IRS
		//
		const char *tmpPtr;
		for (tmpPtr=_irs.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) {
			++_line;
			_linePos = 0;
		}

		//
		// Check for escaped
		//
		if (*_ptr == '\\') {
			++_ptr;
			++_position;
			++_linePos;
			if (*_ptr == rhs) {
				//
				// Check for IRS
				//
				for (tmpPtr=_irs.c_str(); (*tmpPtr); ++tmpPtr) {
					if (*_ptr == *tmpPtr) break;
				}
				if (*tmpPtr) {
					++_line;
					_linePos = 0;
				}

				++_ptr;
				++_position;
				++_linePos;
			}
		}

		++_ptr;
		++_position;
		++_linePos;
	}

	//
	// Get value
	//
	std::string retStr;
	if (startPosition < _position) {
		retStr = _buffer.substr(startPosition,(_position-startPosition));
	} else {
		retStr = "";
	}
	

	
	return(retStr);
}

bool Lexer::nextToken(const char rhs)
{
	skipIrsIfs();
	contextSave();
	if (*_ptr == rhs)
		return(true);
	return(false);
}

bool Lexer::nextNextToken(const char rhs1, const char rhs2)
{
	skipIrsIfs();
	contextSave();
	if ((*_ptr == rhs1) && (*_ptr) && (*(_ptr+1) == rhs2))
		return(true);
	return(false);
}

Lexer& Lexer::operator>>(short& rhs) throw (Exception)
{
	
	long tmpLong = 0L;
	*this >> tmpLong;
	rhs = static_cast<short>(tmpLong);
	
	return *this;
}

Lexer& Lexer::operator>>(int& rhs) throw (Exception)
{
	
	long tmpLong = 0L;
	*this >> tmpLong;
	
	rhs = static_cast<int>(tmpLong);
	
	
	return *this;
}

Lexer& Lexer::operator>>(long& rhs) throw (Exception)
{
	std::string tmpStr;
	
	*this >> tmpStr;
	
	const char *tmpPtr = tmpStr.data();
	if (*tmpPtr == '-') ++tmpPtr;
	for (; *tmpPtr!='\000'; ++tmpPtr) {
		if (! isdigit(*tmpPtr)) {
			contextRestore();
			std::string eStr("Expected digits <");
			eStr += rhs;
			eStr += "> does not match <";
			eStr += tmpStr;
			eStr += "> on line <";
			char tmpBuf[64];
			(void)sprintf(tmpBuf, "%u", _line);
			eStr += tmpBuf;
			eStr += "> position <";
			(void)sprintf(tmpBuf, "%u", _linePos);
			eStr += tmpBuf;
			eStr += ">";
			throw  tt::Exception(__FILE__, __LINE__, eStr);
		}
	}
	rhs = atol(tmpStr.data());
	
	
	return *this;
}

Lexer& Lexer::operator>>(unsigned short& rhs) throw (Exception)
{
	
	unsigned long tmpLong = 0L;
	*this >> tmpLong;
	rhs = static_cast<unsigned short>(tmpLong);
	
	return *this;
}

Lexer& Lexer::operator>>(unsigned int& rhs) throw (Exception)
{
	
	unsigned long tmpLong = 0L;
	*this >> tmpLong;
	rhs = static_cast<unsigned int>(tmpLong);
	
	return *this;
}

Lexer& Lexer::operator>>(unsigned long& rhs) throw (Exception)
{
	std::string tmpStr("");
	
	*this >> tmpStr;
	const char *tmpPtr = tmpStr.data();
	if (*tmpPtr == '-') ++tmpPtr;
	do {
		if (! isdigit(*tmpPtr)) {
			contextRestore();
			std::string eStr("Expected digits instead of <");
			eStr += tmpStr;
			eStr += "> on line <";
			char tmpBuf[64];
			(void)sprintf(tmpBuf, "%u", _line);
			eStr += tmpBuf;
			eStr += "> position <";
			(void)sprintf(tmpBuf, "%u", _linePos);
			eStr += tmpBuf;
			eStr += ">";
			throw  tt::Exception(__FILE__, __LINE__, eStr);
		}
		if (*tmpPtr != '\000') ++tmpPtr;
	} while (*tmpPtr != '\000');
	rhs = static_cast<unsigned long>(atol(tmpStr.data()));
	
	return *this;
}

void Lexer::readData(std::string& data) throw (Exception)
{
	//
	// Read the size
	//
	int tmpLen = 0;
	*this >> tmpLen;
	++_ptr;++_position;++_linePos; // Skip IRS
	

	//
	// Read the data
	//
	const char* tmpPtr;
	for (; ((*_ptr) && (tmpLen>0)); ++_ptr,++_position, ++_linePos) {

		//
		// Check for IRS
		//
		for (tmpPtr=_irs.c_str(); (*tmpPtr); ++tmpPtr) {
			if (*_ptr == *tmpPtr) break;
		}
		if (*tmpPtr) {
			++_line;
			_linePos = 0;
		}

		data += *_ptr;
		--tmpLen;
	}
	return;
}

void Lexer::writeData(const std::string& data, int len) throw (Exception)
{
	//
	// Write number
	//
	 char tmpBuf[128];
        (void)sprintf(tmpBuf, "%d", (int)data.length());
        _buffer += tmpBuf;
	_buffer += *_irs.data();

	//
	// Write data
	//
	const char *ptr = data.data();
	while (len > 0) {
		_buffer += *ptr;
		++ptr;
		--len;
	}
	return;
}

};
