//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/PathEngine_Interface/interface/cContentChunk.h"
#include "LoadedFaceVertexMesh.h"
#include "SaveFaceVertexMesh.h"
#include "SaveAnchorsAndPinnedShapes.h"
#include "InstancedFaceVertexMesh.h"
#include "InstancedFaceVertexMesh_Float.h"
#include "InstancedAnchorsAndShapes.h"
#include "InstancedAnchorsAndShapes_Float.h"
#include "QuantisedRotation.h"
#include "interface/AnchorsAndPinnedShapes.h"
#include "interface/Export.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "libs/ParseXML/interface/TokenisedXMLOutputStream.h"
#include "libs/ParseXML/interface/XMLInputStreamToHandler.h"
#include "iAPIObjectWrapper.h"
#include "platform_common/CoordinateRanges.h"
#include "common/MutexLock.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"

static void
UpdateRange(long& range, long value)
{
    if(value > range)
    {
        range = value;
    }
    if(value < -range)
    {
        range = -value;
    }
}

void
cContentChunk::constructorCommon()
{
    _wrapper = 0;
    _groundRange = 0;
    _anchorsRange = 0;
    _shapesRange = 0;
    long i;
    if(_ground)
    {
        for(i = 0; i != _ground->vertices(); ++i)
        {
            UpdateRange(_groundRange, _ground->vertexX(i));
            UpdateRange(_groundRange, _ground->vertexY(i));
        }
    }
    if(_anchorsAndShapes)
    {
        for(i = 0; i != _anchorsAndShapes->numberOfAnchors(); ++i)
        {
            long x, y;
            _anchorsAndShapes->anchor_HorizontalPosition(i, x, y);
            UpdateRange(_anchorsRange, x);
            UpdateRange(_anchorsRange, y);
        }
        for(i = 0; i != _anchorsAndShapes->numberOfPinnedShapes(); ++i)
        {
            for(long j = 0; j != _anchorsAndShapes->pinnedShape_NumberOfVertices(i); ++j)
            {
                long x, y;
                _anchorsAndShapes->pinnedShape_Vertex(i, j, x, y);
                UpdateRange(_shapesRange, x);
                UpdateRange(_shapesRange, y);
            }
        }
    }
}

bool
cContentChunk::validateGroundAtRotation(long rotation) const
{
    if(_ground == 0)
    {
        return true;
    }
    cInstancedFaceVertexMesh rotated(_ground, rotation, 1, 0, 0, 0.f);
    const char* options[3];
    options[0] = "ignoreConnectionFailure";
    options[1] = "true";
    options[2] = 0;
    iMesh* mesh = nSingletons::pathEngine().buildMeshFromContent(&_ground, 1, options);
    bool result = (mesh != 0);
    delete mesh;
    return result;
}
bool
cContentChunk::validateAnchorsAndShapesAtRotation(long rotation) const
{
    if(_anchorsAndShapes == 0)
    {
        return true;
    }
    cInstancedAnchorsAndShapes rotated(_anchorsAndShapes, rotation, 1, 0, 0, 0.f);
    return PrevalidateAnchorsAndPinnedShapes(nSingletons::pathEngine(), rotated);
}

void
cContentChunk::checkPreValidation(long scale, long translationX, long translationY) const
{
    cMutexLock lock(_mutex);
    if(!_validated)
    {
        Error("Warning", "iContentChunk instanced using integer rotate and scale, but was not prevalidated.");
    }
    else if(scale > _validationMaximumScale)
    {
        Error("Warning", "iContentChunk instanced with scale exceeding the maximum scale specified at prevalidation.");
    }
    else if(translationX > _validationTranslationRange || translationX < -_validationTranslationRange || translationY > _validationTranslationRange || translationY < -_validationTranslationRange)
    {
        Error("Warning", "iContentChunk instanced with translation offset exceeding the range specified at prevalidation.");
    }
}

void
cContentChunk::doSave(iXMLOutputStream& xos) const
{
    cMutexLock lock(_mutex);
    xos.openElement("content");
    xos.addAttribute("majorRelease", MajorRelease());
    xos.addAttribute("minorRelease", MinorRelease());
    if(_validated)
    {
        xos.addAttribute("validationMaximumScale", _validationMaximumScale);
        xos.addAttribute("validationTranslationRange", _validationTranslationRange);
    }
    if(_ground)
    {
        xos.openElement("mesh");
        SaveFaceVertexMesh(*_ground, xos);
        xos.closeElement("mesh");
    }
    if(_anchorsAndShapes)
    {
        SaveAnchorsAndPinnedShapes(*_anchorsAndShapes, xos);
    }
    xos.closeElement("content");
}

cContentChunk::cContentChunk(
            const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes,
            cList_NoSize<cContentChunk*>::iterator heldNode
            )
{
    _heldNode = heldNode;
    _ground = ground;
    _anchorsAndShapes = anchorsAndShapes;
    _deleteBaseObjects = false;
    _validated = false;
    _validationMaximumScale = 0;
    _validationTranslationRange = 0;
    constructorCommon();
}
cContentChunk::cContentChunk(iXMLInputStream& is, cList_NoSize<cContentChunk*>::iterator heldNode)
{
    _heldNode = heldNode;
    _deleteBaseObjects = true;
    const char *const* attributes = is.openElement("content");
    _validated = false;
    _validationMaximumScale = 0;
    _validationTranslationRange = 0;
    if(GetValueForAttribute(attributes, "validationMaximumScale"))
    {
        _validated = true;
        _validationMaximumScale = ValueAsLong(attributes, "validationMaximumScale");
        _validationTranslationRange = ValueAsLong(attributes, "validationTranslationRange");
    }
    _ground = 0;
    _anchorsAndShapes = 0;
    const char* nextElement = is.peekNextElement();
    while(nextElement)
    {
        if(strcmp(nextElement, "mesh") == 0)
        {
            const char *const* attributes = is.openNextElement();
            if(_ground)
            {
                Error("NonFatal", "Content chunk being loaded has more than one mesh element."
                    " Looks like the more general 'content snapshot' format."
                    " This will be loaded, but meshes after the first will be ignored.");
            }
            else
            {
                cLoadedFaceVertexMesh* ground = new cLoadedFaceVertexMesh();
                _ground = ground;
                bool ok = XMLInputStreamToHandler(is, attributes, *ground);
                assertD(ok);
            }
            is.closeElement();
        }
        else
        if(strcmp(nextElement, "anchorsAndPinnedShapes") == 0)
        {
            const char *const* attributes = is.openNextElement();
            assertD(!_anchorsAndShapes);
            cAnchorsAndPinnedShapes* anchorsAndShapes = new cAnchorsAndPinnedShapes;
            _anchorsAndShapes = anchorsAndShapes;
            bool ok = XMLInputStreamToHandler(is, attributes, *anchorsAndShapes);
            assertD(ok);
            is.closeElement();
        }
        else
        {
            is.skipNextElement();
        }
        nextElement = is.peekNextElement();
    }
    is.closeElement();
    constructorCommon();
}
cContentChunk::~cContentChunk()
{
    assertD(!_heldNode.valid());
    deleteAllInstances();
    if(_deleteBaseObjects)
    {
        delete _ground;
        delete _anchorsAndShapes;
    }
}

void 
cContentChunk::destroy()
{
    INTERFACE_ASSERT(_heldNode.valid());
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    nSingletons::pathEngine().lockHeldContentChunks();
    _heldNode.eraseAndSetInvalid();
    nSingletons::pathEngine().unlockHeldContentChunks();
    delete this;
}
bool
cContentChunk::preValidate(long maximumScale, long translationRange)
{
    INTERFACE_ASSERT(maximumScale >= 1);
    INTERFACE_ASSERT(translationRange >= 0);

    if(_groundRange > MaximumValueSupportedByQuantisedRotation())
    {
        Error("NonFatal", "Content chunk ground geometry exceeds the range supported for integer rotation.");
        return false;
    }
    if(_anchorsRange > MaximumValueSupportedByQuantisedRotation())
    {
        Error("NonFatal", "Content chunk anchors exceed the range supported for integer rotation.");
        return false;
    }
    if(_shapesRange > MaximumValueSupportedByQuantisedRotation())
    {
        Error("NonFatal", "Content chunk shape vertices exceed the range supported for integer rotation.");
        return false;
    }
    double transformed;
    transformed = _groundRange;
    transformed *= maximumScale;
    transformed += translationRange;
    if(transformed > WORLD_RANGE)
    {
        Error("NonFatal", "Scaling or translation could put ground geometry outside world range.");
        return false;
    }
    transformed = _anchorsRange;
    transformed *= maximumScale;
    transformed += translationRange;
    if(transformed > WORLD_RANGE)
    {
        Error("NonFatal", "Scaling or translation could put anchors outside world range.");
        return false;
    }
    transformed = _shapesRange;
    transformed *= maximumScale;
    if(transformed > SHAPE_RANGE)
    {
        Error("NonFatal", "Scaling or translation could put shape vertices outside world range.");
        return false;
    }

    long rotation;
    for(rotation = 0; rotation != QuantisedRotationNumberOfRotations() / 4; ++rotation)
    {
        if(!validateGroundAtRotation(rotation))
        {
            return false;
        }
        if(!validateAnchorsAndShapesAtRotation(rotation))
        {
            return false;
        }
    }

    cMutexLock lock(_mutex);

    _validated = true;
    if(maximumScale > _validationMaximumScale)
    {
        _validationMaximumScale = maximumScale;
    }
    if(translationRange > _validationTranslationRange)
    {
        _validationTranslationRange = translationRange;
    }

    return true;
}
void 
cContentChunk::save(iOutputStream* outputStream) const
{
    cElementAndAttributeEnumerator enumerator;
    doSave(enumerator);
    enumerator.writeTo(*outputStream);
    cTokenisedXMLOutputStream xos(enumerator, *outputStream);
    doSave(xos);
}
const iFaceVertexMesh*
cContentChunk::instantiate(long rotation, long scale, long translationX, long translationY, float translationZ)
{
    checkPreValidation(scale, translationX, translationY);
    if(!_ground)
    {
        return 0;
    }
    iFaceVertexMesh* result = new cInstancedFaceVertexMesh(_ground, rotation, scale, translationX, translationY, translationZ);
    cMutexLock lock(_mutex);
    _instancedGround.push_back(result);
    return result;
}
const iFaceVertexMesh*
cContentChunk::instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ)
{
    if(!_ground)
    {
        return 0;
    }
    iFaceVertexMesh* result = new cInstancedFaceVertexMesh_Float(_ground, rotation, scale, translationX, translationY, translationZ);
    cMutexLock lock(_mutex);
    _instancedGround.push_back(result);
    return result;
}
void 
cContentChunk::deleteAllInstances()
{
    cMutexLock lock(_mutex);
    long i;
    for(i = 0; i != SizeL(_instancedGround); ++i)
    {
        delete _instancedGround[i];
    }
    _instancedGround.clear();
    for(i = 0; i != SizeL(_instancedAnchorsAndShapes); ++i)
    {
        delete _instancedAnchorsAndShapes[i];
    }
    _instancedAnchorsAndShapes.clear();
}

void
cContentChunk::addAnchorsAndShapes(iMesh* mesh, long rotation, long scale, long translationX, long translationY, float translationZ, const char* idPrefix)
{
    cMutexLock lock(_mutex);
    checkPreValidation(scale, translationX, translationY);
    if(!_anchorsAndShapes)
    {
        return;
    }
    cInstancedAnchorsAndShapes transformed(_anchorsAndShapes, rotation, scale, translationX, translationY, translationZ);
    AddAnchorsAndPinnedShapes(nSingletons::pathEngine(), mesh, transformed, idPrefix);    
}

void
cContentChunk::addAnchorsAndShapes_FloatRotateAndScale(iMesh* mesh, float rotation, float scale, long translationX, long translationY, float translationZ, const char* idPrefix)
{
    if(!_anchorsAndShapes)
    {
        return;
    }
    cInstancedAnchorsAndShapes_Float transformed(_anchorsAndShapes, rotation, scale, translationX, translationY, translationZ);
    AddAnchorsAndPinnedShapes(nSingletons::pathEngine(), mesh, transformed, idPrefix);    
}
bool
cContentChunk::hasGroundComponent() const
{
    return _ground != 0;
}
bool
cContentChunk::hasAnchorsAndShapesComponent() const
{
    return _anchorsAndShapes != 0;
}

const iAnchorsAndPinnedShapes*
cContentChunk::instantiateAnchorsAndShapes(long rotation, long scale, long translationX, long translationY, float translationZ)
{
    checkPreValidation(scale, translationX, translationY);
    if(!_anchorsAndShapes)
    {
        return 0;
    }
    iAnchorsAndPinnedShapes* result = new cInstancedAnchorsAndShapes(_anchorsAndShapes, rotation, scale, translationX, translationY, translationZ);
    cMutexLock lock(_mutex);
    _instancedAnchorsAndShapes.push_back(result);
    return result;
}
const iAnchorsAndPinnedShapes*
cContentChunk::instantiateAnchorsAndShapes_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ)
{
    if(!_anchorsAndShapes)
    {
        return 0;
    }
    iAnchorsAndPinnedShapes* result = new cInstancedAnchorsAndShapes_Float(_anchorsAndShapes, rotation, scale, translationX, translationY, translationZ);
    cMutexLock lock(_mutex);
    _instancedAnchorsAndShapes.push_back(result);
    return result;
}
