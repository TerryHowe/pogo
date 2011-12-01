#include "ttInfraLog.h"
#include <stdlib.h>


namespace tt {

InfraDebugLog* InfraDebugLog::inst = NULL;

InfraDebugLog::InfraDebugLog()
   : DebugLogBase(getenv(DEBUG_LOG_DIR), getenv(DEBUG_LOG_FILE))
{
	if (getenv(DEBUG_LOG_LEVEL))
		setDebugLevel(atoi(getenv(DEBUG_LOG_LEVEL)));
}

InfraDebugLog& InfraDebugLog::instance()
{
	// TODO Mutex
	return((inst)?*inst:*(inst = new InfraDebugLog));
}

};
