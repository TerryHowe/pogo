#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ttSocketTcpServer.h"
#include "ttInfraLog.h"


namespace tt {


SocketTcpServer::SocketTcpServer(std::string _myIpAddy, unsigned int _myPort, int _backlog)
  : SocketTcp(_myIpAddy, _myPort), backlog(_backlog)
{
	
	bind();
	listen();
	
}

SocketTcpServer::~SocketTcpServer()
{
	
	
}

void
SocketTcpServer::listen()
{
	
	
	if (::listen(sock, backlog) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error listening for <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	
}

SocketTcpConnection*
SocketTcpServer::accept()
{
	
	int fd;
	struct sockaddr clientAddr;
	unsigned int clientAddrSize = sizeof(clientAddr);
	
	if ((fd = ::accept(sock, &clientAddr, &clientAddrSize)) < 0) {
		std::ostringstream errMsg;
		errMsg << "Error accepting for <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	

	//
	// Create connection class
	//
	std::string ipAddy;
	unsigned int port;
	extractAddress(clientAddr, ipAddy, port);
	SocketTcpConnection* newConn = new SocketTcpConnection(getMyIpAddy(), getMyPort(), ipAddy, port, fd);

	
	return(newConn);
}

};
