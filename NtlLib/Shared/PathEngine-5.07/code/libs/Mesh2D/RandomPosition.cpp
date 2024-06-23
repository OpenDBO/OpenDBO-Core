//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/RandomPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/FaceBounds.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/iRandomSequence.h"

cInternalPosition
RandomPosition(tMesh& mesh, long internalFacesInMesh, iRandomSequence& randomSequence, long tries)
{
    long i;
    for(i = 0; i < tries; i++)
    {
        long r = randomSequence.random31();
        long rX = randomSequence.random31();
        long rY = randomSequence.random31();

        long faceIndex = r % internalFacesInMesh;

        tFace f = mesh.faceAt(faceIndex);

        tPoint bottomLeft, topRight;
        GetFaceBounds<tMesh>(f, bottomLeft, topRight);

        tPoint range = topRight - bottomLeft;

        tPoint p(bottomLeft.getX() + rX % range.getX(),
                 bottomLeft.getY() + rY % range.getY(),
                 WORLD_RANGE);

        if(PointIsInFace<tMesh>(f, p))
        {
            return cInternalPosition(f, p);
        }
    }
    return cInternalPosition(tMesh::endFace());
}

cInternalPosition
RandomPosition(tMesh& mesh, long internalFacesInMesh, iRandomSequence& randomSequence)
{
    while(1)
    {
        long r = randomSequence.random31();
        long rX = randomSequence.random31();
        long rY = randomSequence.random31();

        long faceIndex = r % internalFacesInMesh;

        tFace f = mesh.faceAt(faceIndex);

        tPoint bottomLeft, topRight;
        GetFaceBounds<tMesh>(f, bottomLeft, topRight);

        tPoint range = topRight - bottomLeft;

        tPoint p(bottomLeft.getX() + rX % range.getX(),
                 bottomLeft.getY() + rY % range.getY(),
                 WORLD_RANGE);

        if(PointIsInFace<tMesh>(f, p))
        {
            return cInternalPosition(f, p);
        }
    }
}
