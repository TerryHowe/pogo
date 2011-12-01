#ifndef h_ASection
#define h_ASection

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ASection contains a section name and key/value pairs for that section.
// Vectors are used, rather than maps, to retain the key/value ordering
// so subsequent writes will retain the order.

class ASection 
{
public:
   ASection(const char* sectionName);
   ~ASection();

   string GetName();
   string* GetKeyValue(string key);
   bool Parse(ifstream& inStream);
   void Print(ostream& outStream);

private:
   void CleanUp();

   string fName;
   vector<string> fKeys;
   vector<string> fValues;
};

#endif
