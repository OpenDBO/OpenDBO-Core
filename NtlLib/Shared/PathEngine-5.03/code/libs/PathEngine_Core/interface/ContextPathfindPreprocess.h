//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/PathEngine_Core/interface/CombinedCostCalculator.h"

//... these were required by VC6, looks like VC compiler bug
//#include "libs/PathEngine_Core/interface/ShapesCostCalculator.h"
#include "libs/PathEngine_Core/interface/iMovementCostCalculator.h"

#include "libs/PathEngine_Core/interface/ShapeCollisionPreprocess.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class iLineCollisionTester;
class cCornerProvider;
class cCollisionPreprocess;
class cPathfindPreprocess;
class cInternalPosition;
class cFlagVector;
class cQueryContext;
class cLocalisedConvexShape;
class cLocalisedConvexShapeSet;
class cShapesCostCalculator;
class cShapePathfindPreprocess;

class cContextPathfindPreprocess : public bThreadSafeHandleTarget
{
    std::vector<cHandle<cShapeCollisionPreprocess> > _heldPreprocess;

    tPoint _boundsMin, _boundsMax;
    bool _boundsValid;

    cCornerProvider* _cornerProvider;

    cLocalisedConvexShapeSet* _smallConvexOverlapped;
    cLocalisedConvexShapeSet* _obstaclesAndSmallConvexOverlapped;

    cShapesCostCalculator* _costCalculator;
    std::vector<iMovementCostCalculator*> _mesh3DCostCalculators;

    cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator> _combinedCostCalculator;

    std::vector<size_t> _smallConvexAdded;

public:

    cContextPathfindPreprocess(cQueryContext& qc);
    ~cContextPathfindPreprocess();

    void clear();

    void setQueryBounds(const tPoint& boundsMin, const tPoint& boundsMax);

    // ownership is taken
    void setMesh3DCostCalculator(iMovementCostCalculator* calculator);

    void addAgentExpansion(
        cQueryContext& qc,
        const cCollisionPreprocess& cp,
        const cPathfindPreprocess& pp,
        cHandle<cShapeCollisionPreprocess> scp,
        cHandle<cShapePathfindPreprocess> spp);

    void addAgentExpansion_SoftObstacle(
        cQueryContext& qc,
        const cCollisionPreprocess& cp,
        cHandle<cShapeCollisionPreprocess> scp,
        float costToTraverse, const tPoint& directionForCostToTraverse
        );
    
//    void addAgentExpansion_SoftObstacle(cQueryContext& qc,
//        const cCollisionPreprocess& cp,
//        const cLocalisedConvexShape* expansion,
//        float costToTraverse, const tPoint& directionForCostToTraverse
//        );

    void addMesh3DSoftCorners(const cCornerProvider& mesh3DSoftCorners);

    void setCompleted();

// end user

    const cCornerProvider& refCornerProvider() const
    {
        return *_cornerProvider;
    }

    const cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator>* getCostCalculator() const;

    const cLocalisedConvexShapeSet& refObstaclesAndSmallConvexOverlapped() const
    {
        return *_obstaclesAndSmallConvexOverlapped;
    }
    const cLocalisedConvexShapeSet& refSmallConvexOverlapped() const
    {
        return *_smallConvexOverlapped;
    }

    const tPoint& boundsMin() const
    {
        return _boundsMin;
    }
    const tPoint& boundsMax() const
    {
        return _boundsMax;
    }
    bool boundsValid() const
    {
        return _boundsValid;
    }
};
