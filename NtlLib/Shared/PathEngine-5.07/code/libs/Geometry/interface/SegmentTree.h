//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SEGMENT_TREE_INCLUDED
#define SEGMENT_TREE_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class cSegmentTreePoint;

class cSegmentTreeElement
{
public:

    tPoint::T start,end;
    long key;
};

class cSegmentTree
{
    long m_total_segments;
    tPoint::T m_range_start,m_range_end;
    long m_root_index;

    // each non leaf node has a split, 2 children, and a set of keys
    std::vector<tPoint::T> m_splits;
    std::vector<long> m_child_indices;
    std::vector<std::vector<long> > m_segment_sets;

    // each leaf node has just a set of keys
    std::vector<std::vector<long> > m_leaf_segment_sets;

    long buildSubTree(const tPoint::T* unique_points_ptr, long unique_points);
    void insertSegment(const cSegmentTreeElement& segment, long node,
                                const tPoint::T& range_start, const tPoint::T& range_end);
    bool removeEmptySubtrees(long node);

    long querySubTree(  long node,
                        const tPoint::T& stabbing_value, 
                        long* results_buffer) const;

public:

    cSegmentTree(const std::vector<cSegmentTreeElement>& segments);

    long totalNumberOfSegments() const;
    long query(const tPoint::T& stabbing_value, long* resultsBuffer) const;
};

#endif
