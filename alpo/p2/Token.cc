#include <stdio.h>
#include <istream>
#include <iostream>

#include "Token.h"

using namespace std;


const char* const AToken::kWhiteSpace = " \r\n\t\f\v";
const char* const AToken::kTokenString = ":[]";

AToken::AToken()
{
}

AToken::~AToken()
{
}

string AToken::parse(string input)
{
    size_t found;

    this->fValue = "";
    this->fType = kNothing;
    if (input[0] == '[')
    {
        this->fType = kLeftBracket;
        return input.substr(1);
    }

    //
    // Wack leading space
    //
    if (isspace(input[0]))
    {
        if ((found = input.find_first_not_of(kWhiteSpace)) == string::npos)
        {
            input = "";
            return input;
        }
        input = input.substr(found);
    }

    //
    // Look for reserved characters
    //
    switch (input[0])
    {
        case '[':
        this->fType = kUnexpected;
        return input.substr(1);

        case ']':
        this->fType = kRightBracket;
        return input.substr(1);

        case ':':
        this->fType = kColon;
        return input.substr(1);

        case '\000':
        return input;
    }

    //
    // A string
    //
    this->fType = kString;
    if ((found = input.find_first_of(kTokenString)) != string::npos)
    {
        this->fValue = input.substr(0, found);
        input = input.substr(found);
        if (input[0] == '[')
            this->fType = kUnexpected;
    }
    else {
        this->fValue = input;
        input = "";
    }

    //
    // Wack trailing spaces
    //
    string::iterator it = this->fValue.end();
    while (it>this->fValue.begin())
    {
        --it;
        if (! isspace(*it))
            break;
        this->fValue.erase(it);
    }

    return input;
}
