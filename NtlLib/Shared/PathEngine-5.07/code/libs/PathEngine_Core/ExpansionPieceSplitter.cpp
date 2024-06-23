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
#include "ExpansionPieceSplitter.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/tLine.h"

static bool
ElementsRequireSplit(
        const cCircuitElement& element1,
        const cCircuitElement& element2,
        long& faceIndex1, bool& atCorner1,
        long& faceIndex2, bool& atCorner2
        )
{
    eSide side = element1.refLine().side(element2.refLine());
    if(side == SIDE_CENTRE)
    {
        //.. in the context within which this is used, an inline corner will never split a line
        return false;
    }
    if(side == SIDE_RIGHT)
    {
        faceIndex1 = element1.getFaceIndexForIntersection(element2.refLine(), atCorner1);
        if(faceIndex1 == -1)
        {
            return false;
        }
        faceIndex2 = element2.getFaceIndexForIntersection(element1.refLine().reversed(), atCorner2);
        if(faceIndex2 == -1)
        {
            return false;
        }
    }
    else
    {
        faceIndex2 = element2.getFaceIndexForIntersection(element1.refLine(), atCorner2);
        if(faceIndex2 == -1)
        {
            return false;
        }
        faceIndex1 = element1.getFaceIndexForIntersection(element2.refLine().reversed(), atCorner1);
        if(faceIndex1 == -1)
        {
            return false;
        }
    }
    if(atCorner1 && atCorner2)
    {
        return false;
    }
    return element1.getFaceCrossed(faceIndex1) == element2.getFaceCrossed(faceIndex2);
}

long
cExpansionPieceSplitter::lookupInFace(long fIndex, cCircuitElement* element) const
{
    long i;
    for(i = 0; i != SizeL(_lookup[fIndex]); ++i)
    {
        if(_lookup[fIndex][i] == element)
        {
            return i;
        }
    }
    return -1;
}

void
cExpansionPieceSplitter::add(cCircuitElement* element)
{
    assertD(!element->cornerIsInline());

    std::vector<cCircuitElement*> elementsToCheck;
    getElementsInFacesCrossed(element, elementsToCheck);
    std::vector<cCircuitElement*> toAdd(1, element);
    long i;
    for(i = 0; i != SizeL(elementsToCheck); ++i)
    {
        cCircuitElement* toCheck = elementsToCheck[i];
        long j;
        for(j = 0; j != SizeL(toAdd); ++j)
        {
            long faceIndex1, faceIndex2;
            bool atCorner1, atCorner2;
            if(ElementsRequireSplit(*toAdd[j], *toCheck, faceIndex1, atCorner1, faceIndex2, atCorner2))
            {
                if(!atCorner2)
                {
                    splitExisting(toCheck, faceIndex2, toAdd[j]->refLine());
                }
                if(!atCorner1)
                {
                    toAdd[j]->split(faceIndex1, toCheck->refLine());
                    assertF(toAdd[j]->isValid());
                    assertF(toAdd[j]->getNext()->isValid());
                    toAdd.push_back(toAdd[j]->getNext());
                }
                break;
            }
        }
    }
    for(i = 0; i != SizeL(toAdd); ++i)
    {
        cCircuitElement* element = toAdd[i];
        long j;
        for(j = 0; j != element->facesCrossed(); ++j)
        {
            long fIndex = element->faceCrossed(j).index();
            _lookup[fIndex].push_back(element);
        }
    }
}

cExpansionPieceSplitter::cExpansionPieceSplitter(long facesInMesh) :
 _lookup(facesInMesh)
{
}

cExpansionPieceSplitter::~cExpansionPieceSplitter()
{
    long i;
    for(i = 0; i != SizeL(_lookup); ++i)
    {
        long j;
        for(j = 0; j != SizeL(_lookup[i]); ++j)
        {
            assertD(_lookup[i][j] == 0);
        }
    }
}

void
cExpansionPieceSplitter::getElementsInFacesCrossed(const cCircuitElement* testElement, std::vector<cCircuitElement*>& result)
{
    assertD(result.empty());
    long i;
    for(i = 0; i != testElement->facesCrossed(); ++i)
    {
        long fIndex = testElement->getFaceCrossed(i).index();
        long j;
        for(j = 0; j != SizeL(_lookup[fIndex]); ++j)
        {
            if(std::find(result.begin(), result.end(), _lookup[fIndex][j]) == result.end())
            {
                result.push_back(_lookup[fIndex][j]);
            }
        }
    }
}

void
cExpansionPieceSplitter::splitExisting(cCircuitElement* toSplit, long faceIndex, const tLine& splitLine)
{
    toSplit->split(faceIndex, splitLine);
    assertF(toSplit->isValid());
    assertF(toSplit->getNext()->isValid());
    cCircuitElement* after = toSplit->getNext();
    long fIndex = after->getFaceCrossed(0).index();
    _lookup[fIndex].push_back(after);
    
  // update any entries after the new element leaves this face
    long i;
    for(i = 1; i < after->facesCrossed(); i++)
    {
        fIndex = after->getFaceCrossed(i).index();
        long j = lookupInFace(fIndex, toSplit);
        assertD(j != -1);
        _lookup[fIndex][j] = after;
    }
}

void
cExpansionPieceSplitter::addSequence(cCircuitElement* element)
{
    cCircuitElement* firstAdded = element;
    do
    {
        cCircuitElement* next = element->getNext();
//        assertF(element->isValid_SI());  //........... reinstate this and run against flatshead_dungeon.tok
        add(element);
        element = next;
    }
    while(element && element != firstAdded);
    assertF(isValid());
}

void
cExpansionPieceSplitter::stripElement(cCircuitElement* toStrip)
{
    long i;
    for(i = 0; i != toStrip->facesCrossed(); ++i)
    {
        long fIndex = toStrip->faceCrossed(i).index();
        long j = lookupInFace(fIndex, toStrip);
        assertD(j != -1);
        _lookup[fIndex][j] = 0;
    }
}

void
cExpansionPieceSplitter::stripChain(cCircuitElement* midElement, cCircuitElement*& first, cCircuitElement*& last)
{
    cCircuitElement* e = midElement;
    while(e->getPrev() && e->getPrev() != midElement)
    {
         e = e->getPrev();
    }
    if(!e->getPrev())
    {
        first = e;
        while(e->getNext())
        {
            stripElement(e);
            e = e->getNext();
        }
        stripElement(e);
        last = e;
        assertF(isValid());
        return;
    }
    first = e;
    last = 0;
    while(e != midElement)
    {
        stripElement(e);
        e = e->getNext();
    }
    stripElement(e);
    assertF(isValid());
}

bool
cExpansionPieceSplitter::isValid() const
{
    long i;
    for(i = 0; i != SizeL(_lookup); ++i)
    {
        long j;
        for(j = 0; j != SizeL(_lookup[i]); ++j)
        {
            cCircuitElement* element = _lookup[i][j];
            if(!element)
            {
                continue;
            }
            if(!element->isValid_SI())
            {
                return false;
            }
            long fIndex = element->faceCrossed(0).index();
            bool crossesThisFace = false;
            long k;
            for(k = 0; k != element->facesCrossed(); ++k)
            {
                long fIndex = element->faceCrossed(k).index();
                if(lookupInFace(fIndex, element) == -1)
                {
                    return false;
                }
                if(fIndex == i)
                {
                    crossesThisFace = true;
                    if(lookupInFace(fIndex, element) != j)
                    {
                        return false;
                    }
                }
            }
            if(!crossesThisFace)
            {
                return false;
            }
            cCircuitElement* nextEl = element->getNext();
            if(nextEl)
            {
                long fIndex = nextEl->faceCrossed(0).index();
                if(lookupInFace(fIndex, nextEl) == -1)
                {
                    return false;
                }
            }
            cCircuitElement* prevEl = element->getPrev();
            if(prevEl)
            {
                long fIndex = prevEl->faceCrossed(0).index();
                if(lookupInFace(fIndex, prevEl) == -1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
