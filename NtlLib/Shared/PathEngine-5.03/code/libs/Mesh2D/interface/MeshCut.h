//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_CUT_INCLUDED
#define MESH_CUT_INCLUDED

#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "common/HandleTarget.h"
#include "common/Handle.h"
#include "common/HandleList.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "common/STL/list.h"

class cCircuitElement;
class iGraphicsInterface;
template <class T> class iMeshTranslator;
class iSerialiser;
class cAgent;
class cFlagVector;
class iOutputStream;
class cFacesContainedByBoundary;

class cCircuitStats
{
public:

    cCircuitStats()
    {
        _circuits = 0;
        _sequences = 0;
        _cuts = 0;
        _elements = 0;
        _inlineCorners = 0;
    }

    long _circuits, _sequences, _cuts, _elements, _inlineCorners;
};


class cMeshCut : public bHandleTarget
{
    cCircuitElement* _startElement;
    bool _affected;

    const cCircuitElement* getLeftMostVertex() const;
    static bool lessThanForNormalise(const cCircuitElement* e1, const cCircuitElement* e2);
    static long compareEntryOrExitEdges(tEdge lhs, tEdge rhs);
    static long compareIntersections(const tIntersection& lhs, const tIntersection& rhs);
    static long compareEntries(const cCircuitElement* lhs, const cCircuitElement* rhs);
    static long compareCorners(const cCircuitElement* lhs, const cCircuitElement* rhs);
    static long compareExits(const cCircuitElement* lhs, const cCircuitElement* rhs);

    long getNumberOfElements() const;

public:

    cMeshCut()
    {
        _startElement=0;
        _affected = false;
    }
    cMeshCut(cCircuitElement* start_element) :
      _startElement(start_element)
    {
        _affected = false;
    }
    ~cMeshCut();

    cMeshCut(const cMeshCut& rhs);
    const cMeshCut& operator=(const cMeshCut& rhs);

//    void setAgent(long agent);

    void operator>>(iSerialiser&);

    void addToStats(cCircuitStats& stats) const;
    long countElements() const;
    long indexElements(long startIndex) const;

    void setAffected() {_affected = true;}
    void clearAffected() {_affected = false;}
    bool wasAffected() const {return _affected;}

    void changeStartElement();

    void initFromFragment(cCircuitElement& mid_element_in_fragment);
    void initFromUnlinked(cCircuitElement& start_element);
    void initFromFragment(cCircuitElement* first, cCircuitElement* last);

    bool isInternalLoop() const;
    bool isHole() const;

    tEdge entryEdge() const;
    tEdge exitEdge() const;
    tFace entersFrom() const;
    tFace exitsTo() const;
    tFace startFace() const;
    const tLine& refEntryLine() const;
    const tLine& refExitLine() const;

    cCircuitElement* takeCircuit()
    {
        cCircuitElement* taken=_startElement;
        _startElement=0;
        return taken;
    }
//    void takeCircuit(cCircuit& result);

    void swap(cMeshCut& rhs)
    {
        cCircuitElement* tmp=_startElement;
        _startElement=rhs._startElement;
        rhs._startElement=tmp;
    }

    bool operator==(const cMeshCut& rhs) const;
    bool operator!=(const cMeshCut& rhs) const
    {
        return !((*this)==rhs);
    }

    static long compare(const cMeshCut& lhs, const cMeshCut& rhs);

    cCircuitElement* getStartElement()
    {
        return _startElement;
    }
    const cCircuitElement* getStartElement() const
    {
        return _startElement;
    }

    void invert();

    void remap(const iMeshTranslator<tMesh>& translator);

    // standardises start element for looping circuits
    void normalise();

    void normaliseLines();

    bool isSmallConvex(cFacesContainedByBoundary& fcbb, long maximumCircumference) const;

    bool testPointIsOnBoundary(tFace f, const tIntersection& p) const;

    void getStartIntersection(tIntersection& result) const;
    tFace getStartFace() const;

    void joinInlineCorners();
    void checkNoZeroLengthSections();

    void markZeroLengthSections();

    // NOTE this is currently also checked in validation
    bool containsInlineCorner() const;

    bool validateLinks() const;
    bool validate() const;

    bool toNextElement(const cCircuitElement*& e) const;

    tFace remappedEntryFace(const iMeshTranslator<tMesh>& translator) const;
    tFace remappedStartFace(const iMeshTranslator<tMesh>& translator) const;

    tFace iterateOverFacesCrossed(const cCircuitElement*& e, long& faceIndex) const;
};

void InvertCircuits(tMeshCircuits& circuit, tMeshCircuits& result);

void RemapCutSequence(tMeshCutSequence& sequence, iMeshTranslator<tMesh>& translator);
void RemapCircuits(tMeshCircuits& circuits, iMeshTranslator<tMesh>& translator);

// calls normalise() on each cut then puts the cuts in standard order
void NormaliseCutSequence(tMeshCutSequence& sequence);
long CompareCutSequences(const tMeshCutSequence& lhs, const tMeshCutSequence& rhs);
void NormaliseCircuits(tMeshCircuits& circuits);

void NormaliseLinesCutSequence(tMeshCutSequence& sequence);
void NormaliseLinesCircuits(tMeshCircuits& circuits);

void GetIntersectedExternalFaces(long facesInMesh, tMeshCircuits& circuits,
                                 std::list<tFace>& result);

bool CircuitIsInternal(const tMeshCutSequence& circuit);

bool TestPointIsOnBoundary(const tMeshCutSequence& circuit, tFace f, const tIntersection& p);

// TODO get rid of these versions
cHandle<tMeshCutSequence> CopyCutSequence(const tMeshCutSequence& to_copy);
cHandle<tMeshCircuits> CopyCircuits(const tMeshCircuits& to_copy);

void CopyCutSequence(const tMeshCutSequence& to_copy, tMeshCutSequence& appendTo);
void CopyCircuits(const tMeshCircuits& to_copy, tMeshCircuits& appendTo);

// the cuts in the sequence take ownership of all circuit elements
void ConvertLegacyCircuitToCutSequence(
        cCircuitElement* circuit_start,
        tMeshCutSequence& sequence);

// the cut sequence are left empty after this call
// the individual cuts could be held by shared pointers elsewhere, but are left empty
cCircuitElement* ConvertCutSequenceToLegacyCircuit(tMeshCutSequence& sequence);

void JoinInlineCorners(tMeshCutSequence& cuts);

bool CutSequenceContainsInlineCorner(const tMeshCutSequence& cuts);
bool CircuitsContainInlineCorner(const tMeshCircuits& circuits);

bool CircuitIsSmallConvex(cFacesContainedByBoundary& fcbb, const tMeshCutSequence& circuit, long maximumCircumference);
void SplitSmallConvex(long edgesInMesh, tMeshCircuits& circuits, long maximumCircumference, tMeshCircuits& appendTo);

bool ValidateCuts(const tMeshCutSequence& cuts);
bool ValidateCircuitPart(const tMeshCutSequence& circuit);
bool ValidateCircuit(const tMeshCutSequence& circuit);
bool ValidateCircuits(const tMeshCircuits& circuits);

bool CutSequenceContainsAnExternalFace(const tMeshCutSequence& sequence);

void AddSequenceToStats(const tMeshCutSequence& sequence, cCircuitStats& stats);
void AddCircuitsToStats(const tMeshCircuits& circuits, cCircuitStats& stats);
long CountElements(const tMeshCutSequence& sequence);
long CountElements(const tMeshCircuits& circuits);
long IndexElements(const tMeshCutSequence& sequence, long startIndex);
long IndexElements(const tMeshCircuits& circuits, long startIndex);

tFace ReTraverseElement(const cCircuitElement* element, tFace startF);

void GenerateCircuitsCheckSum(const tMeshCircuits& circuits, iOutputStream& os);

class cCircuitsConstIterator
{
    const tMeshCircuits* _circuits;
    tMeshCircuits::const_iterator _circuit;
    const tMeshCutSequence* _cutSequence;
    tMeshCutSequence::const_iterator _cut;
    const cCircuitElement* _startEl;
    const cCircuitElement* _el;

public:

    cCircuitsConstIterator(tMeshCircuits& circuits);

    bool valid() const
    {
        return _el != 0;
    }

    cCircuitsConstIterator& operator++(); // prefix
    const cCircuitElement& operator*() const
    {
        return *_el;
    }
    const cCircuitElement* operator->() const
    {
        return _el;
    }
};

#endif
