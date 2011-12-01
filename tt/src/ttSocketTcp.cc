#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ttSocketTcp.h"
#include "ttInfraLog.h"


namespace tt {


SocketTcp::SocketTcp(std::string _myIpAddy, unsigned int _myPort, int _fd)
  : Socket(_myIpAddy, _myPort, AF_INET, SOCK_STREAM, _fd)
{
	
	
}

SocketTcp::~SocketTcp()
{
	
	
}

};
