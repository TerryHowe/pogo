#include "ttMsgFactory.h"
#include "ttInfraLog.h"
#include "ttMsgUnknown.h"
#include "ttMessageRegistrar.h"

namespace tt {

MsgFactory MsgFactory::factory;

MsgFactory::MsgFactory()
{
	MessageRegistrar::instance().add(this, MsgUnknown::type);
}

MsgFactory::~MsgFactory()
{
	MessageRegistrar::instance().remove(this, MsgUnknown::type);
}

Message*
MsgFactory::createMessage(MessageBuffer& msgBuf, unsigned int msgType)
{
	switch (msgType) {
	case MsgUnknown::type:
		return(new MsgUnknown(msgBuf));
		break;
	}
	return(new MsgUnknown(msgType));
}

};
