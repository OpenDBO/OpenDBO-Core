//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_3D_OPERAND_INCLUDED
#define BSP_3D_OPERAND_INCLUDED

#include "tMesh_Dynamic.h"
#include "tMesh_Simple3D_Header.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "common/STL/vector.h"

class cPlane;
class cVerticalPlane;
class cBitVector;

class cBSP3DOperand
{
    tMesh_Dynamic _mesh;
    long _numberOfUnassignedBounds;

    bool validate(const cBSPVertices& vertices) const;

    void
    markCoplanarBound(
            const cBitVector& sidesBuffer, long sidesStart,
            long splitNode, bool facesOppositeDirection
            );

    cBSP3DOperand(tMesh_Dynamic& swapIn);

    // prevent copying and assignment
    cBSP3DOperand(const cBSP3DOperand&);
    const cBSP3DOperand& operator=(const cBSP3DOperand&);

public:

    cBSP3DOperand(
            cBSPVertices& vertices,
            tMesh_Simple3D& meshSimple3D,
            long operandIndex, std::vector<cPlane>& splitters
            );

    bool
    vertsAreConsolidated() const
    {
        return _mesh.vertices() == _mesh.vertSize();
    }
    tVertex_Dynamic
    beginVertex()
    {
        return _mesh.beginVertex();
    }

// interface for cBSP3DManager

    long
    numberOfVertices() const
    {
        return _mesh.vertices();
    }

    bool
    predict_WillBeSplit(
            const cBitVector& preprocessBuffer,
            long bufferPosition
            );

    void
    predict_UnassignedBoundsSplit(
            const cBitVector& preprocessBuffer,
            long bufferPosition,
            long& unassignedInFront, long& unassignedBehind,
            bool& containsFront, bool& containsBack
            );

    void
    split(
            cBSPVertices& vertices,
            const cPlane& splitter,
            const cBitVector& preprocessBuffer,
            long bufferPosition,
            long splitNode, 
            cBSP3DOperand*& front, cBSP3DOperand*& back,
            long& nextEdgePair
            );

    void
    takeMeshAndInvalidate(tMesh_Dynamic& swapOutTo);

    long
    numberOfUnassignedBounds() const
    {
        return _numberOfUnassignedBounds;
    }
    long
    splitForUnassignedBoundIndex(long index) const;
};

#endif
