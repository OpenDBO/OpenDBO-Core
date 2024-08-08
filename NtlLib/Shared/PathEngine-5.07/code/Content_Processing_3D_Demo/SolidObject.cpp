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
#include "SolidObject.h"
#include "LoadOperands.h"
#include "libs/TestBed_Interface/interface/MeshRenderInfo.h"
#include "libs/Content_Processing_3D/interface/GenerateHullsForVisualisation.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "i_pathengine.h"
#include <math.h>

static void
TransformPoint(
        double cosOfRotation, double sinOfRotation,
        float scale,
        long translationX, long translationY, long translationZ,
        const long* from,
        long* to
        )
{
    to[0] = static_cast<long>((from[0] * cosOfRotation + from[1] * sinOfRotation) * scale) + translationX;
    to[1] = static_cast<long>((from[1] * cosOfRotation - from[0] * sinOfRotation) * scale) + translationY;
    to[2] = static_cast<long>(from[2] * scale) + translationZ;
}

void
cSolidObject::transformOperand(long i)
{
    long numberOfPoints = SizeL(_operands[i]) / 3;
    _cachedTransformedData.push_back(numberOfPoints);
    long startIndex = SizeL(_cachedTransformedData);
    _cachedTransformedData.resize(startIndex + numberOfPoints * 3);
    long j;
    for(j = 0; j != numberOfPoints; ++j)
    {
        TransformPoint(
                _cosOfRotation, _sinOfRotation,
                _scale,
                _translationX, _translationY, _translationZ, 
                &_operands[i][j * 3],
                &_cachedTransformedData[startIndex + j * 3]
                );
    }
}

void
cSolidObject::invalidateCachedData()
{
    delete _cachedRenderInfo;
    _cachedRenderInfo = 0;
}
void
cSolidObject::generateCachedDataIfNotValid()
{
    if(_cachedRenderInfo)
    {
        return;
    }
    _cosOfRotation = cos(_rotation);
    _sinOfRotation = sin(_rotation);
    _cachedTransformedData.resize(0);
    long i;
    for(i = 0; i != _operands.size(); ++i)
    {
        transformOperand(i);
    }
    _cachedTransformedData.push_back(0);
    tMesh_3D hullsMesh3D;
    GenerateHullsForVisualisation(&_cachedTransformedData[0], SizeL(_cachedTransformedData), hullsMesh3D);
    _cachedRenderInfo = new cMeshRenderInfo;
    _cachedRenderInfo->build(hullsMesh3D);
}

cSolidObject::cSolidObject(const char *const* attributes) :
 bSceneObject(attributes)
{
    LoadOperands(_type.c_str(), _operands);
    long i;
    for(i = 0; i != SizeL(_operands); ++i)
    {
        assertR(SizeL(_operands[i]) >= 3);
        long numberOfPoints = SizeL(_operands[i]) / 3;
        assertR(SizeL(_operands[i]) == numberOfPoints * 3);
    }
    _cachedRenderInfo = 0;
}

cSolidObject::~cSolidObject()
{
    delete _cachedRenderInfo;
}

void
cSolidObject::renderEdges()
{
    generateCachedDataIfNotValid();
    _cachedRenderInfo->drawEdges();
}
void
cSolidObject::renderFaces()
{
    generateCachedDataIfNotValid();
    _cachedRenderInfo->drawFaces();
}
void
cSolidObject::addTransformedData(std::vector<long>& buffer)
{
    generateCachedDataIfNotValid();
    long i;
    for(i = 0; i != SizeL(_cachedTransformedData) - 1; ++i)
    {
        buffer.push_back(_cachedTransformedData[i]);
    }
}

void
cSolidObject::addAllPoints(std::vector<tPoint3>& addTo)
{
    generateCachedDataIfNotValid();    
    long* ptr = &_cachedTransformedData[0];
    while(1)
    {
        long numberOfPoints = *ptr++;
        if(numberOfPoints == 0)
        {
            return;
        }
        long i;
        for(i = 0; i != numberOfPoints; ++i)
        {
            long x = *ptr++;
            long y = *ptr++;
            long z = *ptr++;
            if(x < -WORLD_RANGE || x > WORLD_RANGE)
            {
                continue;
            }
            if(y < -WORLD_RANGE || y > WORLD_RANGE)
            {
                continue;
            }
            if(z < -WORLD_RANGE || z > WORLD_RANGE)
            {
                continue;
            }
            addTo.push_back(tPoint3(x, y, z, WORLD_RANGE));            
        }
    }
}
