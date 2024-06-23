//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class cMesh;
class cShape;
class cCollisionContext;
class cInternalPosition;

void
GenerateCurvedPath(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path, const std::vector<long>* connectionIndices, long startOffset,
        long startVectorX, long startVectorY,
        long sectionLength,
        float turnRatio1, float turnRatio2,
        std::vector<cInternalPosition>& pathResult, std::vector<long>*& connectionIndicesResult
        );
