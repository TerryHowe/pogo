#include "ttLog.h"
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


namespace tt {

DebugLog* DebugLog::inst = NULL;

DebugLogBase::DebugLogBase(const char* logdir, const char* filename) throw(Exception) :
	debugLevel(TTLOG_DEFAULT_LEVEL), outPtr(&std::cout)
{
	tzset();
	if ((logdir) || (filename)) {
		logdir = (logdir)?logdir:".";
		filename = (filename)?filename:"debug.%d";
		char fullFileName[1028];
		(void)sprintf(fullFileName, filename, getpid());
		char fullName[1028];
		(void)sprintf(fullName, "%s/%s", logdir, fullFileName);
#if sun
		std::fstream *tmpFstream = new std::fstream((const char*)fullName, std::ios::out);
#else
		std::fstream *tmpFstream = new std::fstream((const char*)fullName, std::ios_base::out);
#endif
		(void)chmod(fullName, 0664);
		if (! tmpFstream->is_open()) {
			std::string eStr("Error opening file <");
			eStr += fullName;
			eStr += ">";
			throw tt::ExceptionErrno(__FILE__, __LINE__, eStr);
		}
		outPtr = tmpFstream;
	}
}

const char*
DebugLogBase::getTime()
{
   time_t saveTime = time((time_t *)NULL) - (timezone - (daylight * 60 * 60));
#ifdef _REENTRANT
   struct tm timeBuffer;
   struct tm* gtime = gmtime_r(&saveTime, &timeBuffer);
#else
   struct tm* gtime = gmtime(&saveTime);
#endif
   sprintf(dateBuffer, "%2.2u/%2.2u/%2.2u-%2.2u:%2.2u:%2.2u ",
		   gtime->tm_year,
		   (gtime->tm_mon+1),
		   gtime->tm_mday,
		   gtime->tm_hour,
		   gtime->tm_min,
		   gtime->tm_sec);
   return(dateBuffer);
}

};
