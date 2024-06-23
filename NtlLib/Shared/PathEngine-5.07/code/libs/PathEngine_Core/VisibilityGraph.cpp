//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "VisibilityGraph.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/STL_Helpers.h"

cVisibilityGraph::cVisibilityGraph(iXMLInputStream& is, const char *const* rootAttributes)
{
    long size = ValueAsLong(rootAttributes, "size");
    std::vector<std::vector<long> > connections(size);
    std::vector<std::vector<long> > costs(size);
    long i;
    for(i = 0; i < SizeL(connections); ++i)
    {
        const char *const* attributes = is.openElement("source");
        long targets = ValueAsLong(attributes, "size");
        connections[i].resize(targets);
        costs[i].resize(targets);
        long j;
        for(j = 0; j < SizeL(connections[i]); ++j)
        {
            const char *const* attributes = is.openElement("target");
            connections[i][j] = ValueAsLong(attributes, "state");
            costs[i][j] = ValueAsLong(attributes, "cost");
            is.closeElement();
        }
        is.closeElement();
    }
    {
        cCollapsedVectorVector<long> toSwap(connections);
        _connections.swap(toSwap);
    }
    {
        cCollapsedVectorVector<long> toSwap(costs);
        _costs.swap(toSwap);
    }
}

void
cVisibilityGraph::save(iXMLOutputStream& os) const
{
    os.addAttribute("size", _connections.size());
    long i;
    for(i = 0; i < _connections.size(); ++i)
    {
        os.openElement("source");
        os.addAttribute("size", connectionsFrom(i));
        long j;
        for(j = 0; j < connectionsFrom(i); ++j)
        {
            os.openElement("target");
            os.addAttribute("state", connectionFrom(i, j));
            os.addAttribute("cost", connectionCost(i, j));
            os.closeElement("target");
        }
        os.closeElement("source");
    }
}

