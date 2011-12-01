#ifndef h_ConfigurationFile
#define h_ConfigurationFile

#include <string>

#include "AlTypes.h"
#include "NamedSections.h"

//
// AConfigurationFile reads and writes configuration files.  It also allows
// geting and setting of values in the configuration files.
//
class AConfigurationFile
{
private:
    const char* fFileName;
    std::string fSection;
    std::string fKeyName;
    ANamedSections fValues;
public:
    AConfigurationFile(const char* fileName);
    ~AConfigurationFile();
    bool read();
    bool write();
    void setSection(std::string name);
    std::string getSection() { return(fSection); }
    void setKeyName(std::string name) { fKeyName = name; }
    std::string getKeyName() { return(fKeyName); }
    void setKeyValue(std::string value);
    void setKeyValue(int value);
    void setKeyValue(float value);
    void setKeyValue(double value);
    std::string get(const std::string& section, const std::string& key);
    int getInt(const std::string& section, const std::string& key);
    float getFloat(const std::string& section, const std::string& key);
};

#endif
