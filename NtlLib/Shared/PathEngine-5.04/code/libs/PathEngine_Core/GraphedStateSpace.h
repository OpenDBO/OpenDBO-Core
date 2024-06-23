//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GRAPHED_STATE_SPACE_INCLUDED
#define GRAPHED_STATE_SPACE_INCLUDED

#include "StateSpaceBase.h"
#include "libs/Instrumentation/interface/Profiler.h"
class iLineCollisionTester;
class cVisibilityGraph;
class cOffMeshConnections;

template <
    class tCorners,
    class tFixedCollision,
    class tFixedCostCalculator,
    class tDynamicCollision,
    class tDynamicPointCollision,
    class tDynamicCostCalculator
    >
class cGraphedStateSpace : public bStateSpace<tCorners>
{
    typedef bStateSpace<tCorners> tParent;

protected:

    cQueryContext& _qc;
    const tFixedCollision& _fixedCollision;
    const tFixedCostCalculator* _fixedCostCalculator;
    const tDynamicCollision& _dynamicCollision;
    const tDynamicPointCollision& _dynamicPointCollision;
    const tDynamicCostCalculator* _dynamicCostCalculator;
    const cVisibilityGraph& _graph;
    const std::vector<std::vector<long> >& _silhouetteLookup;

    long permanentPoints() const
    {
        return _graph.size() / 2;
    }
    bool stateIsFixed(long state) const
    {
        return state >= 4 && state < _graph.size();
    }

public:

    cGraphedStateSpace(
        cQueryContext& qc,
        const cOffMeshConnections& offMeshConnections,
        const tCorners& corners,
        const tFixedCollision& fixedCollision,
        const tFixedCostCalculator* fixedCostCalculator,
        const tDynamicCollision& dynamicCollision,
        const tDynamicPointCollision& dynamicPointCollision,
        const tDynamicCostCalculator* dynamicCostCalculator,
        const cVisibilityGraph& graph,
        const std::vector<std::vector<long> >& silhouetteLookup) :
     tParent(offMeshConnections, corners),
     _qc(qc),
     _fixedCollision(fixedCollision),
     _fixedCostCalculator(fixedCostCalculator),
     _dynamicCollision(dynamicCollision),
     _dynamicPointCollision(dynamicPointCollision),
     _dynamicCostCalculator(dynamicCostCalculator),
     _graph(graph),
     _silhouetteLookup(silhouetteLookup)
    {
    }

    template <class tStateSpace, class cCallBackTarget>
    void generateNonGoalConnections(
                    tStateSpace& stateSpace, cCallBackTarget &callBackTarget,
                    long previousState, long sourceState, unsigned long costSoFar)
    {
        cInternalPosition sourceP = this->positionForState(sourceState);

	// connections to permanent corners

        if(stateIsFixed(sourceState))
		{
        // use visibility graph
            long i;
            if(this->stateIsCorner(sourceState))
            {
            // silhouette conditions are already checked
            // except for side of approach line
                eSilhouetteZone approachZone = this->decodeApproachZone(sourceState);
                tLine line(this->positionForState(previousState).p, sourceP.p);
                eSide side = SIDE_RIGHT;
                if(approachZone == SZ_RIGHTSILHOUETTE)
                {
                    side = SIDE_LEFT;
                }
                for(i = 0; i < _graph.connectionsFrom(sourceState); i++)
                {
                    long targetState = _graph.connectionFrom(sourceState, i);
                    if(line.side(this->positionForState(targetState).p) != side)
                    {
                        continue;
                    }
                    callBackTarget.connectionCallBack(stateSpace, targetState, _graph.connectionCost(sourceState, i));
                }
            }
            else
            {
            // there is no approach line
                for(i = 0; i < _graph.connectionsFrom(sourceState); i++)
                {
                    long targetState = _graph.connectionFrom(sourceState, i);
                    callBackTarget.connectionCallBack(stateSpace, targetState, _graph.connectionCost(sourceState, i));
                }
            }
        }
		else
        {
		// use silhouette lookup
            long faceIndex = sourceP.cell();
			size_t i;
            for(i = 0; i < _silhouetteLookup[faceIndex].size(); i++)
            {
                long targetState = _silhouetteLookup[faceIndex][i];
                cInternalPosition targetP = this->positionForState(targetState);
                if(!this->checkSourceCornerSilhouetteConditions(previousState, sourceState, targetP.p))
                {
                    continue;
                }
                if(this->stateIsCorner(targetState))
                {
                    eSilhouetteZone regionApproachZone = this->decodeApproachZone(targetState);
                    eSilhouetteZone targetApproachZone = zoneFor(this->pointForState(targetState), sourceP.p);
                    if(targetApproachZone != regionApproachZone)
                    {
                        continue;
                    }
                }
                callBackTarget.connectionCallBack(stateSpace, targetState, this->getCost(sourceP, targetP));
            }
		}
        
    // connections to dynamic corners

        if(!this->stateIsCorner(sourceState) && this->decodeIsDeparting(sourceState))
        {
            return;
        }

        long sourcePoint = this->pointForState(sourceState);
        long i;
        for(i = permanentPoints(); i < this->points(); i++)
        {
            if(i == sourcePoint)
            {
                continue;
            }
            cInternalPosition targetP = this->getPosition(i);
            if(!this->checkSourceCornerSilhouetteConditions(previousState, sourceState, targetP.p))
            {
                continue;
            }
            long targetState;
            if(this->pointIsCorner(i))
            {
                eSilhouetteZone targetApproachZone = this->zoneFor(i, sourceP.p);
                if(targetApproachZone == SZ_NOTSILHOUETTE)
                {
                    continue;
                }
                targetState = this->encodeCorner(i, targetApproachZone);
            }
            else
            {
                targetState = this->encodeEndPoint(i, true);
            }
            callBackTarget.connectionCallBack(stateSpace, targetState, this->getCost(sourceP, targetP));
        }
    }

// interface for aStar

    bool validateConnection(long source, long target) const
    {
        cInternalPosition sourceP = this->positionForState(source);
        cInternalPosition targetP = this->positionForState(target);
        if(sourceP == targetP)
        {
            if(!this->stateIsCorner(source) || !this->stateIsCorner(target))
            {
                return true;
            }
            return false;
        }
        if(stateIsFixed(source) && stateIsFixed(target))
        {
        // visibility graph connection
            if(!this->stateIsCorner(source) && this->decodeIsDeparting(source))
            {
                return !_dynamicPointCollision.testPointCollision(_qc, targetP.f, targetP.p);
            }
            return !_dynamicCollision.testCollision(_qc, sourceP, targetP);
        }
        return
            !_fixedCollision.testCollision(_qc, sourceP, targetP)
            &&
            !_dynamicCollision.testCollision(_qc, sourceP, targetP);
    }

    unsigned long getAdditionalCost(long source, long target, bool& blocked) const
    {
        blocked = false;
        unsigned long result = 0;
        cInternalPosition from = this->positionForState(source);
        cInternalPosition to = this->positionForState(target);

      // dynamic costs are always added
        if(_dynamicCostCalculator)
        {
            if(this->stateIsCorner(source) || !this->decodeIsDeparting(source)) // don't process off-mesh connections here
            {
                result = static_cast<unsigned long>(_dynamicCostCalculator->calculate(_qc, from, to.p, blocked));
                if(blocked)
                {
                    return 0ul;
                }
            }
        }

      // fixed costs are added for connections not in the visibility graph
      // (costs in the visibility graph already include fixed costs)
        if(_fixedCostCalculator && (!stateIsFixed(source) || !stateIsFixed(target)))
        {
            unsigned long toAdd = static_cast<unsigned long>(_fixedCostCalculator->calculate(_qc, from, to.p, blocked));
            if(AddWithOverflow(result, toAdd, result))
            {
                result = 0xffffffff;
            }
        }
        return result;
    }
};

#endif
