#ifndef ttMessageBuffer_h
#define ttMessageBuffer_h

#include <sstream>
#include <string>
#include "ttLexer.h"


namespace tt {

class MessageBuffer : public Lexer {
private:
protected:
public:
	MessageBuffer(unsigned int siz);
	~MessageBuffer() {}
	MessageBuffer& operator<<(short val);
	MessageBuffer& operator<<(int val);
	MessageBuffer& operator<<(long val);
	MessageBuffer& operator<<(unsigned short val);
	MessageBuffer& operator<<(unsigned int val);
	MessageBuffer& operator<<(unsigned long val);
	MessageBuffer& operator<<(const std::string& val) throw (Exception);
	MessageBuffer& operator<<(const char* val);

	MessageBuffer& operator>>(std::string& val) throw (Exception);
        MessageBuffer& operator>>(const char* rhs) throw(Exception);
        MessageBuffer& operator>>(const char rhs) throw(Exception);
        MessageBuffer& operator>>(short& rhs) throw (Exception);
        MessageBuffer& operator>>(int& rhs) throw (Exception);
        MessageBuffer& operator>>(long& rhs) throw (Exception);
        MessageBuffer& operator>>(unsigned short& rhs) throw (Exception);
        MessageBuffer& operator>>(unsigned int& rhs) throw (Exception);
        MessageBuffer& operator>>(unsigned long& rhs) throw (Exception);
};

};

#endif
