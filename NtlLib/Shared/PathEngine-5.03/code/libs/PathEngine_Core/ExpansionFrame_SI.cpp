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
#include "ExpansionFrame_SI.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Geometry/interface/CollisionShape.h"

bool
cExpansionFrame_SI::sectionIsRadial(long section) const
{
    return _spokes[section].start() == _spokes[section + 1].start();
}

static long
WorkOutNumberOfSections(tFace convexExternalFace, const cCollisionShape& expandBy)
{
    long result = 0;
    tEdge e = tMesh::beginEdge(convexExternalFace);
    long expand = expandBy.startExpand(GetEdgeAxis(e));
    do
    {
        assertD(!ExternalVertexIsConcave(e));
        result++; // parallel section
        long nextExpand = expandBy.startExpand(GetEdgeAxis(e.next()));
        while(expand != nextExpand)
        {
            result++; // radial section
            expand = expandBy.nextVertex(expand);
        }
    }
    while(e.toNextInFace());
    return result;
}
cExpansionFrame_SI::cExpansionFrame_SI(tFace convexExternalFace, const cCollisionShape& expandBy) :
 _sections(WorkOutNumberOfSections(convexExternalFace, expandBy)),
 _spokes(_sections + 1),
 _baseSequence(_sections)
{
// build spokes

    long section = 0;
    tEdge e = tMesh::beginEdge(convexExternalFace);

    // used after loop for final spoke
    long expand = expandBy.startExpand(GetEdgeAxis(e));
    tPoint p = GetVertexPoint(e);

    do
    {
        // start of parallel section
        _spokes[section++] = tLine(p, p + expandBy.expandOffset(expand));
        p = GetVertexPoint(e.next());
        long nextExpand = expandBy.startExpand(GetEdgeAxis(e.next()));
        while(expand != nextExpand)
        {
            // start of radial section
            _spokes[section++] = tLine(p, p + expandBy.expandOffset(expand));
            expand = expandBy.nextVertex(expand);
        }
    }
    while(e.toNextInFace());

    assertD(section == _sections);
    _spokes[section] = tLine(p, p + expandBy.expandOffset(expand));
    assertD(_spokes.back() == _spokes.front());

// build base sequence

    tEdge first = tMesh::beginEdge(convexExternalFace);
    tEdge last = first.prev();

    section = 0;
    e = first;

    // step e2 back to spoke at start of first parallel section
    tEdge e2 = e.twin().next();
    while(SideOfEdge_SI(e2, _spokes[section].end()) == SIDE_LEFT)
    {
        e2 = e2.twin().next();
    }
    e2 = e2.prev();

    _startType = STARTS_INSIDE;
    _endType = STARTS_INSIDE;
    _startTypeInfo = e2;
    _endTypeInfo = e2;

    while(1)
    {
        assertD(e2.next().vertex() == e.vertex());

        // edges at start of parallel section
        while(e.twin() != e2)
        {
            _baseSequence[section].push_back(e2);
            e2 = e2.twin().prev();
        }

        // edges before end of parallel section and for the radial sections
        --e2;
        do
        {
            while(SideOfEdge_SI(e2, _spokes[section + 1].end()) == SIDE_LEFT)
            {
                _baseSequence[section].push_back(e2);
                e2 = e2.twin().prev();
            }
            ++section;
        }
        while(section < _sections && sectionIsRadial(section));

        if(e == last)
        {
            assertD(section == _sections);
            return;
        }
        ++e;
    }
}

static long
WorkOutNumberOfSections(tEdge first, tEdge last, const cCollisionShape& expandBy)
{
    assertD(first.face() == last.face());
    assertD(ExternalVertexIsConcave(first));
    assertD(ExternalVertexIsConcave(last.next()));
    long result = 0;
    while(first != last)
    {
        assertD(!ExternalVertexIsConcave(first.next()));
      // parallel section + radial sections around corner
        long spinCount = SpinCountAroundVertex(first.next());
        result += 1 + expandBy.getEdgesForCorner_DontConnectInline(GetEdgeAxis(first), GetEdgeAxis(first.next()), spinCount);
        ++first;
    }
    result++; // final parallel section
    result++; // a single radial section at end to ensure overlap with next external edge expansion
    return result;
}
cExpansionFrame_SI::cExpansionFrame_SI(tEdge first, tEdge last, const cCollisionShape& expandBy) :
 _sections(WorkOutNumberOfSections(first, last, expandBy)),
 _spokes(_sections + 1),
 _baseSequence(_sections)
{
// build spokes

    long section = 0;
    tEdge e = first;

    // used after loop for final spoke or final radial section
    long endExpand;
    tPoint p;

    while(e != last)
    {
        long startExpand = expandBy.startExpand(GetEdgeAxis(e));
        endExpand = startExpand;
        p = GetVertexPoint(e);
        _spokes[section++] = tLine(p, p + expandBy.expandOffset(startExpand));
        p = GetVertexPoint(e.next());
        long spinCount = SpinCountAroundVertex(e.next());
        long radialSections = expandBy.getEdgesForCorner_DontConnectInline(GetEdgeAxis(e), GetEdgeAxis(e.next()), spinCount);
        while(radialSections--)
        {
            _spokes[section++] = tLine(p, p + expandBy.expandOffset(endExpand));
            endExpand = expandBy.nextVertex(endExpand);
        }
        assertD(endExpand == expandBy.startExpand(GetEdgeAxis(e.next())));
        ++e;
    }

    // final parallel section
    long startExpand = expandBy.startExpand(GetEdgeAxis(e));
    endExpand = startExpand;
    p = GetVertexPoint(e);
    _spokes[section++] = tLine(p, p + expandBy.expandOffset(startExpand));
    p = GetVertexPoint(e.next());

    // single radial section at end to ensure overlap
    _spokes[section++] = tLine(p, p + expandBy.expandOffset(endExpand));
    endExpand = expandBy.nextVertex(endExpand);

    assertD(section == _sections);
    _spokes[section] = tLine(p, p + expandBy.expandOffset(endExpand));

// build base sequence

    tEdge e2;

    e2 = first.twin().next();
    while(SideOfLine_SI(sectionStart(0), e2.next().vertex()) == SIDE_RIGHT && !e2.twin().face()->isExternal())
    {
        e2 = e2.twin().next();
    }
    if(SideOfLine_SI(sectionStart(0), e2.next().vertex()) == SIDE_RIGHT)
    {
        _startType = BLOCKED;
        _startTypeInfo = e2;
    }
    else if(SideOfLine_SI(sectionStart(0)) == SIDE_LEFT)
    {
        e2 = first.twin().next();
        _startType = MESH_ENTRY;
        _startTypeInfo = first.twin();
    }
    else
    {
        e2 = first.prev().twin();
        _startType = MESH_ENTRY;
        _startTypeInfo = e2;
    }
    --e2;

    section = 0;
    e = first;

    while(e != last)
    {
        // edges at start of parallel section
        assertD(e2.next().vertex() == e.vertex());
        while(e.twin() != e2)
        {
            _baseSequence[section].push_back(e2);
            e2 = e2.twin().prev();
        }

        // edges before end of parallel section and for the radial sections
        --e2;
        do
        {
            while(SideOfEdge_SI(e2, _spokes[section + 1].end()) == SIDE_LEFT)
            {
                _baseSequence[section].push_back(e2);
                e2 = e2.twin().prev();
            }
            ++section;
        }
        while(sectionIsRadial(section));
        ++e;
    }

    // edges at start of last parallel section
    assertD(e2.next().vertex() == e.vertex());
    while(e.twin() != e2)
    {
        _baseSequence[section].push_back(e2);
        e2 = e2.twin().prev();
    }

    long sizeAtEndOfLastParallel = SizeL(_baseSequence);

    // end of last parallel section
    --e2;
    do
    {
        while(SideOfLine_SI(sectionEnd(section), e2.vertex()) == SIDE_LEFT)
        {
            if(e2.twin().face()->isExternal())
            {
                _endType = BLOCKED;
                _endTypeInfo = e2;
              // frame is shortened to put the blocking edge in the last expand frame
                _sections = section + 1;
                return;
            }
            _baseSequence[section].push_back(e2);
            e2 = e2.twin().prev();
        }
        ++section;
    }
    while(section < _sections);

    --section;
    if(SideOfLine_SI(sectionEnd(section)) == SIDE_LEFT)
    {
        _endType = MESH_ENTRY;
        _endTypeInfo = last.next().twin();
        return;
    }

    _baseSequence.resize(sizeAtEndOfLastParallel);
    _endType = MESH_ENTRY;
    _endTypeInfo = last.twin();
}

//static long
//WorkOutNumberOfSections(const cCircuitElement* cutStartElement, const cCollisionShape& expandBy)
//{
//    long result = 0;
//    const cCircuitElement* e = cutStartElement;
//    long expand = expandBy.startExpand(e->refLine().axis());
//    e = e->getNext();
//    while(e != cutStartElement)
//    {
//        assertD(e->cornerIsConvex());
//        result++; // parallel section
//        long nextExpand = expandBy.startExpand(e->refLine().axis());
//        while(expand != nextExpand)
//        {
//            result++; // radial section
//            expand = expandBy.nextVertex(expand);
//        }
//        e = e->getNext();
//    }
//    result++; // final parallel section
//    if(!e->enters())
//    {
//        long nextExpand = expandBy.startExpand(e->refLine().axis());
//        while(expand != nextExpand)
//        {
//            result++; // radial section at loop closure
//            expand = expandBy.nextVertex(expand);
//        }
//    }
//    return result;
//}
//cExpansionFrame_SI::cExpansionFrame_SI(const cCircuitElement* cutStartElement, const cCollisionShape& expandBy) :
// _sections(WorkOutNumberOfSections(cutStartElement, expandBy)),
// _spokes(_sections + 1),
// _baseSequence(_sections)
//{
//// build spokes and base sequence simultaneously
//    
//    long section = 0;
//
//    const cCircuitElement* e = cutStartElement;
//    long expand = expandBy.startExpand(e->refLine().axis());
//    tPoint p, endP;
//    if(e->enters())
//    {
//        p = e->refLine().start();
//        assertD(SideOfEdge_SI(e->entryEdge(), p) == SIDE_LEFT);
//        endP = e->getPrev()->refLine().end();
//        assertD(SideOfEdge_SI(e->getPrev()->exitEdge(), endP) == SIDE_LEFT);
//        _startType = BLOCKED;
//        _endType = BLOCKED;
//        _startTypeInfo = e->entryEdge();
//        _endTypeInfo = e->getPrev()->exitEdge();
//    }
//    else
//    {
//        p = e->cornerPoint();
//        endP = p;
//        _startType = STARTS_INSIDE;
//        _endType = STARTS_INSIDE;
//        tFace f = e->getFaceCrossed(0);
//        _startTypeInfo = tMesh::beginEdge(f);
//        _endTypeInfo = tMesh::beginEdge(f);
//    }
//    e = e->getNext();
//    while(e != cutStartElement)
//    {
//        long i;
//        for(i = 0; i != e->getPrev()->getEdgesCrossed(); ++i)
//        {
//            _baseSequence[section].push_back(e->getPrev()->getEdgeCrossed(i));
//        }
//        // parallel section start
//        _spokes[section++] = tLine(p, p + expandBy.expandOffset(expand));
//        p = e->cornerPoint();
//        long nextExpand = expandBy.startExpand(e->refLine().axis());
//        while(expand != nextExpand)
//        {
//            // radial section start
//            _spokes[section++] = tLine(p, p + expandBy.expandOffset(expand));
//            expand = expandBy.nextVertex(expand);
//        }
//        e = e->getNext();
//    }
//    long i;
//    for(i = 0; i != e->getPrev()->getEdgesCrossed(); ++i)
//    {
//        _baseSequence[section].push_back(e->getPrev()->getEdgeCrossed(i));
//    }
//    // final parallel section start
//    _spokes[section++] = tLine(p, p + expandBy.expandOffset(expand));
//    p = endP;
//    if(!e->enters())
//    {
//        long nextExpand = expandBy.startExpand(e->refLine().axis());
//        while(expand != nextExpand)
//        {
//            // radial section start at loop closure
//            _spokes[section++] = tLine(p, p + expandBy.expandOffset(expand));
//            expand = expandBy.nextVertex(expand);
//        }
//    }
//    assertD(section == _sections);
//    // end of final parallel section, or loop closure
//    _spokes[section] = tLine(p, p + expandBy.expandOffset(expand));
//    assertD(e->enters() || _spokes.back() == _spokes.front());
//}


tLine
cExpansionFrame_SI::expandLine(long section, const tPoint& p) const
{
    tPoint expandDirection;
    if(sectionIsRadial(section))
    {
        expandDirection = p - _spokes[section].start();
    }
    else
    {
        expandDirection = _spokes[section].axis();
    }
    return tLine(p, p + expandDirection);
}
eSide
cExpansionFrame_SI::sense(long section, const tLine& line) const
{
    if(sectionIsRadial(section))
    {
        return line.side(_spokes[section].start());
    }
    return _spokes[section].side(line);
}
eSide
cExpansionFrame_SI::sense(long section, tEdge edge) const
{
    if(sectionIsRadial(section))
    {
        eSide result = SideOfEdge_SI(edge, _spokes[section].start());
        return result;
    }
    return _spokes[section].side(GetEdgeLine(edge));
}
bool
cExpansionFrame_SI::isBefore(long section, const tPoint& lhs, const tPoint& rhs) const
{
    return sense(section, tLine(lhs, rhs)) == SIDE_LEFT;
}

cExpansionFrame_SI::eTraverseResult
cExpansionFrame_SI::traverse(long section, tEdge e) const
{
    tVertex v = e.next().vertex();
    bool crossesToNext = (SideOfLine_SI(_spokes[section + 1], v) == SIDE_RIGHT);
    bool crossesOut = (SideOfLine_SI(boundary(section), v) == SIDE_LEFT);
    if(crossesToNext)
    {
        if(crossesOut)
        {
            if(SideOfEdge_SI(e, _spokes[section + 1].end()) == SIDE_RIGHT)
            {
                return EXITS;
            }
        }
        return CROSSES_TO_NEXT;
    }
    if(crossesOut)
    {
        return EXITS;
    }
    return ENDS;
}

cExpansionFrame_SI::eTraverseResult
cExpansionFrame_SI::reverseTraverse(long section, tEdge e) const
{
    tVertex v = e.next().vertex();
    bool crossesToNext = (SideOfLine_SI(_spokes[section], v) == SIDE_LEFT);
    bool crossesOut = (SideOfLine_SI(boundary(section), v) == SIDE_LEFT);
    if(crossesToNext)
    {
        if(crossesOut)
        {
            if(SideOfEdge_SI(e, _spokes[section].end()) == SIDE_LEFT)
            {
                return EXITS;
            }
        }
        return CROSSES_TO_NEXT;
    }
    if(crossesOut)
    {
        return EXITS;
    }
    return ENDS;
}
