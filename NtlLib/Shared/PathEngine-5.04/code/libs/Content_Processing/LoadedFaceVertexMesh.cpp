//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "LoadedFaceVertexMesh.h"
#include "common/interface/Attributes.h"
#include "common/CLib/string.h"

iXMLHandler*
cLoadedFaceVertexMesh::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "vertex") == 0)
    {
        _vertexXYs.push_back(ValueAsLong(attr, "x"));
        _vertexXYs.push_back(ValueAsLong(attr, "y"));
        _vertexZs.push_back(ValueAsPersistentFloat(attr, "z"));
        return 0;
    }
    if(strcmp(el, "face") == 0)
    {
        _faceVerts.push_back(ValueAsLong(attr, "v1"));
        _faceVerts.push_back(ValueAsLong(attr, "v2"));
        _faceVerts.push_back(ValueAsLong(attr, "v3"));

        assertC(PE_FaceAttribute_Top == 6);
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_surfaceType", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_sectionID", -1));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksPortal", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksExternalShape", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksConnectingEdge", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_userData", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksTerrainPortal", 0));
        return 0;
    }
    return 0;
}

long
cLoadedFaceVertexMesh::faceAttribute(long face, long attributeIndex) const
{
    if(attributeIndex <= PE_FaceAttribute_Top)
    {
        return _faceAttributes[face * (PE_FaceAttribute_Top + 1) + attributeIndex];
    }
    return -1;
}
