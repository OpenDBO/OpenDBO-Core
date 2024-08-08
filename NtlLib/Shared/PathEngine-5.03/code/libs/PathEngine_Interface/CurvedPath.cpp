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

static bool
AttemptCurvedSection(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path,
        long startIndex, long endI,
        float maximumTurnRatio, long minimumSectionLength,
        tPoint previousAxis,
        std::vector<cInternalPosition>& pathResult,
        long& pointsPassed
        )
{
    vector<cInternalPosition> pathAttempt;

    long points = endI + 1 - startIndex;
    long i = startIndex;
    assertD(points);

    cInternalPosition currentPoint;
    currentPoint = pathResult.back();

    eSide side;
    side = Side_Vectors(previousAxis, path[startIndex].p - currentPoint.p);
    assertD(side != SIDE_CENTRE || Direction_Vectors(previousAxis, path[startIndex].p - currentPoint.p) == DIRECTION_BACK);
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
        assertD(i > 0);
        if(points > 1)
        {
            tLine sectionLine(path[i - 1].p, path[i].p);
            tPoint pointAfterTarget = path[i + 1].p;
            if(SideOfLine(sectionLine, pointAfterTarget) != side)
            {
                points = 1;
            }
        }

    // check for overshot end of path
        if(points == 1)
        {
            tLine endBound(path[i].p, path[i - 1].p);
            endBound.rotateLeft90();
            if(SideOfLine(endBound, nextP) == SIDE_LEFT)
            {
                return false;
            }
        }

    // check for path points passed
        while(points > 1)
        {
            tLine sectionLine(path[i].p, path[i + 1].p);
            if(SideOfLine(sectionLine, nextP) != OtherSide(side))
            {
                break;
            }

            if(SideOfLine(line, path[i].p) == OtherSide(side))
            {
            // went through path
                return false;
            }
            points--;
            i++;

        // if path turns in other direction then make this point our final target
            if(points > 1)
            {
                tLine sectionLine(path[i - 1].p, path[i].p);
                tPoint pointAfterTarget = path[i + 1].p;
                if(SideOfLine(sectionLine, pointAfterTarget) != side)
                {
                    points = 1;
                }
            }

        // check for overshot end of path
            if(points == 1)
            {
                tLine endBound(path[i].p, path[i - 1].p);
                endBound.rotateLeft90();
                if(SideOfLine(endBound, nextP) == SIDE_LEFT)
                {
                    return false;
                }
            }
        }

        {
            tLine sectionLine(path[i - 1].p, path[i].p);
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
        tPoint toEnd = path[i].p - nextP;
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
                        path[i].p.getX(), path[i].p.getY(),
                        endCell
                        );
                if(collides || endCell != path[i].f.index())
                {
                    return false;
                }
            }
            size_t sizeBefore = pathResult.size();
            pathResult.resize(sizeBefore + pathAttempt.size());
            copy(pathAttempt.begin(), pathAttempt.end(), pathResult.begin() + sizeBefore);
            pointsPassed = i - startIndex;
            return true;
        }
    }
}

void
GenerateCurvedPathPiece(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path,
        long startI, long endI,
        long startVectorX, long startVectorY,
        long sectionLength,
        float turnRatio1, float turnRatio2,
        std::vector<cInternalPosition>& pathResult
        )
{
    pathResult.push_back(path[startI]);

    tPoint previousAxis;
    if(startVectorX == 0 && startVectorY == 0)
    {
        ++startI;
        pathResult.push_back(path[startI]);
        if(startI == endI)
        {
            return;
        }
        previousAxis = path[startI].p - path[startI - 1].p;
    }
    else
    {
        previousAxis = tPoint(startVectorX, startVectorY, WORLD_RANGE);
    }

    long targetIndex = startI + 1;
    do
    {
        tPoint axis = path[targetIndex].p - path[targetIndex - 1].p;
        if(!SatisfiesTurnRatio(turnRatio1, previousAxis, axis))
        {
            long pointsPassed;
            bool succeeded;
            succeeded = AttemptCurvedSection(
                    mesh, shape, context,
                    path,
                    targetIndex, endI,
                    turnRatio1, sectionLength,
                    previousAxis,
                    pathResult,
                    pointsPassed
                    );
            if(!succeeded && !SatisfiesTurnRatio(turnRatio2, previousAxis, axis))
            {
                succeeded = AttemptCurvedSection(
                            mesh, shape, context,
                            path,
                            targetIndex, endI,
                            turnRatio2, sectionLength,
                            previousAxis,
                            pathResult,
                            pointsPassed
                            );
            }
            if(succeeded)
            {
                targetIndex += pointsPassed;
            }
        }
        previousAxis = path[targetIndex].p - pathResult.back().p;
        pathResult.push_back(path[targetIndex]);
        targetIndex++;
    }
    while(targetIndex <= endI);
}

void
GenerateCurvedPath(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path, const std::vector<long>* connectionIndices, long startOffset,
        long startVectorX, long startVectorY,
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
        GenerateCurvedPathPiece(
                mesh, shape, context,
                path,
                startOffset, SizeL(path) - 1,
                startVectorX, startVectorY,
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
        GenerateCurvedPathPiece(
                mesh, shape, context,
                path,
                i, endI,
                startVectorX, startVectorY,
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
