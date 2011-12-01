#ifndef ttMessage_h
#define ttMessage_h

#include "ttMessageBuffer.h"


namespace tt {

class Message {
private:
	unsigned int msgType;
	const char* msgName;
protected:
	void setMsgType(unsigned int _msgType) { msgType = _msgType; }
public:
	Message(unsigned int _msgType, const char* _msgName);
	virtual ~Message() {}
	virtual void marshall(MessageBuffer& b);
	virtual void unmarshall(MessageBuffer& b);
	std::string toString() { MessageBuffer b(1024)/*TODO config*/; marshall(b); return(b.toString()); }
	unsigned int getMsgType() { return(msgType); }
	const char* getMsgName() { return(msgName); }
};

};

#endif
