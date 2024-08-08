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
#include "interface/Export.h"
#include "common/interface/Error.h"
#include "ResolveAnchors.h"
#include "AddShapes.h"
#include "FaceVertexToMesh3D.h"
#include "ConnectConnectingEdges.h"
#include "ConnectingEdgeInfo.h"
#include "CutExternalShape.h"
#include "RemoveInlineExternalVertices.h"
#include "ConnectPortals.h"
#include "ExternalShapeInfo.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/Mesh3D/interface/ValidateMesh_Import.h"
#include "libs/Mesh3D/interface/PlaneEquation.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/AttributesBuilder.h"
#include "common/TemplateMesh/External.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/interface/Attributes.h"
#include "i_pathengine.h"

using std::vector;

class cMappedAttributesEqualCallBack : public iFacesCanBeJoinedCallBack
{
    const vector<const iFaceVertexMesh*>& _meshes;
    const vector<std::pair<long, long> >& _faceMappings;

public:

    cMappedAttributesEqualCallBack::cMappedAttributesEqualCallBack(
        const vector<const iFaceVertexMesh*>& meshes,
        const vector<std::pair<long, long> >& faceMappings
        ) :
      _meshes(meshes),
      _faceMappings(faceMappings)
    {
    }

    bool facesCanBeJoined(tEdge_3D_FaceTracked e) const
    {
        assertD(!e.face()->isExternal());
        assertD(!e.twin().face()->isExternal());
        long part1, faceInPart1;
        part1 = _faceMappings[e.face()->_originalIndex].first;
        faceInPart1 = _faceMappings[e.face()->_originalIndex].second;
        long part2, faceInPart2;
        part2 = _faceMappings[e.twin().face()->_originalIndex].first;
        faceInPart2 = _faceMappings[e.twin().face()->_originalIndex].second;
        long i;
        for(i = 0; i <= PE_FaceAttribute_Top; ++i)
        {
            if(_meshes[part1]->faceAttribute(faceInPart1, i) != _meshes[part2]->faceAttribute(faceInPart2, i))
            {
                return false;
            }
        }
        return true;
    }
};

class cFaceIsOnTerrainCallBack : public iFaceIsOnTerrainCallBack
{
    const vector<const iFaceVertexMesh*>& _meshes;
    const vector<std::pair<long, long> >& _faceMappings;

public:

    cFaceIsOnTerrainCallBack::cFaceIsOnTerrainCallBack(
        const vector<const iFaceVertexMesh*>& meshes,
        const vector<std::pair<long, long> >& faceMappings
        ) :
      _meshes(meshes),
      _faceMappings(faceMappings)
    {
    }

// iFaceIsOnTerrainCallBack

    bool faceIsOnTerrain(tFace_3D_FaceTracked f)
    {
        long part = _faceMappings[f->_originalIndex].first;
        long faceInPart = _faceMappings[f->_originalIndex].second;
        long sectionID = _meshes[part]->faceAttribute(faceInPart, PE_FaceAttribute_SectionID);
        return sectionID == 0;
    }
};

bool
PerformContentProcessing(
        const vector<const iFaceVertexMesh*>& meshes,
        const cExportOptions& exportOptions,
        bool optimise, float horizontalThreshold, float verticalThreshold,
        const char *const* options,
        tMesh_3D& result,
        std::vector<long>*& surfaceTypes,
        std::vector<long>*& sectionIDs,
        std::vector<long>*& userData
        )
{
    assertD(result.faces() == 0);
    assertD(result.vertices() == 0);
    long i;
    for(i = 0; i < SizeL(meshes); ++i)
    {
        const iFaceVertexMesh& fv = *meshes[i];
        long j;
        for(j = 0; j != fv.vertices(); ++j)
        {
            long x = fv.vertexX(j);
            long y = fv.vertexY(j);
            if(x < -WORLD_RANGE || x > WORLD_RANGE || y < -WORLD_RANGE || y > WORLD_RANGE)
            {
                cAttributesBuilder attributes;
                attributes.add("meshPart", i);
                attributes.add("vertexIndex", j);
                attributes.add("x", x);
                attributes.add("y", y);
                attributes.add("allowedRange", WORLD_RANGE);
                Error("NonFatal", "Building mesh from content: Vertex is outside allowed range. Failed to build, returning null pointer.", attributes.get());
                return false;
            }
        }
    }

    tMesh_3D_FaceTracked baseMesh;
    vector<cExternalShapeInfo*> externalShapes;
    vector<cConnectingEdgeInfo*> connectingEdges;

    vector<std::pair<long, long> > faceMappings;

    for(i = 0; i < SizeL(meshes); ++i)
    {
        cAttributesBuilder errorAttributes;
        errorAttributes.add("meshPart", i);

        FaceVertexToMesh3D(*meshes[i],
            exportOptions.filterFacesBySlope, exportOptions.maxSlope,
            baseMesh,
            i, faceMappings,
            externalShapes, connectingEdges,
            errorAttributes.get());
    }

    if(baseMesh.empty())
    {
        for(i = 0; i < SizeL(externalShapes); ++i)
        {
            delete externalShapes[i];
        }
        for(i = 0; i < SizeL(connectingEdges); ++i)
        {
            delete connectingEdges[i];
        }
        Error("NonFatal", "Building mesh from content: Resulting mesh would be empty. Failed to build, returning null pointer.");
        return 0;
    }

    AddExternalFaces(baseMesh);

    tFace_3D_FaceTracked f;
    i = 0;
    for(f = baseMesh.beginFace(); f != baseMesh.endFace(); ++f)
    {
        f->_originalIndex = i++;
    }

    if(!ValidateMesh_Import_AllowDeadEntries(baseMesh, &faceMappings))
    {
        for(i = 0; i < SizeL(externalShapes); ++i)
        {
            delete externalShapes[i];
        }
        for(i = 0; i < SizeL(connectingEdges); ++i)
        {
            delete connectingEdges[i];
        }
        return false;
    }

    {
        bool ignoreConnectionFailure = ValueAsBoolWithDefault(options, "ignoreConnectionFailure", false);

        RemoveExternalFaces(baseMesh);
        ConnectConnectingEdges(baseMesh, connectingEdges, ignoreConnectionFailure);
        for(i = 0; i < SizeL(connectingEdges); ++i)
        {
            delete connectingEdges[i];
        }
        AddExternalFaces(baseMesh);

        if(!externalShapes.empty())
        {
            cFaceIsOnTerrainCallBack callBack(meshes, faceMappings);
            CalculatePlaneEquations(baseMesh);
            ConnectPortals(baseMesh, callBack, externalShapes, ignoreConnectionFailure);
        }
    }

    assertD(ValidateMesh_Import_AllowDeadEntries(baseMesh));
    CalculatePlaneEquations(baseMesh);

    for(i = 0; i < SizeL(externalShapes); ++i)
    {
        cExternalShapeInfo& shape = *externalShapes[i];
        shape._behindPortalEdgePoints.resize(shape._behindPortalEdges.size() * 2);
        long j;
        for(j = 0; j != SizeL(shape._behindPortalEdges); ++j)
        {
            if(shape._behindPortalEdges[j].valid())
            {
                shape._behindPortalEdgePoints[j * 2] = GetVertexPoint(shape._behindPortalEdges[j]);
                shape._behindPortalEdgePoints[j * 2 + 1] = GetVertexPoint(shape._behindPortalEdges[j].next());
            }
        }
    }

    for(i = 0; i < SizeL(externalShapes); ++i)
    {
        cExternalShapeInfo& shape = *externalShapes[i];
        bool portalsAffected = CutExternalShape(
                baseMesh,
                shape._shapeExternalFace, shape._behindPortalEdges,
                shape._behindPortalEdgePoints
                );
        assertF(ValidateMeshConnections(baseMesh));
        assertF(ValidateMesh_Import_AllowDeadEntries(baseMesh));
        if(portalsAffected)
        {
            cAttributesBuilder attributes;
            attributes.add(shape._errorAttributes.get());
            attributes.add("faceIndex", shape._shapeFaceIndex);
            Error("NonFatal", "One or more portals on this external shape were cut by previously applied external shapes."
                " The shape will not be applied at affected portals.",
                attributes.get());
        }
        delete externalShapes[i];
    }

    if(optimise)
    {
        baseMesh.consolidate(); 
        assertD(ValidateMesh_Import(baseMesh));

        cMappedAttributesEqualCallBack callBack(meshes, faceMappings);
        RemoveInlineExternalVertices(baseMesh, horizontalThreshold, verticalThreshold, callBack);

        // standardise connectivity to external faces
        RemoveExternalFaces(baseMesh);
        AddExternalFaces(baseMesh);
    }

    baseMesh.consolidate(); 
    assertD(ValidateMesh_Import(baseMesh));

    surfaceTypes = 0;
    sectionIDs = 0;
    userData = 0;
    long internalFacesInBaseMesh = 0;
    f = baseMesh.beginFace();
    for(i = 0; i < baseMesh.faces(); ++i, ++f)
    {
        if(f->isExternal())
        {
            break;
        }
        internalFacesInBaseMesh++;
    }
    f = baseMesh.beginFace();
    for(i = 0; i < baseMesh.faces(); ++i, ++f)
    {
        if(f->isExternal())
        {
            break;
        }
        long part = faceMappings[f->_originalIndex].first;
        long faceInPart = faceMappings[f->_originalIndex].second;
        long surfaceType = meshes[part]->faceAttribute(faceInPart, PE_FaceAttribute_SurfaceType);
        if(surfaceType != 0)
        {
            if(!surfaceTypes)
            {
                surfaceTypes = new std::vector<long>(internalFacesInBaseMesh, 0);
            }
            (*surfaceTypes)[i] = surfaceType;
        }
        long sectionID = meshes[part]->faceAttribute(faceInPart, PE_FaceAttribute_SectionID);
        if(sectionID != -1)
        {
            if(!sectionIDs)
            {
                sectionIDs = new std::vector<long>(internalFacesInBaseMesh, -1);
            }
            (*sectionIDs)[i] = sectionID;
        }
        long userDataValue = meshes[part]->faceAttribute(faceInPart, PE_FaceAttribute_UserData);
        if(userDataValue != 0)
        {
            if(!userData)
            {
                userData = new std::vector<long>(internalFacesInBaseMesh, 0);
            }
            (*userData)[i] = userDataValue;
        }
    }

    CalculatePlaneEquations(baseMesh);

    result.copy(baseMesh);

    return true;
}

bool
PrevalidateAnchorsAndPinnedShapes(iPathEngine& pathEngine, const iAnchorsAndPinnedShapes& anchorsAndShapes)
{
//.. could also check that anchor ids are not repeated
    return PrevalidatePinnedShapes(pathEngine, anchorsAndShapes);
}

void
AddAnchorsAndPinnedShapes(iPathEngine& pathEngine, iMesh* mesh, const iAnchorsAndPinnedShapes& anchorsAndShapes, const char* idPrefix)
{
    vector<cPosition> anchorPositions;
    ResolveAnchors(*mesh, anchorsAndShapes, anchorPositions);
    AddPinnedShapes(pathEngine, anchorsAndShapes, anchorPositions, mesh);
    long i;
    for(i = 0; i < SizeL(anchorPositions); i++)
    {
        if(anchorPositions[i].cell == -1)
        {
            // already emitted an error when resolving anchors
            continue;
        }
        const char* id = anchorsAndShapes.anchor_Name(i);
        if(id[0] && id[0] != '#')
        {
            std::string fullID(idPrefix);
            fullID.append(id);
            cPosition retrieved;
            {
                long orientation_ignored;
                retrieved = mesh->retrieveAnchor(fullID.c_str(), orientation_ignored);
            }
            if(retrieved.cell != -1)
            {
                cAttributesBuilder attributes;
                attributes.add("id", fullID.c_str());
                Error("NonFatal", "Anchor id already used. Anchor will not be placed.", attributes.get());
                continue;
            }
            mesh->storeAnchor(fullID.c_str(), anchorPositions[i], anchorsAndShapes.anchor_Orientation(i));
        }
    }
}
