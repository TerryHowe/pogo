
TOP=.
OSTYPE := $(shell uname -s)
SUBDIRNAME=.
include $(TOP)/make.$(OSTYPE).defines

SUBDIRS = ProcessManager

SRCS = \
	ttAtomicCounter.cc \
	ttCallback.cc \
	ttDate.cc \
	ttDateTime.cc \
	ttConfiguration.cc \
	ttInfraConfiguration.cc \
	ttMsgFactory.cc \
	ttMsgUnknown.cc \
	ttException.cc \
	ttFileLexer.cc \
	ttInfraLog.cc \
	ttLexer.cc \
	ttLog.cc \
	ttMessage.cc \
	ttMessageBuffer.cc \
	ttMessageFactory.cc \
	ttMessageRegistrar.cc \
	ttMutex.cc \
	ttMutexLock.cc \
	ttNumber.cc \
	ttSelectable.cc \
	ttSelector.cc \
	ttShMem.cc \
	ttSocket.cc \
	ttSocketBroadcast.cc \
	ttSocketMulticast.cc \
	ttSocketTcp.cc \
	ttSocketTcpClient.cc \
	ttSocketTcpConnection.cc \
	ttSocketTcpServer.cc \
	ttSocketUdp.cc \
	ttSocketUdpClient.cc \
	ttSocketUdpServer.cc \
	ttThread.cc \
	ttTime.cc \
	ttTimer.cc \
	ttTimerContext.cc \
	ttTimeVal.cc
OBJS = $(SRCS:cc=o)
INCS = $(SRCS:cc=h)


LIB1 = ttInfra
LIB1_OBJS = $(OBJS)

EXE1 = tester
EXE1_OBJS = $(LIBTT) tester.o
EXE1_LIBS = $(SOCKETLIBS) $(PTHREADLIBS) -L$(LIBDIR) -lttInfra

include $(TOP)/make.targets
