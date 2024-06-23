//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_CHUNK_INCLUDED
#define CONTENT_CHUNK_INCLUDED

#include "i_pathengine.h"
#include "platform_common/Mutex.h"
#include "common/Containers/ReplacementList.h"
#include "common/Containers/VectorWithFreeList.h"
#include "common/STL/vector.h"

class iAPIObjectWrapper;
class iXMLOutputStream;
class iXMLInputStream;

class cContentChunk : public iContentChunk
{
  // immutable, except on construction and deletion
    cList_NoSize<cContentChunk*>::iterator _heldNode;
    const iFaceVertexMesh* _ground;
    const iAnchorsAndPinnedShapes* _anchorsAndShapes;
    bool _deleteBaseObjects;
    long _groundRange;
    long _anchorsRange;
    long _shapesRange;
    std::vector<long> _connectingEdgeInfo;

    void constructorCommon();

    cMutex _mutex; // covers all mutable member data
    bool _validated;
    long _validationMaximumScale, _validationTranslationRange;
    cVectorWithFreeList<iContentChunkInstance*> _instances;

    bool validateGroundAtRotation(long rotation) const;
    bool validateAnchorsAndShapesAtRotation(long rotation) const;
    void checkPreValidation(long scale, long translationX, long translationY) const;
    void doSave(iXMLOutputStream& xos) const;

public:

    iAPIObjectWrapper* _wrapper;

    // does not take ownership of the base objects passed in to this constructor
    // so these must remain valid over the lifetime of the constructed object, and then be deleted independantly
    cContentChunk(
            const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes,
            cList_NoSize<cContentChunk*>::iterator heldNode
            );
    cContentChunk(iXMLInputStream& is, cList_NoSize<cContentChunk*>::iterator heldNode);
    ~cContentChunk();

    void getConnectingEdgeInfo(long connectingEdgeIndex, long& face, long& firstVertexInFace) const;

    void releaseInstance(long index);

// iContentChunk

    void destroy();
    bool preValidate(long maximumScale, long translationRange);
    void save(iOutputStream* outputStream) const;
    iContentChunkInstance* instantiate(long rotation, long scale, long translationX, long translationY, float translationZ, long sectionID);
    iContentChunkInstance* instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ, long sectionID);
    void deleteAllInstances();
    bool hasGroundComponent() const;
    bool hasAnchorsAndShapesComponent() const;
    long numberOfConnectingEdges() const;
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cContentChunk* FromI(iContentChunk* ptr)
{
    return static_cast<cContentChunk*>(ptr);
}

#endif
