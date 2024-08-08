//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMBINE_EDGE_EXPANSION_PIECES_INCLUDED
#define COMBINE_EDGE_EXPANSION_PIECES_INCLUDED

#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "common/STL/vector.h"

class cQueryContext;
class cCircuitElement;
class cCollisionShape;

// the input vector will be clearer and no reference to these pieces should be retained externally
void
CombineEdgeExpansionPieces(
        cQueryContext& qc,
        std::vector<cCircuitElement*>& pieces,
        long facesInMesh,
        const cCollisionShape& expandShape,
        tMeshCutSequence& result
        );

#endif
