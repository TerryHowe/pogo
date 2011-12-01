#ifndef h_AFileParser
#define h_AFileParser

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ASection.h"

using namespace std;

// AFileParser will parse the following sample file format and return
// targetted information from the file.
// 
// [section name 1]
// key: value
// key2 : value
//
// [section name 2]
// ...
// 
// Note that AFileParser will read/parse the entire contents of the file
// into memory, so it will not handle dynamic changes to the file being
// parsed.  

class AFileParser 
{
public:
   AFileParser(const char* fileName);
   ~AFileParser();

   bool Read();
   void Print();
   void Write(string fileName);

   int GetSection(string section);

   string GetStringValue(string section, string key);
   int GetIntValue(string section, string key);
   float GetFloatValue(string section, string key);

   void SetValue(string section, string key, string newValue, string newFile);
   void SetValue(string section, string key, int newValue, string newFile);
   void SetValue(string section, string key, float newValue, string newFile);
   
private:
   void CleanUp();
   string* GetStringPointer(string section, string key);

   string fFileName;
   vector<ASection> fSections;

   ifstream fInStream;
};

#endif
