//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VERTICAL_EDGE_COLLAPSES_INCLUDED
#define VERTICAL_EDGE_COLLAPSES_INCLUDED

#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;

class cVerticalEdgeCollapses
{
    std::vector<long> _vertexCollapseTable;
    cBitVector _faceWasCollapsed;
    std::vector<float> _zRanges;

    bool collapseEdgeIfVertical(const iFaceVertexMesh& mesh, long v0, long v1);

public:

    cVerticalEdgeCollapses(const iFaceVertexMesh& mesh, cBitVector& faceSkipFlags);

    long getCollapsedIndex(long vertex) const
    {
        return _vertexCollapseTable[vertex];
    }
    bool faceWasCollapsed(long faceIndex) const
    {
        return _faceWasCollapsed[faceIndex];
    }
    void vertexZRange(long vertex, float& minZ, float& maxZ) const
    {
        minZ = _zRanges[vertex * 2];
        maxZ = _zRanges[vertex * 2 + 1];
    }
};

#endif
