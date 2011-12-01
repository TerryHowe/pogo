#ifndef h_FixedPoint
#define h_FixedPoint

#include <string>

#include "AlTypes.h"

//
// This class handles fixed point numbers 16 bits decimal and 15 bits fraction.
// It can be constructed with an unsigned 32 or a float.
//
class AFixedPoint
{
private:
    UInt32 fValue;
    static const UInt32 kDecimalMask = 0xFFFF;
    static const UInt32 kFractionMask = 0x7FFF;
    static const UInt32 kFractionBits = 15;
    static const UInt32 kFractionDenominator = 0x8000;
    static const UInt32 kSignBit = 0x80000000;
    static const UInt32 kMultiplier = 10000.0;
public:
    AFixedPoint();
    AFixedPoint(UInt32 val);
    AFixedPoint(float val);
    ~AFixedPoint();
    void setValue(UInt32 val);
    void setValue(float val);
    float getFloat();
    UInt32 getValue() { return this->fValue; }
    std::string toString();
};

#endif
