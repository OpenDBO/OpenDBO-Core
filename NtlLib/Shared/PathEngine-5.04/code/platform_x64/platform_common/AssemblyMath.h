//**********************************************************************
//
// Copyright (c) 2000-2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

namespace nAssemblyMath
{

inline void add(unsigned long source, unsigned long& dest, unsigned long& carry)
{
    unsigned long result = dest + source;
    carry = 0;
    if(result < dest)
    {
        carry = 1;
    }
    dest = result;
}
inline void adc(unsigned long source, unsigned long& dest, unsigned long& carry)
{
    unsigned long result = dest + carry;
    carry = 0;
    if(result < dest)
    {
        carry = 1;
    }
    result += source;
    if(result < source)
    {
        carry = 1;
    }
    dest = result;
}
inline void sub(unsigned long source, unsigned long& dest, unsigned long& borrow)
{
    unsigned long result = dest - source;
    borrow = 0;
    if(result > dest)
    {
        borrow = 1;
    }
    dest = result;
}
inline void sbb(unsigned long source, unsigned long& dest, unsigned long& borrow)
{
    unsigned long result = dest - source;
    if(result > dest)
    {
        result -= borrow;
        borrow = 1;
    }
    else if(result == 0)
    {
        result -= borrow;
    }
    else
    {
        result -= borrow;
        borrow = 0;
    }
    dest = result;
}

inline void
mul(unsigned long source1, 
                unsigned long source2, 
                unsigned long& destHigh, 
                unsigned long& destLow)
{
    __int64 result = source1;
    result *= source2;
    destLow = static_cast<unsigned long>(result);
    result >>= 32;
    destHigh = static_cast<unsigned long>(result);
}
inline void
imul(long source1, 
                long source2, 
                unsigned long& destHigh, 
                unsigned long& destLow)
{
    __int64 result = source1;
    result *= source2;
    destLow = static_cast<unsigned long>(result);
    result >>= 32;
    destHigh = static_cast<unsigned long>(result);
}
inline void
div(unsigned long sourceHigh, 
                unsigned long sourceLow,
                unsigned long divisor, 
                unsigned long& dest,
                unsigned long& remainder
                )
{
    unsigned __int64 asI64;
    asI64 = sourceHigh;
    asI64 <<= 32;
    asI64 += sourceLow;
    unsigned __int64 result = asI64 / divisor;
    unsigned __int64 remultiplied = result * divisor;
    remainder = static_cast<unsigned long>(asI64 - remultiplied);
    dest = static_cast<unsigned long>(result);
}

}
