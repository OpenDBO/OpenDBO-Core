//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Assert.h"
#include "i_pathengine.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "common/Containers/ReplacementList.h"
#include "common/STL/vector.h"

class iAgent;
class cMesh;
class iAPIObjectWrapper;
template <class T> class cPool;

class cCollisionInfo : public iCollisionInfo
{
    cMesh* _mesh;
    tLine _collidingLine;
    iAgent* _collidingAgent;
    cList_NoSize<cCollisionInfo*>::iterator _nodeForThis;

    // support for pool

    long _poolIndex;
    friend class cPool<cCollisionInfo>;
    void setPoolIndex(long value)
    {
        _poolIndex = value;
    }
    long getPoolIndex() const
    {
        return _poolIndex;
    }

public:

    iAPIObjectWrapper* _wrapper;

    cCollisionInfo();
    ~cCollisionInfo();

    void init(cMesh* mesh, const tLine& collidingLine, iAgent* collidingAgent, cList_NoSize<cCollisionInfo*>::iterator node);
    void init(cMesh* mesh, iAgent* collidingAgent, cList_NoSize<cCollisionInfo*>::iterator node);

// iCollisionInfo

    void destroy();
    void getCollidingLine(long* coords) const;
    iAgent* getCollidingAgent() const;
    void operator delete(void* p) {::operator delete(p);}

//==================
};
