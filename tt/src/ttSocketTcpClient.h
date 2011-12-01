#ifndef ttSocketTcpClient_h
#define ttSocketTcpClient_h

#include "ttSocketTcpConnection.h"
#include "ttException.h"


namespace tt {

class SocketTcpClient : public SocketTcpConnection {
public:
	SocketTcpClient(std::string _myIpAddy, unsigned int _myPort, std::string _serverIpAddy, unsigned int _serverPort);
	virtual ~SocketTcpClient();
protected:
	void connect();
private:
	SocketTcpClient(SocketTcpClient& rhs);
	SocketTcpClient& operator=(SocketTcpClient& rhs);
};

};

#endif
