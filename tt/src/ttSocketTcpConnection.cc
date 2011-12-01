#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ttSocketTcpConnection.h"
#include "ttInfraLog.h"

extern int errno;

namespace tt {


SocketTcpConnection::SocketTcpConnection(std::string _myIpAddy, unsigned int _myPort, std::string _theirIpAddy, unsigned int _theirPort, int _fd)
  : SocketTcp(_myIpAddy, _myPort, _fd),
    theirIpAddy(_theirIpAddy),
    theirPort(_theirPort)
{
	
	
}

SocketTcpConnection::~SocketTcpConnection()
{
	
	
}

void
SocketTcpConnection::writeData(const void* data, size_t siz)
{
	
	if (send(sock, data, siz, 0) == -1) {
		std::ostringstream errMsg;
		errMsg << "Error writing data from <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	
}

int
SocketTcpConnection::readData(void* data, size_t siz)
{
	
	ssize_t datalen;
	if ((datalen = recv(sock, data, siz, 0)) == -1) {
		if (errno == EWOULDBLOCK)
			return(0);
		std::ostringstream errMsg;
		errMsg << "Error reading data on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	
	return(datalen);
}

std::string
SocketTcpConnection::getTheirAddress() const
{
	std::ostringstream tmpStr;
	tmpStr << theirIpAddy << ":" << theirPort;
	return(tmpStr.str());
}

};
