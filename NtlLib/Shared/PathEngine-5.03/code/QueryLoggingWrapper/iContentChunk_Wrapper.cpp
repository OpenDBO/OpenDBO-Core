//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iContentChunk_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iContentChunk_Wrapper::destroy()
{
    unsigned long queryCount = gLogger.methodEntry(10, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(10, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iContentChunk_Wrapper::preValidate(long maximumScale, long translationRange)
{
    unsigned long queryCount = gLogger.methodEntry(10, 1, (void*)this);
    gLogger.log(maximumScale);
    gLogger.log(translationRange);
    gLogger.methodEntry_Completed();
    bool result = _target->preValidate(maximumScale, translationRange);
    gLogger.methodExit(10, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iContentChunk_Wrapper::save(iOutputStream* outputStream) const
{
    unsigned long queryCount = gLogger.methodEntry(10, 2, (void*)this);
    gLogger.methodEntry_Completed();
    _target->save(outputStream);
    gLogger.methodExit(10, 2, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

const iFaceVertexMesh*
iContentChunk_Wrapper::instantiate(long rotation, long scale, long translationX, long translationY, float translationZ)
{
    unsigned long queryCount = gLogger.methodEntry(10, 3, (void*)this);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.methodEntry_Completed();
    const iFaceVertexMesh* result = _target->instantiate(rotation, scale, translationX, translationY, translationZ);
    gLogger.methodExit(10, 3, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

const iFaceVertexMesh*
iContentChunk_Wrapper::instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ)
{
    unsigned long queryCount = gLogger.methodEntry(10, 4, (void*)this);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.methodEntry_Completed();
    const iFaceVertexMesh* result = _target->instantiate_FloatRotateAndScale(rotation, scale, translationX, translationY, translationZ);
    gLogger.methodExit(10, 4, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

void
iContentChunk_Wrapper::deleteAllInstances()
{
    unsigned long queryCount = gLogger.methodEntry(10, 5, (void*)this);
    gLogger.methodEntry_Completed();
    _target->deleteAllInstances();
    gLogger.methodExit(10, 5, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iContentChunk_Wrapper::addAnchorsAndShapes(iMesh* mesh, long rotation, long scale, long translationX, long translationY, float translationZ, const char* idPrefix)
{
    unsigned long queryCount = gLogger.methodEntry(10, 6, (void*)this);
    gLogger.log((void*)mesh);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.log(idPrefix);
    gLogger.methodEntry_Completed();
    _target->addAnchorsAndShapes(ExtractWrapped<iMesh_Wrapper>(mesh), rotation, scale, translationX, translationY, translationZ, idPrefix);
    gLogger.methodExit(10, 6, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iContentChunk_Wrapper::addAnchorsAndShapes_FloatRotateAndScale(iMesh* mesh, float rotation, float scale, long translationX, long translationY, float translationZ, const char* idPrefix)
{
    unsigned long queryCount = gLogger.methodEntry(10, 7, (void*)this);
    gLogger.log((void*)mesh);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.log(idPrefix);
    gLogger.methodEntry_Completed();
    _target->addAnchorsAndShapes_FloatRotateAndScale(ExtractWrapped<iMesh_Wrapper>(mesh), rotation, scale, translationX, translationY, translationZ, idPrefix);
    gLogger.methodExit(10, 7, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iContentChunk_Wrapper::hasGroundComponent() const
{
    unsigned long queryCount = gLogger.methodEntry(10, 8, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->hasGroundComponent();
    gLogger.methodExit(10, 8, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iContentChunk_Wrapper::hasAnchorsAndShapesComponent() const
{
    unsigned long queryCount = gLogger.methodEntry(10, 9, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->hasAnchorsAndShapesComponent();
    gLogger.methodExit(10, 9, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

const iAnchorsAndPinnedShapes*
iContentChunk_Wrapper::instantiateAnchorsAndShapes(long rotation, long scale, long translationX, long translationY, float translationZ)
{
    unsigned long queryCount = gLogger.methodEntry(10, 10, (void*)this);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.methodEntry_Completed();
    const iAnchorsAndPinnedShapes* result = _target->instantiateAnchorsAndShapes(rotation, scale, translationX, translationY, translationZ);
    gLogger.methodExit(10, 10, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

const iAnchorsAndPinnedShapes*
iContentChunk_Wrapper::instantiateAnchorsAndShapes_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ)
{
    unsigned long queryCount = gLogger.methodEntry(10, 11, (void*)this);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.methodEntry_Completed();
    const iAnchorsAndPinnedShapes* result = _target->instantiateAnchorsAndShapes_FloatRotateAndScale(rotation, scale, translationX, translationY, translationZ);
    gLogger.methodExit(10, 11, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}
void iContentChunk_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
