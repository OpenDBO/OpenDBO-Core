//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ShapeCorners.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/QueryContext.h"
#include "interface/CornerProvider.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"

void
AddShapeCorners(cQueryContext& qc, const cLocalisedConvexShape& shape, cCornerProvider& addTo)
{
    const std::vector<long>& faces = shape.refFacesOverlapped();
    long i;
    for(i = 0; i < shape.sides(); ++i)
    {
        tPoint p = shape[i + 1];
        long j;
        for(j = 0; j < SizeL(faces); ++j)
        {
            tFace f = qc.refMesh().faceAt(faces[j]);
            if(!PointIsInFace<tMesh>(f, p))
            {
                continue;
            }
            addTo.push_back(f, p, p - shape[i], shape[i + 2] - p);
        }
    }
}

void
AddShapeCorners(
        cQueryContext& qc,
        const cLocalisedConvexShape& shape,
        const tPoint& boundsMin,
        const tPoint& boundsMax,
        cCornerProvider& addTo
        )
{
    const std::vector<long>& faces = shape.refFacesOverlapped();
    long i;
    for(i = 0; i < shape.sides(); ++i)
    {
        tPoint p = shape[i + 1];
        if(!p.insideOrOnBounds(boundsMin, boundsMax))
        {
            continue;
        }
        long j;
        for(j = 0; j < SizeL(faces); ++j)
        {
            tFace f = qc.refMesh().faceAt(faces[j]);
            if(!PointIsInFace<tMesh>(f, p))
            {
                continue;
            }
            addTo.push_back(f, p, p - shape[i], shape[i + 2] - p);
        }
    }
}
