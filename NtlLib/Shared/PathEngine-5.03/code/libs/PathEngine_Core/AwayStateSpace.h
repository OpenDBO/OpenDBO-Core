//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "GraphedStateSpace.h"

class cOffMeshConnections;

template <
    class tCorners,
    class tTargets,
    class tFixedCollision,
    class tFixedCostCalculator,
    class tDynamicCollision,
    class tDynamicPointCollision,
    class tDynamicCostCalculator
    >
class cAwayStateSpace : public cGraphedStateSpace<
                                tCorners, 
                                tFixedCollision,
                                tFixedCostCalculator,
                                tDynamicCollision,
                                tDynamicPointCollision,
                                tDynamicCostCalculator
                                >
{
    typedef cGraphedStateSpace<
                tCorners,
                tFixedCollision,
                tFixedCostCalculator,
                tDynamicCollision,
                tDynamicPointCollision,
                tDynamicCostCalculator
                > tParent;

    const tTargets& _targets;
    iQueryCallBack* _callBack;
    long _callBackFrequency;
    long _callBackCount;

    long stateForTarget(long target) const
    {
        return tParent::states() + target;
    }
    unsigned long getCostToXY(const cInternalPosition& from, const tPoint& to) const
    {
        return Length(to - from.p);
    }

public:

    cAwayStateSpace(
        cQueryContext& qc,
        const cOffMeshConnections& offMeshConnections,
        const tCorners& corners,
        const tTargets& targets,
        const tFixedCollision& fixedCollision,
        const tFixedCostCalculator* fixedCostCalculator,
        const tDynamicCollision& dynamicCollision,
        const tDynamicPointCollision& dynamicPointCollision,
        const tDynamicCostCalculator* dynamicCostCalculator,
        const cVisibilityGraph& graph,
        const std::vector<std::vector<long> >& silhouetteLookup,
        iQueryCallBack* queryCallBack
        ) :
     tParent(qc,
            offMeshConnections,
            corners,
            fixedCollision,
            fixedCostCalculator,
            dynamicCollision,
            dynamicPointCollision,
            dynamicCostCalculator,
            graph, silhouetteLookup),
     _targets(targets),
     _callBack(queryCallBack)
    {
        if(_callBack)
        {
            _callBackFrequency = _callBack->desiredCallBackFrequency();
            assertR(_callBackFrequency >= 1);
            _callBackCount = _callBackFrequency;
        }
    }

// interface to astar

    long getNumberOfStates() const
    {
        return tParent::states() + _targets.getNumberOfTargets();
    }
    bool isGoal(long index)
    {
        if(_callBack)
        {
            --_callBackCount;
            if(_callBackCount == 0)
            {
                _callBackCount = _callBackFrequency;
                if(_callBack->abort())
                {
                    return true;
                }
            }
        }
        return
            index >= tParent::states()
            ||
            (!this->stateIsCorner(index) && !this->decodeIsDeparting(index) && !_targets.pointIsInside(this->positionForState(index).p))
            ;
    }

    unsigned long getMinimumCostToGoal(long state) const
    {
        if(state >= tParent::states())
        {
            return 0;
        }
        return _targets.minimumDistanceToTarget(tParent::positionForState(state).p);
    }
    unsigned long getAdditionalCost(long source, long target, bool& blocked) const
    {
        if(target < tParent::states())
        {
            return tParent::getAdditionalCost(source, target, blocked);
        }
        blocked = false;
        cInternalPosition sourceIP = this->positionForState(source);
        tPoint targetP = getGoalPoint(target, source);
        unsigned long result = 0;
        if(this->_dynamicCostCalculator)
        {
            result = static_cast<unsigned long>(this->_dynamicCostCalculator->calculate(this->_qc, sourceIP, targetP, blocked));
            if(blocked)
            {
                return 0ul;
            }
        }
        if(this->_fixedCostCalculator)
        {
            unsigned long toAdd = static_cast<unsigned long>(this->_fixedCostCalculator->calculate(this->_qc, sourceIP, targetP, blocked));
            if(AddWithOverflow(result, toAdd, result))
            {
                result = 0xffffffff;
            }
        }
        return result;
    }

    template <class cCallBackTarget>
    void generatePotentialConnections(cCallBackTarget &callBackTarget,
                    long previousState, long sourceState, unsigned long costSoFar)
    {
        cInternalPosition sourceP = this->positionForState(sourceState);

    // connections to targets
        {
            long i;
            for(i = 0; i < _targets.getNumberOfTargets(); i++)
            {
                tPoint targetPp = _targets.getTargetPoint(i, sourceP.p);
                if(this->checkSourceCornerSilhouetteConditions(previousState, sourceState, targetPp))
                {
                    callBackTarget.connectionCallBack(*this, stateForTarget(i), getCostToXY(sourceP, targetPp));
                }
            }
        }

        tParent::generateNonGoalConnections(*this, callBackTarget, previousState, sourceState, costSoFar);
    }

    bool validateConnection(long source, long target) const
    {
        cInternalPosition sourceP = this->positionForState(source);

        if(target >= tParent::states())
        {
        // connection goes to a target point
            target -= tParent::states();
            tPoint targetPp = _targets.getTargetPoint(target, sourceP.p);
            tLine testLine(sourceP.p, targetPp);
            if(this->_fixedCollision.testCollisionToXY(this->_qc, sourceP.f, testLine))
            {
                return false;
            }
            return !this->_dynamicCollision.testCollisionToXY(this->_qc, sourceP.f, testLine);
        }

        cInternalPosition targetP = this->positionForState(target);

        if(!_targets.pointIsInside(targetP.p))
        {
            return false;
        }

        return tParent::validateConnection(source, target);
    }

// path building

    bool exitedThroughOffMeshConnection(long goalState) const
    {
        return goalState < tParent::states();
    }
    tPoint getGoalPoint(long goalState, long previousState) const
    {
        if(goalState >= tParent::states())
        {
          // goal is at boundary
            goalState -= tParent::states();
            tPoint previousP = tParent::positionForState(previousState).p;
            return _targets.getTargetPoint(goalState, previousP);
        }
        // search was aborted before reachine boundary
        return tParent::positionForState(goalState).p;
    }
};

