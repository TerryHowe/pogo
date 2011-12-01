#include <iostream>
#include <sstream>
#include <math.h>

#include "FixedPoint.h"

using namespace std;


AFixedPoint::AFixedPoint() : fValue(0)
{
}

AFixedPoint::AFixedPoint(UInt32 val) : fValue(val)
{
}

AFixedPoint::AFixedPoint(float val) : fValue(0)
{
    setValue(val);
}

AFixedPoint::~AFixedPoint()
{
}

void
AFixedPoint::setValue(UInt32 val)
{
    this->fValue = val;
}

void
AFixedPoint::setValue(float val)
{
    double partInteger;
    double partFraction;
    this->fValue = 0x0;
    partFraction = modf(val, &partInteger);
    partFraction *= kFractionDenominator;
    if (partInteger < 0)
    {
        this->fValue = kSignBit;
        partInteger = -partInteger;
    }
    this->fValue |= ((UInt16)partInteger) << kFractionBits;
    this->fValue |= (((UInt16)partFraction) & kFractionMask);
    return;
}

float
AFixedPoint::getFloat()
{
    float retVal = 0.0;

    retVal = ((this->fValue >> kFractionBits)&kDecimalMask) +
        (((float)(this->fValue&kFractionMask))/(float)(kFractionDenominator));
    if (this->fValue & kSignBit)
        retVal = -retVal;
    retVal = round(retVal * kMultiplier) / kMultiplier;
    return retVal;
}

std::string
AFixedPoint::toString()
{
    std::stringstream strung;
    strung << this->getFloat();
    return strung.str();
}
