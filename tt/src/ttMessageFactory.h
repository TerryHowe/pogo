#ifndef ttMessageFactory_h
#define ttMessageFactory_h

#include "ttException.h"
#include "ttMessage.h"
#include "ttMessageBuffer.h"

namespace tt {

class MessageFactory {
private:
protected:
	MessageFactory() {}
	~MessageFactory() {}
public:
	virtual Message* createMessage(MessageBuffer& msgBuf, unsigned int msgType) = 0;
};

};

#endif
