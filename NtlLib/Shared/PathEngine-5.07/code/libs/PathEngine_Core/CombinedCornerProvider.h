//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

template <class T1, class T2>
class cCombinedCornerProvider
{
    const T1& base1;
    const T2& base2;
public:
    cCombinedCornerProvider(const T1& base1, const T2& base2) :
     base1(base1),
     base2(base2)
    {
    }
    long size() const
    {
        return base1.size() + base2.size();
    }
    tFace cornerFace(long index) const
    {
        if(index < base1.size())
        {
            return base1.cornerFace(index);
        }
        return base2.cornerFace(index - base1.size());
    }
    tPoint cornerPoint(long index) const
    {
        if(index < base1.size())
        {
            return base1.cornerPoint(index);
        }
        return base2.cornerPoint(index - base1.size());
    }
    tPoint axisBefore(long index) const
    {
        if(index < base1.size())
        {
            return base1.axisBefore(index);
        }
        return base2.axisBefore(index - base1.size());
    }
    tPoint axisAfter(long index) const
    {
        if(index < base1.size())
        {
            return base1.axisAfter(index);
        }
        return base2.axisAfter(index - base1.size());
    }
};
