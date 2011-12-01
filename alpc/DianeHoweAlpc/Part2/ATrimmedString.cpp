#include <iostream>
#include "ATrimmedString.h"

ATrimmedString::ATrimmedString(string str, size_t startPosition)
{
   // Trim whitespace off the front

   char* strValue = NULL;

   if (startPosition > str.size() || startPosition == string::npos)
      return;

   while (startPosition <= str.size() && isspace(str[startPosition]))
   {
      ++startPosition;
   }
   if (startPosition > str.size())
      return;

   strValue = new char[str.size() - startPosition + 2];
   memset(strValue, 0, sizeof(strValue));
   memcpy(strValue, &str[startPosition], str.size() - startPosition + 1);

   fValue.assign(strValue);
   delete[] strValue;
}

ATrimmedString::ATrimmedString(string str, 
 size_t startPosition, size_t endPosition)
{
   // Trim whitespace off both the front and back

   char* strValue = NULL;

   if(startPosition > endPosition || endPosition > str.size() || 
      startPosition == string::npos || endPosition == string::npos)
      return;

   while (endPosition >= startPosition && isspace(str[endPosition])) 
   {
      --endPosition;
   }
   if (endPosition < startPosition)
      return;

   while (startPosition <= endPosition && isspace(str[startPosition]))
   {
      ++startPosition;
   }
   if (startPosition > endPosition)
      return;

   strValue = new char[endPosition - startPosition + 2];
   memset(strValue, 0, endPosition - startPosition + 2);
   memcpy(strValue, &str[startPosition], endPosition - startPosition + 1 );

   fValue.assign(strValue);
   delete[] strValue;
}

ATrimmedString::~ATrimmedString()
{
   fValue.clear();
}


string ATrimmedString::GetValue()
{
   return fValue;
}
