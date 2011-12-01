#include <iostream>
#include "ttProcess.h"
#include "ttInfraLog.h"


namespace tt {

ProcessBase::ProcessBase()
{
}

void
ProcessBase::start() throw(Exception)
{

   if ((pid = fork()) == -1) {
	std::string eStr("Error creating process");
	throw tt::ExceptionErrno(__FILE__, __LINE__, eStr);
   }

   if (pid == 0) { exit(run()); } /* the child */

   return;
}

ProcessBase::~ProcessBase()
{
}

};
