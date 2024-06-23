//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/cPathEngine.h"
#include "Pool.h"
#include "RandomSequence.h"
#include "interface/Singletons.h"
#include "interface/cPath.h"
#include "interface/cCollisionInfo.h"
#include "libs/PathEngine_Core/interface/CollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/PathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "libs/PathEngine_Core/interface/AStarHeapPool.h"
#include "libs/Content_Processing/interface/Export.h"
#include "libs/Content_Processing/interface/SaveSourceContent.h"
#include "libs/MeshPair/interface/LoadMesh3DAndMapping.h"
#include "libs/MeshPair/interface/GenerateMapping.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh3D/interface/ValidateMesh_Import.h"
#include "libs/Mesh3D/interface/PlaneEquation.h"
#include "libs/Geometry/interface/CollisionShape.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "libs/ParseXML/interface/DocumentHandler.h"
#include "libs/ParseXML/interface/TokenisedXMLInputStream.h"
#include "libs/ParseXML/interface/StandardXMLOutputStream.h"
#include "libs/ParseXML/interface/TokenisedXMLOutputStream.h"
#include "platform_common/CoordinateRanges.h"
#include "platform_common/Mutex.h"
#include "common/MemoryTracking_Interface.h"
#include "common/interface/Error.h"
#include "common/interface/Assert.h"
#include "common/interface/Attributes.h"
#include "common/AttributesBuilder.h"
#include "common/Containers/SwapOut.h"
#include "common/TemplateMesh/External.h"
#include "common/MutexLock.h"

class cPreprocessShapes
{
    std::vector<cCollisionShape> _shapes;
    std::vector<tPoint> _bounds;
    cMutex _mutex;

public:

    long size() const
    {
        cMutexLock lock(_mutex);
        return SizeL(_shapes);
    }
    const cCollisionShape& lookupCS(long index) const
    {
        cMutexLock lock(_mutex);
        return _shapes[index];
    }
    void lookupBounds(long index, tPoint& boundsMin, tPoint& boundsMax) const
    {
        cMutexLock lock(_mutex);
        boundsMin = _bounds[index * 2];
        boundsMax = _bounds[index * 2 + 1];
    }
//    long addOrReuse(const cCollisionShape& cs)
//    {
//        cMutexLock lock(_mutex);
//        long i;
//        for(i = 0; i < SizeL(_shapes); ++i)
//        {
//            if(_shapes[i] == cs)
//            {
//                return i;
//            }
//        }
//        _shapes.push_back(cs);
//        return i;
//    }
    long add(const cCollisionShape& cs)
    {
        cMutexLock lock(_mutex);
        long result = SizeL(_shapes);
        _shapes.push_back(cs);
        tPoint boundsMin, boundsMax;
        cs.getBounds(boundsMin, boundsMax);
        _bounds.push_back(boundsMin);
        _bounds.push_back(boundsMax);
        return result;
    }

    void clear()
    {
    // cannot be called asynchronously
        _shapes.clear();
        _bounds.clear();
    }
};

cPathEngine::cPathEngine()
{
    _preprocessShapes = new cPreprocessShapes;
    _heapPool = new cAStarHeapPool;
    _heldShapesMutex = new cMutex;
    _heldMeshesMutex = new cMutex;
    _heldContentChunksMutex = new cMutex;
    _randomSequenceMutex = new cMutex;
    _generalMutex = new cMutex;
    _pathReserveSize = 40;
    _randomSequence = new cRandomSequence;
}
void
cPathEngine::stage2Initialisation()
{
    _pathPool = new cPool<cPath>(40);
    _collisionInfoPool = new cPool<cCollisionInfo>(20);
}
cPathEngine::~cPathEngine()
{
    deleteAllObjects();
    delete _pathPool;
    delete _collisionInfoPool;
    delete _preprocessShapes;
    delete _heapPool;
    delete _randomSequence;
    delete _heldContentChunksMutex;
    delete _heldShapesMutex;
    delete _heldMeshesMutex;
    delete _randomSequenceMutex;
    delete _generalMutex;
}

// iPathEngine

const char *const*
cPathEngine::getVersionAttributes() const
{
    cMutexLock lock(*_generalMutex);
    static bool firsttime=true;
    static const char *const* attributes;
    if(firsttime)
    {
        static cAttributesBuilder builder;
        firsttime=false;
        builder.add("buildstring", VersionTimeString());
        builder.add("release", MajorRelease());
        builder.add("minor_release", MinorRelease());
        builder.add("interface_major_version",PATHENGINE_INTERFACE_MAJOR_VERSION);
        builder.add("interface_minor_version",PATHENGINE_INTERFACE_MINOR_VERSION);
        builder.add("collision_preprocess_major_version", cCollisionPreprocess::majorVersion());
        builder.add("collision_preprocess_minor_version", cCollisionPreprocess::minorVersion());
        builder.add("pathfind_preprocess_major_version", cPathfindPreprocess::majorVersion());
        builder.add("pathfind_preprocess_minor_version", cPathfindPreprocess::minorVersion());
        if(MemoryTrackingIsEnabled())
        {
            builder.add("memory_tracking_enabled", "true");
        }
#ifdef ASSERTIONS_ON
        builder.add("assertions_on", "true");
#endif
#ifdef RANGE_CHECKING
        builder.add("range_checking", "true");
#endif
#ifdef PATHENGINE_FULL_RELEASE
        builder.add("full_release", "true");
#endif
#ifdef _DEBUG
        builder.add("debug_build", "true");
#endif
        attributes=builder.get();
    }
    return attributes;
}

long
cPathEngine::getInterfaceMajorVersion() const
{
    return PATHENGINE_INTERFACE_MAJOR_VERSION;
}
long
cPathEngine::getInterfaceMinorVersion() const
{
    return PATHENGINE_INTERFACE_MINOR_VERSION;
}

void
cPathEngine::getReleaseNumbers(long& majorReleaseNumber, long& minorReleaseNumber, long& internalReleaseNumber) const
{
    majorReleaseNumber = MajorRelease();
    minorReleaseNumber = MinorRelease();
    internalReleaseNumber = InternalRelease();
}

void
cPathEngine::setErrorHandler(iErrorHandler *new_handler)
{
    cMutexLock lock(*_generalMutex);
    SetGlobalErrorHandler(new_handler);
}

iErrorHandler*
cPathEngine::getErrorHandler()
{
    cMutexLock lock(*_generalMutex);
    return GetGlobalErrorHandler();
}

void cPathEngine::setPathPoolParameters(long pathsPooled, long pointsPerPath)
{
    cMutexLock lock(*_generalMutex);
    INTERFACE_ASSERT(pathsPooled >= 0);
    INTERFACE_ASSERT(pointsPerPath >= 0);
    _pathReserveSize = pointsPerPath;
    _pathPool->resize(0);
    _pathPool->resize(pathsPooled);
}

bool
cPathEngine::shapeIsValid(long numberOfPoints, const long* coordinateData)
{
    if(numberOfPoints < 3)
    {
        return false;
    }
    {
        long i;
        for(i = 0; i < numberOfPoints * 2; i++)
        {
            if(coordinateData[i] > SHAPE_RANGE || coordinateData[i] < -SHAPE_RANGE)
            {
                return false;
            }
        }
        std::vector<tPoint> vertices(numberOfPoints);
        for(i = 0; i < numberOfPoints; i++)
        {
            vertices[i] = tPoint(coordinateData[i * 2], coordinateData[i * 2 + 1], SHAPE_RANGE);
        }
        if(!cCollisionShape::polygonIsConvex(numberOfPoints, &vertices.front()))
        {
            return false;
        }
        if(!cCollisionShape::pointInPolygon(numberOfPoints, &vertices.front(), tPoint(0,0,0)))
        {
            return false;
        }
    }    
    return true;
}

iShape*
cPathEngine::newShape(long numberOfPoints, const long* coordinateData)
{
    if(numberOfPoints < 3)
    {
        Error("NonFatal", "iPathEngine::newShape() - shape must have at least 3 vertices", 0);
        return 0;
    }
    {
        long i;
        for(i = 0; i < numberOfPoints * 2; i++)
        {
            if(coordinateData[i] > SHAPE_RANGE || coordinateData[i] < -SHAPE_RANGE)
            {
                Error("NonFatal", "iPathEngine::newShape() - vertex coordinate exceeds allowed range", 0);
                return 0;
            }
        }
        std::vector<tPoint> vertices(numberOfPoints);
        for(i = 0; i < numberOfPoints; i++)
        {
            vertices[i] = tPoint(coordinateData[i * 2], coordinateData[i * 2 + 1], SHAPE_RANGE);
        }
        if(!cCollisionShape::polygonIsConvex(numberOfPoints, &vertices.front()))
        {
            Error("NonFatal", "iPathEngine::newShape() - polygon is not convex", 0);
            return 0;
        }
        if(!cCollisionShape::pointInPolygon(numberOfPoints, &vertices.front(), tPoint(0,0,0)))
        {
            Error("NonFatal", "iPathEngine::newShape() - polygon does not contain the origin", 0);
            return 0;
        }
    }    
    cMutexLock lock(*_heldShapesMutex);
    _heldShapes.push_front(0);
    cShape* s = new cShape(numberOfPoints, coordinateData, _heldShapes.begin());
    _heldShapes.front() = s;
    return s;
}

iMesh*
cPathEngine::loadMeshFromBuffer(
                    const char* format,
                    const char* dataBuffer,
                    unsigned long bufferSize,
                    const char *const* options
                    )
{
    if( strcmp(format,"xml")!=0
        && strcmp(format,"tok")!=0)
    {
        std::vector<const char*> attributes;
        attributes.push_back("format");
        attributes.push_back(format);
        attributes.push_back(0);
        Error("NonFatal", "loadMeshFromBuffer_WithOptions(): unrecognised format, returning 0");
        return 0;
    }

    tMesh_3D baseMesh;
    tMesh mappingAsMesh2D;

    bool tokenised = (strcmp(format,"tok")==0);
	if(!tokenised)
	{
		INTERFACE_CHECK(strcmp(format,"xml") == 0, "iPathEngine::loadMeshFromBuffer() - unknown format");
	}

    {
        PROFILER_SCOPE("iPathEngine::loadMeshFromBuffer load base mesh");
        cMesh3DAndMappingLoader loader;
        cDocumentHandler documentHandler(loader, "mesh");
        ParseXML(format, dataBuffer, bufferSize, documentHandler);
        loader.swapOutResultAndClear(baseMesh, mappingAsMesh2D);
    }
    if(baseMesh.empty())
    {
        Error("NonFatal", "loadMeshFromBuffer_WithOptions(): "
            "No base mesh loaded, perhaps the buffer is in the legacy mesh format (i.e. generated before release 4_52).  "
            "The legacy mesh format is no longer supported (from release 4_81).  "
            "Returning zero.");
        return 0;
    }

    {
        if(!ValidateMesh_Import(baseMesh))
        {
            return 0;
        }
        CalculatePlaneEquations(baseMesh);
    }

    cMutexLock lock(*_heldMeshesMutex);
    _heldMeshes.push_front(0);
    cMesh* m = new cMesh(baseMesh, mappingAsMesh2D, format, dataBuffer, bufferSize, options, _heldMeshes.begin());
    _heldMeshes.front() = m;
    return m;
}

iMesh*
cPathEngine::buildMeshFromContent(
                       const iFaceVertexMesh* const* geometryObjectPointers,
                       long numberOfGeometryObjects,
                       const char *const* options)
{
    std::vector<const iFaceVertexMesh*> geometryObjectVector(numberOfGeometryObjects);
    long i;
    for(i = 0; i != numberOfGeometryObjects; ++i)
    {
        geometryObjectVector[i] = geometryObjectPointers[i];
    }
    cExportOptions exportOptions;
    exportOptions.filterFacesBySlope = false;
    const char* value;
    value = GetValueForAttribute(options, "filterFacesWithSlopeAbove");
    if(value)
    {
        exportOptions.filterFacesBySlope = true;
        exportOptions.maxSlope = ValueAsFloat(value);
    }
    bool optimise = false;
    float threshold = 0.f;
    value = GetValueForAttribute(options, "optimiseWithThreshold");
    if(value)
    {
        optimise = true;
        threshold = ValueAsFloat(value);
    }

    tMesh_3D baseGround;
    std::vector<long>* surfaceTypes;
    std::vector<long>* sectionIDs;
    std::vector<long>* userData;

    bool success = PerformContentProcessing(
            geometryObjectVector, exportOptions, optimise, threshold, threshold, options,
            baseGround, surfaceTypes, sectionIDs, userData
            );
    if(!success)
    {
        return 0;
    }
    return constructMeshFrom3D(baseGround, surfaceTypes, sectionIDs, userData, options);
}


void
cPathEngine::saveContentData(
        const iFaceVertexMesh* const* geometryObjectPointers,
        long numberOfGeometryObjects,
        const char* format,
        iOutputStream* os
        )
{
    saveContentData(geometryObjectPointers, numberOfGeometryObjects, 0, format, os);
}

unsigned long
cPathEngine::totalMemoryAllocated()
{
    return GetTotalMemoryAllocated();
}
unsigned long
cPathEngine::maximumMemoryAllocated()
{
    return GetMaximumMemoryAllocated();
}
void
cPathEngine::resetMaximumMemoryAllocated()
{
    ResetMaximumMemoryAllocated();
}

void
cPathEngine::setRandomSeed(unsigned long value)
{
    value &= 0x7fffffffl;
    cMutexLock lock(*_randomSequenceMutex);
    _randomSequence->setSeed(value);
}

bool
cPathEngine::collisionPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize)
{
    long majorVersion, minorVersion;
    cTokenisedXMLInputStream is(dataBuffer, bufferSize);
    {
        const char *const* attributes;
        attributes = is.openElement("collisionPreprocess");
        majorVersion = ValueAsLong(attributes, "majorVersion");
        minorVersion = ValueAsLong(attributes, "minorVersion");
    }
    if(majorVersion != cCollisionPreprocess::majorVersion() || minorVersion < cCollisionPreprocess::minorVersion())
    {
        return false;
    }
    return true;
}
bool
cPathEngine::pathfindPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize)
{
    long majorVersion, minorVersion;
    cTokenisedXMLInputStream is(dataBuffer, bufferSize);
    {
        const char *const* attributes;
        attributes = is.openElement("pathfindPreprocess");
        majorVersion = ValueAsLong(attributes, "majorVersion");
        minorVersion = ValueAsLong(attributes, "minorVersion");
    }
    if(majorVersion != cPathfindPreprocess::majorVersion() || minorVersion < cPathfindPreprocess::minorVersion())
    {
        return false;
    }
    return true;
}

void
cPathEngine::deleteAllObjects()
{
    cMutexLock lock(*_generalMutex);
// trigger calls to destroy on owned objects
    while(!_heldContentChunks.empty())
    {
        _heldContentChunks.front()->destroy();
    }
    while(!_heldMeshes.empty())
    {
        _heldMeshes.front()->destroy();
    }
    while(!_heldShapes.empty())
    {
        _heldShapes.front()->destroy();
    }
// internal
    _preprocessShapes->clear();
}

iContentChunk*
cPathEngine::newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes)
{
    cMutexLock lock(*_heldContentChunksMutex);
    _heldContentChunks.push_front(0);
    cContentChunk* result = new cContentChunk(ground, anchorsAndShapes, attributes, _heldContentChunks.begin());
    _heldContentChunks.front() = result;
    return result;
}
iContentChunk*
cPathEngine::loadContentChunk(const char* dataBuffer, unsigned long bufferSize)
{
    cTokenisedXMLInputStream is(dataBuffer, bufferSize);
    cMutexLock lock(*_heldContentChunksMutex);
    _heldContentChunks.push_front(0);
    cContentChunk* result = new cContentChunk(is, _heldContentChunks.begin());
    _heldContentChunks.front() = result;
    return result;
}

//=========================

void
cPathEngine::saveContentData(
        const iFaceVertexMesh* const* geometryObjectPointers,
        long numberOfGeometryObjects,
        const iAnchorsAndPinnedShapes* anchorsAndShapes,
        const char* format,
        iOutputStream* os
        )
{
    std::vector<const iFaceVertexMesh*> geometryObjectVector(numberOfGeometryObjects);
    long i;
    for(i = 0; i != numberOfGeometryObjects; ++i)
    {
        geometryObjectVector[i] = geometryObjectPointers[i];
    }
    if(strcmp(format,"xml") == 0)
    {
        cStandardXMLOutputStream xos(*os);
        SaveSourceContent(geometryObjectVector, anchorsAndShapes, xos);
    }
    else
    {
        INTERFACE_ASSERT(strcmp(format, "tok") == 0);
        cElementAndAttributeEnumerator enumerator;
        SaveSourceContent(geometryObjectVector, anchorsAndShapes, enumerator);
        enumerator.writeTo(*os);
        cTokenisedXMLOutputStream xos(enumerator, *os);
        SaveSourceContent(geometryObjectVector, anchorsAndShapes, xos);
    }
}

iMesh*
cPathEngine::constructMeshFrom3D(
        tMesh_3D& baseMesh,
        std::vector<long>* surfaceTypes,
        std::vector<long>* sectionIDs,
        std::vector<long>* userData,
        const char *const* options
        )        
{
    cMutexLock lock(*_heldMeshesMutex);
    _heldMeshes.push_front(0);
    cMesh* m = new cMesh(baseMesh, surfaceTypes, sectionIDs, userData, options, _heldMeshes.begin());
    _heldMeshes.front() = m;
    return m;
}

long
cPathEngine::allocateIndexForPreprocessShape(const cCollisionShape& shape)
{
//    return _preprocessShapes->addOrReuse(shape);
    return _preprocessShapes->add(shape);
}
long
cPathEngine::numberOfPreprocessShapes() const
{
    return _preprocessShapes->size();
}
const cCollisionShape&
cPathEngine::refPreprocessShape(long index) const
{
    return _preprocessShapes->lookupCS(index);
}
void
cPathEngine::getPreprocessShapeBounds(long index, tPoint& boundsMin, tPoint& boundsMax) const
{
    _preprocessShapes->lookupBounds(index, boundsMin, boundsMax);
}

void
cPathEngine::lockHeldShapes() const
{
    _heldShapesMutex->lock();
}
void
cPathEngine::unlockHeldShapes() const
{
    _heldShapesMutex->unlock();
}
void
cPathEngine::lockHeldMeshes() const
{
    _heldMeshesMutex->lock();
}
void
cPathEngine::unlockHeldMeshes() const
{
    _heldMeshesMutex->unlock();
}
void
cPathEngine::lockHeldContentChunks() const
{
    _heldContentChunksMutex->lock();
}
void
cPathEngine::unlockHeldContentChunks() const
{
    _heldContentChunksMutex->unlock();
}

long
cPathEngine::getRandomSeed() const
{
    cMutexLock lock(*_randomSequenceMutex);
    return _randomSequence->getSeed();
}
void
cPathEngine::setRandomSeed_Long(long value)
{
    cMutexLock lock(*_randomSequenceMutex);
    _randomSequence->setSeed(value);
}

long
cPathEngine::random15()
{
    cMutexLock lock(*_randomSequenceMutex);
    return _randomSequence->random15();
}

long
cPathEngine::random31()
{
    cMutexLock lock(*_randomSequenceMutex);
    return _randomSequence->random31();
}

iRandomSequence&
cPathEngine::lockRandomSequence()
{
    _randomSequenceMutex->lock();
    return *_randomSequence;
}
void
cPathEngine::releaseRandomSequence()
{
    _randomSequenceMutex->unlock();
}

long
cPathEngine::getPathReserveSize()
{
    cMutexLock lock(*_generalMutex);
    return _pathReserveSize;
}

iShape*
cPathEngine::newShape_Internal(const cCollisionShape& cs)
{
    cMutexLock lock(*_heldShapesMutex);
    _heldShapes.push_front(0);
    cShape* s = new cShape(cs, _heldShapes.begin());
    _heldShapes.front() = s;
    return s;
}
