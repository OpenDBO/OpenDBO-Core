//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "HorizontallyOffsetAnchorsAndShapes.h"

cHorizontallyOffsetAnchorsAndShapes::cHorizontallyOffsetAnchorsAndShapes(const iAnchorsAndPinnedShapes* base, long offsetX, long offsetY) :
 _base(base),
 _offsetX(offsetX),
 _offsetY(offsetY)
{
}

long
cHorizontallyOffsetAnchorsAndShapes::numberOfAnchors() const
{
    return _base->numberOfAnchors();
}
const char*
cHorizontallyOffsetAnchorsAndShapes::anchor_Name(long anchorIndex) const
{
    return _base->anchor_Name(anchorIndex);
}
void
cHorizontallyOffsetAnchorsAndShapes::anchor_HorizontalPosition(long anchorIndex, long& x, long& y) const
{
    long baseX, baseY;
    _base->anchor_HorizontalPosition(anchorIndex, baseX, baseY);
    x = baseX + _offsetX;
    y = baseY + _offsetY;
}
void
cHorizontallyOffsetAnchorsAndShapes::anchor_VerticalRange(long anchorIndex, float& zStart, float& zEnd) const
{
    _base->anchor_VerticalRange(anchorIndex, zStart, zEnd);
}
long
cHorizontallyOffsetAnchorsAndShapes::anchor_Orientation(long anchorIndex) const
{
    return _base->anchor_Orientation(anchorIndex);
}
long
cHorizontallyOffsetAnchorsAndShapes::anchor_Attribute(long anchorIndex, long attributeIndex) const
{
    return _base->anchor_Attribute(anchorIndex, attributeIndex);
}

long
cHorizontallyOffsetAnchorsAndShapes::numberOfPinnedShapes() const
{
    return _base->numberOfPinnedShapes();
}
const char*
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_Name(long shapeIndex) const
{
    return _base->pinnedShape_Name(shapeIndex);
}
long
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_AnchorIndex(long shapeIndex) const
{
    return _base->pinnedShape_AnchorIndex(shapeIndex);
}
long
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_NumberOfVertices(long shapeIndex) const
{
    return _base->pinnedShape_NumberOfVertices(shapeIndex);
}
void
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_Vertex(long shapeIndex, long vertexIndex, long& x, long& y) const
{
    _base->pinnedShape_Vertex(shapeIndex, vertexIndex, x, y);
}
long
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_Attribute(long shapeIndex, long attributeIndex) const
{
    return _base->pinnedShape_Attribute(shapeIndex, attributeIndex);
}
