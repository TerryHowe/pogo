#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ttSocketBroadcast.h"
#include "ttInfraLog.h"


namespace tt {


SocketBroadcast::SocketBroadcast(std::string _serverIpAddy, unsigned int _serverPort)
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
	if (! isBroadcastAddy()) {
		std::ostringstream errMsg;
		errMsg << "The ip address provided is not a broadcast address <" << getMyAddress() << ">.  Use an address such as 255.255.255.255 or your IP address ored with the ~netmask";
		throw TTEXCEPTIONERR(errMsg.str());
	}
	setBroadcast();

	
}

SocketBroadcast::~SocketBroadcast()
{
	
	
}

};
