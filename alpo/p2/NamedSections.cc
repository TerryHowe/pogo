#include <iostream>

#include "NamedSections.h"

using namespace std;


ANamedSections::ANamedSections()
{
}

ANamedSections::~ANamedSections()
{
}

bool
ANamedSections::sectionExists(const string& section)
{
    ASections::iterator it;
    if ((it = this->fSections.find(section)) == this->fSections.end())
    {
        return false;
    }
    return true;
}

void
ANamedSections::set(const string& section,
                    const string& key,
                    const string& value)
{
    ASections::iterator it;
    if ((it = this->fSections.find(section)) == this->fSections.end())
    {
        ASectionPair pair;
        pair.first = section;
        this->fSections.insert(pair);
        if ((it = this->fSections.find(section)) == this->fSections.end())
        {
            cerr << "ERROR: Error inserting value in map" << endl;
            return;
        }
    }

    if ((*it).second.find(key) != (*it).second.end())
    {
        cerr << "ERROR: Duplicate key <" << key << "> in section <" << section
            <<">" << endl;
        return;
    }
    (*it).second.insert(AKeyValuePair(key, value));
    return;
}

string
ANamedSections::get(const string& section, const string& key)
{
    ASections::iterator it;
    if ((it = this->fSections.find(section)) == this->fSections.end())
    {
        return("");
    }
    AKeyValues::iterator itkv;
    if ((itkv = (*it).second.find(key)) == (*it).second.end())
    {
        return("");
    }
    return (*itkv).second;
}

string
ANamedSections::encode()
{
    ASections::iterator itSections;
    AKeyValues::iterator itValues;
    std::string retValue("");

    itSections = this->fSections.begin();
    while (itSections != this->fSections.end())
    {
        itValues = (*itSections).second.begin();
        retValue += "[" + (*itSections).first + "]\n";
        while (itValues != (*itSections).second.end())
        {
            retValue += (*itValues).first + ":" + (*itValues).second + "\n";
            ++itValues;
        }
        retValue += "\n";
        ++itSections;
    }
    return retValue;
}
