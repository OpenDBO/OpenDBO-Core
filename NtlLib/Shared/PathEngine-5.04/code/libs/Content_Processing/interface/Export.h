//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPORT_INCLUDED
#define EXPORT_INCLUDED

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;
class iAnchorsAndPinnedShapes;
class iMesh;
class cPathEngine;
class iPathEngine;

class cExportOptions
{
public:

    bool tokenise;
    bool addMapping;
    bool filterFacesBySlope;
    float maxSlope;

    cExportOptions()
    {
        tokenise = false;
        addMapping = false;
        filterFacesBySlope = false;
        maxSlope = 1.f;
    }
};

bool
PerformContentProcessing(
        const std::vector<const iFaceVertexMesh*>& meshes,
        const cExportOptions& exportOptions,
        bool optimise, float horizontalThreshold, float verticalThreshold,
        const char *const* options,
        tMesh_3D& result,
        std::vector<long>*& surfaceTypes,
        std::vector<long>*& sectionIDs,
        std::vector<long>*& userData
        );

bool
PrevalidateAnchorsAndPinnedShapes(iPathEngine& pathEngine, const iAnchorsAndPinnedShapes& anchorsAndShapes);

void
AddAnchorsAndPinnedShapes(iPathEngine& pathEngine, iMesh* mesh, const iAnchorsAndPinnedShapes& anchorsAndShapes, const char* idPrefix);

#endif
