#ifndef ttMsgFactory_h
#define ttMsgFactory_h

#include "ttException.h"
#include "ttMessageFactory.h"

namespace tt {

class MsgFactory : public MessageFactory {
private:
	static MsgFactory factory;
protected:
public:
	MsgFactory();
	~MsgFactory();
	virtual Message* createMessage(MessageBuffer& msgBuf, unsigned int msgType);
};

};

#endif
