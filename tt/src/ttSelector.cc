#include <errno.h>
#include <memory.h>
#include "ttSelector.h"
#include "ttInfraLog.h"

extern int errno;

namespace tt {


Selector::Selector()
  : nfds(-1), readfds(NULL), writefds(NULL), errorfds(NULL), timerContext(NULL)
{
	
	
}

Selector::~Selector()
{
	
	
}

void
Selector::add(Selectable& entry)
{
	
	if (entry.getReadFd() != -1)
		readers.insert(Selection::value_type(entry.getReadFd(), &entry));
	if (entry.getWriteFd() != -1)
		writers.insert(Selection::value_type(entry.getWriteFd(), &entry));
	if (entry.getErrorFd() != -1)
		errors.insert(Selection::value_type(entry.getErrorFd(), &entry));
	doUpdate();
	
}

void
Selector::remove(Selectable& entry)
{
	
	Selection::iterator itr;
	itr = readers.find(entry.getReadFd());
	if (itr != readers.end())
		readers.erase(itr);
	itr = writers.find(entry.getWriteFd());
	if (itr != writers.end())
		writers.erase(itr);
	itr = errors.find(entry.getErrorFd());
	if (itr != errors.end())
		errors.erase(itr);
	doUpdate();
	
}

void
Selector::select()
{
	

	//
	// Prep
	//
	fd_set spacereadfds;
	fd_set spacewritefds;
	fd_set spaceerrorfds;
	fd_set* tmpreadfds = NULL;
	fd_set* tmpwritefds = NULL;
	fd_set* tmperrorfds = NULL;
	if (readfds) {
		tmpreadfds = &spacereadfds;
		(void)memcpy(tmpreadfds, readfds, sizeof(fd_set));
	}
	if (writefds) {
		tmpwritefds = &spacewritefds;
		(void)memcpy(tmpwritefds, writefds, sizeof(fd_set));
	}
	if (errorfds) {
		tmperrorfds = &spaceerrorfds;
		(void)memcpy(tmperrorfds, errorfds, sizeof(fd_set));
	}

	//
	// Select
	//
	int rc;
	if  ((rc = ::select(nfds, tmpreadfds, tmpwritefds, tmperrorfds, ((timerContext)?timerContext->getTimeOut():NULL))) < 0) {
		if (errno == EINTR)
			return;
		throw TTEXCEPTIONERR("Error in select(3c)");
	}

	//
	// Time out
	//
	if (rc == 0) {
		if (timerContext) {
			timerContext->callbackMethod();
			timerContext->incrementTimer();
		}
		return;
	}

	//
	// Reads
	//
	if (tmpreadfds) {
		Selection::iterator itr = readers.begin();
		while (itr != readers.end()) {
			if (FD_ISSET(itr->second->getReadFd(), tmpreadfds)) {
				itr->second->doRead();
			}
			++itr;
		}
	}

	//
	// Writes
	//
	if (tmpwritefds) {
		Selection::iterator itr = writers.begin();
		while (itr != writers.end()) {
			if (FD_ISSET(itr->second->getWriteFd(), tmpwritefds)) {
				itr->second->doWrite();
			}
			++itr;
		}
	}

	//
	// Errors
	//
	if (tmperrorfds) {
		Selection::iterator itr = errors.begin();
		while (itr != errors.end()) {
			if (FD_ISSET(itr->second->getErrorFd(), tmperrorfds)) {
				itr->second->doError();
			}
			++itr;
		}
	}

	
}

void
Selector::doUpdate()
{
	
	int tmpFd;
	nfds = 0;

	//
	// Readers
	//
	Selection::iterator itr = readers.begin();
	while (itr != readers.end()) {
		tmpFd = itr->second->getReadFd();
		if (tmpFd > -1) {
			if (readfds == NULL) {
				readfds = new fd_set;
				FD_ZERO(readfds);
			}
			FD_SET(tmpFd, readfds);
			if (nfds < tmpFd)
				nfds = tmpFd;
		}
		++itr;
	}

	//
	// Writers
	//
	itr = writers.begin();
	while (itr != writers.end()) {
		tmpFd = itr->second->getWriteFd();
		if (tmpFd > -1) {
			if (readfds == NULL) {
				readfds = new fd_set;
				FD_ZERO(readfds);
			}
			FD_SET(tmpFd, readfds);
			if (nfds < tmpFd)
				nfds = tmpFd;
		}
		++itr;
	}

	//
	// Errors
	//
	itr = errors.begin();
	while (itr != errors.end()) {
		tmpFd = itr->second->getErrorFd();
		if (tmpFd > -1) {
			if (readfds == NULL) {
				readfds = new fd_set;
				FD_ZERO(readfds);
			}
			FD_SET(tmpFd, readfds);
			if (nfds < tmpFd)
				nfds = tmpFd;
		}
		++itr;
	}
	++nfds;
	
}

void
Selector::setTimer(TimerContext* val)
{
	timerContext = val;
}

};
