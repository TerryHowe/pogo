#ifndef ttLexer_h
#define ttLexer_h

#include <string>
#include "ttException.h"

namespace tt {

class Lexer {
private:
protected:
	std::string _buffer;
	std::string _ifs;
	std::string _irs;
	std::string _tokens;
	const char* _ptr;
	unsigned int _line;
	unsigned int _linePos;
	unsigned long _position;
	const char* _rollbackPtr;
	unsigned int _rollbackLine;
	unsigned int _rollbackLinePos;
	unsigned long _rollbackPosition;
	void contextRestore();
	void contextSave();
	void skipIrsIfs();
	void set_Buffer(const char* buffer);
protected:
	void readData(std::string& data) throw (Exception);
	void writeData(const std::string& data, int len) throw (Exception);
public:
	Lexer(const char* buffer=NULL);
	Lexer(unsigned int siz);
	std::string toString();
	const std::string& getBuffer() { return(_buffer); }
	int getSize() { return(_buffer.size()); }
	void setTokens(std::string val) { _tokens = val; }
	const std::string& getTokens() { return(_tokens); }
	bool nextToken(const char rhs);
	bool nextNextToken(const char rhs1, const char rhs2);
	std::string getUntil(const char rhs) throw(Exception);
	bool eos() { return (*_ptr == '\000'); }
	Lexer& operator>>(std::string& rhs) throw(Exception);
	Lexer& operator>>(const char* rhs) throw(Exception);
	Lexer& operator>>(const char rhs) throw(Exception);
	Lexer& operator>>(short& rhs) throw (Exception);
	Lexer& operator>>(int& rhs) throw (Exception);
	Lexer& operator>>(long& rhs) throw (Exception);
	Lexer& operator>>(unsigned short& rhs) throw (Exception);
	Lexer& operator>>(unsigned int& rhs) throw (Exception);
	Lexer& operator>>(unsigned long& rhs) throw (Exception);
	void reset();
};

};
#endif
