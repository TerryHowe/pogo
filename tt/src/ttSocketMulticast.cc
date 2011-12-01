#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ttSocketMulticast.h"
#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {


SocketMulticast::SocketMulticast(std::string _serverIpAddy, unsigned int _serverPort)
   : SocketUdpClient(_serverIpAddy, _serverPort)
{
	

	//
	// Bind
	//
	setMyPort(_serverPort);
	allow_reuse(true);
	bind();

	//
	// Join the group
	//
	setMyIpAddy(_serverIpAddy);
	if (! isMulticastAddy()) {
		std::ostringstream errMsg;
		errMsg << "The ip address provided is not a multicast address <" << getMyAddress() << ">.  Use address ranging from (225.0.0.0 to 239.255.255.255 )";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	join();

	
}

SocketMulticast::~SocketMulticast()
{
	
	
}

};
