//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Assert.h"
#include "SimpleHeap.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "platform_common/AddWithOverflow.h"
#include "common/Containers/BitVector.h"
#include "common/Containers/PriorityQueue.h"

class cAStarHeap
{
    class cConnectionNode
    {
        long _previousPoint;

    public:

        unsigned long _costSoFar;
        unsigned long _minimumTotalCost;
        cConnectionNode* _next;

        void initialise(long previousPoint, unsigned long costSoFar, unsigned long minimumTotalCost)
        {
            _previousPoint = previousPoint * 2; //.. check for overflow
            _costSoFar = costSoFar;
            _minimumTotalCost = minimumTotalCost;
        }

        long getPreviousPoint()
        {
            return _previousPoint / 2;
        }
        bool costHasBeenFinalised() const
        {
            return (_previousPoint & 1) == 1;
        }
        void finaliseCostSoFar(unsigned long additionalCost)
        {
            assertD(!costHasBeenFinalised());
            _previousPoint++;
            if(AddWithOverflow(_costSoFar, additionalCost, _costSoFar))
            {
                _costSoFar = 0xffffffff;
            }
            if(AddWithOverflow(_minimumTotalCost, additionalCost, _minimumTotalCost))
            {
                _minimumTotalCost = 0xffffffff;
            }
        }
    };

    class cOpenPoint
    {
        unsigned long _minimumCostToGoal;
        cConnectionNode* _firstConnection;

    public:

        void initialiseWithCostToGoal(unsigned long minimumCostToGoal)
        {
            _firstConnection = 0;
            _minimumCostToGoal = minimumCostToGoal;
        }
        bool hasConnection() const
        {
            return _firstConnection != 0;
        }
        void addFirstConnection(cConnectionNode* node)
        {
            assertD(!hasConnection());
            _firstConnection = node;
            node->_next = 0;
        }

        // returns true if this connection is the new first connection
        bool addConnection(cConnectionNode* node)
        {
            assertD(hasConnection());
            cConnectionNode* addAfter;

        // on equality, add after first
            if(node->_minimumTotalCost < _firstConnection->_minimumTotalCost)
            {
                node->_next = _firstConnection;
                _firstConnection = node;
                return true;
            }

            addAfter = _firstConnection;
        // for trailing nodes, on equality add before
            while(addAfter->_next && addAfter->_next->_minimumTotalCost < node->_minimumTotalCost)
            {
                addAfter = addAfter->_next;
            }

            node->_next = addAfter->_next;
            addAfter->_next = node;

            return false;
        }
        cConnectionNode* removeFirstConnection()
        {
            assertD(hasConnection());
            cConnectionNode* returnvalue;

            returnvalue = _firstConnection;
            _firstConnection = _firstConnection->_next;
            return returnvalue;
        }

        bool firstConnectionCostHasBeenFinalised() const
        {
            assertD(hasConnection());
            return _firstConnection->costHasBeenFinalised();
        }

        // returns true if the first connection gets moved back as a result
        bool finaliseFirstConnectionCost(unsigned long additionalCost)
        {
            assertD(hasConnection());
            _firstConnection->finaliseCostSoFar(additionalCost);

            cConnectionNode* node = _firstConnection;
            if(!node->_next)
            {
                return false;
            }
        // on equality, this connection stays as first
            if(node->_next->_minimumTotalCost >= node->_minimumTotalCost)
            {
                return false;
            }

            cConnectionNode* addAfter = node->_next;
            _firstConnection = addAfter;
        // on equality add before
            while(addAfter->_next && addAfter->_next->_minimumTotalCost < node->_minimumTotalCost)
            {
                addAfter = addAfter->_next;
            }
            node->_next = addAfter->_next;
            addAfter->_next = node;
            return true;
        }
        unsigned long getMinimumCostToGoal() const
        {
            return _minimumCostToGoal;
        }

        // added for info about next best
        unsigned long getMinimumTotalCost() const
        {
            assertD(hasConnection());
            return _firstConnection->_minimumTotalCost;
        }

//        bool operator<(const cOpenPoint& rhs) const
//        {
//            assertD(hasConnection());
//            assertD(rhs.hasConnection());
//            return 
//                _firstConnection->_minimumTotalCost
//                <
//                rhs._firstConnection->_minimumTotalCost;
//        }

        long getConnectionSource() const
        {
            assertD(hasConnection());
            return _firstConnection->getPreviousPoint();
        }

        cConnectionNode* takeNodeChain()
        {
            assertD(hasConnection());
            return _firstConnection;
        }
    };

    class cDiscardedPoint
    {
    public:

        unsigned long _costSoFar;
        long _previousPoint;

        void initialiseAsStart()
        {
            _costSoFar = 0;
            _previousPoint = -1;
        }
    };

    class cPointInfo
    {
    public:

        union
        {
            cOpenPoint _open;
            cDiscardedPoint _discarded;
        };

        cConnectionNode* discardConnectionCompleted()
        {
            assertD(_open.hasConnection());
            cConnectionNode* returnvalue;
            returnvalue = _open.takeNodeChain();
            _discarded._costSoFar = returnvalue->_costSoFar;
            _discarded._previousPoint = returnvalue->getPreviousPoint();
            return returnvalue;
        }
    };

    class cMinimumTotalCostPredicate
    {
    public:

        cPointInfo* _pointArray;

//        cMinimumTotalCostPredicate()
//        {
//        }
//        cMinimumTotalCostPredicate(const cMinimumTotalCostPredicate& rhs)
//        {
//            _pointArray = rhs._pointArray;
//        }

        // predicate for open queue
        // returns true if rhs total distance is less than lhs total distance
        bool operator()(long lhs, long rhs)
        {
            unsigned long lhsCost = _pointArray[lhs]._open.getMinimumTotalCost();
            unsigned long rhsCost = _pointArray[rhs]._open.getMinimumTotalCost();
            if(rhsCost < lhsCost)
            {
                return true;
            }
            if(rhsCost > lhsCost)
            {
                return false;
            }
            // in the case of identical costs, point with lower index is preferred
            // this is done to ensure that path found is independant of algorithm / data structure implementation details in this case
            //..... optimise - benchmark to find out if this additional test makes any noticeable difference to performance
            return rhs < lhs;
        }        
    };

    cMinimumTotalCostPredicate _predicate;

    long _maximumPoints;
    long _currentPoints;
    cPointInfo* _pointArray;
//    unsigned char* _pointInitialisedArray;
    cBitVector _pointStatusFlags;

    cSimpleHeap<cConnectionNode, 1500> _nodeHeap;

    cPriorityQueue<long, cMinimumTotalCostPredicate> _openQueue;

    // state for connections callback
    unsigned long _costSoFar;
    long _currentPoint;

    void setAllPointsUninitialised()
    {
        _pointStatusFlags.fill(false);
    }
    void setPointHasBeenInitialised(const long pointIndex)
    {
        _pointStatusFlags.setBit(pointIndex * 2, true);
    }
    bool getPointHasBeenInitialised(const long pointIndex) const
    {
        return _pointStatusFlags[pointIndex * 2];
    }
    void setPointHasBeenDiscarded(const long pointIndex)
    {
        _pointStatusFlags.setBit(pointIndex * 2 + 1, true);
    }
    bool getPointHasBeenDiscarded(const long pointIndex) const
    {
        return _pointStatusFlags[pointIndex * 2 + 1];
    }

    void releaseChainOfNodes(cConnectionNode* node)
    {
        assertD(node);
        cConnectionNode* torelease;
        do
        {
            torelease = node;
            node = node->_next;
            _nodeHeap.release(torelease);
        }
        while(node);
    }

    void setNumberOfPoints(long points)
    {
        if(points > _maximumPoints)
        {
            delete [] _pointArray;
            _maximumPoints = points;
            _currentPoints = points;
            _pointArray = new cPointInfo [_maximumPoints];
            _predicate._pointArray = _pointArray;
            _pointStatusFlags.resize(_maximumPoints * 2);
        }
    }

    // prevent copying and assignment
    cAStarHeap(const cAStarHeap&);
    const cAStarHeap& operator=(const cAStarHeap&);

public:

    cAStarHeap() :
      _maximumPoints(1000),
      _currentPoints(_maximumPoints),
      _pointStatusFlags(_maximumPoints * 2),
      _openQueue(500)
    {
        _pointArray = new cPointInfo [_maximumPoints];
        _predicate._pointArray = _pointArray;
        _openQueue.setPredicate(&_predicate);
    }
    ~cAStarHeap()
    {
        delete [] _pointArray;
    }

    template <class cStateSpace>
    void connectionCallBack(
        const cStateSpace& stateSpace, 
        long target, unsigned long connectionCost
        )
    {
        bool initialised;
        unsigned long costAfterConnection;
        unsigned long costToGoal;
        unsigned long totalCost;
        cConnectionNode* node;

        if(getPointHasBeenDiscarded(target))
        {
            return;
        }
        initialised = getPointHasBeenInitialised(target);

        if(AddWithOverflow(_costSoFar, connectionCost, costAfterConnection))
        {
//    // on path cost overflow, connection is simply ignored
//            return;
    // on path cost overflow, cost is set to maximum representable value
            costAfterConnection = 0xffffffff;
        }

        if(!initialised)
        {
            costToGoal = stateSpace.getMinimumCostToGoal(target);
            _pointArray[target]._open.initialiseWithCostToGoal(costToGoal);
            setPointHasBeenInitialised(target);
        }
        else
        {
            costToGoal = _pointArray[target]._open.getMinimumCostToGoal();
        }

        if(AddWithOverflow(costAfterConnection, costToGoal, totalCost))
        {
//            // on path cost overflow, connection is simply ignored
//                    continue;
    // on path cost overflow, cost is set to maximum representable value
            totalCost = 0xffffffff;
        }

        node = _nodeHeap.get();
        node->initialise(_currentPoint, costAfterConnection, totalCost);

        if(_pointArray[target]._open.hasConnection())
        {
            if(_pointArray[target]._open.addConnection(node))
            {
            // node is new shortest so far to this point
            // reinsert in best first ordering
            // (in front of previous position)
                _openQueue.updateValueForwards(target);
            }
        }
        else
        {
            _pointArray[target]._open.addFirstConnection(node);

        // node is first connection to this point
        // insert in best first ordering
            _openQueue.insert(target);
        }
    }

    
    template <class cStateSpace>
    long search(
        cStateSpace* const stateSpace, 
        const long start
        )
    {
        PROFILER_SCOPE("ASTAR");
        long currentPoint;
        long previousPoint;

        setNumberOfPoints(stateSpace->getNumberOfStates());

        setAllPointsUninitialised();

        currentPoint = start;
        _pointArray[currentPoint]._discarded.initialiseAsStart();
        setPointHasBeenInitialised(currentPoint);
        setPointHasBeenDiscarded(currentPoint);

        previousPoint = -1;
        
        while(currentPoint != -1 && !stateSpace->isGoal(currentPoint))
        {
        // _bestPointIndex has already been validated and discarded
        // now generate and process connections for this point

            assertD(getPointHasBeenDiscarded(currentPoint));

            _costSoFar = _pointArray[currentPoint]._discarded._costSoFar;
            _currentPoint = currentPoint;

            stateSpace->generatePotentialConnections(
                *this, 
                previousPoint, currentPoint, _costSoFar);

        // validate best, removing invalid entries, 
        // until valid point found or ran out of points

            while(1)
            {
                if(_openQueue.empty())
                {
                    currentPoint = -1;
                    break;
                }
              // remove first item from best first ordering as next current point
                currentPoint = _openQueue.popFront();

              // validate this connection
                previousPoint = _pointArray[currentPoint]._open.getConnectionSource();
                if(!stateSpace->validateConnection(previousPoint, currentPoint))
                {
                  // did not validate
                  // remove this connection
                    _nodeHeap.release(_pointArray[currentPoint]._open.removeFirstConnection());
                    if(_pointArray[currentPoint]._open.hasConnection())
                    {
                        // insert again into best first ordering
                        _openQueue.insert(currentPoint);
                    }
                    continue;
                }

              // calculate any additional costs now if necessary
                bool continueAfterWhile = false;
                while(!_pointArray[currentPoint]._open.firstConnectionCostHasBeenFinalised())
                {
                    bool blocked;
                    unsigned long additionalCost = stateSpace->getAdditionalCost(previousPoint, currentPoint, blocked);
                    if(blocked)
                    {
                    // remove this connection
                        _nodeHeap.release(_pointArray[currentPoint]._open.removeFirstConnection());
                        if(_pointArray[currentPoint]._open.hasConnection())
                        {
                            // insert again in best first ordering
                            _openQueue.insert(currentPoint);
                        }
                        continueAfterWhile = true;
                        break;
                    }
                    if(additionalCost) // ok to discard without marking as finalised
                    {
                        _pointArray[currentPoint]._open.finaliseFirstConnectionCost(additionalCost);
                        if(!_openQueue.empty() && _predicate(currentPoint, _openQueue.front()))
                        {
                          // currentPoint is no longer best after additional cost
                          // reinsert in ordering
                            _openQueue.insert(currentPoint);
                            continueAfterWhile = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                if(continueAfterWhile)
                {
                    continue;
                }

              // currentPoint is valid and still first after additional costs
              // discard this point and release node chain
                cConnectionNode* nodeChain;
                nodeChain = _pointArray[currentPoint].discardConnectionCompleted();
                setPointHasBeenDiscarded(currentPoint);
                releaseChainOfNodes(nodeChain);
                break;
            }
        }

        long i;
        for(i = 0; i != _openQueue.size(); ++i)
        {
            releaseChainOfNodes(_pointArray[_openQueue[i]]._open.takeNodeChain());
//            setPointHasBeenDiscarded(_openQueue[i]); // doesn't actually need to be flagged as discarded at this point
        }
        _openQueue.clear();

        return currentPoint;
    }


    long getParentPoint(long point) const
    {
        return _pointArray[point]._discarded._previousPoint;
    }
    unsigned long getPathLengthTo(long point) const
    {
        return _pointArray[point]._discarded._costSoFar;
    }
    long getPathPointsTo(long point) const
    {
        long result = 0;
        while(point != -1)
        {
            result++;
            point = getParentPoint(point);
        }
        return result;
    }
};
