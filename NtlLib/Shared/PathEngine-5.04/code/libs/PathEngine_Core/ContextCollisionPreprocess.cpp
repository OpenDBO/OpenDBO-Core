//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ContextCollisionPreprocess.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"

bool
cContextCollisionPreprocess::testPointCollision_IgnoringOneAgent(const cInternalPosition& p, const cAgent* agentToIgnore) const
{
    long i;
    for(i = 0; i != refShapeSet().shapesInFace(p.cell()); i++)
    {
        long j = refShapeSet().indexForShapeInFace(p.cell(), i);
        if(_ownerAgents[j] == agentToIgnore)
        {
            continue;
        }
        if(refShapeSet()[j].testPointCollision(p))
        {
            return true;
        }
    }
    return false;
}

long
cContextCollisionPreprocess::getAllAgentsOverlapped(
        const cInternalPosition& p,
        cAgent** resultsBuffer
        ) const
{
    long numberOverlapped = 0;
    long i;
    for(i = 0; i != refShapeSet().shapesInFace(p.cell()); i++)
    {
        long j = refShapeSet().indexForShapeInFace(p.cell(), i);
        if(refShapeSet()[j].testPointCollision(p))
        {
            *resultsBuffer++ = _ownerAgents[j];
            ++numberOverlapped;
        }
    }
    return numberOverlapped;
}

void
cContextCollisionPreprocess::firstCollision(
        tFace startF, const tLine& line, bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        long& collidingShape
        )
{
    FirstCollision_AgainstShapeSet(refShapeSet(), startF, line, collides, distanceAlong, collidingLine, collidingShape);
}
