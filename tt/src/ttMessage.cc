#include "ttMessage.h"
#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {

Message::Message(unsigned int _msgType, const char* _msgName)
  : msgType(_msgType), msgName(_msgName)
{
	
	
}

void
Message::marshall(MessageBuffer& b)
{
   
   b << msgType;
   
   return;
}

void
Message::unmarshall(MessageBuffer& b)
{
   
   // handled by factory b >> msgType;
   
   return;
}

};
