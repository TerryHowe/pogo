#ifndef ttSocketTcpConnection_h
#define ttSocketTcpConnection_h

#include "ttSocketTcp.h"
#include "ttException.h"


namespace tt {

class SocketTcpConnection : public SocketTcp {
public:
	SocketTcpConnection(std::string _myIpAddy, unsigned int _myPort, std::string _theirIpAddy, unsigned int _theirPort, int _fd=-1);
	virtual ~SocketTcpConnection();
	void writeData(const void* data, size_t siz);
	int readData(void* data, size_t siz);
	std::string getTheirAddress() const;
protected:
	std::string theirIpAddy;
	unsigned int theirPort;
private:
	SocketTcpConnection(SocketTcpConnection& rhs);
	SocketTcpConnection& operator=(SocketTcpConnection& rhs);
};

};

#endif
