//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_AGENT_INCLUDED
#define C_AGENT_INCLUDED

//#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/PathEngine_Core/interface/ShapeCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/ShapePathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/ShapeRenderPreprocess.h"
#include "common/interface/Assert.h"
#include "common/Containers/SwapOutMirror.h"
#include "common/Handle.h"
#include "common/ModifiedStateTracker.h"
#include "platform_common/Mutex.h"
#include "common/STL/vector.h"
#include "i_pathengine.h"

class cMesh;
class cShapeCollisionPreprocess;
class cShapePathfindPreprocess;
class cPathfindPreprocess;
class iAPIObjectWrapper;

class cAgent : public iAgent
{
  // immutable
    cMesh* _mesh;
    cHandle<cShape> _shape;
    tPoint _shapeBoundsMin, _shapeBoundsMax;
    long _indexInMesh;

    cMutex _stateMutex; // applied on external interface to synchronise access to the following state information
    cModifiedStateTracker _stateTracker;
    cPosition _position;
    cInternalPosition _internalPosition;
    void* _userData;
    float _costToTraverse;
    tPoint _directionForCostToTraverse;
    cHandle<cShapeCollisionPreprocess> _unexpandedPreprocess;
    std::vector<cHandle<cShapeCollisionPreprocess> > _collisionPreprocess;
    std::vector<cHandle<cShapePathfindPreprocess> > _pathfindPreprocess;
    std::vector<cHandle<cShapeRenderPreprocess> > _renderPreprocess;

    // protected by iMesh::_agentsAndObstacleSetsMutex
    std::vector<long> _indexInObstacleSetVector;

    void setPosition(const cPosition& value);

public:

    iAPIObjectWrapper* _wrapper;

    cAgent(cMesh* mesh, cShape* shape, const cPosition& position);
    void setIndexInMesh(long value)  // must be called directly post construction
    {
        _indexInMesh = value;
    }

    ~cAgent();

// iAgent

    void destroy();
    iMesh* getMesh() const;
    iShape* getShape() const;
    void setUserData(void* voidPointer);
    void* getUserData() const;
    cPosition getPosition() const;
    void moveTo(const cPosition& newPosition);
    bool canCollide() const;
    bool canPathfind() const;
    bool testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const;
    bool testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const;
    bool testCollisionTo_XY(iCollisionContext* context, long x, long y, long& cell) const;
    iCollisionInfo* firstCollisionTo(iCollisionContext* context, long x, long y, long& cell) const;
    cPosition findClosestUnobstructedPosition(iCollisionContext* context, long maximumDistance) const;
    void getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, long& numberOverlapped) const;
    bool testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const;
    iPath* findShortestPathTo(iCollisionContext* context, const cPosition& target) const;
    iPath* findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const;
    iPath* findPathAway(iCollisionContext* context, const cPosition& awayFrom, long distanceAway) const;
    iPath* findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, long distanceAway, iQueryCallBack* queryCallBack) const;
    iCollisionInfo* advanceAlongPath(iPath* path, float distance, iCollisionContext* context);
    iCollisionInfo* advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY);
    iPath* generateCurvedPath(iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;
    void setTraverseCost(float cost);
    void setTraverseCostDirection(long directionVectorX, long directionVectorY);
    float getTraverseCost() const;
    void getTraverseCostDirection(long& directionVectorX, long& directionVectorY) const;
    iPath* generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long endVectorX, long endVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;
    void operator delete(void* p) {::operator delete(p);}

//=================

    bool isSoftObstacle() const;
    float getCostToTraverse() const;
    tPoint getDirectionForCostToTraverse() const;

    // these must only be called under protection of _mesh->_agentsAndObstacleSetsMutex
    void notifyAddedToObstacleSet(long setIndex, long indexInSet);
    long notifyRemovedFromObstacleSet(long setIndex);
    void notifyIndexInObstacleSetChanged(long setIndex, long newIndexInSet);
    bool isInObstacleSet(long setIndex);

    cHandle<cShapeCollisionPreprocess> getUnexpandedPreprocess(cQueryContext& qc);
    cHandle<cShapeCollisionPreprocess> getCollisionPreprocess(cQueryContext& qc, long expandShape);
    cHandle<cShapePathfindPreprocess> getPathfindPreprocess(cQueryContext& qc, const cPathfindPreprocess& cp, long expandShape);
    cHandle<cShapeRenderPreprocess> getRenderPreprocess(long expandShape);

    bool
    expansionPotentiallyOverlapsBounds(
            long expandShape,
            const tPoint& boundsMin,
            const tPoint& boundsMax
            );

    cShape& refShape()
    {
        assertD(_shape.isSet());
        return* _shape;
    }
    cHandle<cShape> getShapeHandle()
    {
        return _shape;
    }
    cMesh& refMesh()
    {
        assertD(_mesh);
        return* _mesh;
    }

    cInternalPosition getInternalPosition() const
    {
        return _internalPosition;
    }

    long getIndexInMesh() const
    {
        return _indexInMesh;
    }
};

inline cAgent* FromI(iAgent* ptr)
{
    return static_cast<cAgent*>(ptr);
}

inline void Destroy(cAgent* ptr)
{
    ptr->destroy();
}

#endif
