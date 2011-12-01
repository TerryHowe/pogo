#include <sys/types.h>
#include <sys/socket.h>
#include "ttSocketUdpClient.h"
#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {


SocketUdpClient::SocketUdpClient(std::string _serverIpAddy, unsigned int _serverPort, std::string _myIpAddy, unsigned int _myPort)
  : SocketUdp(_myIpAddy, _myPort), serverIpAddy(_serverIpAddy), serverPort(_serverPort)
{
	
	if (_myPort != 0) {
		bind();
	}

	//
	// Set the send to the group
	//
	createAddress(sockAddr, serverIpAddy, serverPort);
	

	
}

SocketUdpClient::~SocketUdpClient()
{
	
	
}

void
SocketUdpClient::writeData(const void* data, size_t siz)
{
	

	//
	// Write
	//
	
	if (sendto(sock, data, siz, 0, &sockAddr, sizeof(sockAddr)) == -1) {
		std::ostringstream errMsg;
		errMsg << "Error writing data to <" << serverIpAddy << ":" << serverPort << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}

	
}

void
SocketUdpClient::writeData(const void* data, size_t siz, std::string destIp, unsigned int destPort)
{
	SocketUdp::writeData(data, siz, destIp, destPort);
}
};
