#ifndef ttSocketUdpClient_h
#define ttSocketUdpClient_h

#include "ttSocketUdp.h"
#include "ttException.h"


namespace tt {

class SocketUdpClient : public SocketUdp {
public:
        SocketUdpClient(std::string _serverIpAddy = "0.0.0.0", unsigned int _serverPort = 0, std::string _myIpAddy = "", unsigned int _myPort = 0);

	virtual ~SocketUdpClient();
	void writeData(const void* data, size_t siz);
	void writeData(const void* data, size_t siz, std::string destIp, unsigned int destPort);
	void setServerPort(unsigned int val) { serverPort = val; createAddress(sockAddr, serverIpAddy, serverPort); }
	void setServerIpAddy(std::string val) { serverIpAddy = val; createAddress(sockAddr, serverIpAddy, serverPort); }
	unsigned int getServerPort() const { return(serverPort); }
	const std::string& getServerIpAddy() const { return(serverIpAddy); }
private:
	SocketUdpClient(SocketUdpClient& rhs);
	SocketUdpClient& operator=(SocketUdpClient& rhs);
	struct sockaddr sockAddr;
	std::string serverIpAddy;
	unsigned int serverPort;
};

};

#endif
