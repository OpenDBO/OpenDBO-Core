//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "GraphedStateSpace.h"
#include "i_pathengine.h"

class cOffMeshConnections;

template <
    class tCorners,
    class tFixedCollision,
    class tFixedCostCalculator,
    class tDynamicCollision,
    class tDynamicPointCollision,
    class tDynamicCostCalculator
    >
class cToTargetStateSpace : public cGraphedStateSpace<
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

    std::vector<char> _stateCanPotentiallyConnectToGoal;
    iQueryCallBack* _callBack;
    long _callBackFrequency;
    long _callBackCount;

public:

    cToTargetStateSpace(
        cQueryContext& qc,
        const cOffMeshConnections& offMeshConnections,
        const tCorners& corners,
        const tFixedCollision& fixedCollision,
        const tFixedCostCalculator* fixedCostCalculator,
        const tDynamicCollision& dynamicCollision,
        const tDynamicPointCollision& dynamicPointCollision,
        const tDynamicCostCalculator* dynamicCostCalculator,
        const cVisibilityGraph& graph,
        const std::vector<std::vector<long> >& silhouetteLookup,
        iQueryCallBack* queryCallBack
        ) :
     tParent(qc, offMeshConnections, corners, fixedCollision, fixedCostCalculator, dynamicCollision, dynamicPointCollision, dynamicCostCalculator, graph, silhouetteLookup),
     _stateCanPotentiallyConnectToGoal(tParent::getNumberOfStates(), false),
     _callBack(queryCallBack)
    {
        if(_callBack)
        {
            _callBackFrequency = _callBack->desiredCallBackFrequency();
            assertR(_callBackFrequency >= 1);
            _callBackCount = _callBackFrequency;
        }
    }

    // must be called once and only once
    void setGoal(const cInternalPosition& value)
    {
        tParent::setGoal(value);
		size_t i;
        long faceIndex = value.cell();
		for(i = 0; i < this->_silhouetteLookup[faceIndex].size(); i++)
		{
            long targetState = this->_silhouetteLookup[faceIndex][i];
            long reverseState = tParent::reverseState(targetState);
            _stateCanPotentiallyConnectToGoal[reverseState] = true;
		}
    }

// (remainder of) interface for AStar

    template <class cCallBackTarget>
    void generatePotentialConnections(cCallBackTarget &callBackTarget,
                    long previousState, long sourceState, unsigned long costSoFar)
    {
        cInternalPosition sourceP = this->positionForState(sourceState);

	// connection to goal

        if(!this->stateIsFixed(sourceState) || _stateCanPotentiallyConnectToGoal[sourceState])
		{
			if(this->checkSourceCornerSilhouetteConditions(previousState, sourceState, this->_goal.p))
            {
				callBackTarget.connectionCallBack(*this, 0, this->getCost(sourceP, this->_goal));
            }
		}

        tParent::generateNonGoalConnections(*this, callBackTarget, previousState, sourceState, costSoFar);
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
        return tParent::isGoal(index);
    }
};
