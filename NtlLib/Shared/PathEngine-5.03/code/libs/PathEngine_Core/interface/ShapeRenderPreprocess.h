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
#include "libs/Mesh2D/interface/MeshCut.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class cCollisionShape;

class cShapeRenderPreprocess : public bThreadSafeHandleTarget
{
    cInternalPosition _preprocessedPosition;
    tMeshCircuits _circuits;

public:

    cShapeRenderPreprocess(
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& baseShape,
        const cCollisionShape& expandShape
        );

    bool isValidFor(const cInternalPosition& baseShapePosition) const;
    void reinit(
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& baseShape,
        const cCollisionShape& expandShape
        );

    const tMeshCircuits& refCircuits() const
    {
        return _circuits;
    }
};
