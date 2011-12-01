#include <sys/types.h>
#include <sys/socket.h>
#include "ttSocketTcpClient.h"
#include "ttInfraLog.h"


namespace tt {


SocketTcpClient::SocketTcpClient(std::string _myIpAddy, unsigned int _myPort, std::string _serverIpAddy, unsigned int _serverPort)
  : SocketTcpConnection(_myIpAddy, _myPort, _serverIpAddy, _serverPort)
{
	
	connect();
	
}

SocketTcpClient::~SocketTcpClient()
{
	
	
}

void
SocketTcpClient::connect()
{
	
	struct sockaddr sockAddr;
	createAddress(sockAddr, theirIpAddy, theirPort);
	
	if (::connect(sock, &sockAddr, sizeof(sockAddr)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error connecting for <" << getTheirAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	
}

};
