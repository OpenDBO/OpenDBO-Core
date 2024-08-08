// **********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
// **********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/STL/vector.h"

class cExternalShapeInfo;

bool
CutExternalShape(
        tMesh_3D_FaceTracked& mesh,
        tFace_3D_FaceTracked& shapeExternalFace,
        const std::vector<tEdge_3D_FaceTracked>& edgeBehindPortal,
        const std::vector<tPoint>& edgeBehindPortalPoints
        );
