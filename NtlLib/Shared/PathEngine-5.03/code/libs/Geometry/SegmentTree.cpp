//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/SegmentTree.h"
#include "common/STL/algorithm.h"
#include "common/STL_Helpers.h"

using namespace std;

enum ePointType
{
// order is important
    PT_END,
    PT_START,
};

class cSegmentTreePoint
{
public:

    cSegmentTreePoint()
    {
    }
    cSegmentTreePoint(ePointType type, const tPoint::T& value, long key) :
      type(type),
      value(value),
      key(key)
    {
    }

    ePointType type;
    tPoint::T value;
    long key;

    bool operator<(const cSegmentTreePoint& rhs) const
    {
        if(value < rhs.value)
            return true;
        if(value > rhs.value)
            return false;
        return false;
//        return type < rhs.type;
    }
};

long
cSegmentTree::buildSubTree(const tPoint::T* unique_points_ptr, long unique_points)
{
    assertD(unique_points > 1);

    if(unique_points == 2)
    {
    // leaf node
        long leaf_node = SizeL(m_leaf_segment_sets);
        m_leaf_segment_sets.push_back(vector<long>());
        return -2 - leaf_node;
    }

    // split

    long index_for_this = SizeL(m_splits);

    long split_index = unique_points / 2;
    tPoint::T split_value = unique_points_ptr[split_index];
    m_splits.push_back(split_value);

    m_segment_sets.push_back(vector<long>());

    // reserve space for child indices;
    m_child_indices.resize(m_child_indices.size()+2);

    long before = buildSubTree(unique_points_ptr, split_index + 1);
    long after = buildSubTree(unique_points_ptr + split_index, unique_points - split_index);

    m_child_indices[index_for_this * 2 + 0] = before;
    m_child_indices[index_for_this * 2 + 1] = after;

    return index_for_this;
}

void 
cSegmentTree::insertSegment(const cSegmentTreeElement& segment,
                            long node,
                            const tPoint::T& node_range_start,
                            const tPoint::T& node_range_end)
{
    if(node < 0)
    {
    // leaf node
        long leaf_index = -2 - node;
        m_leaf_segment_sets[leaf_index].push_back(segment.key);
        return;
    }

    bool starts_before_split = (segment.start < m_splits[node]);
    bool ends_before_split = (segment.end <= m_splits[node]);

    if(starts_before_split != ends_before_split)
    {
    // check if segment completely overlaps this range
        if(segment.start <= node_range_start && segment.end >= node_range_end)
        {
        // yes - in this case, segment is entered here and not in any subtrees
            m_segment_sets[node].push_back(segment.key);
            return;
        }
    }

    if(starts_before_split)
    {
        insertSegment(segment, m_child_indices[node * 2 + 0], node_range_start, m_splits[node]);
    }
    if(!ends_before_split)
    {
        insertSegment(segment, m_child_indices[node * 2 + 1], m_splits[node], node_range_end);
    }
}

bool
cSegmentTree::removeEmptySubtrees(long node)
{
    if(node < 0)
    {
    // leaf node
        long leaf_index = -2 - node;
        return m_leaf_segment_sets[leaf_index].empty();
    }

    bool isEmpty = m_segment_sets[node].empty();
    if(removeEmptySubtrees(m_child_indices[node * 2 + 0]))
    {
        m_child_indices[node * 2 + 0] = -1;
    }
    else
    {
        isEmpty = false;
    }
    if(removeEmptySubtrees(m_child_indices[node * 2 + 1]))
    {
        m_child_indices[node * 2 + 1] = -1;
    }
    else
    {
        isEmpty = false;
    }
    return isEmpty;
}

cSegmentTree::cSegmentTree(const std::vector<cSegmentTreeElement>& segments)
{
    m_total_segments = 0;

    vector<cSegmentTreePoint> points(segments.size() * 2);
    size_t i;
    for(i = 0; i < segments.size(); i++)
    {
        if(segments[i].start != segments[i].end)
        {
            m_total_segments++;
            points[i] = cSegmentTreePoint(PT_START,segments[i].start,segments[i].key);
            points[segments.size() + i] = cSegmentTreePoint(PT_END,segments[i].end,segments[i].key);
        }
    }

    if(m_total_segments == 0)
    {
        m_range_start = m_range_end = 0;
        return;
    }

    sort(points.begin(),points.end());

    vector<tPoint::T> unique_points(points.size());
    {
        long actual_size = 0;
        size_t i;
        for(i = 0; i < points.size(); i++)
        {
            if(i == 0 || points[i-1] < points[i])
            {
                unique_points[actual_size++] = points[i].value;
            }
        }
        unique_points.resize(actual_size);
    }

    m_root_index = buildSubTree(&unique_points.front(), SizeL(unique_points));
    m_range_start = points.front().value;
    m_range_end = points.back().value;

    for(i = 0; i < segments.size(); i++)
    {
        if(segments[i].start != segments[i].end)
        {
            insertSegment(segments[i], m_root_index, m_range_start, m_range_end);
        }
    }

    bool all_empty = removeEmptySubtrees(m_root_index);
    assertD(!all_empty);

    ShrinkToFit(m_splits);
    ShrinkToFit(m_child_indices);
    ShrinkToFit(m_segment_sets);
    ShrinkToFit_Compound(m_leaf_segment_sets);
}

long
cSegmentTree::totalNumberOfSegments() const
{
    return m_total_segments;
}

long 
cSegmentTree::querySubTree( long node,
                            const tPoint::T& stabbing_value, 
                            long* results_buffer) const
{
    long results = 0;

    if(node < 0)
    {
    // leaf node
        long leaf_index = -2 - node;
        {
            const vector<long>& v = m_leaf_segment_sets[leaf_index];
//            if(!v.empty())
//            {
//                memcpy(results_buffer + results, &v.front(), v.size() * sizeof(long));
//            }
//            results += v.size();
            size_t i;
            for(i = 0; i < v.size(); i++)
            {
                results_buffer[results++] = v[i];
            }
        }
        return results;
    }

    {
        const vector<long>& v = m_segment_sets[node];
//        if(!v.empty())
//        {
//            memcpy(results_buffer + results, &v.front(), v.size() * sizeof(long));
//        }
//        results += v.size();
        size_t i;
        for(i = 0; i < v.size(); i++)
        {
            results_buffer[results++] = v[i];
        }
    }

    if(stabbing_value < m_splits[node])
    {
        long before_node = m_child_indices[node * 2 + 0];
        if(before_node != -1)
        {
            results += querySubTree(before_node, stabbing_value, results_buffer + results);
        }
    }
    else
    {
        long after_node = m_child_indices[node * 2 + 1];
        if(after_node != -1)
        {
            results += querySubTree(after_node, stabbing_value, results_buffer + results);
        }
    }

    return results;
}

long
cSegmentTree::query(const tPoint::T& stabbing_value, long* resultsBuffer) const
{
//    PROFILER_SCOPE("cSegmentTree::query");
    if(stabbing_value < m_range_start || stabbing_value >= m_range_end)
        return 0;
    return querySubTree(m_root_index, stabbing_value, resultsBuffer);
}

