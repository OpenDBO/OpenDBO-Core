//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VISIBILITY_GRAPH_INCLUDED
#define VISIBILITY_GRAPH_INCLUDED

#include "FixedStates.h"
#include "interface/CornerProvider.h"
#include "interface/OffMeshConnections.h"
#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "platform_common/AddWithOverflow.h"
#include "common/Containers/BitVector.h"
#include "common/Containers/CollapsedVectorVector.h"
#include "common/STL/vector.h"

class iXMLOutputStream;
class iXMLInputStream;
class cQueryContext;

class cVisibilityGraph
{
//    std::vector<std::vector<long> > _connections;
//    std::vector<std::vector<long> > _costs;
    cCollapsedVectorVector<long> _connections;
    cCollapsedVectorVector<long> _costs;

    // prevent copying and assignment
    cVisibilityGraph(const cVisibilityGraph&);
    const cVisibilityGraph& operator=(const cVisibilityGraph&);

public:

    cVisibilityGraph(iXMLInputStream& is, const char *const* rootAttributes);

    template <class tCorners, class tCollision, class tCostCalculator>
    cVisibilityGraph(
            cQueryContext& qc,
            const cFixedStates<tCorners, tCollision, tCostCalculator>& fixedStates,
            const std::vector<std::vector<long> >& silhouetteLookup,
            const cOffMeshConnections& offMeshConnections,
            const cBitVector& endPointObstructed
            )
    {
        std::vector<std::vector<long> > connections(fixedStates.states());
        std::vector<std::vector<long> > costs(fixedStates.states());

    // connections in mesh

        long sourcePoint;
        for(sourcePoint = 2; sourcePoint < fixedStates.points(); sourcePoint++)
        {
            cInternalPosition sourceP = fixedStates.getPosition(sourcePoint);

            long sourceState;
            if(!fixedStates.pointIsCorner(sourcePoint))
            {
                if(endPointObstructed[fixedStates.endPointForPoint(sourcePoint)])
                {
                    continue;
                }
            // source state corresponds to arriving at this endpoint
                sourceState = fixedStates.encodeEndPoint(sourcePoint, false);
            }
            // otherwise source state depends on zone of target with respect to source corner

            size_t j;
            for(j = 0; j < silhouetteLookup[sourceP.cell()].size(); j++)
            {
                long targetState = silhouetteLookup[sourceP.cell()][j];

                long targetPoint = fixedStates.pointForState(targetState);
                if(targetPoint <= sourcePoint)
                {
                    continue;
                }

                cInternalPosition targetP = fixedStates.getPosition(targetPoint);

                if(fixedStates.pointIsCorner(sourcePoint))
                {
                    eSilhouetteZone sourceZone = fixedStates.zoneFor(sourcePoint, targetP.p);
                    if(sourceZone == SZ_NOTSILHOUETTE)
                    {
                        continue;
                    }
                    sourceZone = OppositeZone(sourceZone); // encodes by approach zone
                    sourceState = fixedStates.encodeCorner(sourcePoint, sourceZone);
                }

                if(fixedStates.pointIsCorner(targetPoint))
                {
                    eSilhouetteZone targetZone = fixedStates.decodeApproachZone(targetState);
                    if(fixedStates.zoneFor(targetPoint, sourceP.p) != targetZone)
                    {
                        continue;
                    }
                }

                if(fixedStates.testCollision(sourceP, targetP))
                {
                    continue;
                }

                unsigned long costOut, costBack;
                bool blockedOut, blockedBack;
                fixedStates.getCosts(sourceP, targetP, costOut, costBack, blockedOut, blockedBack);

                if(!blockedOut)
                {
                    connections[sourceState].push_back(targetState);
                    costs[sourceState].push_back(costOut);
                }
                if(!blockedBack)
                {
                    long rSourceState = fixedStates.reverseState(sourceState);
                    long rTargetState = fixedStates.reverseState(targetState);
                    connections[rTargetState].push_back(rSourceState);
                    costs[rTargetState].push_back(costBack);
                }
            }
        }

    // connections off mesh

        long i;
        for(i = 0; i < offMeshConnections.numberOfEndPoints(); i++)
        {
            if(endPointObstructed[i])
            {
                continue;
            }
            long sourcePoint = fixedStates.pointForEndPoint(i);
            long sourceState = fixedStates.encodeEndPoint(sourcePoint, true);
            cInternalPosition sourceP = fixedStates.getPosition(sourcePoint);
            long j;
            for(j = 0; j < offMeshConnections.connectionsFromEndPoint(i); j++)
            {
                long targetEndPoint;
                long penalty;
                offMeshConnections.connectionFromEndPoint(i, j, targetEndPoint, penalty);
                if(endPointObstructed[targetEndPoint])
                {
                    continue;
                }
                long targetPoint = fixedStates.pointForEndPoint(targetEndPoint);
                long targetState = fixedStates.encodeEndPoint(targetPoint, false);

                cInternalPosition targetP = fixedStates.getPosition(targetPoint);

                unsigned long cost = fixedStates.getCost(sourceP, targetP);
                cost += penalty; //...... check for overflow

                connections[sourceState].push_back(targetState);
                costs[sourceState].push_back(cost);
            }
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

    long
    size() const
    {
        return SizeL(_connections);
    }
    long
    connectionsFrom(long sourceState) const
    {
        return _connections.subVectorSize(sourceState);
    }
    long
    connectionFrom(long sourceState, long connectionIndex) const
    {
        return _connections[sourceState][connectionIndex];
    }
    long connectionCost(long sourceState, long connectionIndex) const
    {
        return _costs[sourceState][connectionIndex];
    }

    void save(iXMLOutputStream& os) const;
};

#endif
