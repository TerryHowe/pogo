#ifndef ttInfraConfiguration_h
#define ttInfraConfiguration_h

#include "ttConfiguration.h"

namespace tt {

#define INFRA_CONFIG_DIR	"."
#define	INFRA_CONFIG_FILE	"infraConfig.xml"
class InfraConfiguration : public Configuration {
private:
	static InfraConfiguration* inst;
	InfraConfiguration();
	~InfraConfiguration();
protected:
public:
	static InfraConfiguration& instance();
};

};

#endif
