#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "ttSocket.h"
#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {


Socket::Socket(std::string _myIpAddy, unsigned int _myPort, int _domain, int _type, int _fd)
  : sock(_fd), myIpAddy(_myIpAddy), myPort(_myPort), domain(_domain), type(_type)
{
	if (sock == -1) openSock();
}

Socket::~Socket()
{
	closeSock();
}

void
Socket::bind()
{
	
	struct sockaddr sockAddr;
	createAddress(sockAddr, myIpAddy, myPort);
	
	if (::bind(sock, &sockAddr, sizeof(sockAddr)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error binding for <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	
}

void
Socket::openSock()
{
	
	if ((sock = socket(domain, type, 0)) < 0) {
		std::ostringstream errMsg;
		errMsg << "Error creating socket for <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	
}

void
Socket::closeSock()
{
	
	if (sock != -1)
		(void)close(sock);
	sock = -1;
	
}

std::string
Socket::getMyAddress()
{
	std::ostringstream tmpStr;
	tmpStr << myIpAddy << ":" << myPort;
	return(tmpStr.str());
}


//
// This method should be put in its own class
//
void
Socket::createAddress(struct sockaddr& addy, std::string _ipAddy, unsigned int _port)
{
	
	(void)memset(&addy, 0, sizeof(struct sockaddr));
	struct sockaddr_in* addyIn = (struct sockaddr_in*)(&addy);
	addyIn->sin_family = domain;
	addyIn->sin_port = htons(_port);
	addyIn->sin_addr.s_addr = INADDR_ANY;
	if (_ipAddy.length() != 0) {
		if (inet_pton(domain, _ipAddy.data(), &(addyIn->sin_addr)) != 1) {
			std::ostringstream errMsg;
			errMsg << "Error with inet_pton for <" << _ipAddy << ":" << _port << ">";
			throw TTEXCEPTIONERR(errMsg.str());
		}
	}
	
}

//
// This method should be put in its own class
//
void
Socket::extractAddress(struct sockaddr& addy, std::string& _ipAddy, unsigned int& _port)
{
	
	// Only supporting one type right now
	char buf[INET_ADDRSTRLEN];
	struct sockaddr_in* addyIn = (struct sockaddr_in*)(&addy);
	_port = ntohs(addyIn->sin_port);
	if (inet_ntop(domain, &(addyIn->sin_addr), buf, sizeof(buf)) == NULL) {
		std::ostringstream errMsg;
		errMsg << "Error with inet_ntop";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	_ipAddy = buf;
	

	
}

bool
Socket::isMulticastAddy()
{
	struct sockaddr addy;
	createAddress(addy, myIpAddy, myPort);
	struct sockaddr_in* addyIn = (struct sockaddr_in*)(&addy);
	return(IN_MULTICAST(addyIn->sin_addr.s_addr));
}

bool
Socket::isBroadcastAddy()
{
	/* IpAddress|(~Netmask) */
	TtLogWarn("Socket::isBroadcastAddy() not implemented");
	return(true);
}
void
Socket::allow_reuse(bool val)
{
	//
	// Reuse
	//
	int one = val?1:0;
	
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (SO_REUSEADDR) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	return;
}

void
Socket::join()
{
	//
	// Set parameter
	//
	struct ip_mreq imreq;
	memset(&imreq, 0, sizeof(imreq));
	if (inet_pton(domain, myIpAddy.data(), &(imreq.imr_multiaddr.s_addr)) != 1) {
		std::ostringstream errMsg;
		errMsg << "Error with inet_pton for <" << myIpAddy << ":" << myPort << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	imreq.imr_interface.s_addr = INADDR_ANY;

	//
	// Join a multicast group on then default interface
	//
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&imreq, sizeof(imreq)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (IP_ADD_MEMBERSHIP) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}

	return;
}

void
Socket::timeToLive(unsigned char val)
{
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &val, sizeof(val)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (IP_MULTICAST_TTL) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	return;
}

void
Socket::loopBack(unsigned char val)
{
	//
	// Setting loopback will allow multiple listeners
	//
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (IP_MULTICAST_LOOP) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	return;
}

void
Socket::setDefaultInterface()
{
	//
	// Set the outgoing interface to DEFAULT
	//
	struct in_addr iaddr;
	(void)memset(&iaddr, 0, sizeof(struct in_addr));
	iaddr.s_addr = INADDR_ANY;
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr, sizeof(struct in_addr)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (IP_MULTICAST_IF) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
}

void
Socket::setBroadcast()
{
	//
	// Set the outgoing interface to DEFAULT
	//
	struct in_addr iaddr;
	(void)memset(&iaddr, 0, sizeof(struct in_addr));
	iaddr.s_addr = INADDR_ANY;
	int yes = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) != 0) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (SO_BROADCAST) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
}

void
Socket::setNonBlocking()
{
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
		std::ostringstream errMsg;
		errMsg << "Error settting socket option (SO_BROADCAST) on <" << getMyAddress() << ">";
		throw TTEXCEPTIONERR(errMsg.str());
	}
}

};
