#include <sys/types.h>
#include <sys/socket.h>
#include "ttSocketUdpServer.h"
#include "ttInfraLog.h"


namespace tt {


SocketUdpServer::SocketUdpServer(unsigned int _myPort)
  : SocketUdp("", _myPort)
{
	
	bind();
	
}

SocketUdpServer::SocketUdpServer(std::string _myIpAddy, unsigned int _myPort)
  : SocketUdp(_myIpAddy, _myPort)
{
	
	bind();
	
}

SocketUdpServer::~SocketUdpServer()
{
	
	
}

};
