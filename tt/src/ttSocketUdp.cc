#include <errno.h>
#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ttSocketUdp.h"

extern int errno;

namespace tt {


SocketUdp::SocketUdp(std::string _myIpAddy, unsigned int _myPort)
  : Socket(_myIpAddy, _myPort, AF_INET, SOCK_DGRAM)
{
}

SocketUdp::~SocketUdp()
{
}

void
SocketUdp::writeData(const void* data, size_t siz, std::string destIp, unsigned int destPort)
{
	struct sockaddr sockAddr;
	createAddress(sockAddr, destIp, destPort);

	if (sendto(sock, data, siz, 0, &sockAddr, sizeof(sockAddr)) == -1) {
		std::ostringstream errMsg;
		errMsg << "Error writing data from <" << getMyAddress() << "> to <" << destIp << ":" << destPort << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
}

int
SocketUdp::readData(void* data, size_t siz, std::string& srcIp, unsigned int& srcPort)
{
	struct sockaddr from;
	int fromlen = sizeof(from);
	ssize_t datalen;
	(void)memset(&from, 0, fromlen);
	if ((datalen = recvfrom(sock, data, siz, 0, &from, (socklen_t*)&fromlen)) == -1) {
		if (errno == EWOULDBLOCK)
			return(0);
		std::ostringstream errMsg;
		errMsg << "Error reading data on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}

	extractAddress(from, srcIp, srcPort);

	return(datalen);
}

int
SocketUdp::readData(void* data, size_t siz)
{
	ssize_t datalen;
	if ((datalen = recvfrom(sock, data, siz, 0, NULL, NULL)) == -1) {
		if (errno == EWOULDBLOCK)
			return(0);
		std::ostringstream errMsg;
		errMsg << "Error reading data on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}

	return(datalen);
}
};
