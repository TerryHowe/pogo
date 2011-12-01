#include "ASection.h"
#include "ATrimmedString.h"

ASection::ASection(const char* sectionName)
   :fName(sectionName)
{
}

ASection::~ASection()
{
   CleanUp();
}

void ASection::CleanUp()
{
   fName.clear();
}

string ASection::GetName()
{
   return fName;
}

bool ASection::Parse(ifstream& inStream)
{
   // Parse the key/value pairs for this section, e.g.
   // key: value
   // key2 : value
   // key names must appear in column 0 and subsequent lines in value
   // must have whitespace in column 0
   // Note that only whitespace around ":" is discarded, as is the
   // CRLF at end of value.  There is no requirement to trim the whitespace
   // of key values further.

   string buffer;
   char nextChar = '\0';

   while (!inStream.eof())
   {
      nextChar = inStream.peek();
      if (nextChar == '[')
      {
         break;
      }
      getline(inStream, buffer);
      size_t separator = buffer.find_first_of(':');
      if(separator !=  string::npos)
      {
         string value;

         ATrimmedString key(buffer, 0, separator - 1);
         ATrimmedString partialValue(buffer, separator + 1);
         value += partialValue.GetValue();
         // Keep reading until entire value is found
         nextChar = inStream.peek();
         while (isspace(nextChar))
         {
            buffer.clear();
            getline(inStream, buffer);
            if(buffer.size() == 0)
            {
               break;
            }
            value += buffer;
            nextChar = inStream.peek();
         }

         fKeys.push_back(key.GetValue());
         fValues.push_back(value);
      }
      else
      {
         break;
      }
   }

   return true;
}

string* ASection::GetKeyValue(string key)
{
   string* returnValue = NULL;
   for (int i = 0; i < fKeys.size(); ++i)
   {
      if (fKeys[i] == key)
      {
         returnValue = &fValues[i];
         break;
      }
   }
   return returnValue;
}

void ASection::Print(ostream& outStream)
{
   outStream << "[" << GetName() << "]" << endl;
   for (int i = 0; i < fKeys.size(); ++i)
   {
      outStream << fKeys[i] << ": " << fValues[i] << endl;
   }
}
