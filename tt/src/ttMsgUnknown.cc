#include "ttMsgUnknown.h"
#include "ttInfraLog.h"


namespace tt {

const char* MsgUnknown::name = "MsgUnknown";

MsgUnknown::MsgUnknown(unsigned int _unknownType)
  : Message(MsgUnknown::type, MsgUnknown::name), unknownType(_unknownType)
{
	
	
}

MsgUnknown::MsgUnknown(MessageBuffer& b)
  : Message(MsgUnknown::type, MsgUnknown::name), unknownType(0)
{
	
	unmarshall(b);
	
}

void
MsgUnknown::marshall(MessageBuffer& b)
{
   
   Message::marshall(b);
   b << unknownType;
   
   return;
}

void
MsgUnknown::unmarshall(MessageBuffer& b)
{
   
   Message::unmarshall(b);
   b >> unknownType;
   
   return;
}

};
