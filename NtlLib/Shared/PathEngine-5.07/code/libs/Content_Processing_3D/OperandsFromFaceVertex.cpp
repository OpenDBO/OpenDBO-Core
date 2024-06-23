//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "OperandsFromFaceVertex.h"
#include "tMesh_Simple3D.h"
#include "platform_common/CoordinateRanges.h"
#include "i_pathengine.h"
#include "common/interface/Error.h"

typedef tMesh_Simple3D::tFaceData tFaceData;
typedef tMesh_Simple3D::tEdgeData tEdgeData;
typedef tMesh_Simple3D::tVertexData tVertexData;

static tMesh_Simple3D*
OperandFromFace(const iFaceVertexMesh& faceVertex, long faceIndex)
{
    tMesh_Simple3D* result = new tMesh_Simple3D;
    tEdgeData edges[3];
    tVertexData vertices[3];
    long i;
    for(i = 0; i != 3; ++i)
    {
        long v = faceVertex.vertexIndex(faceIndex, i);
        tPoint3 p(faceVertex.vertexX(v), faceVertex.vertexY(v), static_cast<long>(faceVertex.vertexZ(v)), WORLD_RANGE);
        vertices[i] = tVertexData(p);
    }
    result->addFace(tFaceData(), 3, edges, vertices);
    return result;
}

static bool
VertsIdentical(const iFaceVertexMesh& faceVertex, long v1, long v2)
{
    return faceVertex.vertexX(v1) == faceVertex.vertexX(v2)
            && faceVertex.vertexY(v1) == faceVertex.vertexY(v2)
            && static_cast<long>(faceVertex.vertexZ(v1)) == static_cast<long>(faceVertex.vertexZ(v2))
            ;
}
static bool
VertIsOutsideRange(const iFaceVertexMesh& faceVertex, long v)
{
    return faceVertex.vertexX(v) < -WORLD_RANGE
        || faceVertex.vertexX(v) > WORLD_RANGE
        || faceVertex.vertexY(v) < -WORLD_RANGE
        || faceVertex.vertexY(v) > WORLD_RANGE
        || static_cast<long>(faceVertex.vertexZ(v)) < -WORLD_RANGE
        || static_cast<long>(faceVertex.vertexY(v)) > WORLD_RANGE
        ;
}
static bool
FaceIsOutsideRange(const iFaceVertexMesh& faceVertex, long f)
{
    return VertIsOutsideRange(faceVertex, faceVertex.vertexIndex(f, 0))
        || VertIsOutsideRange(faceVertex, faceVertex.vertexIndex(f, 1))
        || VertIsOutsideRange(faceVertex, faceVertex.vertexIndex(f, 2))
        ;
}
static bool
FaceHasArea(const iFaceVertexMesh& faceVertex, long f)
{
    long v0 = faceVertex.vertexIndex(f, 0);
    long v1 = faceVertex.vertexIndex(f, 1);
    long v2 = faceVertex.vertexIndex(f, 2);
    if(VertsIdentical(faceVertex, v0, v1))
    {
        return false;
    }
    if(VertsIdentical(faceVertex, v0, v2))
    {
        return false;
    }
    if(VertsIdentical(faceVertex, v1, v2))
    {
        return false;
    }
    return true;
}

void
OperandsFromFaceVertex(const iFaceVertexMesh& faceVertex, std::vector<tMesh_Simple3D*>& appendTo)
{
//    bool reportedFaceWithNoArea = false;
    bool reportedFaceOutsideRange = false;
    long i;
    for(i = 0; i != faceVertex.faces(); ++i)
    {
        if(FaceIsOutsideRange(faceVertex, i))
        {
            if(!reportedFaceOutsideRange)
            {
                Error("NonFatal", "OperandsFromFaceVertex() - Mesh contains one or more faces which are outside the supported coordinate range. These will be ignored.");
                reportedFaceOutsideRange = true;
            }
            continue;
        }
        if(!FaceHasArea(faceVertex, i))
        {
//            if(!reportedFaceWithNoArea)
//            {
//                Error("Warning", "OperandsFromFaceVertex() - Mesh contains one or more faces with no area. These will be ignored.");
//                reportedFaceWithNoArea = true;
//            }
            continue;
        }
        appendTo.push_back(OperandFromFace(faceVertex, i));
    }
}
