//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_OBSTACLE_SET_INCLUDED
#define C_OBSTACLE_SET_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/ModifiedStateTracker.h"
#include "common/Handle.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "platform_common/Mutex.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "i_pathengine.h"

class cQueryContext;
class cMesh;
class cAgent;
class cCollisionPreprocess;
class cPathfindPreprocess;
class cContextCollisionPreprocess;
class cContextPathfindPreprocess;
class cCompoundContextCollisionPreprocess;
class iAPIObjectWrapper;

class cObstacleSet : public iObstacleSet, public bThreadSafeHandleTarget
{
  // immutable
    cMesh* _mesh;
    long _indexInMesh; // for removal from meshes held vector, and for agents to track containing sets
    bool _markedForPreprocessing;

  // protected by _mesh->_agentsAndObstacleSetsMutex
    std::vector<cAgent*> _agentVector;

    cMutex _preprocessMutex;
    cHandle<cContextCollisionPreprocess> _unexpandedPreprocess;
    mutable std::vector<cHandle<cContextCollisionPreprocess> > _collisionPreprocess;
    cModifiedStateTracker _pathfindingStateTracker;
    mutable std::vector<cHandle<cPathfindPreprocess> > _pathfindPreprocess;

    bool _hasBeenDestroyed;

    void collisionAndPathfindingStateModified();
    void pathfindingStateOnlyModified();

public:

    iAPIObjectWrapper* _wrapper;

    cObstacleSet(cMesh* mesh, const char *const* attributes);
    void setIndexInMesh(long value); // must be called post construction
    
    ~cObstacleSet();

// iObstacleSet

    void destroy();
    void addAgent(iAgent* agent);
    void removeAgent(iAgent* agent);
    long getNumberOfAgents() const;
    iAgent* getAgent(long index) const;
    bool includes(iAgent* agent) const;
    long size() const;
    bool empty() const;
    void updateCollisionPreprocessFor(iShape* shape) const;
    void updatePathfindingPreprocessFor(iShape* shape) const;
    void operator delete(void* p) {::operator delete(p);}

//=================

    cHandle<cContextCollisionPreprocess> getUnexpandedPreprocess(cQueryContext& qc);
    cHandle<cContextCollisionPreprocess> getCollisionPreprocessFor(cQueryContext& qc, long i) const;
    cHandle<cPathfindPreprocess> getPathfindPreprocessFor(cQueryContext& qc, long i) const;

    void addAgentsToPathfindPreprocess(
        cQueryContext& qc, 
        long expansionShapeIndex,
        const cCollisionPreprocess& cp, const cPathfindPreprocess& pp,
        bool boundsValid,
        const tPoint& boundsMin, const tPoint& boundsMax,
        cContextPathfindPreprocess& addTo
        );

    void notifyAgentChangeAffectingCollisionAndPathfindingState();
    void notifyAgentChangeAffectingPathfindingStateOnly();

    void addUnexpandedPreprocess(cQueryContext& qc, cCompoundContextCollisionPreprocess& addTo);
    void addCollisionPreprocessFor(cQueryContext& qc, long index, cCompoundContextCollisionPreprocess& addTo);

    unsigned long getPathfindingStateHandle();
    bool pathfindingStateHasBeenModifiedSince(unsigned long stateHandle) const;

    bool hasBeenDestroyed() const
    {
        return _hasBeenDestroyed;
    }

    bool isMarkedForPreprocessing() const
    {
        return _markedForPreprocessing;
    }
};

inline cObstacleSet* FromI(iObstacleSet* ptr)
{
    return static_cast<cObstacleSet*>(ptr);
}

#endif
