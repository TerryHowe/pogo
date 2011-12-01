#include <inttypes.h>

#include "AFileParser.h"
#include "ATrimmedString.h"

AFileParser::AFileParser(const char* fileName)
   : fFileName(fileName)
{
}

AFileParser::~AFileParser()
{
   CleanUp();
}

bool AFileParser::Read()
{
   // Open and read in config file being parsed.  If file cannot be found
   // or no sections exist, return false, otherwise return true.
   // Note: if a section fails to parse, the program continues but that
   // section is not included in Config File return results.

   string buffer;

   fInStream.open(fFileName.data());
   if (fInStream.fail())
   {
      cerr << "File doesn't exist or failed to open\n";
      return false;
   }

   while (!fInStream.eof())
   {
      buffer.clear();
      getline(fInStream, buffer);

      if (buffer[0] != '[')
         continue;
      size_t rightBracket = buffer.find_first_of(']');
      if (rightBracket == string::npos)
         continue;

      ATrimmedString sectionName(buffer, 1, rightBracket - 1);
      if (!sectionName.GetValue().empty())
      {
         ASection nextSection(sectionName.GetValue().data());
         if(nextSection.Parse(fInStream))
         {
            fSections.push_back(nextSection);
         }
         else
         {
            cerr << "There is an unparseable section for [" 
             << nextSection.GetName() << "]" << endl; 
         }
      }
   }

   if(fSections.empty())
   {
      cerr << "File is empty or no sections parsed\n";
      return false;
   }

   fInStream.close();
   return true;
}

void AFileParser::Write(string fileName)
{
   if (fSections.empty())
   {
      cout << "Empty Config File\n";
      return;
   }
   ofstream outStream;
   outStream.open(fileName.data());
   if (outStream.fail())
   {
      cerr << "Error opening file to write: " << fileName << endl;
      return;
   }

   for (int i = 0; i < fSections.size(); i++)
   {
      fSections.at(i).Print(outStream);
   }

   outStream.close();
}

void AFileParser::Print()
{
   cout << "Config File: " << fFileName << endl;
   if (fSections.empty())
   {
      cout << "Empty Config File\n";
      return;
   }

   for (int i = 0; i < fSections.size(); i++)
   {
      fSections.at(i).Print(cout);
   }
}

string AFileParser::GetStringValue(string section, string key)
{
   string returnValue;
   int sectionIndex = GetSection(section);
   if(sectionIndex == -1)
   {
      throw("Cannot find section");
   }
   string* str = fSections.at(sectionIndex).GetKeyValue(key);
   if(str != NULL)
   {
      returnValue = *str;
   }

   return returnValue;
}

string* AFileParser::GetStringPointer(string section, string key)
{
   string* returnValue = NULL;  
   int sectionIndex = GetSection(section);
   if(sectionIndex == -1)
   {
      throw("Cannot find section");
   }
   returnValue = fSections.at(sectionIndex).GetKeyValue(key);
   return returnValue;
}

int AFileParser::GetIntValue(string section, string key)
{
   int returnValue = 0;

   string* strValue = GetStringPointer(section, key);
   if (strValue != NULL)
   {
      returnValue = strtoimax((*strValue).data(), NULL, 10);
   }
   else
   {
      throw("Cannot find key/value");
   }

   return returnValue;
}

float AFileParser::GetFloatValue(string section, string key)
{
   float returnValue = 0;
   string* strValue = GetStringPointer(section, key);
   if(strValue != NULL)
   {
      sscanf((*strValue).data(), "%f", &returnValue);
   }
   else
   {
      throw("Cannot find key/value");
   }

   return returnValue;
}

void AFileParser::SetValue(string section, string key, 
 string newValue, string newFile)
{
   string* oldValue = GetStringPointer(section, key);
   if(oldValue == NULL)
   {
      cerr << "Section or key not found.  No update.\n";
      return;
   }
   (*oldValue).assign(newValue);
   Write(newFile);
}

void AFileParser::SetValue(string section, string key, 
 int newValue, string newFile)
{
   char buffer[128];
   sprintf(buffer, "%i", newValue);
   string newStringValue = buffer;
   SetValue(section, key, newStringValue, newFile);
}

void AFileParser::SetValue(string section, string key, 
 float newValue, string newFile)
{
   char buffer[128];
   sprintf(buffer, "%f", newValue);
   string newStringValue = buffer;
   SetValue(section, key, newStringValue, newFile);
}

int AFileParser::GetSection(string section)
{
   int index = 0;
   while (index < fSections.size())
   {
      if (fSections.at(index).GetName() == section)
         break;
      ++index;
   }

   if (index >= fSections.size())
      return -1;
   else
      return index;
}

void AFileParser::CleanUp()
{
}
