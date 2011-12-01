#ifndef ttLog_h
#define ttLog_h

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "ttException.h"


//
// Various debug levels, feel free to add more
//
#define DEBUG_LOG_FILE	"DEBUG_LOG_FILE"
#define DEBUG_LOG_DIR	"DEBUG_LOG_DIR"

//
// Various debug levels, feel free to add more
//
#define TTDEBUG0	1<<0
#define TTDEBUG1	1<<1
#define TTDEBUG2	1<<2
#define TTDEBUG3	1<<3
#define TTDEBUG4	1<<4
#define TTDEBUG5	1<<5
#define TTDEBUG6	1<<6
#define TTDEBUG7	1<<7
#define TTDEBUG8	1<<8
#define TTDEBUG9	1<<9

//
// Various log levels
//
#define TTLOG_INFO	1<<28
#define TTLOG_WARN	1<<29
#define TTLOG_ERR	1<<30
#define TTLOG_CRIT	1<<31
// #define TTLOG_DEFAULT_LEVEL	(TTLOG_INFO|TTLOG_WARN|TTLOG_ERR|TTLOG_CRIT)
#define TTLOG_DEFAULT_LEVEL	(~0)

#define TtLogInfo(_x_)	if (tt::DebugLog::instance().isDebug(TTLOG_INFO)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtLogWarn(_x_)	if (tt::DebugLog::instance().isDebug(TTLOG_WARN)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtLogErr(_x_)	if (tt::DebugLog::instance().isDebug(TTLOG_ERR)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtLogCrit(_x_)	if (tt::DebugLog::instance().isDebug(TTLOG_CRIT)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 

//
// Various debug macros, feel free to add more
//
#ifdef TT_NODEBUG

//
// Compiled out
//
#define TtDebug0(_x_)
#define TtDebug1(_x_)
#define TtDebug2(_x_)
#define TtDebug3(_x_)
#define TtDebug4(_x_)
#define TtDebug5(_x_)
#define TtDebug6(_x_)
#define TtDebug7(_x_)
#define TtDebug8(_x_)
#define TtDebug9(_x_)
#define TtDebugEnter()
#define TtDebugExit()

#else

//
// Compiled in
//
#define TtDebug0(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG0)) \
	 tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug1(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG1)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug2(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG2)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug3(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG3)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug4(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG4)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug5(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG5)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug6(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG6)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug7(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG7)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug8(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG8)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtDebug9(_x_) if (tt::DebugLog::instance().isDebug(TTDEBUG9)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 

//
// Couple specialized macros
//
#define TtDebugEnter() if (tt::DebugLog::instance().isDebug(TTDEBUG0)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << "Enter" << std::endl
#define TtDebugExit() if (tt::DebugLog::instance().isDebug(TTDEBUG0)) \
	tt::DebugLog::instance().getOut() << tt::DebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << "Exit" << std::endl

#endif

namespace tt {

//
// The debug log class base class
//
class DebugLogBase {
private:
	unsigned long debugLevel;
	std::ostream* outPtr;
	char dateBuffer[64];
protected:
	DebugLogBase(const char* logdir, const char* filename) throw(Exception);
	~DebugLogBase() { if (outPtr != &std::cout) delete outPtr; }
public:
	// TODO: Mutex this
	inline bool isDebug(unsigned long level) { return(level & debugLevel); }
	unsigned long getDebugLevel() { return debugLevel; }
	void setDebugLevel(unsigned long newLevel) { debugLevel = newLevel; }
	void addDebugLevel(unsigned long newLevel) { debugLevel |= newLevel; }
	void subtractDebugLevel(unsigned long oldLevel) { debugLevel &= ~oldLevel; }
	std::ostream& getOut() { return *outPtr; }
	const char* getTime();
};
	
//
// The debug log class a singleton
//
class DebugLog : public DebugLogBase {
private:
	static DebugLog* inst;
	DebugLog() : DebugLogBase(getenv(DEBUG_LOG_DIR), getenv(DEBUG_LOG_FILE)) {}
public:
	static DebugLog& instance() { return((inst)?*inst:*(inst = new DebugLog)); }
};

};

#endif
