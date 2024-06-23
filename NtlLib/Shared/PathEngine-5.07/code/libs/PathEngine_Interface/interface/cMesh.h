//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_MESH_INCLUDED
#define C_MESH_INCLUDED

#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/PathEngine_Core/interface/CollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/PathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/OffMeshConnections.h"
#include "libs/PathEngine_Core/interface/ObstacleInfo.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/interface/Assert.h"
#include "common/Containers/ReplacementList.h"
#include "common/Containers/VectorWithFreeList.h"
#include "common/HandleTarget.h"
#include "common/Handle.h"
#include "common/Containers/SwapOutMirror.h"
#include "common/Containers/LazyHandleVector.h"
#include "platform_common/Mutex.h"
#include "i_pathengine.h"
#include "common/STL/vector.h"
#include "common/STL/string.h"

class cCollisionContext;
class cObstacleSet;
class cAgent;
class cPath;
class iGraphicsInterface;
class cPreprocessShape;
class cCollisionInfo;
class i3DObstacles;
class iXMLOutputStream;
template <class T> class cMeshWithTrackedTransform;
class cQueryContext;
class cMesh3DPartitioning;
class cMutex;
class cParsedXMLElement;
class cSurfaceTypeCosts;
class cTranslationToOverlapped;
class cOffMeshConnections;
class iAPIObjectWrapper;
class cCornerProvider;
class cRepresentedRegionOverlaps;
class iUserData;

class cMesh : public iMesh, public bHandleTarget
{
  // immutable data, not changed apart from in construction and deletion

    cList_NoSize<cMesh*>::iterator _heldNode;
    cMesh3DPartitioning* _partitioning;
    mutable tMesh_3D* _mesh3D; // support const meshes
    long _mesh3DInternalFaces;
    mutable tMesh* _mesh2D; // support const meshes
    long _mesh2DInternalFaces;
    cTranslationToOverlapped* _translation;
    bool _hasInternalVertices;
    cMeshWithTrackedTransform<tMesh>* _mesh2D_NoInternalVertices;

    // information channels per face of 3d mesh
    std::vector<long>* _surfaceTypes;
    long _topSurfaceType;
    std::vector<long>* _sectionIDs;
    std::vector<long>* _userData;

    // only set immediately after construction, and in same thread
    const cRepresentedRegionOverlaps* _representedRegionOverlaps;

  // end of immutable data

    cMutex _meshMutex;

   // stuff protected by _meshMutex

    // the following cannot be changed after any preprocess has been generated
    cOffMeshConnections* _offMeshConnections;
    std::vector<cCollisionShape> _baseObstacleShapes;
    std::vector<cObstacleInfo> _baseObstacles;
    std::vector<cSurfaceTypeCosts*> _baseSurfaceTypeCosts;

    bool _pathfindPreprocessHasBeenGenerated;
    // the following are built the first time pathfind preprocess is generated
    cCornerProvider* _mesh3DCorners;
    std::vector<iMovementCostCalculator*> _mesh3DCostCalculators;

    bool _allowCollisionWithoutPreprocess;
    cLocalisedConvexShapeSet* _unexpandedBaseObstacles;

    mutable cList_NoSize<cPath*> _heldPaths;
    mutable cList_NoSize<cCollisionInfo*> _heldCollisionInfos;

    cLazyHandleVector<cCollisionPreprocess> _collisionPreprocess;
    cLazyHandleVector<cPathfindPreprocess> _pathfindPreprocess;

    bool _preprocessGenerated;
    bool _preprocessGenerationCompleted;

    std::vector<std::string> _anchorIDs;
    std::vector<cPosition> _anchorPositions;
    std::vector<long> _anchorOrientations;

    std::vector<std::string> _namedObstacleIDs;
    std::vector<cCollisionShape> _namedObstacleShapes;
    std::vector<cObstacleInfo> _namedObstacles;

    mutable std::vector<long> _partitioningResultsBuffer;

    iUserData* _testBedUserData;

   // end of stuff protected by _meshMutex

    cMutex _heldContextsMutex;
    mutable cVectorWithFreeList<cCollisionContext*> _heldContexts;

    cMutex _agentsAndObstacleSetsMutex; // high level mutex for changes to containment of agents by obstacle sets in general
    cMutex _heldObstacleSetsMutex; // low level mutex just for access to the vector
    mutable cVectorWithFreeList<cHandle<cObstacleSet> > _heldObstacleSets;
    cMutex _heldAgentsMutex; // low level mutex just for access to the vector
    mutable cVectorWithFreeList<cAgent*> _heldAgents;

    cMutex _queryContextsMutex;
    long _maxElementCountRequirement;
    mutable std::vector<cQueryContext*> _queryContexts;
    void addElementCountRequirement(long elementCount);

    std::vector<iTerrainCallBack*> _terrainCallBacks;
    std::vector<tPoint> _sectionBounds;

    mutable long _checkSum;
    mutable bool _checkSumValid;
    void makeCheckSumValid() const;

    void buildAnythingRequiredForPathfindPreprocess();

    void releasePreprocessFor(long i);
    void generateCollisionPreprocessFor(cShape* shape);
    void generatePathfindPreprocessFor(cShape* shape);
    void doSaveGround(iXMLOutputStream& os, bool include_mapping) const;
    void doSavePathfindPreprocess(iXMLOutputStream& os, cShape* shape) const;

//    void swapIn3DMeshAndIndex(tMesh_3D& mesh, cMeshIndex<tMesh_3D>& index);
    void swapInOrBuild2DMeshAndIndex(tMesh& mesh, const char *const* options);
    void constructorCommon(const char *const* options);
    void shrinkStuffToFit();

  // copying and assignment not permitted
    cMesh(const cMesh&);
    const cMesh& operator=(const cMesh&);

public:

    iAPIObjectWrapper* _wrapper;

    // baseMesh is swapped in by both constructors
    cMesh(tMesh_3D& baseMesh, tMesh& mappingAsMesh2D,
          const char* format, const char* dataBuffer, unsigned long bufferSize,
          const char *const* options,
          cList_NoSize<cMesh*>::iterator heldNode);
    cMesh(tMesh_3D& baseMesh,
            std::vector<long>* surfaceTypes,
            std::vector<long>* sectionIDs,
            std::vector<long>* userData,
            const char *const* options,
            cList_NoSize<cMesh*>::iterator heldNode);

    ~cMesh();

// iMesh

    void destroy();
    long getNumberOf3DFaces() const;
    long get3DFaceAtPosition(const cPosition& position) const;
    void get3DFaceVertex(long faceIndex, long vertexIndexInFace, long& x, long& y, float& z) const;
    long get3DFaceConnection(long faceIndex, long edgeIndexInFace) const;
    void get3DFaceNormal(long faceIndex, float& resultX, float& resultY, float& resultZ) const;
    long get3DFaceAttribute(long faceIndex, long attributeIndex) const;
    long getNumberOfSections() const;
    long getSurfaceType(const cPosition& position) const;
    long getSectionID(const cPosition& position) const;
    void storeAnchor(const char* id, const cPosition& position, long orientation);
    cPosition retrieveAnchor(const char* id, long& orientation) const;
    long getNumberOfAnchors() const;
    void retrieveAnchorByIndex(long index, cPosition& position, long& orientation, const char*& id) const;
    void storeNamedObstacle(const char* id, iAgent* agent);
    iAgent* retrieveNamedObstacle(const char* id) const;
    long getNumberOfNamedObstacles() const;
    void retrieveNamedObstacleByIndex(long index, iAgent*& agent, const char*& id) const;
    cPosition positionFor3DPoint(const long* point) const;
    cPosition positionFor3DPointF(const float* point) const;
    cPosition positionNear3DPoint(const long* point, long horizontalRange, long verticalRange) const;
    cPosition positionInSection(long sectionID, long x, long y) const;
    cPosition positionInSectionFor3DPoint(long sectionID, const long* point) const;
    cPosition positionInSectionFor3DPointF(long sectionID, const float* point) const;
    cPosition positionInSectionNear3DPoint(long sectionID, const long* point, long horizontalRange, long verticalRange) const;
    cPosition positionFor3DPoint_ExcludeTerrain(const long* point, long numberOfTerrainLayers) const;
    cPosition generateRandomPosition() const;
    cPosition generateRandomPositionLocally(const cPosition& centre, long range) const;
    cPosition generateRandomPositionInSection(long sectionID) const;
    long heightAtPosition(const cPosition& position) const;
    float heightAtPositionF(const cPosition& position) const;
    float heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const;
    long getCellForEndOfLine(const cPosition& start, long endX, long endY) const;
    bool positionIsValid(const cPosition& p) const;
    void burnContextIntoMesh(iCollisionContext* context);
    void saveGround(const char* format, bool includeMapping, iOutputStream* outputStream) const;
    void setTerrainCallBack(long terrainLayer, iTerrainCallBack* callBack);
    long addEndPoint(const cPosition& position);
    long addOffMeshConnection(long fromEndPoint, long toEndPoint, long penalty);
    long getNumberOfEndPoints() const;
    cPosition getEndPoint(long index) const;
    long getNumberOfOffMeshConnections() const;
    void getOffMeshConnectionInfo(long index, long& fromEndPoint, long& toEndPoint, long& penalty) const;
    void clearOffMeshConnectionsAndEndPoints();
    void generateCollisionPreprocessFor(iShape* shape, const char *const* attributes);
    void generatePathfindPreprocessFor(iShape* shape, const char *const* attributes);
    void releaseAllPreprocessFor(iShape* shape);
    void releasePathfindPreprocessFor(iShape* shape);
    void preprocessGenerationCompleted();
    bool shapeCanCollide(const iShape* shape) const;
    bool shapeCanPathfind(const iShape* shape) const;
    const char *const* getCollisionPreprocessAttributes(iShape* shape) const;
    const char *const* getPathfindPreprocessAttributes(iShape* shape) const;
    void saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const;
    void savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const;
    void loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, unsigned long bufferSize);
    void loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, unsigned long bufferSize);
    iAgent* placeAgent(iShape* shape, const cPosition& position) const;
    iCollisionContext* newContext() const;
    iObstacleSet* newObstacleSet() const;
    bool testPointCollision(iShape* shape, iCollisionContext* context, const cPosition& position) const;
    bool testLineCollision(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& end) const;
    bool testLineCollision_XY(iShape* shape, iCollisionContext* context, const cPosition& start, long x, long y, long& cell) const;
    iCollisionInfo* firstCollision(iShape* shape, iCollisionContext* context, const cPosition& start, long x, long y, long& cell) const;
    cPosition findClosestUnobstructedPosition(iShape* shape, iCollisionContext* context, const cPosition& position, long maximumDistance) const;
    void getAllAgentsOverlapped(iShape* shape, iCollisionContext* context, const cPosition& position, iAgent** resultsBuffer, long& numberOverlapped) const;
    iPath* findShortestPath(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal) const;
    iPath* findShortestPath_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack* queryCallBack) const;
    iPath* findPathAway(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, long distanceAway) const;
    iPath* findPathAway_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, long distanceAway, iQueryCallBack* queryCallBack) const;
    iPath* generateCurvedPath(iShape* shape, iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;
    iPath* constructPath(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength) const;
    iPath* constructPath_Reversed(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength) const;
    void savePath(iPath* path, iOutputStream* os) const;
    iPath* loadPath(const char* dataBuffer, unsigned long bufferSize) const;
    void renderLineOnGround(const cPosition& start, long endX, long endY, long originX, long originY, iRender3DLinesCallBack& callBack) const;
    iObstacleSet* newObstacleSet_WithAttributes(const char *const* attributes) const;
    cPosition positionInSectionInShape(long sectionID, long shapeOriginX, long shapeOriginY, long numberOfPoints, const long* coordinateData) const;
    iPath* generateCurvedPath_WithEndVector(iShape* shape, iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long endVectorX, long endVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;
    void operator delete(void* p) {::operator delete(p);}

//======================
    
// queries, but not currently provided through iMesh

//    iPath* findShortestPath_Incremental(
//                        iShape* shape, 
//                        iCollisionContext* context, 
//                        const cPosition& start, const cPosition& end);

    cPosition findClosestUnobstructedPoint_Reference(
                iShape* shape, 
                iCollisionContext* context,
                const cPosition& position,
                long maximumdistance);

    // the existing path must not be a failed path,
    // the existing path and the line from current end point to new end point must both be unobstructed for the shape and context given
    void extendPathEnd(cPath& path, cShape& shape, cCollisionContext* context, const cInternalPosition& newEndPosition);
    void extendPathStart(cPath& path, cShape& shape, cCollisionContext* context, const cInternalPosition& newStartPosition);

// internal

    iCollisionInfo* firstCollision_Internal(
            iShape* shape, 
            iCollisionContext* context, 
            tFace startF,
            tLine testLine, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
            tFace& endF
            ) const;

    bool testPointCollision_IgnoreOneAgent(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& position,
                const cAgent* agentToIgnore
                ) const;
    bool testLineCollision_IgnoreOneAgent(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& start, const cPosition& end,
                const cAgent* agentToIgnore
                ) const;
    bool testLineCollision_XY_IgnoreOneAgent(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& start, long x, long y, long& cell,
                const cAgent* agentToIgnore
                ) const;

    void lockAgentsAndObstacleSets() const;
    void unlockAgentsAndObstacleSets() const;
    const cMutex& refAgentsAndObstacleSetsMutex() const
    {
        return _agentsAndObstacleSetsMutex;
    }

    const std::vector<long>* getSurfaceTypesVector()
    {
        return _surfaceTypes;
    }
    long getTopSurfaceType() const
    {
        return _topSurfaceType;
    }
    const std::vector<long>* getUserDataVector() const
    {
        return _userData;
    }
    const std::vector<long>* getSectionIDsVector() const
    {
        return _sectionIDs;
    }

    tMesh& ref2D()
    {
        return* _mesh2D;
    }
    //..... sort out const meshes
    tMesh& ref2D() const
    {
        return const_cast<tMesh&>(*_mesh2D);
    }
    tMesh_3D& ref3D()
    {
        return* _mesh3D;
    }
    const tMesh_3D& ref3D() const
    {
        return* _mesh3D;
    }

    const cTranslationToOverlapped& refTranslation() const
    {
        return* _translation;
    }

    // in general query context should only be entered and exited from c_Mesh
    // but c_CollisionContext also needs to do this is some cases
    // and it can also be used in tests
    cQueryContext& enterQuery() const;
    void exitQuery(cQueryContext& qc) const;

    void notifyContextDestroyed(long index);
    void notifyAgentDestroyed(long index);
    void notifyObstacleSetDestroyed(long index);

//    cCollisionContext& refContext(long index)
//    {
//        assertD(index >= 0 && index < _heldContexts.size() && _heldContexts.indexIsInUse(index));
//        return *_heldContexts[index];
//    }
    cObstacleSet& refObstacleSet(long index)
    {
        assertD(index >= 0 && index < _heldObstacleSets.size() && _heldObstacleSets.indexIsInUse(index));
        return *_heldObstacleSets[index];
    }

    bool shapeCanCollide(const cShape* shape) const;
    bool shapeCanPathfind(const cShape* shape) const;
    bool shapeCanCollide(long index) const;
    bool shapeCanPathfind(long index) const;

    void convertPosition(const cPosition& p, cInternalPosition& result) const;
    void convertPosition(const cInternalPosition& p, cPosition& result) const;

    tEdge lookupEdge(long index) const;

    const tMeshCutSequence& refEdgeExpansion(cShape* shape) const;

    long size() const;

    cHandle<cCollisionPreprocess> getCollisionPreprocessFor(long index) const;
    cHandle<cPathfindPreprocess> getPathfindPreprocessFor(long index) const;

    const cMeshWithTrackedTransform<tMesh>* getSilhouetteRegionPartitioned(const cShape& shape) const;
    const cMeshWithTrackedTransform<tMesh>& refCircuitCombinationPartitioned(const cShape& shape);
    const cMeshWithTrackedTransform<tMesh>& refSmallConvexPartitioned(const cShape& shape) const;
    const std::vector<std::vector<long> >& refSilhouetteLookup(const cShape& shape) const;
    cPosition positionForBaseCorner(cQueryContext& qc, const cShape& shape, long stateIndex) const;

    void registerPath(cPath* path) const;
    void detachPath(cList_NoSize<cPath*>::iterator heldNode) const;
    void detachCollisionInfo(cList_NoSize<cCollisionInfo*>::iterator heldNode) const;

    bool testLineCollision_Internal(cQueryContext& qc, long shapeIndex, 
                                    cCollisionContext* context, 
                                    const cInternalPosition& start, const cInternalPosition& end) const;
    bool testLineCollision_Internal_ExceptSmallConvex(cQueryContext& qc, long shapeIndex, 
                                    cCollisionContext* context, 
                                    const cInternalPosition& start, const cInternalPosition& end) const;
    
    bool testLineCollision_XY_Internal(
                    cQueryContext& qc,
                    long shapeIndex,
                    cCollisionContext* context, 
                    const cInternalPosition& start, const tPoint& endP,
                    tFace& endF) const;
    bool testLineCollision_XY_Internal_ExceptSmallConvex(
                    cQueryContext& qc,
                    long shapeIndex,
                    cCollisionContext* context, 
                    const cInternalPosition& start, const tPoint& endP,
                    tFace& endF) const;

    bool
    testForSnagsToSegmentEnd(
            cQueryContext& qc,
            long shapeIndex,
            cCollisionContext* context,
            const tLine& segmentLine,
            const cInternalPosition& approximation
            ) const;
    bool
    testForSnagsToSegmentEnd(
            iShape* shape,
            iCollisionContext* context,
            const tLine& segmentLine,
            const cPosition& approximation
            ) const;

    void findShortestPath_Internal(
        cQueryContext& qc,
        cShape& shape,
        cCollisionContext* context,
        const cInternalPosition& start,
        const cInternalPosition& goal,
        std::vector<cInternalPosition>& resultPath,
        std::vector<long>*& resultIndices,
        iQueryCallBack* queryCallBack
        ) const;

    cCollisionInfo* newCollisionInfo(iAgent* collidingAgent) const;

    float heightAtPointIn3DFace(long f3DIndex, const tPoint& p) const;
    float heightAtPointIn3DFace(long f3DIndex, float x, float y) const;

    const std::vector<cObstacleInfo>&
    refBaseObstacles() const
    {
        return _baseObstacles;
    }
    const std::vector<cCollisionShape>&
    refBaseObstacleShapes() const
    {
        return _baseObstacleShapes;
    }
    cHandle<tMeshCircuits> generateOldStyleBaseCircuitsForBaseObstacles() const;

    const std::vector<cObstacleInfo>&
    refNamedObstacles() const
    {
        return _namedObstacles;
    }
    const std::vector<cCollisionShape>&
    refNamedObstacleShapes() const
    {
        return _namedObstacleShapes;
    }
    const std::vector<std::string>&
    refNamedObstacleIDs() const
    {
        return _namedObstacleIDs;
    }

    const std::vector<std::string>&
    refAnchorIDs() const
    {
        return _anchorIDs;
    }
    const std::vector<cPosition>&
    refAnchorPositions() const
    {
        return _anchorPositions;
    }
    const std::vector<long>&
    refAnchorOrientations() const
    {
        return _anchorOrientations;
    }

    const cCollisionPreprocess& refCollisionPreprocess(long index) const;
    const cPathfindPreprocess& refPathfindPreprocess(long index) const;

    const cCornerProvider* getMesh3DCorners() const
    {
        return _mesh3DCorners;
    }
    const std::vector<iMovementCostCalculator*>& refMesh3DCostCalculators() const
    {
        return _mesh3DCostCalculators;
    }

    tFace getFaceForEndOfLine(const cInternalPosition& start, const tPoint& end) const;
    tFace_3D getOverlappingFaceOn3DJustAfterStartOfEdge(tEdge e) const;

    bool hasInternalVertices() const
    {
        return _hasInternalVertices;
    }

    const cOffMeshConnections& refOffMeshConnections() const;
    unsigned long lookupPenaltyForOffMeshConnection(long index) const;

    cPosition translateFrom3D(long f3D, long x, long y) const;

    long getNumberOfInternalFacesIn2D() const;

    iAgent* placeAgent(const cCollisionShape& cs, const cPosition& position) const;

    void getRange(tPoint& bottomLeft, tPoint& topRight) const;

    // should only be called straight after construction, and in same thread
    void
    attachRepresentedRegionOverlaps(const cRepresentedRegionOverlaps* overlaps)
    {
        assertD(_representedRegionOverlaps == 0);
        _representedRegionOverlaps = overlaps;
    }

    const cRepresentedRegionOverlaps*
    getRepresentedRegionOverlaps() const
    {
        return _representedRegionOverlaps;
    }

    // should only be called straight after construction, and in same thread
    void
    attachBaseObstacles(
            std::vector<cCollisionShape>& baseObstacleShapes,
            std::vector<cObstacleInfo>& baseObstacleInfos
            );

    // should only be called straight after construction, and in same thread
    void
    attachNamedObstacles(
            std::vector<std::string>& namedObstacleIDs,
            std::vector<cCollisionShape>& namedObstacleShapes,
            std::vector<cObstacleInfo>& namedObstacleInfos
            );

    // should only be called straight after construction, and in same thread
    void
    attachAnchors(
            std::vector<std::string> anchorIDs,
            std::vector<cPosition> anchorPositions,
            std::vector<long> anchorOrientations
            );

    bool hasOffMeshConnections() const;
    bool hasBaseSurfaceTypeCosts() const;

    iUserData* getTestBedUserData()
    {
        return _testBedUserData;
    }
    void setTestBedUserData(iUserData* value)
    {
        _testBedUserData = value;
    }

    long lookup3DFaceInSectionWithCoords(long sectionID, long* horizontalCoords, float* verticalCoords) const;
};

inline cMesh* FromI(iMesh* ptr)
{
    return static_cast<cMesh*>(ptr);
}
inline const cMesh* FromI(const iMesh* ptr)
{
    return static_cast<const cMesh*>(ptr);
}

#endif

