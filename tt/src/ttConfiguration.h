#ifndef ttConfiguration_h
#define ttConfiguration_h

#include <map>
#include <list>
#include "ttLexer.h"

namespace tt {

//
// Configuration element repository
//
class ConfigElement;
class ConfigElementRepository {
private:
typedef std::map<std::string, ConfigElement*> Repository;
	Repository repo;
protected:
public:
	ConfigElementRepository();
	~ConfigElementRepository();
	void add(std::string flat, ConfigElement* that);
	const std::string& getValue(std::string name) throw(Exception);
};

//
// Configuration element
//
class ConfigElement {
private:
	std::string _name;
	std::string _value;
	ConfigElement(const ConfigElement& rhs); // Undefined
protected:
public:
	ConfigElement(std::string name = "");
	~ConfigElement();
	void parse(Lexer& lex, ConfigElementRepository& repo, std::string flat);
	const std::string& getValue() { return(_value); }
	const std::string& getName() { return(_name); }
};

class Configuration;
class ConfigValidate {
private:
protected:
	const std::string& _val;
public:
	ConfigValidate(Configuration *cptr, const char* name) throw(Exception);
	~ConfigValidate() {}
};
class ConfigValidateMinMax : public ConfigValidate {
private:
protected:
public:
	ConfigValidateMinMax(Configuration *cptr, const char* name, long min, long max) throw(Exception);
	~ConfigValidateMinMax() {}
};
class ConfigValidateList : public ConfigValidate {
private:
protected:
public:
	ConfigValidateList(Configuration *cptr, const char* name, const char** listVals) throw(Exception);
	~ConfigValidateList() {}
};

#define DEFAULT_CONFIG_DIR "."
#define DEFAULT_CONFIG_FILE "config.xml"

class Configuration {
private:
	std::string _fileName;
	ConfigElementRepository repo;
	ConfigElement* mainElement;
protected:
	Configuration(const char* dirName = DEFAULT_CONFIG_DIR, const char* fileName = DEFAULT_CONFIG_FILE);
	~Configuration();
public:
	const std::string& getValue(const char* name) throw(Exception);
	const std::string& getValue(std::string name) throw(Exception);
};

};

#endif
