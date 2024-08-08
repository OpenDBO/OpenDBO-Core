//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPANSION_PIECE_SPLITTER_INCLUDED
#define EXPANSION_PIECE_SPLITTER_INCLUDED

#include "libs/Geometry/interface/tLine_Header.h"
#include "common/STL/vector.h"

class cCircuitElement;

class cExpansionPieceSplitter
{
    long lookupInFace(long fIndex, cCircuitElement* element) const;
    void add(cCircuitElement* element);

public:

    std::vector<std::vector<cCircuitElement*> > _lookup;

    cExpansionPieceSplitter(long facesInMesh);
    ~cExpansionPieceSplitter();

    void getElementsInFacesCrossed(const cCircuitElement* testElement, std::vector<cCircuitElement*>& result);

    void splitExisting(cCircuitElement* toSplit, long faceIndex, const tLine& splitLine);

    void addSequence(cCircuitElement* element);

    void stripElement(cCircuitElement* toStrip);
    void stripChain(cCircuitElement* midElement, cCircuitElement*& first, cCircuitElement*& last);

    bool isValid() const;
};

#endif
