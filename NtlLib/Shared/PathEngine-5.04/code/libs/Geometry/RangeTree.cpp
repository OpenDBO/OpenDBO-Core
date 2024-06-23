//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/RangeTree.h"
//#include "libs/Instrumentation/interface/Profiler.h"
#include "common/STL/algorithm.h"
#include "common/STL_Helpers.h"

using namespace std;

class cRangeTreePredicate
{
public:
	cRangeTreePredicate() // Stop MSVC71 complaining about the object not being 'defined'
	{
	}

    bool operator()(const pair<tPoint::T,long>& lhs, const pair<tPoint::T,long>& rhs) const
    {
        return lhs.first < rhs.first;
    }
};

void
cRangeTree::storeSubset(const std::pair<tPoint::T, long>* subset_begin,
                        const std::pair<tPoint::T, long>* subset_end,
                        std::vector<long>& indices, std::vector<long>& data)
{
    long start_index = SizeL(data);
    indices.push_back(start_index);

    long subset_size = static_cast<long>(subset_end - subset_begin) + 1;
    assertD(subset_size >= 0);
    data.resize(start_index + subset_size);

    while(1)
    {
        data[start_index++] = subset_begin->second;
        if(subset_begin == subset_end)
            break;
        subset_begin++;
    }
}

long
cRangeTree::buildSubTree(const std::vector<std::pair<tPoint::T, long> >& pairs,
                        const long* unique_points_ptr, long unique_points
                        )
{
    assertD(unique_points > 0);

    long node = SizeL(m_values);

// store canonical subset
    {
        long subset_begin = unique_points_ptr[0];
        long subset_end = unique_points_ptr[unique_points] - 1;

        storeSubset(    &pairs[subset_begin], &pairs[subset_end],
                        m_element_indices,
                        m_element_data);
    }

    long split = unique_points / 2;

// store subset exactly at split
    {
        long subset_begin = unique_points_ptr[split];
        long subset_end = unique_points_ptr[split + 1] - 1;

        storeSubset(    &pairs[subset_begin], &pairs[subset_end],
                        m_element_indices,
                        m_element_data);
    }

    {
        long i = unique_points_ptr[split];
        m_values.push_back(pairs[i].first);
    }

    // reserve space for child indices;
    m_child_nodes.resize(m_child_nodes.size()+2);

    long before = -1;
    if(split > 0)
    {
        before = buildSubTree(pairs, unique_points_ptr, split);
    }
    long after = -1;
    long first_after_split = split + 1;
    if(first_after_split < unique_points)
    {
        after = buildSubTree(pairs, unique_points_ptr + first_after_split, unique_points - first_after_split);
    }

    m_child_nodes[node * 2 + 0] = before;
    m_child_nodes[node * 2 + 1] = after;
    
    return node;
}

long 
cRangeTree::querySubTree(   long node,
                            bool extends_left, bool extends_right,
                            const tPoint::T& start, const tPoint::T& end,
                            long* results_buffer) const
{
    long results = 0;

    if(extends_left && extends_right)
    {
    // report canonical subset for this node and return
        long begin = m_element_indices[node * 2 + 0];
        long end = m_element_indices[node * 2 + 1];
        while(begin < end)
        {
            results_buffer[results++] = m_element_data[begin++];
        }

        return results;
    }

    tPoint::T split_value = m_values[node];

    if(extends_left || start < split_value - 1)
    {
    // expand left sub tree
        long before = m_child_nodes[node * 2 + 0];
        if(before != -1)
        {
            bool extends_right = (end >= split_value);
            results += querySubTree(before, extends_left, extends_right, start, end, results_buffer + results);
        }
    }
    if(start < split_value && end > split_value)
    {
    // report subset exactly at this node
        long begin = m_element_indices[node * 2 + 1];
        long end = m_element_indices[node * 2 + 2];
        while(begin < end)
        {
            results_buffer[results++] = m_element_data[begin++];
        }
    }
    if(extends_right || end > split_value + 1)
    {
    // expand right sub tree
        long after = m_child_nodes[node * 2 + 1];
        if(after != -1)
        {
            bool extends_left = (start <= split_value);
            results += querySubTree(after, extends_left, extends_right, start, end, results_buffer + results);
        }
    }

    return results;
}

cRangeTree::cRangeTree(const std::vector<std::pair<tPoint::T, long> >& pairs)
{
    m_total_elements = SizeL(pairs);
    if(m_total_elements == 0)
    {
        return;
    }

    vector<pair<tPoint::T, long> > working_pairs = pairs;
    cRangeTreePredicate predicate;
    sort(working_pairs.begin(), working_pairs.end(), predicate);

    m_range_start = working_pairs.front().first;
    m_range_end = working_pairs.back().first;

    vector<long> unique_points(working_pairs.size() + 1);
    {
        long actual_size = 0;
        long i;
        for(i = 0; i < SizeL(working_pairs); i++)
        {
            if(i == 0 || predicate(working_pairs[i-1],working_pairs[i]))
            {
                unique_points[actual_size++] = i;
            }
        }
        unique_points[actual_size++] = i;
        unique_points.resize(actual_size);
    }

    long root_index = buildSubTree(working_pairs, &unique_points.front(), SizeL(unique_points) - 1);
    assertD(root_index == 0);

    m_element_indices.push_back(SizeL(m_element_data));

    ShrinkToFit(m_values);
    ShrinkToFit(m_child_nodes);
    ShrinkToFit(m_element_indices);
    ShrinkToFit(m_element_data);
}

long
cRangeTree::totalNumberOfElements() const
{
    return m_total_elements;
}

long
cRangeTree::query(const tPoint::T& start, const tPoint::T& end, long* resultsBuffer) const
{
    if(m_total_elements == 0)
        return 0;
    if(start >= m_range_end || end <= m_range_start)
        return 0;
    bool extends_left = (start < m_range_start);
    bool extends_right = (end > m_range_end);
    return querySubTree(0, extends_left, extends_right, start, end, resultsBuffer);
}

