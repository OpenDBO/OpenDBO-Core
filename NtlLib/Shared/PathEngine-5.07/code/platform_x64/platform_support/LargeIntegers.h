//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LARGE_INTEGERS_INCLUDED
#define LARGE_INTEGERS_INCLUDED

#define LARGE_INTEGERS_TYPE_2

#include "common/interface/Assert.h"
#include <intrin.h>
#include <limits.h>

#pragma intrinsic(_umul128)

namespace nLargeIntegers
{

inline void
multiplyTo128Bits(
        __int64 lhs, __int64 rhs,
        bool& isNegative, unsigned __int64& resultHigh, unsigned __int64& resultLow
        )
{
    isNegative = false;
    if(lhs < 0)
    {
        assertD(lhs != LLONG_MIN);
        lhs = -lhs;
        isNegative = true;
    }
    unsigned __int64 lhsU = static_cast<unsigned __int64>(lhs);
    if(rhs < 0)
    {
        assertD(rhs != LLONG_MIN);
        rhs = -rhs;
        isNegative = !isNegative;
    }
    unsigned __int64 rhsU = static_cast<unsigned __int64>(rhs);
	resultLow = _umul128(lhsU, rhsU, &resultHigh);
}

// dividend and divisor must be such that quotientResult fits in a long
// note, also, the limit on the range of divisor
inline void
divideFrom128Bits(bool dividendIsNegative, unsigned __int64 dividendHigh, unsigned __int64 dividendLow, __int64 divisor, long& quotientResult, bool& hasRemainder)
{
    assertD(divisor != 0);
    assertD(dividendHigh >= 0);

// normalise to make everything unsigned

    bool negateResult = dividendIsNegative;
    if(divisor < 0)
    {
        assertD(divisor != LLONG_MIN);
        negateResult = !negateResult;
        divisor = -divisor;
    }

    assertD(divisor <= LLONG_MAX / 2);

//// check for dividend zero or divisor greater than or equal to dividend
//
//    if(dividendHigh == 0)
//    {
//        if(dividendLow == 0)
//        {
//            quotientResult = 0;
//            hasRemainder = false;
//            return;
//        }
//        // remember that dividendLow is actually a full unsigned 64 but value
//        // dividendLow < 0 would indicate, therefore, that the top bit is set
//        if(dividendLow > 0 && divisor >= dividendLow)
//        {
//            if(divider == dividendLow)
//            {
//                quotientResult = 1;
//                hasRemainder = false;
//                if(negateResult)
//                {
//                    quotientResult = -1;
//                }
//                return;
//            }
//            // divisor > dividendLow
//            quotientResult = 0;
//            hasRemainder = true;
//            return;
//        }
//    }

// repeatedly shift up into a single digit with high bit set (or at end of divisor)

    const __int64 highMask = LLONG_MAX / 2 + 1;
    const __int64 lowMask = LLONG_MIN;

    __int64 quotientResult_I64 = 0;

    __int64 toDivide = dividendHigh;
    __int64 below = dividendLow;
    int bitPosition = 64;
    do
    {
        while((toDivide & highMask) == 0 && bitPosition > 0)
        {
            toDivide *= 2;
            if(below & lowMask)
            {
                toDivide++;
            }
            below *= 2;
            bitPosition--;
        }
        __int64 partialQuotient = toDivide / divisor;
        quotientResult_I64 += partialQuotient << bitPosition;
        __int64 remultiplied = partialQuotient * divisor;
        toDivide -= remultiplied;
    }
    while(bitPosition > 0);
    hasRemainder = toDivide != 0;
    assertD(quotientResult_I64 <= 0x7ffffffffL);
    quotientResult = static_cast<long>(quotientResult_I64);
    if(negateResult)
    {
        quotientResult = -quotientResult;
    }
}

}

#endif
