//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CORNER_PROVIDER_INCLUDED
#define CORNER_PROVIDER_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/STL/vector.h"

class iXMLOutputStream;
class iXMLInputStream;

class cCornerProvider
{
    tMesh* _mesh;
    std::vector<tPoint> _pointAndAxisData;
    std::vector<long> _faces;

public:

    cCornerProvider(tMesh* mesh);

    void push_back(tFace f, const tPoint& p, const tPoint& axisBefore, const tPoint& axisAfter)
    {
        _faces.push_back(f.index());
        _pointAndAxisData.push_back(p);
        _pointAndAxisData.push_back(axisBefore);
        _pointAndAxisData.push_back(axisAfter);
    }

    void clearWithoutFreeingBuffers();
    void completelyClear();

    void shrinkToFit();

    void add(const cCornerProvider& rhs);
    void addFiltered(const cCornerProvider& rhs, const tPoint& boundsMin, const tPoint& boundsMax);

    void sort();

    void save(long numberToSave, iXMLOutputStream& os) const;
    void load(iXMLInputStream& is);

// used internally for the sort predicate
    
    bool operator()(long lhsIndex, long rhsIndex) const;

// interface to support the state space

    long size() const
    {
        return SizeL(_faces);
    }
    tFace cornerFace(long index) const
    {
        return _mesh->faceAt(_faces[index]);
    }
    tPoint cornerPoint(long index) const
    {
        return _pointAndAxisData[index * 3];
    }
    tPoint axisBefore(long index) const
    {
        return _pointAndAxisData[index * 3 + 1];
    }
    tPoint axisAfter(long index) const
    {
        return _pointAndAxisData[index * 3 + 2];
    }
};

#endif
