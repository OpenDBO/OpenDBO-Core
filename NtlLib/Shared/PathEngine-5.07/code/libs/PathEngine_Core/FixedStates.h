//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FIXED_STATES_INCLUDED
#define FIXED_STATES_INCLUDED

#include "StateSpaceBase.h"
#include "platform_common/AddWithOverflow.h"

class cQueryContext;
class iLineCollisionTester;
class cOffMeshConnections;

template <class tCorners, class tCollision, class tCostCalculator>
class cFixedStates : public bStateSpace<tCorners>
{
    typedef bStateSpace<tCorners> tParent;

    cQueryContext& _qc;
    const tCollision& _collision;
    const tCostCalculator* _costCalculator;

public:

    cFixedStates(
        cQueryContext& qc,
        const cOffMeshConnections& connections,
        const tCorners& corners,
        const tCollision& collision,
        const tCostCalculator* costCalculator
        ) :
     tParent(connections, corners),
     _qc(qc),
     _collision(collision),
     _costCalculator(costCalculator)
    {
    }

    bool testCollision(const cInternalPosition& from, const cInternalPosition& to) const
    {
        return _collision.testCollision(_qc, from, to);
    }

    void getCosts(const cInternalPosition& from, const cInternalPosition& to,
                  unsigned long& costOut, unsigned long& costBack,
                  bool& blockedOut, bool& blockedBack
                  ) const
    {
        unsigned long distanceComponent = Length(to.p - from.p);

        costOut = distanceComponent;
        costBack = distanceComponent;
        blockedOut = false;
        blockedBack = false;

        if(_costCalculator)
        {
            unsigned long additionalCost;
            additionalCost = static_cast<unsigned long>(_costCalculator->calculate(_qc, from, to.p, blockedOut));
            if(AddWithOverflow(costOut, additionalCost, costOut))
            {
                costOut = 0xffffffff;
            }
            additionalCost = static_cast<unsigned long>(_costCalculator->calculate(_qc, to, from.p, blockedBack));
            if(AddWithOverflow(costBack, additionalCost, costBack))
            {
                costBack = 0xffffffff;
            }
        }
    }
};

#endif
