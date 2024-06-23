//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMPOUND_CONTEXT_COLLISION_PREPROCESS_INCLUDED
#define COMPOUND_CONTEXT_COLLISION_PREPROCESS_INCLUDED

#include "libs/PathEngine_Core/interface/ContextCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class cInternalPosition;
class cAgent;
class cAxisFraction;

class cCompoundCCP_ShapesInFaceIterator
{
public:

    long _faceIndex;
    long _preprocessIndex;
    long _indexInFace;
};

class cCompoundContextCollisionPreprocess : public iLineCollisionTester, public iPointCollisionTester
{
    long _numberOfPreprocesses;
    cHandle<cContextCollisionPreprocess> _preprocessArray[4];
    long _firstIndexArray[5];
    std::vector<cHandle<cContextCollisionPreprocess> >* _preprocessVector;
    std::vector<long>* _firstIndexVector;

    cHandle<cContextCollisionPreprocess>* preprocessPtr();
    const cHandle<cContextCollisionPreprocess>* preprocessPtr() const;
    long* firstIndexPtr();
    const long* firstIndexPtr() const;

public:

    cCompoundContextCollisionPreprocess();
    ~cCompoundContextCollisionPreprocess();

    void addPreprocess(cHandle<cContextCollisionPreprocess> preprocess);

    long size() const;

  // shapes iteration
//    typedef cCompoundCCP_ShapesIterator tShapesIterator;
//    static void initShapesIterator(tShapesIterator& it);
//    bool nextShape(tShapesIterator& it, long& index, const cLocalisedConvexShape*& shapePtr) const;
//    bool nextShape(tShapesIterator& it, long& index, const cLocalisedConvexShape*& shapePtr) const;

  // shapes in face iteration
    typedef cCompoundCCP_ShapesInFaceIterator tShapesInFaceIterator;
    static void initShapesInFaceIterator(long faceIndex, tShapesInFaceIterator& it);
    bool nextShapeInFace(tShapesInFaceIterator& it, long& index, const cLocalisedConvexShape*& shapePtr) const;
    bool nextShapeInFace(tShapesInFaceIterator& it, const cLocalisedConvexShape*& shapePtr) const;
    bool nextShapeInFace(tShapesInFaceIterator& it, long& index, const cLocalisedConvexShape*& shapePtr, cAgent*& agentPtr) const;

    bool testPointCollision_IgnoringOneAgent(const cInternalPosition& p, const cAgent* agentToIgnore) const;
    long getAllAgentsOverlapped(const cInternalPosition& p, cAgent** resultsBuffer) const;
    void firstCollision(
        tFace startF, const tLine& line, bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        cAgent*& collidingAgent
        );
    void firstCollision(
        tFace startF,
        const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        cAgent*& collidingAgent
        );

// interface for iPointCollisionTester

    bool testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const;

// interface for iLineCollisionTester

    bool testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const;
    bool testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const;
};

#endif
