#include "Scanner.h"

using namespace std;


AScanner::AScanner()
{
}

AScanner::~AScanner()
{
}

AToken
AScanner::getNextToken()
{
    AToken rt;
    this->fLine = rt.parse(this->fLine);
    return rt;
}
