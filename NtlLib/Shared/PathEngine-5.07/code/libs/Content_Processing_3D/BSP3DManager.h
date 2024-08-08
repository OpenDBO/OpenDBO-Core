//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_3D_OPERAND_MANAGER_INCLUDED
#define BSP_3D_OPERAND_MANAGER_INCLUDED

#include "libs/PathEngine_Interface/RandomSequence.h" //.... move to a lower level module
#include "Plane.h"
#include "tMesh_Dynamic.h"
#include "tMesh_Simple3D_Header.h"
#include "BSPStats.h"
#include "common/STL/vector.h"
#include "common/Containers/BitVector.h"

class cBSP3DOperand;
class iProgressCallBack;
template <class T> class cBSPTree;

class cBSP3DManager
{
    cRandomSequence _sequence;
    long _nextEdgePair;
    long _bestSplit;
    long _bestSplitCost;
    cBitVector _bestSplitPreprocess;
    cBitVector _candidateSplitPreprocess;
    long _nextClippedOperand;
    long _totalUnassignedBounds;
    long _totalStartingUnassignedBounds;
    long _bestUnassignedBounds;
    iProgressCallBack* _progressCallBack;
    bool _keepDiscardedFaces;
    bool _keepTree;
    std::vector<cBSP3DOperand*> _initialOperands;
    cBSPTree<cBSP3DManager>* _tree;

    void
    extractFacesFromContainingOperand(cBSP3DOperand* operand);
    void
    extractFacesFromNonContainingOperand(cBSP3DOperand* operand);

    void
    considerSplit(
            std::vector<cBSP3DOperand*>& operands,
            long splitterIndex
            );

    long
    doBestSplit(
            std::vector<cBSP3DOperand*>& operands,
            long splitNode,
            std::vector<cBSP3DOperand*>& backOperands,
            bool& frontContained, bool& backContained
            );

    // prevent copying and assignment
    cBSP3DManager(const cBSP3DManager&);
    const cBSP3DManager& operator=(const cBSP3DManager&);

public:

    std::vector<cPlane> _splitters;
    cBSPVertices _vertices;
    tMesh_Dynamic _result;
    cBSPStats _stats;

    cBSP3DManager(
            iProgressCallBack* progressCallBack,
            bool keepDiscardedFaces,
            bool keepTree = false
            );
    ~cBSP3DManager();

    void addOperand(tMesh_Simple3D& primitive);
    void buildTreeAndSplitFragments();

    const cBSPTree<cBSP3DManager>& refTree() const
    {
        assertD(_keepTree);
        assertD(_tree);
        return *_tree;
    }

// ---- interface for cBSPTree

    typedef cBSP3DOperand tClippedOperand;
    typedef cPlane tSplitter;

    void
    splitOperands(
            std::vector<tClippedOperand*>& operands,
            long splitNode, 
            std::vector<tClippedOperand*>& backOperands,
            long& splitterSelected, 
            bool& frontContained, bool& backContained
            );

    const tSplitter&
    refSplitter(long index) const
    {
        return _splitters[index];
    }

    void
    splitFragment(tFace_Dynamic toSplit, long splitterIndex, bool& extendsInFront, tFace_Dynamic& inFront, bool& extendsBehind, tFace_Dynamic& behind);
    
    void
    fragmentIsInside(tFace_Dynamic fragment);
};

#endif
