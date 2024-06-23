//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/InternalPosition.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/STL/vector.h"

class cCollisionPreprocess;
class cPathfindPreprocess;
class cShapeCollisionPreprocess;

class cShapePathfindPreprocess : public bThreadSafeHandleTarget
{
    cInternalPosition _preprocessedPosition;
    std::vector<long> _smallConvexOverlapped;

public:

    cShapePathfindPreprocess(
            const cInternalPosition& baseShapePosition,
            const cPathfindPreprocess& pp,
            const cShapeCollisionPreprocess& scp);

    bool isValidFor(const cInternalPosition& baseShapePosition) const;
    void reinit(
            const cInternalPosition& baseShapePosition,
            const cPathfindPreprocess& pp,
            const cShapeCollisionPreprocess& scp);

    const std::vector<long>& refSmallConvexOverlapped() const
    {
        return _smallConvexOverlapped;
    }
};
