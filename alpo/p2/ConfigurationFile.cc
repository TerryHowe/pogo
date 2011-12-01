#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "ConfigurationFile.h"
#include "Scanner.h"
#include "State.h"

using namespace std;


AConfigurationFile::AConfigurationFile(const char *fileName)
    : fSection(""), fKeyName("")
{
    this->fFileName = fileName;
}

AConfigurationFile::~AConfigurationFile()
{
}

bool
AConfigurationFile::read()
{
    try {
        fstream filestr;
        AScanner scanner;
        char buf[1024];
        const AState* state = &AStateSectionStart::instance;
        AToken token;

        filestr.open(this->fFileName, fstream::in);
        if (!filestr.good())
        {
            cerr << "ERROR: Error opening configuration file <" <<
                this->fFileName << ">" << endl;
            filestr.close();
            return false;
        }

        *buf = '\000';
        while (filestr.good())
        {
            //
            // Read until no line continuation
            //
            scanner.getLine() = buf;
            while (filestr.good())
            {
                filestr.getline(buf, sizeof(buf));
                if ((filestr.fail()) && (!filestr.eof()))
                {
                    cerr << "ERROR: Error reading configuration file <" <<
                        this->fFileName << ">" << endl;
                    filestr.close();
                    return false;
                }
                if (scanner.getLine().size() == 0)
                {
                    scanner.getLine() = buf;
                    *buf = '\000';
                    continue;
                }

                if (isspace(*buf))
                {
                    scanner.getLine() += buf;
                    *buf = '\000';
                    continue;
                }
                break;
            }

            //
            // Scan the line for tokens
            //
            token = scanner.getNextToken();
            while (token.getType() != AToken::kNothing)
            {
                state = state->process(*this, token);
                token = scanner.getNextToken();
            }
        }
        filestr.close();
        return true;
    }
    catch (...)
    {
        cerr << "ERROR: Exception caught reading configuration file <" <<
            this->fFileName << ">" << endl;
    }
    return false;
}

bool
AConfigurationFile::write()
{
    try {
        fstream filestr(this->fFileName,
                fstream::in|fstream::out|fstream::trunc);
        if (!filestr.good())
        {
            cerr << "ERROR: Error opening configuration file <" <<
                this->fFileName << ">" << endl;
            filestr.close();
            return false;
        }

        filestr << this->fValues.encode() << flush << endl;
        if (!filestr.good())
        {
            cerr << "ERROR: Error writing configuration file <" <<
                this->fFileName << ">" << endl;
            filestr.close();
            return false;
        }
        filestr.close();
        return true;
    }
    catch (...)
    {
        cerr << "ERROR: Exception caught writing configuration file <" <<
            this->fFileName << ">" << endl;
    }
    return false;
}

void
AConfigurationFile::setSection(std::string name)
{
    if (this->fValues.sectionExists(name))
    {
        cerr << "ERROR: Section <" << name << "> already exists" << endl;
    }
    this->fSection = name;
}

void
AConfigurationFile::setKeyValue(std::string value)
{
    cout << "set(" << this->fSection << ", " << this->fKeyName << ")=<" <<
        value << ">" << endl;
    this->fValues.set(this->fSection, this->fKeyName, value);
    return;
}

void
AConfigurationFile::setKeyValue(int value)
{
    std::stringstream strung;
    strung << value;
    this->setKeyValue(strung.str());
    return;
}

void
AConfigurationFile::setKeyValue(float value)
{
    std::stringstream strung;
    strung << value;
    this->setKeyValue(strung.str());
    return;
}

void
AConfigurationFile::setKeyValue(double value)
{
    std::stringstream strung;
    strung << value;
    this->setKeyValue(strung.str());
    return;
}

std::string
AConfigurationFile::get(const std::string& section, const std::string& key)
{
    cout << "get(" << section << ", " << key << ")=<" <<
        this->fValues.get(section,key) << ">" << endl;
    return(this->fValues.get(section, key));
}

int
AConfigurationFile::getInt(const std::string& section, const std::string& key)
{
    return(atoi(this->get(section, key).c_str()));
}

float
AConfigurationFile::getFloat(const std::string& section, const std::string& key)
{
    return(atof(this->get(section, key).c_str()));
}
