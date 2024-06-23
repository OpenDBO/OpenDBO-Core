//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/Mesh3DPartitioning.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/RangeTree.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"

void
cMesh3DPartitioning::initRangeTree(const std::vector<cSegmentTreeElement>& segments)
{
    std::vector<std::pair<tPoint::T, long> > pairs(segments.size());
    size_t i;
    for(i = 0; i < segments.size(); i++)
    {
        pairs[i].first = segments[i].start;
        pairs[i].second = segments[i].key;
    }
    _rangeTree = new cRangeTree(pairs);
}

cMesh3DPartitioning::~cMesh3DPartitioning()
{
    delete _segmentTree;
    delete _rangeTree;
}

void
cMesh3DPartitioning::filterByYRange(long startY, long endY, long* resultsBuffer, long& results) const
{
    long i;
    for(i = 0; i < results;)
    {
        long f = resultsBuffer[i];
        if(_yRanges[f * 2] > endY || _yRanges[f * 2 + 1] < startY)
        {
            resultsBuffer[i] = resultsBuffer[results - 1];
            results--;
        }
        else
        {
            i++;
        }
    }
}

long
cMesh3DPartitioning::maximumResultsSize()
{
    return _rangeTree->totalNumberOfElements();
}
long
cMesh3DPartitioning::queryFacesPotentiallyStabbed(const tPoint& p, long* resultsBuffer) const
{
    long results = _segmentTree->query(p.getX(), resultsBuffer);
    filterByYRange(p.getY(), p.getY(), resultsBuffer, results);
    return results;
}
long
cMesh3DPartitioning::queryFacesPotentiallyOverlapped(const tPoint& bottomLeft, const tPoint& topRight, long* resultsBuffer) const
{
    long results = _rangeTree->query(bottomLeft.getX(), topRight.getX(), resultsBuffer);
    results += _segmentTree->query(bottomLeft.getX(), resultsBuffer + results);
    filterByYRange(bottomLeft.getY(), topRight.getY(), resultsBuffer, results);
    return results;
}
