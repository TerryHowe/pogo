#ifndef ttSocket_h
#define ttSocket_h

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include "ttException.h"


namespace tt {

class Socket {
public:
	Socket(std::string _myIpAddy, unsigned int _myPort, int _domain, int _type, int _fd=-1);
	virtual ~Socket();
	std::string getMyAddress();
	std::string getMyIpAddy() { return(myIpAddy); }
	unsigned int getMyPort() { return(myPort); }
	int getFd() { return(sock); }
	void createAddress(struct sockaddr& addy, std::string _ipAddy, unsigned int _port);
	void extractAddress(struct sockaddr& addy, std::string& _ipAddy, unsigned int& _port);
	bool isMulticastAddy();
	bool isBroadcastAddy();
	void allow_reuse(bool val);
	void join();
	void timeToLive(unsigned char val);
	void loopBack(unsigned char val);
	void setDefaultInterface();
	void setBroadcast();
	void setNonBlocking();
protected:
	int sock;
	void bind();
	void connect(); // move to SocketTcp
	void setMyIpAddy(const std::string& val) { myIpAddy = val; }
	void setMyPort(const unsigned int val) { myPort = val; }
private:
	Socket(Socket& rhs);
	Socket& operator=(Socket& rhs);
	void openSock();
	void closeSock();
	std::string myIpAddy;
	unsigned int myPort;
	int domain;
	int type;
};

};

#endif
