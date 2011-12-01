#ifndef ttSocketTcpServer_h
#define ttSocketTcpServer_h

#include "ttSocketTcp.h"
#include "ttSocketTcpConnection.h"
#include "ttException.h"


namespace tt {

class SocketTcpServer : public SocketTcp {
public:
	SocketTcpServer(std::string _myIpAddy, unsigned int _myPort, int _backlog=32);
	virtual ~SocketTcpServer();
	void setBacklog(const int val) { backlog = val; }
	int getBacklog() { return(backlog); }
	SocketTcpConnection* accept();
protected:
	void listen();
private:
	SocketTcpServer(SocketTcpServer& rhs);
	SocketTcpServer& operator=(SocketTcpServer& rhs);
	int backlog;
};

};

#endif
