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
#include "InstancedAnchorsAndShapes_Float.h"
#include <math.h>

cInstancedAnchorsAndShapes_Float::cInstancedAnchorsAndShapes_Float(
        const iAnchorsAndPinnedShapes* base,
        float rotation,
        float scale,
        long translationX, long translationY,
        float translationZ
        )
{
    _base = base;
    _rotationAsPathEngineAngle = static_cast<long>(rotation * 1048576.f / 6.283185f);
    _cosOf = cosf(rotation);
    _sinOf = sinf(rotation);
    _scale = scale;
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
}

// iAnchorsAndPinnedShapes interface

long
cInstancedAnchorsAndShapes_Float::numberOfAnchors() const
{
    return _base->numberOfAnchors();
}
const char*
cInstancedAnchorsAndShapes_Float::anchor_Name(long anchorIndex) const
{
    return _base->anchor_Name(anchorIndex);
}
void
cInstancedAnchorsAndShapes_Float::anchor_HorizontalPosition(long anchorIndex, long& x, long& y) const
{
    long baseX, baseY;
    _base->anchor_HorizontalPosition(anchorIndex, baseX, baseY);
    x = static_cast<long>((baseX * _cosOf + baseY * _sinOf) * _scale) + _translationX;
    y = static_cast<long>((baseY * _cosOf - baseX * _sinOf) * _scale) + _translationY;
}
void
cInstancedAnchorsAndShapes_Float::anchor_VerticalRange(long anchorIndex, float& zStart, float& zEnd) const
{
    _base->anchor_VerticalRange(anchorIndex, zStart, zEnd);
    zStart = zStart * _scale + _translationZ;
    zEnd = zEnd * _scale + _translationZ;
}
long
cInstancedAnchorsAndShapes_Float::anchor_Orientation(long anchorIndex) const
{
    return (_base->anchor_Orientation(anchorIndex) + _rotationAsPathEngineAngle) & 1048575;
}
long
cInstancedAnchorsAndShapes_Float::anchor_Attribute(long anchorIndex, long attributeIndex) const
{
    return _base->anchor_Attribute(anchorIndex, attributeIndex);
}

long
cInstancedAnchorsAndShapes_Float::numberOfPinnedShapes() const
{
    return _base->numberOfPinnedShapes();
}
const char*
cInstancedAnchorsAndShapes_Float::pinnedShape_Name(long shapeIndex) const
{
    return _base->pinnedShape_Name(shapeIndex);
}
long
cInstancedAnchorsAndShapes_Float::pinnedShape_AnchorIndex(long shapeIndex) const
{
    return _base->pinnedShape_AnchorIndex(shapeIndex);
}
long
cInstancedAnchorsAndShapes_Float::pinnedShape_NumberOfVertices(long shapeIndex) const
{
    return _base->pinnedShape_NumberOfVertices(shapeIndex);
}
void
cInstancedAnchorsAndShapes_Float::pinnedShape_Vertex(long shapeIndex, long vertexIndex, long& x, long& y) const
{
    long baseX, baseY;
    _base->pinnedShape_Vertex(shapeIndex, vertexIndex, baseX, baseY);
    x = static_cast<long>((baseX * _cosOf + baseY * _sinOf) * _scale);
    y = static_cast<long>((baseY * _cosOf - baseX * _sinOf) * _scale);
}
long
cInstancedAnchorsAndShapes_Float::pinnedShape_Attribute(long shapeIndex, long attributeIndex) const
{
    return _base->pinnedShape_Attribute(shapeIndex, attributeIndex);
}
