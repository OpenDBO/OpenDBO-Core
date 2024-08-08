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
#include "Mesh3DWrapper.h"
#include "SplitVerticalDiscontinuities.h"
#include "common/TemplateMesh/External.h"
#include "common/STL/sstream.h"

cMesh3DWrapper::cMesh3DWrapper(tMesh_3D& meshToWrap,
            const std::vector<long>* surfaceTypes,
            const std::vector<long>* sectionIDs,
            const std::vector<long>* userData
            )
{
    _mesh.copy(meshToWrap);
    long i;
    for(i = 0; i != _mesh.faces(); ++i)
    {
        _mesh.faceAt(i)->_originalIndex = i;
    }

  // attributes are indexed on the original mesh

    if(surfaceTypes)
    {
        _surfaceTypes = *surfaceTypes;
    }
    if(sectionIDs)
    {
        _sectionIDs = *sectionIDs;
    }
    if(userData)
    {
        _userData = *userData;
    }

  // edges with vertical discontinuities in the copied mesh are split
  // this will add faces to the mesh, for which face index will be -1

    RemoveExternalFaces(_mesh);
    long internalFacesOnOriginal = _mesh.faces();
    SplitVerticalDiscontinuities(_mesh);

  // build a remap for converting back to indices on the original mesh

    _attributesMapping.resize(_mesh.faces());
    tFace_3D_FaceTracked f = _mesh.beginFace();
    i = 0;
    long checkCount = 0;
    do
    {
        long remapped = f->_originalIndex;
        if(remapped != -1)
        {
            checkCount++;
        }
        _attributesMapping[i++] = remapped;
    }
    while((++f).valid());
    assertD(checkCount == internalFacesOnOriginal);

    _mesh.consolidate();
}

long
cMesh3DWrapper::faceAttribute(long face, long attributeIndex) const
{
    long originalFace = _attributesMapping[face];
    if(originalFace == -1)
    {
        return -1;
    }
    if(attributeIndex == PE_FaceAttribute_SurfaceType)
    {
        if(_surfaceTypes.empty())
        {
            return 0;
        }
        assertD(originalFace < SizeL(_surfaceTypes));
        return _surfaceTypes[originalFace];
    }
    if(attributeIndex == PE_FaceAttribute_SectionID)
    {
        if(_sectionIDs.empty())
        {
            return -1;
        }
        assertD(originalFace < SizeL(_sectionIDs));
        return _sectionIDs[originalFace];
    }
    if(attributeIndex == PE_FaceAttribute_UserData)
    {
        if(_userData.empty())
        {
            return 0;
        }
        assertD(originalFace < SizeL(_userData));
        return _userData[originalFace];
    }
    return -1;
}
