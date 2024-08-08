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
#include "MaxMeshWrapper.h"
#include "Max.h"
#include "common/interface/Assert.h"

static long
ApproximateTowardsMinusInfinity(float value)
{
    long result = static_cast<long>(value);
    if(static_cast<float>(result) != value && value < 0.f)
    {
        result--;
    }
    return result;
}

static void
TransformVertices(const Mesh& mesh, const Matrix3& matrix, float scale,
                  std::vector<long>& resultXY,
                  std::vector<float>& resultZ)
{
    long verts = mesh.getNumVerts();
    resultXY.resize(verts * 2);
    resultZ.resize(verts);
    long i;
    for(i = 0; i < verts; i++)
    {
        Point3 multiplied = mesh.verts[i] * matrix;
        resultXY[i * 2 + 0] = ApproximateTowardsMinusInfinity(multiplied.x * scale);
        resultXY[i * 2 + 1] = ApproximateTowardsMinusInfinity(multiplied.y * scale);
        resultZ[i] = multiplied.z * scale;
    }
}

static bool
FaceColoursMatch(
        const Mesh& mesh,
        DWORD colourIndex0,
        DWORD colourIndex1,
        DWORD colourIndex2,
        float r, float g, float b
        )
{
    return mesh.vertCol[colourIndex0].x == r
        && mesh.vertCol[colourIndex0].y == g
        && mesh.vertCol[colourIndex0].z == b
        && mesh.vertCol[colourIndex1].x == r
        && mesh.vertCol[colourIndex1].y == g
        && mesh.vertCol[colourIndex1].z == b
        && mesh.vertCol[colourIndex2].x == r
        && mesh.vertCol[colourIndex2].y == g
        && mesh.vertCol[colourIndex2].z == b
        ;
}

static bool
FaceColoursMatchAShadeOfGreen(
        const Mesh& mesh,
        DWORD colourIndex0,
        DWORD colourIndex1,
        DWORD colourIndex2,
        float r, int& shade, float b
        )
{
    float g = mesh.vertCol[colourIndex0].y;
    if(g == 0.f)
    {
        return false;
    }
    if(mesh.vertCol[colourIndex0].x == r
        && mesh.vertCol[colourIndex0].z == b
        && mesh.vertCol[colourIndex1].x == r
        && mesh.vertCol[colourIndex1].y == g
        && mesh.vertCol[colourIndex1].z == b
        && mesh.vertCol[colourIndex2].x == r
        && mesh.vertCol[colourIndex2].y == g
        && mesh.vertCol[colourIndex2].z == b
        )
    {
        g *= 255.f;
        g += 0.5f;
        shade = static_cast<int>(g);
        assertD(shade > 0 && shade <= 255);
        return true;
    }
    else
    {
        return false;
    }
}

cMaxMeshWrapper::cMaxMeshWrapper(
     const Mesh& mesh, const Matrix3& matrix,
     bool useMaterialIDs, bool setTerrainLayer, bool externalShapeAndPortalMarkers,
     float scale) :
 _mesh(mesh)
{
    TransformVertices(mesh, matrix, scale, _transformedVertexXY, _transformedVertexZ);

    long i;
    for(i = 0; i < faces(); ++i)
    {
        if(useMaterialIDs && _mesh.faces[i].getMatID() != 0)
        {
            _surfaceTypes.resize(_mesh.getNumFaces(), 0);
            _surfaceTypes[i] = _mesh.faces[i].getMatID();
        }
        if(_mesh.vertCol)
        {
            TVFace& colourFace = _mesh.vcFace[i];
            DWORD colourIndex0 = colourFace.t[2];
            DWORD colourIndex1 = colourFace.t[1];
            DWORD colourIndex2 = colourFace.t[0];
            int shadeOfGreen;
            if(setTerrainLayer
             && FaceColoursMatchAShadeOfGreen(mesh, colourIndex0, colourIndex1, colourIndex2, 0.f, shadeOfGreen, 0.f)
             )
            {
                _sectionIDs.resize(_mesh.getNumFaces(), -1);
                _sectionIDs[i] = 255 - shadeOfGreen;
            }
            if(externalShapeAndPortalMarkers)
            {
                if(FaceColoursMatch(mesh, colourIndex0, colourIndex1, colourIndex2, 0.f, 0.f, 1.f))
                {
                    _markerTypes.resize(_mesh.getNumFaces(), -1);
                    _markerTypes[i] = PE_FaceAttribute_MarksPortal;
                }
                else if(FaceColoursMatch(mesh, colourIndex0, colourIndex1, colourIndex2, 1.f, 0.f, 1.f))
                {
                    _markerTypes.resize(_mesh.getNumFaces(), -1);
                    _markerTypes[i] = PE_FaceAttribute_MarksTerrainPortal;
                }
                else if(FaceColoursMatch(mesh, colourIndex0, colourIndex1, colourIndex2, 0.f, 0.f, 0.f))
                {
                    _markerTypes.resize(_mesh.getNumFaces(), -1);
                    _markerTypes[i] = PE_FaceAttribute_MarksExternalShape;
                }
                else if(FaceColoursMatch(mesh, colourIndex0, colourIndex1, colourIndex2, 0.f, 1.f, 1.f))
                {
                    _markerTypes.resize(_mesh.getNumFaces(), -1);
                    _markerTypes[i] = PE_FaceAttribute_MarksConnectingEdge;
                }
            }
        }
    }
}

long
cMaxMeshWrapper::faces() const
{
    return _mesh.getNumFaces();
}
long
cMaxMeshWrapper::vertices() const
{
    return _mesh.getNumVerts();
}

long
cMaxMeshWrapper::vertexIndex(long face, long vertexInFace) const
{
    return _mesh.faces[face].v[2 - vertexInFace];
}
long
cMaxMeshWrapper::vertexX(long vertex) const
{
    return _transformedVertexXY[vertex * 2 + 0];
}
long
cMaxMeshWrapper::vertexY(long vertex) const
{
    return _transformedVertexXY[vertex * 2 + 1];
}
float
cMaxMeshWrapper::vertexZ(long vertex) const
{
    return _transformedVertexZ[vertex];
}

long
cMaxMeshWrapper::faceAttribute(long face, long attributeIndex) const
{
    if(attributeIndex == PE_FaceAttribute_SurfaceType)
    {
        if(_surfaceTypes.empty())
        {
            return 0;
        }
        return _surfaceTypes[face];
    }
    if(attributeIndex == PE_FaceAttribute_SectionID)
    {
        if(_sectionIDs.empty())
        {
            return -1;
        }
        return _sectionIDs[face];
    }
    if(attributeIndex == PE_FaceAttribute_MarksPortal
     || attributeIndex == PE_FaceAttribute_MarksTerrainPortal
     || attributeIndex == PE_FaceAttribute_MarksExternalShape
     || attributeIndex == PE_FaceAttribute_MarksConnectingEdge
        )
    {
        if(_markerTypes.empty())
        {
            return 0;
        }
        if(_markerTypes[face] == attributeIndex)
        {
            return 1;
        }
        return 0;
    }
    if(attributeIndex == PE_FaceAttribute_UserData)
    {
        return 0;
    }
    return -1;
}
