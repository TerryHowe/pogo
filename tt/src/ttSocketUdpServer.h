#ifndef ttSocketUdpServer_h
#define ttSocketUdpServer_h

#include "ttSocketUdp.h"
#include "ttException.h"

namespace tt {

class SocketUdpServer : public SocketUdp {
public:
	SocketUdpServer(unsigned int _myPort);
	SocketUdpServer(std::string _myIpAddy, unsigned int _myPort);
	virtual ~SocketUdpServer();
private:
	SocketUdpServer(SocketUdpServer& rhs);
	SocketUdpServer& operator=(SocketUdpServer& rhs);
};

};

#endif
