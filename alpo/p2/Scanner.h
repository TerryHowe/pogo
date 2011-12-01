#ifndef h_Scanner
#define h_Scanner

#include <string>

#include "AlTypes.h"
#include "Token.h"

//
// Scanner is used to accumulate a line for parsing.  The token classes are
// used to parse and create new tokens
//
class AScanner
{
private:
    std::string fLine;
public:
    AScanner();
    ~AScanner();
    std::string& getLine() { return fLine; }
    AToken getNextToken();
};

#endif
