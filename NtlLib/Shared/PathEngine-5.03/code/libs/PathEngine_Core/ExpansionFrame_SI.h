//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPANSION_FRAME_SI_DEFINED
#define EXPANSION_FRAME_SI_DEFINED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/STL/vector.h"

class cCollisionShape;
//class cCircuitElement;

class cExpansionFrame_SI
{
public:
    enum eStartType
    {
        STARTS_INSIDE,
        MESH_ENTRY,
        BLOCKED,
    };
private:

    long _sections;
    std::vector<tLine> _spokes;
    std::vector<std::vector<tEdge> > _baseSequence;
    eStartType _startType, _endType;
    tEdge _startTypeInfo, _endTypeInfo;

    bool sectionIsRadial(long section) const;

public:

     // a reference to expandBy is retained by the constructors for the lifetime of the frame
    cExpansionFrame_SI(tFace convexExternalFace, const cCollisionShape& expandBy);
    cExpansionFrame_SI(tEdge first, tEdge last, const cCollisionShape& expandBy);
//    cExpansionFrame_SI(const cCircuitElement* cutStartElement, const cCollisionShape& expandBy);

    void getStartInfo(eStartType& type, tEdge& info) const
    {
        type = _startType;
        info = _startTypeInfo;
    }
    void getEndInfo(eStartType& type, tEdge& info) const
    {
        type = _endType;
        info = _endTypeInfo;
    }

    long size() const
    {
        return _sections;
    }

    tFace startFace() const
    {
        return _startTypeInfo.face();
    }
    long baseSequenceSize(long section) const
    {
        return SizeL(_baseSequence[section]);
    }
    tEdge baseSequenceEdge(long section, long indexInSection) const
    {
        if(indexInSection >= baseSequenceSize(section))
        {
            assertD(indexInSection == baseSequenceSize(section));
            return tMesh::endEdge();
        }
        return _baseSequence[section][indexInSection];
    }

    tLine boundary(long section) const
    {
        return tLine(_spokes[section].end(), _spokes[section + 1].end());
    }
    tLine base(long section) const
    {
        assertD(!sectionIsRadial(section));
        return tLine(_spokes[section].start(), _spokes[section + 1].start());
    }
    tLine sectionStart(long section) const
    {
        return _spokes[section];
    }
    tLine sectionEnd(long section) const
    {
        return _spokes[section + 1];
    }
    tLine expandLine(long section, const tPoint& p) const;
    eSide sense(long section, const tLine& line) const;
    eSide sense(long section, tEdge edge) const;
    bool isBefore(long section, const tPoint& lhs, const tPoint& rhs) const;

    enum eTraverseResult
    {
        ENDS,
        EXITS,
        CROSSES_TO_NEXT,
    };

    eTraverseResult traverse(long section, tEdge e) const;
    eTraverseResult reverseTraverse(long section, tEdge e) const;
};

#endif
