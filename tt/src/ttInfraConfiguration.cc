#include <stdlib.h>
#include "ttInfraConfiguration.h"

// #define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {

InfraConfiguration* InfraConfiguration::inst = NULL;

InfraConfiguration::InfraConfiguration() :
  Configuration(getenv(INFRA_CONFIG_DIR), getenv(INFRA_CONFIG_FILE))
{
	
	ConfigValidate(this, "DebugLogFile");
	ConfigValidateMinMax(this, "DebugLogLevel", 0, 16);
	const char *lister[] = {"info","warn","err","crit", 0};
	ConfigValidateList(this, "DebugLogType", lister);
	
}

InfraConfiguration::~InfraConfiguration()
{
	
	
}

InfraConfiguration&
InfraConfiguration::instance()
{
	// TODO Mutex
        if (inst == NULL) inst = new InfraConfiguration;
        return(*inst);
}

};
