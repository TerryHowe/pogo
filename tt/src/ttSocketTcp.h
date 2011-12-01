#ifndef ttSocketTcp_h
#define ttSocketTcp_h

#include "ttSocket.h"
#include "ttException.h"


namespace tt {

class SocketTcp : public Socket {
public:
	SocketTcp(std::string _myIpAddy, unsigned int _myPort, int _fd=-1);
	virtual ~SocketTcp();
protected:
private:
	SocketTcp(SocketTcp& rhs);
	SocketTcp& operator=(SocketTcp& rhs);
};

};

#endif
