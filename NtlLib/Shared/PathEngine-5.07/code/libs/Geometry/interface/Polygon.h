//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

template <class T>
bool PolygonIsConvex(long sides, const T* vertexarray)
{
    long i;
    long j;
    long nexti;
    T base;
    T axis;
    T offset;

    for(i=0;i<sides;i++)
    {
        nexti=i+1;
        if(nexti==sides)
            nexti=0;

        base=vertexarray[i];
        axis=vertexarray[nexti]-base;

        for(j=0;j<sides;j++)
        {
            if(j == i || j == nexti)
            {
                continue;
            }
            offset=vertexarray[j]-base;
            if(axis.side(offset) != SIDE_RIGHT)
            // polygon is not convex (or not clockwise)
            {
                return false;
            }
        }
    }
    return true;
}

template <class T, class T2>
bool PointInPolygon(long sides, const T* vertexarray, const T2 &point)
{
    long i;
    long nexti;
    T axis;
    T2 offset;

    for(i=0;i<sides;i++)
    {
        nexti=i+1;
        if(nexti==sides)
            nexti=0;

        axis=vertexarray[nexti]-vertexarray[i];
        offset=point-vertexarray[i];
        if(axis.side(offset)!=SIDE_RIGHT)
            return false;
    }
    return true;
}
