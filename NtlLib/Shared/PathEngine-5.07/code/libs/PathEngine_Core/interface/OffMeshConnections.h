//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef OFF_MESH_CONNECTIONS_INCLUDED
#define OFF_MESH_CONNECTIONS_INCLUDED

#include "libs/Mesh2D/interface/InternalPosition.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class iXMLOutputStream;
class iXMLInputStream;

class cOffMeshConnection
{
public:

    long fromEndPoint, toEndPoint;
    long penalty;
};

class cOffMeshConnections
{
    std::vector<cInternalPosition> _endPoints;
    std::vector<cOffMeshConnection> _connections;
    std::vector<std::vector<long> > _connectionsPerEndPoint;

    // prevent copying and assignment
    cOffMeshConnections(const cOffMeshConnections&);
    const cOffMeshConnections& operator=(const cOffMeshConnections&);

public:

    cOffMeshConnections()
    {
    }
//    cOffMeshConnections(iXMLInputStream& is, const cMeshIndex<tMesh>& index);

    void save(iXMLOutputStream& os) const;

    void clear()
    {
        _endPoints.clear();
        _connections.clear();
        _connectionsPerEndPoint.clear();
    }
    bool empty() const
    {
        return _endPoints.empty();
    }

    long addEndPoint(const cInternalPosition&);
    long addConnection(long from, long to, long penalty);

    long lookupConnection(long from, long to) const;

    long numberOfEndPoints() const
    {
        return static_cast<long>(_endPoints.size());
    }
    long numberOfConnections() const
    {
        return static_cast<long>(_connections.size());
    }
    long connectionsFromEndPoint(long from) const
    {
        return SizeL(_connectionsPerEndPoint[from]);
    }
    void connectionFromEndPoint(long from, long index, long& to, long& penalty) const
    {
        const cOffMeshConnection& c = _connections[_connectionsPerEndPoint[from][index]];
        to = c.toEndPoint;
        penalty = c.penalty;
    }

    const cInternalPosition& refEndPoint(long index) const
    {
        return _endPoints[index];
    }

    const std::vector<cInternalPosition>& refEndPoints() const
    {
        return _endPoints;
    }

    const cOffMeshConnection& refConnection(long index) const
    {
        return _connections[index];
    }
};

#endif
