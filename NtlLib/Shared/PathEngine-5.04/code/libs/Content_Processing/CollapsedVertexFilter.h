//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "VerticalEdgeCollapses.h"
#include "i_pathengine.h"

class cCollapsedVertexFilter : public iFaceVertexMesh
{
    const iFaceVertexMesh& _base;
    const cVerticalEdgeCollapses& _collapses;

public:

    cCollapsedVertexFilter(const iFaceVertexMesh& base, const cVerticalEdgeCollapses& collapses) :
     _base(base),
     _collapses(collapses)
    {
    }

    long getCollapsedIndex(long face, long vertexInFace) const
    {
        return _collapses.getCollapsedIndex(_base.vertexIndex(face, vertexInFace));
    }
    bool faceWasCollapsed(long face) const
    {
        return _collapses.faceWasCollapsed(face);
    }
    void vertexZRange(long collapsedVertex, float& minZ, float& maxZ) const
    {
        _collapses.vertexZRange(collapsedVertex, minZ, maxZ);
    }

// iFaceVertexMesh

    long faces() const
    {
        return _base.faces();
    }
    long vertices() const
    {
        return _base.vertices();
    }
    long vertexIndex(long face, long vertexInFace) const
    {
        assertD(!_collapses.faceWasCollapsed(face));
        return _collapses.getCollapsedIndex(_base.vertexIndex(face, vertexInFace));
    }
    long vertexX(long vertex) const
    {
        return _base.vertexX(vertex);
    }
    long vertexY(long vertex) const
    {
        return _base.vertexY(vertex);
    }
    float vertexZ(long vertex) const
    {
        return _base.vertexZ(vertex);
    }
    long faceAttribute(long face, long attributeIndex) const
    {
        return _base.faceAttribute(face, attributeIndex);
    }
};
