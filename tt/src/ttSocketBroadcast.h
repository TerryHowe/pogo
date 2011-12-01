#ifndef ttSocketBroadcast_h
#define ttSocketBroadcast_h

#include "ttSocketUdpClient.h"
#include "ttException.h"


namespace tt {

class SocketBroadcast : public SocketUdpClient {
public:
	SocketBroadcast(std::string _serverIpAddy, unsigned int _serverPort);
	virtual ~SocketBroadcast();
private:
	SocketBroadcast(SocketBroadcast& rhs);
	SocketBroadcast& operator=(SocketBroadcast& rhs);
};

};

#endif
