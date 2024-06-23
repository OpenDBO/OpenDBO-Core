//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "MeshRenderInfo.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "libs/Geometry/interface/tPoint3.h"

void cMeshRenderInfo::clear()
{
    _faceElements.clear();
    _edgeElements.clear();
    if(_gip)
    {
        _gip->deleteGeometrySet(_geometrySet);
    }
    _geometrySet = 0;
}
cMeshRenderInfo::~cMeshRenderInfo()
{
    clear();
}
void
cMeshRenderInfo::drawFaces() const
{
    long i;
    for(i = 0; i < SizeL(_faceElements); i++)
    {
        if(_faceElements[i])
        {
            _gip->render(_faceElements[i]);
        }
    }
}
void cMeshRenderInfo::drawEdges() const
{
    long i;
    for(i = 0; i < SizeL(_edgeElements); i++)
    {
        _gip->render(_edgeElements[i]);
    }
}
void cMeshRenderInfo::drawFace(long index) const
{
    if(_faceElements[index])
    {
        _gip->render(_faceElements[index]);
    }
}
void cMeshRenderInfo::drawEdge(long index) const
{
    _gip->render(_edgeElements[index]);
}


