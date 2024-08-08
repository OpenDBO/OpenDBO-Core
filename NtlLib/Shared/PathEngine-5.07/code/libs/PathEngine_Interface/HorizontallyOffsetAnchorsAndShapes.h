//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef HORIZONTALLY_OFFSET_ANCHORS_AND_SHAPES__INCLUDED
#define HORIZONTALLY_OFFSET_ANCHORS_AND_SHAPES__INCLUDED

#include "i_pathengine.h"

class cHorizontallyOffsetAnchorsAndShapes : public iAnchorsAndPinnedShapes
{
    const iAnchorsAndPinnedShapes* _base;
    long _offsetX, _offsetY;

public:

    cHorizontallyOffsetAnchorsAndShapes(const iAnchorsAndPinnedShapes* base, long offsetX, long offsetY);

// iAnchorsAndPinnedShapes interface

    long numberOfAnchors() const;
    const char* anchor_Name(long anchorIndex) const;
    void anchor_HorizontalPosition(long anchorIndex, long& x, long& y) const;
    void anchor_VerticalRange(long anchorIndex, float& zStart, float& zEnd) const;
    long anchor_Orientation(long anchorIndex) const;
    long anchor_Attribute(long anchorIndex, long attributeIndex) const;

    long numberOfPinnedShapes() const;
    const char* pinnedShape_Name(long shapeIndex) const;
    long pinnedShape_AnchorIndex(long shapeIndex) const;
    long pinnedShape_NumberOfVertices(long shapeIndex) const;
    void pinnedShape_Vertex(long shapeIndex, long vertexIndex, long& x, long& y) const;
    long pinnedShape_Attribute(long shapeIndex, long attributeIndex) const;
};

#endif
