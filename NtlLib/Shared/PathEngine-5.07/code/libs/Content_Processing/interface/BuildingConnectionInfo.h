//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include <vector>
#include <string>

class Interface;
class INode;
class Point3;

class cBuildingConnectionInfo
{

public:

    std::string _nodeName;
    std::string _name;
    long _orientation;
    long _x, _y, _zStart, _zEnd;
};

class cShape
{
public:

    std::string _nodeName;
    std::string _name;
    size_t _anchorIndex;
    std::vector<long> _coords;
};

class cAnchorsAndShapes
{
public:

    std::vector<cAnchor> _anchors;
    std::vector<cShape> _shapes;
    std::vector<cShape> _namedObstructionShapes;
};
