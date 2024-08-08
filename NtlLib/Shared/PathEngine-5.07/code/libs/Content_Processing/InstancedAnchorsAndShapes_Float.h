//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"

class cInstancedAnchorsAndShapes_Float : public iAnchorsAndPinnedShapes
{
    const iAnchorsAndPinnedShapes* _base;
    long _rotationAsPathEngineAngle;
    float _cosOf, _sinOf;
    float _scale;
    long _translationX, _translationY;
    float _translationZ;

public:

    cInstancedAnchorsAndShapes_Float(
        const iAnchorsAndPinnedShapes* base,
        float rotation,
        float scale,
        long translationX, long translationY,
        float translationZ
        );

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
