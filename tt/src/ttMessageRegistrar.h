#ifndef ttMessageRegistrar_h
#define ttMessageRegistrar_h

#include "ttException.h"
#include "ttMessageFactory.h"
#include <map>

typedef std::map<unsigned int, tt::MessageFactory*> FactoryMap;

namespace tt {

class MessageRegistrar {
private:
	MessageRegistrar() {}
	~MessageRegistrar() {}
	static MessageRegistrar* inst;
	FactoryMap mapper;
protected:
public:
	static MessageRegistrar& instance();
	void add(MessageFactory* factory, unsigned int msgType);
	void remove(MessageFactory* factory, unsigned int msgType);
	void remove(MessageFactory* factory);
	Message* createMessage(MessageBuffer& msgBuf);
};

};

#endif
