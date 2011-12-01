#include <sstream>
#include "ttMessageRegistrar.h"
#include "ttMsgUnknown.h"
#define TT_NODEBUG 1
#include "ttInfraLog.h"

namespace tt {

MessageRegistrar* MessageRegistrar::inst = NULL;

MessageRegistrar&
MessageRegistrar::instance()
{
	// TODO Mutex
	if (inst == NULL) inst = new MessageRegistrar;
	return(*inst);
}

void
MessageRegistrar::add(MessageFactory* factory, unsigned int msgType)
{
	
	mapper.insert(std::make_pair(msgType, factory));
	
	return;
}

void
MessageRegistrar::remove(MessageFactory* factory, unsigned int msgType)
{
	
	FactoryMap::iterator itr;
	itr = mapper.find(msgType);
	if (itr == mapper.end()) {
		std::ostringstream errMsg;
		errMsg << "Factory not found for message type <" << msgType << ">";
		TtLogWarn(errMsg.str());
		return;
	}
	mapper.erase(itr);
	
	return;
}

Message*
MessageRegistrar::createMessage(MessageBuffer& msgBuf)
{
	
	unsigned int msgType = 0;
	msgBuf >> msgType;
	FactoryMap::iterator itr;
	itr = mapper.find(msgType);
	if (itr == mapper.end()) {
		
		return new MsgUnknown(msgType);
	}
	
	MessageFactory* factory = itr->second;
	Message* msg = factory->createMessage(msgBuf, msgType);
	
	return(msg);
}

};
