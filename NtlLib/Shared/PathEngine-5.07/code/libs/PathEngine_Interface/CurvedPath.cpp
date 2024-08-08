//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "CurvedPath.h"
#include "interface/cCollisionContext.h"
#include "interface/cMesh.h"
#include "interface/cShape.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/tLine.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"

using std::vector;

static bool
SatisfiesTurnRatio(float ratio, const tPoint& previousAxis, const tPoint& axis)
{
    assertD(previousAxis != tPoint(0, 0, 0));
    assertD(axis != tPoint(0, 0, 0));
    tPoint offset;
    tPoint bound;
    offset = previousAxis.right90().multipliedByFloat(ratio);
    bound = previousAxis + offset;
    if(Side_Vectors(bound, axis) == SIDE_RIGHT)
    {
        return false;
    }
    bound = previousAxis - offset;
    if(Side_Vectors(bound, axis) == SIDE_LEFT)
    {
        return false;
    }
    return true;
}

template <class tIt> bool
AttemptCurvedSection(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        tIt startI, tIt endI,
        float maximumTurnRatio, long minimumSectionLength,
        tPoint previousAxis,
        std::vector<cInternalPosition>& addTo,
        long& pointsPassed
        )
{
    vector<cInternalPosition> pathAttempt;

    long points = static_cast<long>(endI - startI);
    assertD(points);
    tIt i = startI;
    cInternalPosition currentPoint = *i;
    ++i;

    eSide side;
    side = Side_Vectors(previousAxis, i->p - currentPoint.p);
    assertD(side != SIDE_CENTRE || Direction_Vectors(previousAxis, i->p - currentPoint.p) == DIRECTION_BACK);
    if(side == SIDE_CENTRE)
    {
        side = SIDE_RIGHT;
    }

    while(1)
    {
        tPoint nextAxis = previousAxis;
        if(side == SIDE_RIGHT)
        {
            nextAxis += previousAxis.right90().multipliedByFloat(maximumTurnRatio);
        }
        else
        {
            nextAxis -= previousAxis.right90().multipliedByFloat(maximumTurnRatio);
        }

        SetLength(nextAxis, minimumSectionLength);

        tPoint nextP = currentPoint.p + nextAxis;

        tLine line = tLine(currentPoint.p, nextP);

    // if path turns in other direction then make this point our final target
        assertD(i > startI);
        tIt prevI = i;
        --prevI;
        tIt nextI = i;
        ++nextI;
        if(points > 1)
        {
            tLine sectionLine(prevI->p, i->p);
            tPoint pointAfterTarget = nextI->p;
            if(SideOfLine(sectionLine, pointAfterTarget) != side)
            {
                points = 1;
            }
        }

    // check for overshot end of path
        if(points == 1)
        {
            tLine endBound(i->p, prevI->p);
            endBound.rotateLeft90();
            if(SideOfLine(endBound, nextP) == SIDE_LEFT)
            {
                return false;
            }
        }

    // check for path points passed
        while(points > 1)
        {
            tLine sectionLine(i->p, nextI->p);
            if(SideOfLine(sectionLine, nextP) != OtherSide(side))
            {
                break;
            }

            if(SideOfLine(line, i->p) == OtherSide(side))
            {
            // went through path
                return false;
            }
            points--;
            ++i;
            ++prevI;
            ++nextI;

        // if path turns in other direction then make this point our final target
            if(points > 1)
            {
                tLine sectionLine(prevI->p, i->p);
                tPoint pointAfterTarget = nextI->p;
                if(SideOfLine(sectionLine, pointAfterTarget) != side)
                {
                    points = 1;
                }
            }

        // check for overshot end of path
            if(points == 1)
            {
                tLine endBound(i->p, prevI->p);
                endBound.rotateLeft90();
                if(SideOfLine(endBound, nextP) == SIDE_LEFT)
                {
                    return false;
                }
            }
        }

        {
            tLine sectionLine(prevI->p, i->p);
            if(SideOfLine(sectionLine, nextP) == side)
            {
                return false;
            }
        }

// check collision for section
        bool collides;
        long cellForNextP;
        {
            cPosition currentPointIP;
            mesh->convertPosition(currentPoint, currentPointIP);
            collides = mesh->testLineCollision_XY(
                    shape, context,
                    currentPointIP,
                    nextP.getX(), nextP.getY(),
                    cellForNextP
                    );
        }
        if(collides)
        {
            return false;
        }

        pathAttempt.push_back(cInternalPosition(mesh->ref2D().faceAt(cellForNextP), nextP));

        previousAxis = nextAxis;
        currentPoint = pathAttempt.back();

// try to connect to end
        tPoint toEnd = i->p - nextP;
        if(Length(toEnd) >= minimumSectionLength
         && SatisfiesTurnRatio(maximumTurnRatio, previousAxis, toEnd)
         )
        {
            {
                cPosition currentPointIP;
                mesh->convertPosition(currentPoint, currentPointIP);
                bool collides;
                long endCell;
                collides = mesh->testLineCollision_XY(
                        shape, context,
                        currentPointIP,
                        i->p.getX(), i->p.getY(),
                        endCell
                        );
                if(collides || endCell != i->f.index())
                {
                    return false;
                }
            }
            size_t sizeBefore = addTo.size();
            addTo.resize(sizeBefore + pathAttempt.size());
            copy(pathAttempt.begin(), pathAttempt.end(), addTo.begin() + sizeBefore);
            pointsPassed = static_cast<long>(i - startI) - 1;
            return true;
        }
    }
}

template <class tIt> long
CurvedPathGenerationStep(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        long sectionLength,
        float turnRatio1, float turnRatio2,
        tIt startI, tIt endI,
        tPoint& startVector,
        bool& startVectorValid,
        std::vector<cInternalPosition>& addTo
        )
{
    assertD(startI != endI);
    tIt i = startI;
    addTo.push_back(*i);
    if(!startVectorValid)
    {
        ++i;
        startVector = i->p - addTo.back().p;
        startVectorValid = true;
        return static_cast<long>(i - startI);
    }
    tIt nextI = i;
    ++nextI;
    tPoint axis = nextI->p - i->p;
    if(!SatisfiesTurnRatio(turnRatio1, startVector, axis))
    {
        long pointsPassed;
        bool succeeded;
        succeeded = AttemptCurvedSection(
                mesh, shape, context,
                i, endI,
                turnRatio1, sectionLength,
                startVector,
                addTo,
                pointsPassed
                );
        if(!succeeded && !SatisfiesTurnRatio(turnRatio2, startVector, axis))
        {
            succeeded = AttemptCurvedSection(
                    mesh, shape, context,
                    i, endI,
                    turnRatio2, sectionLength,
                    startVector,
                    addTo,
                    pointsPassed
                    );
        }
        if(succeeded)
        {
            i += pointsPassed;
        }
    }
    ++i;
    startVector = i->p - addTo.back().p;
    startVectorValid = true;
    return static_cast<long>(i - startI);
}

//static void
//GenerateCurvedPathPiece(
//        cMesh* mesh, cShape* shape, cCollisionContext* context,
//        const std::vector<cInternalPosition>& path,
//        long startIndex, long endIndex,
//        long startVectorX, long startVectorY,
//        long sectionLength,
//        float turnRatio1, float turnRatio2,
//        std::vector<cInternalPosition>& pathResult
//        )
//{
//    assertD(endIndex > startIndex);
//    tPoint startVector;
//    bool startVectorValid = false;
//    if(startVectorX != 0 || startVectorY != 0)
//    {
//        startVector = tPoint(startVectorX, startVectorY, WORLD_RANGE);
//        startVectorValid = true;
//    }
//    do
//    {
//        std::vector<cInternalPosition>::const_iterator startI, endI;
//        startI = path.begin() + startIndex;
//        endI = path.begin() + endIndex;
//        long iteratorIncrement = CurvedPathGenerationStep(
//                mesh, shape, context, 
//                sectionLength, turnRatio1, turnRatio2,
//                startI, endI,
//                startVector, startVectorValid,
//                pathResult
//                );
//        startIndex += iteratorIncrement;
//        assertD(startIndex <= endIndex);
//    }
//    while(startIndex < endIndex);
//    pathResult.push_back(path[endIndex]);
//}

static int
ApproximateAngle(tPoint base, const tPoint& test)
{
    eDirection dir = base.direction(test);
    switch(dir)
    {
    default:
    case DIRECTION_NONE:
        invalid();
    case DIRECTION_FORWARD:
        return 0;
    case DIRECTION_BACK:
        return 5;
    case DIRECTION_LEFT:
        {
            base.rotateRight90();
            if(base.side(test) == SIDE_RIGHT)
            {
                base.rotateRight45();
                if(base.side(test) == SIDE_RIGHT)
                {
                    return 4;
                }
                return 3;
            }
            else
            {
                base.rotateLeft45();
                if(base.side(test) == SIDE_RIGHT)
                {
                    return 2;
                }
                return 1;
            }
        }
    case DIRECTION_RIGHT:
        {
            base.rotateLeft90();
            if(base.side(test) == SIDE_LEFT)
            {
                base.rotateLeft45();
                if(base.side(test) == SIDE_LEFT)
                {
                    return 4;
                }
                return 3;
            }
            else
            {
                base.rotateRight45();
                if(base.side(test) == SIDE_LEFT)
                {
                    return 2;
                }
                return 1;
            }
        }
    }
}

static bool
EndConstraintIsMoreImportant(
        const std::vector<cInternalPosition>& path,
        long startIndex, long endIndex,
        tPoint& startVector,
        bool& startVectorValid,
        tPoint& endVector,
        bool& endVectorValid
        )
{
    assertD(endIndex > startIndex);
    if(!endVectorValid)
    {
        return false;
    }
    if(!startVectorValid)
    {
        return true;
    }
    tPoint startSegment = path[startIndex + 1].p - path[startIndex].p;
    tPoint endSegment = path[endIndex - 1].p - path[endIndex].p;
    return ApproximateAngle(endSegment, endVector) > ApproximateAngle(startSegment, startVector);
}

static void
GenerateCurvedPathPiece_WithEndVector(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path,
        long startIndex, long endIndex,
        long startVectorX, long startVectorY,
        long endVectorX, long endVectorY, // end vector passed in is the desired facing direction at end of path
        long sectionLength,
        float turnRatio1, float turnRatio2,
        std::vector<cInternalPosition>& pathResult
        )
{
//    if(endVectorX == 0 && endVectorY == 0)
//    {
//        GenerateCurvedPathPiece(
//                mesh, shape, context,
//                path, startIndex, endIndex,
//                startVectorX, startVectorY,
//                sectionLength, turnRatio1, turnRatio2,
//                pathResult
//                );
//        return;
//    }
    std::vector<cInternalPosition> pathResultEnd;

    assertD(endIndex > startIndex);
    tPoint startVector;
    bool startVectorValid = false;
    if(startVectorX != 0 || startVectorY != 0)
    {
        startVector = tPoint(startVectorX, startVectorY, WORLD_RANGE);
        startVectorValid = true;
    }
    tPoint endVector;
    bool endVectorValid = false;
    if(endVectorX != 0 || endVectorY != 0)
    {
        // end vector is reversed here so that start and end cases can be treated symmetrically
        endVector = tPoint(-endVectorX, -endVectorY, WORLD_RANGE);
        endVectorValid = true;
    }

    do
    {
        if(EndConstraintIsMoreImportant(
                path, startIndex, endIndex,
                startVector, startVectorValid,
                endVector, endVectorValid
                ))
        {
            std::vector<cInternalPosition>::const_reverse_iterator startI, endI;
            startI = path.rbegin() + (SizeL(path) - 1 - endIndex);
            endI = path.rbegin() + (SizeL(path) - 1 - startIndex);
            long iteratorIncrement = CurvedPathGenerationStep(
                    mesh, shape, context, 
                    sectionLength, turnRatio1, turnRatio2,
                    startI, endI,
                    endVector, endVectorValid,
                    pathResultEnd
                    );            
            endIndex -= iteratorIncrement;
        }
        else
        {
            std::vector<cInternalPosition>::const_iterator startI, endI;
            startI = path.begin() + startIndex;
            endI = path.begin() + endIndex;
            long iteratorIncrement = CurvedPathGenerationStep(
                    mesh, shape, context, 
                    sectionLength, turnRatio1, turnRatio2,
                    startI, endI,
                    startVector, startVectorValid,
                    pathResult
                    );
            startIndex += iteratorIncrement;
        }
        assertD(startIndex <= endIndex);
    }
    while(startIndex < endIndex);

    long sizeBefore = SizeL(pathResult);
    long endSize = SizeL(pathResultEnd);
    pathResult.reserve(SizeL(pathResult) + endSize);
    pathResult.push_back(path[endIndex]);
    for(long i = 0; i != SizeL(pathResultEnd); ++i)
    {
        pathResult.push_back(pathResultEnd[endSize - 1 - i]);
    }
}

void
GenerateCurvedPath(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path, const std::vector<long>* connectionIndices, long startOffset,
        long startVectorX, long startVectorY,
        long endVectorX, long endVectorY,
        long sectionLength,
        float turnRatio1, float turnRatio2,
        std::vector<cInternalPosition>& pathResult, std::vector<long>*& connectionIndicesResult
        )
{
    assertD(pathResult.empty());
    assertD(connectionIndicesResult == 0);
    if(path.empty())
    {
        return;
    }
    if(SizeL(path) == 1)
    {
        pathResult.push_back(path[startOffset]);
        return;
    }
    if(!connectionIndices)
    {
      // just one piece
        GenerateCurvedPathPiece_WithEndVector(
                mesh, shape, context,
                path,
                startOffset, SizeL(path) - 1,
                startVectorX, startVectorY,
                endVectorX, endVectorY,
                sectionLength, turnRatio1, turnRatio2,
                pathResult
                );
        return;
    }

    const std::vector<long>& indices = *connectionIndices;
    connectionIndicesResult = new std::vector<long>;
    std::vector<long>& indicesResult = *connectionIndicesResult;

    long i = startOffset;
    pathResult.push_back(path[i]);

    if(indices[i] != -1)
    {
        indicesResult.push_back(indices[i]);
        ++i;
        pathResult.push_back(path[i]);
    }
    if(i + 1 == SizeL(path))
    {
        return;
    }
    while(1)
    {
        assertD(indices[i] == -1);
        long endI = i;
        do
        {
            ++endI;
        }
        while(endI + 1 != SizeL(path) && indices[endI] == -1);

        if(i != startOffset)
        {
            startVectorX = 0;
            startVectorY = 0;
        }
        long useEndVectorX = 0;
        long useEndVectorY = 0;
        if(endI + 1 == SizeL(path))
        {
            useEndVectorX = endVectorX;
            useEndVectorY = endVectorY;
        }
        GenerateCurvedPathPiece_WithEndVector(
                mesh, shape, context,
                path,
                i, endI,
                startVectorX, startVectorY,
                useEndVectorX, useEndVectorY,
                sectionLength, turnRatio1, turnRatio2,
                pathResult
                );
        indicesResult.resize(pathResult.size() - 1, -1);
        
        if(endI + 1 == SizeL(path))
        {
            return;
        }

        i = endI;
        assertD(indices[i] != -1);
        indicesResult.push_back(indices[i]);
        ++i;
        pathResult.push_back(path[i]);
        if(i + 1 == SizeL(path))
        {
            return;
        }
    }
}
