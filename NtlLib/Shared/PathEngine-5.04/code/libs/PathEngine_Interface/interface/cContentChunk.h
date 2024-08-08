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

    void constructorCommon();

    cMutex _mutex; // covers all mutable member data
    std::vector<iFaceVertexMesh*> _instancedGround;
    std::vector<iAnchorsAndPinnedShapes*> _instancedAnchorsAndShapes;
    bool _validated;
    long _validationMaximumScale, _validationTranslationRange;

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

// iContentChunk

    void destroy();
    bool preValidate(long maximumScale, long translationRange);
    void save(iOutputStream* outputStream) const;
    const iFaceVertexMesh* instantiate(long rotation, long scale, long translationX, long translationY, float translationZ);
    const iFaceVertexMesh* instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ);
    void deleteAllInstances();
    void addAnchorsAndShapes(iMesh* mesh, long rotation, long scale, long translationX, long translationY, float translationZ, const char* idPrefix);
    void addAnchorsAndShapes_FloatRotateAndScale(iMesh* mesh, float rotation, float scale, long translationX, long translationY, float translationZ, const char* idPrefix);
    bool hasGroundComponent() const;
    bool hasAnchorsAndShapesComponent() const;
    const iAnchorsAndPinnedShapes* instantiateAnchorsAndShapes(long rotation, long scale, long translationX, long translationY, float translationZ);
    const iAnchorsAndPinnedShapes* instantiateAnchorsAndShapes_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ);
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cContentChunk* FromI(iContentChunk* ptr)
{
    return static_cast<cContentChunk*>(ptr);
}

#endif
