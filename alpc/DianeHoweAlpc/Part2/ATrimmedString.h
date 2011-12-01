#ifndef h_ATrimmedString
#define h_ATrimmedString

#include <string>

using namespace std;

// ATrimmedString will accept various constuctions of a string
// and will strip leading and/or ending white space

class ATrimmedString 
{
public:
   ATrimmedString(string str, size_t startPosition);
   ATrimmedString(string str, size_t startPosition, size_t endPosition);
   ~ATrimmedString();

   string GetValue();

private:
   string fValue;
};

#endif
