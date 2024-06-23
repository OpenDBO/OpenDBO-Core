//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cQueryContext;
class cCollisionShape;
class cInternalPosition;
class cFlagVector;

void
CircuitsForOverlappingPolygon(
        tFace startF,
        const tPoint* pointVector,
        long points,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        );
void
CircuitsForOverlappingPolygon(
        const cInternalPosition& centre,
        const cCollisionShape& polygon,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        );

void
FacesOverlappedByOverlappingPolygon_SI(
        tFace centreF,
        const tPoint* pointArray,
        long points,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        );

void
FacesOverlappedByOverlappingPolygon_OF(
        const cInternalPosition& centre,
        const cCollisionShape& shape,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        );
void
FacesOverlappedByOverlappingPolygon_OF(
        tFace centreF, const tIntersection& centreP,
        const cCollisionShape& shape,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        );

bool
ShapeCollidesWithMeshEdge(
        cQueryContext& qc,
        const cInternalPosition& centre,
        const cCollisionShape& shape
        );

cHandle<tMeshCircuits> CircuitsForInfiniteCylinder(
        tMesh& mesh,
        const tPoint& centre,
        const std::vector<tPoint>& polygon,
        std::list<tFace>& containedExternalFaces
        );

cHandle<tMeshCircuits>
CircuitsForInfiniteCylinder(
        tMesh& mesh,
        const tPoint& centre,
        const cCollisionShape& polygon,
        std::list<tFace>& containedExternalFaces
        );

void
GetSquareBounds(
        const cInternalPosition& centre,
        long radius,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        );

cHandle<tMeshCircuits>
GetSquareBounds_InfiniteCylinder(
        tMesh& mesh,
        const tPoint& centre,
        long radius,
        std::list<tFace>& containedExternalFaces
        );

