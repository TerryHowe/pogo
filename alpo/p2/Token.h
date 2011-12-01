#ifndef h_Token
#define h_Token

#include <string>

#include "AlTypes.h"

//
// The token class handles the parsing of tokens and uses type and value
// to identify the token and its value
//
class AToken
{
public:
    enum Type
    {
        kNothing,
        kUnexpected,
        kLeftBracket,
        kRightBracket,
        kColon,
        kString
    };
    AToken();
    ~AToken();
    std::string parse(std::string input);
    void setType(const Type val) { this->fType = val; }
    void setValue(const std::string val) { this->fValue = val; }
    Type getType() const { return this->fType; }
    std::string getValue() const { return this->fValue; }
private:
    static const char* const kWhiteSpace;
    static const char* const kTokenString;
    Type fType;
    std::string fValue;
};

#endif
