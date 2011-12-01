#include <iostream>
#include <stdio.h>
#include "ttConfiguration.h"
#include "ttFileLexer.h"

#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {


ConfigElement::ConfigElement(std::string name)
  : _name(name), _value("")
{
	
	
}

ConfigElement::~ConfigElement()
{
	
	
}

void ConfigElement::parse(Lexer& lex, ConfigElementRepository& repo, std::string flat)
{
	//
	// Get name
	//
	lex >> '<';
	if (_name != "") {
		lex >> _name.c_str();
	}
	else {
		lex >> _name;
	}

	//
	// Make flat name
	//
	std::string tmpFlatName;
	if (flat == "") {
		tmpFlatName = _name;
	}
	else {
		tmpFlatName = flat + "." + _name;
	}

	//
	// Check for empty node
	//
	if (lex.nextToken('/')) {
		_name = tmpFlatName;
		repo.add(_name, this);
		lex >> '/' >> '>';
		return;
	}
	else {
		lex >> '>';
	}

	//
	// Parse children
	//
	ConfigElement* newElem = NULL;
	while (lex.nextToken('<')) {
		if (lex.nextNextToken('<', '/')) {
			lex >> '<' >> '/' >> _name.c_str() >> '>';
			_name = tmpFlatName;
			repo.add(_name, this);
			return;
		}
		newElem = new ConfigElement;
		newElem->parse(lex, repo, tmpFlatName);
	}
	_value = lex.getUntil('<');
	lex >> '<' >> '/' >> _name.c_str() >> '>';
	_name = tmpFlatName;
	repo.add(_name, this);
	return;
}

ConfigElementRepository::ConfigElementRepository() : repo()
{
	
	
}

ConfigElementRepository::~ConfigElementRepository()
{
	
	Repository::iterator itr = repo.begin();
	while (itr != repo.end()) {
		delete (*itr).second;
		++itr;
	}
	
}

void ConfigElementRepository::add(std::string flat, ConfigElement* that)
{
	
	TtLogInfo("adding configuration element=<" << flat << "> value=<" << that->getValue() << ">");
	repo.insert(Repository::value_type(flat, that));
	
}

const std::string& ConfigElementRepository::getValue(const std::string name) throw(Exception)
{
	
	Repository::iterator itr = repo.find(name);
	if (itr == repo.end()) {
		std::string eStr("Configuration value not found <");
		eStr += name;
		eStr += ">";
		throw Exception(__FILE__, __LINE__, eStr);
	}
	TtLogInfo("found configuration element=<" << name << "> value=<" << ((*itr).second->getValue()) << ">");
	
	return((*itr).second->getValue());
}

ConfigValidate::ConfigValidate(Configuration *cptr, const char* name) throw(Exception) :
  _val(cptr->getValue(name))
{
}
ConfigValidateMinMax::ConfigValidateMinMax(Configuration *cptr, const char* name, long min, long max) throw(Exception) :
  ConfigValidate(cptr, name)
{
	//
	// Check it
	//
	long longVal = atol(_val.data());
	if ((longVal >= min) && (longVal <= max)) return;

	//
	// Throw exception
	//
	char rangeBuffer[128];
	(void)sprintf(rangeBuffer, "%ld and less than or equal to %ld.", min, max);
	std::string eStr("Configuration value <");
	eStr += name;
	eStr += "> value <";
	eStr += _val;
	eStr += "> is not valid.  It must be greater than or equal to ";
	eStr += rangeBuffer;
	throw Exception(__FILE__, __LINE__, eStr);
}
ConfigValidateList::ConfigValidateList(Configuration *cptr, const char* name, const char **listVals) throw(Exception) :
  ConfigValidate(cptr, name)
{
	//
	// Check values
	//
	bool first = true;
	std::string validValuesStr("");
	while (*listVals != NULL) {
		if (_val == *listVals) return;
		if (!first) {
			validValuesStr += ",";
		}
		validValuesStr += *listVals;
		++listVals;
		first = false;
	}

	//
	// Throw exception
	//
	std::string eStr("Configuration value <");
	eStr += name;
	eStr += "> value <";
	eStr += _val;
	eStr += "> is not in list of valid values <";
	eStr += validValuesStr;
	eStr += ">";
	throw Exception(__FILE__, __LINE__, eStr);
}

Configuration::Configuration(const char* dirName, const char* fileName) :
  _fileName(((dirName)?dirName:DEFAULT_CONFIG_DIR)), mainElement(NULL)
{
	
	_fileName += "/";
	_fileName += (fileName)?fileName:DEFAULT_CONFIG_FILE;
	
	FileLexer configFile(_fileName.data());
	mainElement = new ConfigElement("Configure");
	mainElement->parse(configFile, repo, "");
	
}

Configuration::~Configuration()
{
}


const std::string& Configuration::getValue(std::string name) throw(Exception)
{
	name = mainElement->getName() + "." + name;
	TtLogInfo("name=<" << name << ">");
	return(repo.getValue(name));
}

const std::string& Configuration::getValue(const char* name) throw(Exception)
{
	return(getValue(std::string(name)));
}

};
