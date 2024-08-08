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
#include "interface/LoadedFaceVertexMesh.h"
#include "common/interface/Attributes.h"
#include "common/CLib/string.h"

long
cLoadedFaceVertexMesh::addVertex(long x, long y, float z)
{
    long result = vertices();
    _vertexXYs.push_back(x);
    _vertexXYs.push_back(y);
    _vertexZs.push_back(z);
    return result;
}
long
cLoadedFaceVertexMesh::addFace(long v0, long v1, long v2)
{
    assertD(v0 >= 0 && v0 < vertices());
    assertD(v1 >= 0 && v1 < vertices());
    assertD(v2 >= 0 && v2 < vertices());
    assertD(v0 != v1 && v0 != v2 && v1 != v2);
    long result = faces();
    _faceVerts.push_back(v0);
    _faceVerts.push_back(v1);
    _faceVerts.push_back(v2);
    assertC(PE_FaceAttribute_Top == 6);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(-1);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    return result;
}
void
cLoadedFaceVertexMesh::setAttribute(long face, long attribute, long value)
{
    assertD(face >= 0 && face < faces());
    assertD(attribute >= 0 && attribute < PE_FaceAttribute_Top);
    _faceAttributes[face * (PE_FaceAttribute_Top + 1) + attribute] = value;
}

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
