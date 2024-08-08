//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_SHAPE_INCLUDED
#define C_SHAPE_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Geometry/interface/CollisionShape.h"
#include "i_pathengine.h"
#include "common/Containers/ReplacementList.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class iAPIObjectWrapper;

class cShape : public iShape, public bThreadSafeHandleTarget
{
    cCollisionShape _shape;
    cList_NoSize<cShape*>::iterator _heldNode;
    long _preprocessIndex;

public:

    iAPIObjectWrapper* _wrapper;

    cShape(long numberOfPoints, const long *coordinateData,
            cList_NoSize<cShape*>::iterator heldNode);
    cShape(const cCollisionShape& cs,
            cList_NoSize<cShape*>::iterator heldNode);
    ~cShape();

// iShape

    void destroy();
    long size() const;
    void vertex(long index, long& x, long& y) const;
    void operator delete(void* p) {::operator delete(p);}

//====================

    void declareWillBePreprocessed();
    long getPreprocessIndex() const
    {
        return _preprocessIndex;
    }

    const cCollisionShape& refCS() const
    {
        return _shape;
    }
};

inline cShape* FromI(iShape* ptr)
{
    return static_cast<cShape*>(ptr);
}

#endif
