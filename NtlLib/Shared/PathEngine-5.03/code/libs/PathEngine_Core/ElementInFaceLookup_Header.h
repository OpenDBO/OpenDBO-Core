//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ELEMENT_IN_FACE_LOOKUP_HEADER_DEFINED
#define ELEMENT_IN_FACE_LOOKUP_HEADER_DEFINED

#include "common/Containers/ReplacementList.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"

class cCircuitElement;

class cElementInFaceEntry;

std::vector<std::vector<cElementInFaceEntry> >*
ConstructAndBuildElementsInFaceLookup(const tMeshCutSequence& sequence, long facesInMesh);
std::vector<std::vector<cElementInFaceEntry> >*
ConstructAndBuildElementsInFaceLookup(const tMeshCircuits& circuits, long facesInMesh);
std::vector<std::vector<cElementInFaceEntry> >*
ConstructElementsInFaceLookup(long facesInMesh);
void ClearElementsInFaceLookup(std::vector<std::vector<cElementInFaceEntry> >& lookup);
void DestroyElementsInFaceLookup(std::vector<std::vector<cElementInFaceEntry> >* ptr);

void LookupElementsInFace(tFace f, const tMeshCutSequence& circuit, std::vector<cElementInFaceEntry>& result);
void LookupElementsInFace(tFace f, const tMeshCircuits& circuits, std::vector<cElementInFaceEntry>& result);

void AddCutToLookup(const cMeshCut& cut, std::vector<std::vector<cElementInFaceEntry> >& lookup);
void BuildElementsInFaceLookup(const tMeshCutSequence& circuit,
                      std::vector<std::vector<cElementInFaceEntry> >& result);
void BuildElementsInFaceLookup(const tMeshCircuits& circuits,
                      std::vector<std::vector<cElementInFaceEntry> >& result);

class cCornerInFaceEntry
{
public:
    cCornerInFaceEntry(const cCircuitElement* element, long cornerIndex) :
      element(element),
      cornerIndex(cornerIndex)
    {
    }

    const cCircuitElement* element;
    long cornerIndex;
};

class cExternalSectionEntry
{
public:

    const cCircuitElement* _exitingElement;
    const cCircuitElement* _enteringElement;
    enum eType
    {
        ENTRY_AFTER_EXIT,
        EXIT_AFTER_ENTRY,
        ENTRY_ONLY,
        EXIT_ONLY,
        CONTAINED,
    } _type;

    cExternalSectionEntry() {}

    cExternalSectionEntry(const cCircuitElement* exitingElement, const cCircuitElement* enteringElement, eType type) :
      _exitingElement(exitingElement),
      _enteringElement(enteringElement),
      _type(type)
    {
    }

    static void addSection(std::vector<std::list<cExternalSectionEntry> >& lookup,
                        const cCircuitElement* exitingElement,
                        const cCircuitElement* enteringElement
                        );

    bool lineExitsInside(tEdge exitEE, const tLine& exitLine) const;
    static long countExits(
        const std::vector<std::list<cExternalSectionEntry> >& lookup,
        tEdge exitEE, const tLine& exitLine);
};

void BuildExternalSectionLookup(const tMeshCircuits& circuits,
                      std::vector<std::list<cExternalSectionEntry> >& result);


const cCircuitElement*
FindMatchingCorner(const std::vector<cElementInFaceEntry>& fLookup, const cCircuitElement& matchWith);

#endif
