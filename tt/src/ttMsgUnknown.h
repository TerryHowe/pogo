#ifndef ttMsgUnknown_h
#define ttMsgUnknown_h

#include "ttMessage.h"


namespace tt {

class MsgUnknown : public Message {
private:
	unsigned int unknownType;
protected:
public:
	static const unsigned int type = 1;
	static const char* name;
	MsgUnknown(unsigned int _unknownType);
	MsgUnknown(MessageBuffer& b);
	virtual ~MsgUnknown() {}
	virtual void marshall(MessageBuffer& b);
	virtual void unmarshall(MessageBuffer& b);
};

};

#endif
