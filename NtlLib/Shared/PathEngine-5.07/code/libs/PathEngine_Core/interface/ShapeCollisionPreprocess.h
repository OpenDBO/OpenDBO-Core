//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SHAPE_COLLISION_PREPROCESS_INCLUDED
#define SHAPE_COLLISION_PREPROCESS_INCLUDED

#include "libs/Mesh2D/interface/InternalPosition.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class cCollisionShape;
class cLocalisedConvexShape;
class cQueryContext;

class cShapeCollisionPreprocess : public bThreadSafeHandleTarget
{
    cInternalPosition _preprocessedPosition;
    cLocalisedConvexShape* _expansion2D;

public:

    cShapeCollisionPreprocess(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandShape
        );
    cShapeCollisionPreprocess(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition
        );
    ~cShapeCollisionPreprocess();

    bool isValidFor(const cInternalPosition& baseShapePosition) const;
    void reinit(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandShape);
    void reinit(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition
        );

    const cLocalisedConvexShape* getExpansion2D() const
    {
        return _expansion2D;
    }
};

#endif
