#ifndef h_NamedSections
#define h_NamedSections

#include <ostream>
#include <string>
#include <map>

#include "AlTypes.h"
#include "Scanner.h"

using namespace std;

typedef map<string, string> AKeyValues;
typedef pair<string, string> AKeyValuePair;
typedef map<string, AKeyValues> ASections;
typedef pair<string, AKeyValues> ASectionPair;

//
// ANamedSections stores sections in a hash map of key value pairs
//
class ANamedSections
{
private:
    ASections fSections;
public:
    ANamedSections();
    ~ANamedSections();
    bool sectionExists(const string& section);
    void set(const string& section, const string& key, const string& value);
    string get(const string& section, const string& key);
    string encode();
};

#endif
