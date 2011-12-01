#ifndef ttSocketUdp_h
#define ttSocketUdp_h

#include "ttSocket.h"
#include "ttException.h"


namespace tt {

class SocketUdp : public Socket {
public:
	SocketUdp(std::string _myIpAddy, unsigned int _myPort);
	virtual ~SocketUdp();
	void writeData(const void* data, size_t siz, std::string destIp, unsigned int destPort);
	int readData(void* data, size_t siz, std::string& srcIp, unsigned int& srcPort);
	int readData(void* data, size_t siz);
private:
	SocketUdp(SocketUdp& rhs);
	SocketUdp& operator=(SocketUdp& rhs);
};

};

#endif
