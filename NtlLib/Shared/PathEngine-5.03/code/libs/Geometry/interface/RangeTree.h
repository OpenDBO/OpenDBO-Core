//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef RANGE_TREE_INCLUDED
#define RANGE_TREE_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class cRangeTree
{
    long m_total_elements;
    tPoint::T m_range_start, m_range_end;
    std::vector<tPoint::T> m_values;
    std::vector<long> m_child_nodes;
    std::vector<long> m_element_indices;
    std::vector<long> m_element_data;

    static void storeSubset(const std::pair<tPoint::T, long>* subset_begin,
                        const std::pair<tPoint::T, long>* subset_end,
                        std::vector<long>& indices, std::vector<long>& data);
    long buildSubTree(const std::vector<std::pair<tPoint::T, long> >& pairs,
                        const long* unique_points_ptr, long unique_points
                        );
    long querySubTree(  long node,
                        bool extends_left, bool extends_right,
                        const tPoint::T& start, const tPoint::T& end,
                        long* results_buffer) const;
public:

    cRangeTree(const std::vector<std::pair<tPoint::T, long> >& pairs);

    long totalNumberOfElements() const;
    long query(const tPoint::T& start, const tPoint::T& end, long* resultsBuffer) const;
};

#endif
