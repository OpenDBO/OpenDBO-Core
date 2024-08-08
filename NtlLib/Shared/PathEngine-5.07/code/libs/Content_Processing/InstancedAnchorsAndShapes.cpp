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
#include "InstancedAnchorsAndShapes.h"
#include "QuantisedRotation.h"

cInstancedAnchorsAndShapes::cInstancedAnchorsAndShapes(
        const iAnchorsAndPinnedShapes* base,
        long rotation,
        long scale,
        long translationX, long translationY,
        float translationZ
        )
{
    _base = base;
    _rotationAsPathEngineAngle = QuantisedRotationAsPathEngineAngle(rotation);
    QuantisedRotationCoefficients(rotation, _sinOf, _cosOf);
    _scale = scale;
    _scaleF = static_cast<float>(scale);
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
}

// iAnchorsAndPinnedShapes interface

long
cInstancedAnchorsAndShapes::numberOfAnchors() const
{
    return _base->numberOfAnchors();
}
const char*
cInstancedAnchorsAndShapes::anchor_Name(long anchorIndex) const
{
    return _base->anchor_Name(anchorIndex);
}
void
cInstancedAnchorsAndShapes::anchor_HorizontalPosition(long anchorIndex, long& x, long& y) const
{
    long baseX, baseY;
    _base->anchor_HorizontalPosition(anchorIndex, baseX, baseY);
    x = (baseX * _cosOf + baseY * _sinOf) / QuantisedRotationDivisor() * _scale + _translationX;
    y = (baseY * _cosOf - baseX * _sinOf) / QuantisedRotationDivisor() * _scale + _translationY;
}
void
cInstancedAnchorsAndShapes::anchor_VerticalRange(long anchorIndex, float& zStart, float& zEnd) const
{
    _base->anchor_VerticalRange(anchorIndex, zStart, zEnd);
    zStart = zStart * _scaleF + _translationZ;
    zEnd = zEnd * _scaleF + _translationZ;
}
long
cInstancedAnchorsAndShapes::anchor_Orientation(long anchorIndex) const
{
    return (_base->anchor_Orientation(anchorIndex) + _rotationAsPathEngineAngle) & 1048575;
}
long
cInstancedAnchorsAndShapes::anchor_Attribute(long anchorIndex, long attributeIndex) const
{
    return _base->anchor_Attribute(anchorIndex, attributeIndex);
}

long
cInstancedAnchorsAndShapes::numberOfPinnedShapes() const
{
    return _base->numberOfPinnedShapes();
}
const char*
cInstancedAnchorsAndShapes::pinnedShape_Name(long shapeIndex) const
{
    return _base->pinnedShape_Name(shapeIndex);
}
long
cInstancedAnchorsAndShapes::pinnedShape_AnchorIndex(long shapeIndex) const
{
    return _base->pinnedShape_AnchorIndex(shapeIndex);
}
long
cInstancedAnchorsAndShapes::pinnedShape_NumberOfVertices(long shapeIndex) const
{
    return _base->pinnedShape_NumberOfVertices(shapeIndex);
}
void
cInstancedAnchorsAndShapes::pinnedShape_Vertex(long shapeIndex, long vertexIndex, long& x, long& y) const
{
    long baseX, baseY;
    _base->pinnedShape_Vertex(shapeIndex, vertexIndex, baseX, baseY);
    x = (baseX * _cosOf + baseY * _sinOf) / QuantisedRotationDivisor() * _scale;
    y = (baseY * _cosOf - baseX * _sinOf) / QuantisedRotationDivisor() * _scale;
}
long
cInstancedAnchorsAndShapes::pinnedShape_Attribute(long shapeIndex, long attributeIndex) const
{
    return _base->pinnedShape_Attribute(shapeIndex, attributeIndex);
}
