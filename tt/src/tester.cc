#include <iostream>
#include <string.h>
#include <unistd.h>
#include "ttAtomicCounter.h"
#include "ttException.h"
#include "ttLexer.h"
#include "ttLog.h"
#include "ttMutex.h"
#include "ttMutexLock.h"
#include "ttThread.h"
#include "ttConfiguration.h"
#include "ttInfraConfiguration.h"
#include "ttMessage.h"
#include "ttMessageBuffer.h"
#include "ttMessageFactory.h"
#include "ttMessageRegistrar.h"
#include "ttMsgUnknown.h"
#include "ttShMem.h"
#include "ttSocketUdpClient.h"
#include "ttSocketUdpServer.h"
#include "ttSocketTcpClient.h"
#include "ttSocketTcpServer.h"
#include "ttSocketMulticast.h"
#include "ttSocketBroadcast.h"
#include "ttSelector.h"
#include "ttSelectable.h"
#include "ttTimeVal.h"
#include "ttTimer.h"

using namespace std;
using namespace tt;

class MyThread : public ThreadBase {
private:
		int threadid;
public:
		void* run() { return NULL; }
		MyThread(int id) : ThreadBase(), threadid(id) {}
};

class MyCallback : public tt::Callback {
	virtual void callbackMethod() {
		cerr << "In my callback!" << endl;
	}
};
class MyMsg : public tt::Message {
private:
        std::string firstStr;
	int someInt;
        std::string secondStr;
protected:
public:
        static const unsigned int type = 9000;
        static const char* name;
        MyMsg() : tt::Message(MyMsg::type, MyMsg::name), firstStr("asdfasdf asdf"), someInt(33), secondStr("kdkdkdkd") {}
        MyMsg(tt::MessageBuffer& b) : tt::Message(MyMsg::type, MyMsg::name), firstStr(""), someInt(0), secondStr("") {unmarshall(b);}

        ~MyMsg() {}
        virtual void marshall(tt::MessageBuffer& b) {
		Message::marshall(b);
		b << firstStr << someInt << secondStr;
	}
        virtual void unmarshall(tt::MessageBuffer& b) {
		Message::unmarshall(b);
		b >> firstStr >> someInt >> secondStr;
	}
};
const char* MyMsg::name = "MyMsg";
class MyMsgFactory : public tt::MessageFactory {
private:
        static MyMsgFactory factory;
protected:
public:
        MyMsgFactory() { tt::MessageRegistrar::instance().add(this, MyMsg::type); }
        ~MyMsgFactory() { tt::MessageRegistrar::instance().remove(this, MyMsg::type); }
        virtual tt::Message* createMessage(tt::MessageBuffer& msgBuf, unsigned int msgType) {
	        switch (msgType) {
	        case MyMsg::type:
	                return(new MyMsg(msgBuf));
	                break;
	        }
        	return(new tt::MsgUnknown(msgType));
	}
};
MyMsgFactory MyMsgFactory::factory;



int
main(int argc, char *argv[])
{
   try {
	//
	// Simple command line arg
	//
	char component;
	if (argc < 2) {
		component = '?';
	}
	else {
		if (argv[1][0] != '-')
			component = '?';
		else
			component = argv[1][1];
	}

	//
	// Test the desired component
	//
	switch (component) {
	case 'e': {
		try {
			throw tt::Exception(__FILE__, __LINE__, "Why did you do that?");
		}
		catch (tt::Exception except) {
			std::cout << except << endl;
		}
		break;
	}

	case 'x': {
		//
		// Put in string
		//
		cout << "starting" << endl;
		std::string inputString;
		for (int i=2; i<argc; ++i) {
			if (i != 1) inputString += " ";
			inputString += argv[i];
		}

		//
		// Construct
		//
		tt::Lexer lexer(inputString.c_str());
		cout << "lexer=[" << lexer.toString() << "]" << endl;

		//
		// Parse
		//
		std::string value;
		while (! lexer.eos()) {
			try {
			  lexer >> value;
			}
			catch (const char* ex) {
				cout << "Exception: [" << ex << "]" << endl;
				break;
			}
			cout << "value=[" << value << "]" << endl;
		}

		cout << "ending" << endl;
		break;
	}

	case 'l': {
		TtDebugEnter();
		TtDebug0("THIS! enter above and ... What ever = " << 1111);
		unsigned long saveLevel = tt::DebugLog::instance().getDebugLevel();
		tt::DebugLog::instance().setDebugLevel(0);
		TtDebug0("NOT THIS What ever = " << 2222);
		TtDebugEnter();
		TtDebugExit();
		tt::DebugLog::instance().setDebugLevel(saveLevel);
		TtDebug0("THIS! What ever = " << 3333);
		TtDebugEnter();
		TtDebugExit();
		TtDebug0("THIS! enter and exit above");
		tt::DebugLog::instance().subtractDebugLevel(TTDEBUG0);
		TtDebug0("NOT THIS What ever = " << 4444);
		TtDebugEnter();
		TtDebugExit();
		tt::DebugLog::instance().addDebugLevel(TTDEBUG0);
		TtDebug0("THIS! just an exit...");
		TtDebugExit();
		break;
	}

	case 'm': {
		tt::Mutex mutie;
		tt::MutexLock lock(mutie);
		break;
	}

	case 'a': {
		tt::AtomicCounter ac(1);
		++ac;
		--ac;
		ac += 2;
		ac -= 3;
		ac = ac - 3;
		ac = ((int)ac + 6);
		cout << "ac should be 3 = " << (int)ac << endl;
		break;
	}

	case 't': {
		MyThread one(1);
		MyThread two(2);
		MyThread three(3);

		one.start();
		two.start();
		three.start();
		tt::ThreadJoiner::instance().wait();
		break;
	}
	case 'c': {
		std::string val = tt::InfraConfiguration::instance().getValue("DebugLogFile");
		TtDebug0("configuration value DebugLogFile=<" << val << ">");
		break;
	}
	case 'M': {
		// tt::MsgUnknown msg(12);
		MyMsg msg;
		tt::MessageBuffer msgBuf(0);
		msg.marshall(msgBuf);
		TtDebug0("marshalled message=<" << msgBuf.toString() << ">");
		TtDebug0("marshalled message=<" << msgBuf.getBuffer() << ">");
		TtDebug0("marshalled message=<" << msgBuf.getBuffer().length() << ">");
		TtDebug0("marshalled message=<" << msgBuf.getBuffer().size() << ">");
		msgBuf.reset();
		tt::Message *msgPtr;
		msgPtr = tt::MessageRegistrar::instance().createMessage(msgBuf);
		tt::MessageBuffer msgBuf2(1024);
		msgPtr->marshall(msgBuf2);
		TtDebug0("marshalled message=<" << msgBuf2.toString() << ">");
		delete msgPtr;
		break;
	}
	case 's': {
		tt::ShMem<int> val(1);
		break;
	}
	case 'U': {
		tt::SocketUdpClient client;
		tt::SocketUdpServer server("127.0.0.1", 4568);
		client.writeData("asdf", 5, "127.0.0.1", 4568);
		char buf[512];
		std::string srcIp;
		unsigned int srcPort;
		int ret = server.readData(buf, sizeof(buf), srcIp, srcPort);
		TtDebug0("buf=<" << buf << ">");
		TtDebug0("ret=<" << ret << ">");
		TtDebug0("srcIp=<" << srcIp << ">");
		TtDebug0("srcPort=<" << srcPort << ">");
		break;
	}
	case 'S': {
		tt::SocketTcpServer serv("127.0.0.1", 4568);
		tt::SocketTcpConnection* con = serv.accept();
		char buf[512];
		int ret = con->readData(buf, sizeof(buf));
		TtDebug0("buf=<" << buf << ">");
		TtDebug0("ret=<" << ret << ">");
		break;
	}
	case 'T': {
		tt::SocketTcpClient client("127.0.0.1", 4569, "127.0.0.1", 4568);
		client.writeData("asdf", 5);
		break;
	}
	case '0': {
		tt::SocketMulticast serv("230.0.0.3", 4568);
		char buf[512];
		int ret = serv.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("buf=<" << buf << ">");
		TtDebug0("ret=<" << ret << ">");
		serv.writeData("fdsa", 5);
		serv.writeData("qwerty", 7);
		ret = serv.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		ret = serv.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		break;
	}
	case '1': {
		tt::SocketMulticast client("230.0.0.3", 4568);
		client.writeData("asdf", 5);
		char buf[512];
		int ret = client.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		ret = client.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("2 buf=<" << buf << ">");
		TtDebug0("2 ret=<" << ret << ">");
		ret = client.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		break;
	}
	case '2': {
		tt::SocketMulticast client("127.0.0.1", 4568);
		TtDebug0("Shouldn't get here, should throw");
	}
	case '3': {
		tt::SocketBroadcast cli("255.255.255.255", 4569);
		char buf[512];
		int ret = cli.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("buf=<" << buf << ">");
		TtDebug0("ret=<" << ret << ">");
		cli.writeData("fdsa", 5);
		cli.writeData("qwerty", 7);
		ret = cli.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		ret = cli.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		break;
	}
	case '4': {
		tt::SocketBroadcast cli("255.255.255.255", 4569);
		char buf[512];
		cli.writeData("asdf", 5);
		int ret = cli.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		ret = cli.readData(buf, sizeof(buf));
		buf[ret-1] = '\000';
		TtDebug0("1 buf=<" << buf << ">");
		TtDebug0("1 ret=<" << ret << ">");
		break;
	}
	case '6': {
		//
		// SIP server dummy
		//
		tt::SocketUdpServer testServer("192.168.2.2", 5060);
		char buf[4096];
		char resp[4096];
		std::string srcIp;
	       	unsigned int srcPort;
		(void)strcpy(resp, "SIP/2.0 200 OK");
		int ret;
		while ((ret = testServer.readData(buf, sizeof(buf), srcIp, srcPort)) > 0) {
			testServer.writeData(resp, strlen(resp), srcIp, srcPort);
//			(void)fwrite(buf, ret, 1, stdout);
		}
		break;
	}
	case 'L': {
		tt::Selector selector;
		class QuickServer : public tt::Selectable {
			virtual void doRead() {
				cin >> input;
				cerr << "input=<"  << input << ">" << endl;
			}
		public:
			std::string input;
		} quickServer;
		quickServer.setFd(0);
		selector.add(quickServer);
		selector.setTimer(new tt::TimerContext(3, new MyCallback));
		while (quickServer.input != "done") {
			selector.select();
		}
		break;
	}
	case 'X': {
		tt::Timer::instance().set(4, new MyCallback);
		pause();
		break;
	}
	case 'i': {
		tt::TimeVal t1;
		tt::TimeVal t2(t1);
		tt::TimeVal t3(2);
		if (t2 == t1) {
		 ++t2.tv_sec;
		 if (t2 > t1) {
		  if (t1 < t2) {
		   if (t1 <= t2) {
		    t1 = t2 + t3;
		    if (t1 >= t2) {
		     t1 = t2 - t3;
		     if (t1 <= t2) {
		      t2 = t1;
		      t1 -= t3;
		      if (t1 <= t2) {
		       t2 = t1;
		       t1 += t3;
		       if (t1 >= t2) {
		        cerr << "Perfect!" << endl;
		       }
		      }
		     }
		    }
		   }
		  }
		 }
		}
		break;
	}
	default: {
		cerr << "Usage: " << argv[0] << " -c|-e|-l|-m|-t|-x [args]" << endl;
		exit(1);
	}
	}

	exit(0);
   }
   catch (tt::Exception ex) {
	cerr << "Exception caught: " << ex << endl;
	exit(1);
   }
   catch (...) {
	cerr << "Unknown exception caught" << endl;
	exit(1);
   }
}
