#ifndef ttInfraLog_h
#define ttInfraLog_h

#include "ttLog.h"

#define DEBUG_LOG_LEVEL   "DEBUG_LOG_LEVEL"

//
// Various debug macros, feel free to add more
//
#ifdef TT_NODEBUG

#define TtInfraDebug0(_x_)
#define TtInfraDebug1(_x_)
#define TtInfraDebug2(_x_)
#define TtInfraDebugEnter()
#define TtInfraDebugExit()

#else

#define TtInfraDebug0(_x_) if (tt::InfraDebugLog::instance().isDebug(TTDEBUG0)) \
	 tt::InfraDebugLog::instance().getOut() << tt::InfraDebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtInfraDebug1(_x_) if (tt::InfraDebugLog::instance().isDebug(TTDEBUG1)) \
	tt::InfraDebugLog::instance().getOut() << tt::InfraDebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 
#define TtInfraDebug2(_x_) if (tt::InfraDebugLog::instance().isDebug(TTDEBUG2)) \
	tt::InfraDebugLog::instance().getOut() << tt::InfraDebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << _x_ << std::endl 


#define TtInfraDebugEnter() if (tt::InfraDebugLog::instance().isDebug(TTDEBUG0)) \
	tt::InfraDebugLog::instance().getOut() << tt::InfraDebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << "Enter" << std::endl 
#define TtInfraDebugExit() if (tt::InfraDebugLog::instance().isDebug(TTDEBUG0)) \
	tt::InfraDebugLog::instance().getOut() << tt::InfraDebugLog::instance().getTime() << __FILE__ << ":" << __LINE__ << " " << "Exit" << std::endl 

#endif

namespace tt {

//
// The infrastructure debug log class a singleton
//
class InfraDebugLog : public DebugLogBase {
private:
	static InfraDebugLog* inst;
	InfraDebugLog();
public:
	static InfraDebugLog& instance();
};

};

#endif
