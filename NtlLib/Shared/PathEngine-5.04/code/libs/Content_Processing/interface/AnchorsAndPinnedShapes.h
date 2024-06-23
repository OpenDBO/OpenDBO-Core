//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"
#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"
#include "common/STL/string.h"

class cAnchor
{
public:
    std::string _name;
    long _orientation;
    long _x, _y;
    float _zStart, _zEnd;
    bool _resolvesToTerrain;
};

class cPinnedShape
{
public:
    std::string _name;
    long _anchorIndex;
    std::vector<long> _coords;
    long _type;
};

class cAnchorsAndPinnedShapes : public iAnchorsAndPinnedShapes, public iXMLHandler
{
public:

    std::vector<cAnchor> _anchors;
    std::vector<cPinnedShape> _pinnedShapes;

// iAnchorsAndPinnedShapes

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

// iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attr);

};
