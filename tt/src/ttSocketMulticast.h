#ifndef ttSocketMulticast_h
#define ttSocketMulticast_h

#include "ttSocketUdpClient.h"
#include "ttException.h"


namespace tt {

class SocketMulticast : public SocketUdpClient {
public:
	SocketMulticast(std::string _serverIpAddy, unsigned int _serverPort);
	virtual ~SocketMulticast();
private:
	SocketMulticast(SocketMulticast& rhs);
	SocketMulticast& operator=(SocketMulticast& rhs);
};

};

#endif
