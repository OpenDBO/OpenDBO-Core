//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPORTER_COMMON__COMBINED_FACE_VERTEX_MESHES_INCLUDED
#define EXPORTER_COMMON__COMBINED_FACE_VERTEX_MESHES_INCLUDED

#include "i_pathengine.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"

class cBridgingModifier : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    std::vector<long> _baseVerts;
    std::vector<long> _baseFaces;
    std::vector<long> _extendedVertHorizontalCoords;
    std::vector<float> _extendedVertZs;

    long numberOfConnections() const
    {
        return SizeL(_baseFaces);
    }

    long
    extendedVertexX(long vertex) const
    {
        long connection = vertex / 6;
        switch(vertex % 6)
        {
        default:
            invalid();
        case 0:
        case 2:
            return _base->vertexX(_baseVerts[connection * 2]);
        case 1:
            return _base->vertexX(_baseVerts[connection * 2 + 1]);
        case 3:
        case 5:
            return _extendedVertHorizontalCoords[connection * 4];
        case 4:
            return _extendedVertHorizontalCoords[connection * 4 + 2];
        }
    }
    long
    extendedVertexY(long vertex) const
    {
        long connection = vertex / 6;
        switch(vertex % 6)
        {
        default:
            invalid();
        case 0:
        case 2:
            return _base->vertexY(_baseVerts[connection * 2]);
        case 1:
            return _base->vertexY(_baseVerts[connection * 2 + 1]);
        case 3:
        case 5:
            return _extendedVertHorizontalCoords[connection * 4 + 1];
        case 4:
            return _extendedVertHorizontalCoords[connection * 4 + 3];
        }
    }

    bool connectionIsOn(long i) const
    {
        return  extendedVertexX(i * 6 + 0) != extendedVertexX(i * 6 + 3)
            ||  extendedVertexY(i * 6 + 0) != extendedVertexY(i * 6 + 3)
            ||  extendedVertexX(i * 6 + 1) != extendedVertexX(i * 6 + 4)
            ||  extendedVertexY(i * 6 + 1) != extendedVertexY(i * 6 + 4)
            ;
    }

public:

    // base mesh must remain valid (and unmodified) for the lifetime of this object
    cBridgingModifier(const iFaceVertexMesh* base, const std::vector<long>& connectingEdgeInfo);

    void setConnectionTarget(long i, long startX, long startY, float startZ, long endX, long endY, float endZ);

// iFaceVertexMesh interface

    long faces() const
    {
        return _base->faces() + numberOfConnections() * 4;
    }
    long vertices() const
    {
        return _base->vertices() + numberOfConnections() * 6;
    }
    long vertexIndex(long face, long vertexInFace) const
    {
        if(face < _base->faces())
        {
            return _base->vertexIndex(face, vertexInFace);
        }
        face -= _base->faces();
        long connection = face / 4;
        face &= 3;
        long startV = _base->vertices() + connection * 6;
        switch(face)
        {
        default:
            invalid();
        case 0:
            switch(vertexInFace)
            {
            default:
                invalid();
            case 0:
                return startV + 0;
            case 1:
                return startV + 3;
            case 2:
                return startV + 1;
            }
        case 1:
            switch(vertexInFace)
            {
            default:
                invalid();
            case 0:
                return startV + 1;
            case 1:
                return startV + 3;
            case 2:
                return startV + 4;
            }
        case 2:
            switch(vertexInFace)
            {
            default:
                invalid();
            case 0:
                return startV + 0;
            case 1:
                return startV + 2;
            case 2:
                return startV + 1;
            }
        case 3:
            switch(vertexInFace)
            {
            default:
                invalid();
            case 0:
                return startV + 3;
            case 1:
                return startV + 4;
            case 2:
                return startV + 5;
            }
        }
    }
    long vertexX(long vertex) const
    {
        if(vertex < _base->vertices())
        {
            return _base->vertexX(vertex);
        }
        return extendedVertexX(vertex - _base->vertices());
    }
    long vertexY(long vertex) const
    {
        if(vertex < _base->vertices())
        {
            return _base->vertexY(vertex);
        }
        return extendedVertexY(vertex - _base->vertices());
    }
    float vertexZ(long vertex) const
    {
        if(vertex < _base->vertices())
        {
            return _base->vertexZ(vertex);
        }
        vertex -= _base->vertices();
        long connection = vertex / 6;
        switch(vertex % 6)
        {
        default:
            invalid();
        case 0:
            return _base->vertexZ(_baseVerts[connection * 2]);
        case 1:
            return _base->vertexZ(_baseVerts[connection * 2 + 1]);
        case 2:
            return _base->vertexZ(_baseVerts[connection * 2]) + 10.f;
        case 3:
            return _extendedVertZs[connection * 2];
        case 4:
            return _extendedVertZs[connection * 2 + 1];
        case 5:
            return _extendedVertZs[connection * 2] + 10.f;
        }
    }
    long faceAttribute(long face, long attributeIndex) const
    {
        if(face < _base->faces())
        {
            return _base->faceAttribute(face, attributeIndex);
        }
        face -= _base->faces();
        long connection = face / 4;
        face = (face & 3);
        if(face < 2)
        {
          // bridging geometry
            return _base->faceAttribute(_baseFaces[connection], attributeIndex);
        }
        // connecting edge markers
        if(attributeIndex == PE_FaceAttribute_MarksConnectingEdge)
        {
            if(connectionIsOn(connection))
            {
                return 1;
            }
            return 0;
        }
        return -1;
    }
};

#endif
