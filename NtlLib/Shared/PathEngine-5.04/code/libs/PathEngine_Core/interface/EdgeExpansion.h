//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE_EDGE_EXPANSION_DEFINED
#define PATHENGINE_CORE_EDGE_EXPANSION_DEFINED

#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cQueryContext;
class cCollisionShape;
class cElementInFaceEntry;
class cElementLookupCollisionTester;
class cInternalPosition;
class cCombinedLineCollisionTester;
class cCombinedPointCollisionTester;
class iXMLInputStream;
class iXMLOutputStream;
class cAxisFraction;
template <class T> class cMeshWithTrackedTransform;

class cEdgeExpansion : public bThreadSafeHandleTarget
{
    cHandle<tMeshCutSequence> _edgeExpansion;
    long _edgeExpansionElementCount;
    std::vector<std::vector<cElementInFaceEntry> >* _edgeExpansionLookup;
    cElementLookupCollisionTester* _edgeExpansionCollisionTester;

public:

    // perform expansion
    cEdgeExpansion(
        cQueryContext& qc,
        cMeshWithTrackedTransform<tMesh>* mesh2D_NoInternalVertices,
        const cCollisionShape& expandShape
        );

    // load from persistence
    cEdgeExpansion::cEdgeExpansion(
        cQueryContext& qc,
        iXMLInputStream& is
        );

    ~cEdgeExpansion();

    void save(iXMLOutputStream& os) const;

    long getElementCountRequirement() const;

    const tMeshCutSequence& refEdgeExpansion() const
    {
        return *_edgeExpansion;
    }
    const std::vector<std::vector<cElementInFaceEntry> >& refEdgeExpansionLookup() const
    {
        return *_edgeExpansionLookup;
    }

    void firstCollision(cQueryContext& qc, tFace startF, const tLine& line, bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine) const;
    void firstCollision(
            cQueryContext& qc,
            tFace startF,
            const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
            bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine
            ) const;

    const cElementLookupCollisionTester& refLineTester() const
    {
        return *_edgeExpansionCollisionTester;
    }
    const cElementLookupCollisionTester& refPointTester() const
    {
        return *_edgeExpansionCollisionTester;
    }
};

#endif
