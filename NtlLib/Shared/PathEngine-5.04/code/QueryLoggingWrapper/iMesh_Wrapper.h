//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IMESH_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IMESH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"
#include "common/STL/list.h"

class iMesh_Wrapper : public iMesh, public iAPIObjectWrapper
{
public:

    typedef iMesh tWrapped;

	iMesh* _target;
    std::list<iAPIObjectWrapper*> _classCallBacks;

    static long interfaceIndex()
    {
        return 2;
    }

    void
    destroy();

    long
    getNumberOf3DFaces() const;

    long
    get3DFaceAtPosition(const cPosition& position) const;

    void
    get3DFaceVertex(long faceIndex, long vertexIndexInFace, long& x, long& y, float& z) const;

    long
    get3DFaceConnection(long faceIndex, long edgeIndexInFace) const;

    void
    get3DFaceNormal(long faceIndex, float& resultX, float& resultY, float& resultZ) const;

    long
    get3DFaceAttribute(long faceIndex, long attributeIndex) const;

    long
    getNumberOfSections() const;

    long
    getSurfaceType(const cPosition& position) const;

    long
    getSectionID(const cPosition& position) const;

    void
    storeAnchor(const char* id, const cPosition& position, long orientation);

    cPosition
    retrieveAnchor(const char* id, long& orientation) const;

    long
    getNumberOfAnchors() const;

    void
    retrieveAnchorByIndex(long index, cPosition& position, long& orientation, const char*& id) const;

    void
    storeNamedObstacle(const char* id, iAgent* agent);

    iAgent*
    retrieveNamedObstacle(const char* id) const;

    long
    getNumberOfNamedObstacles() const;

    void
    retrieveNamedObstacleByIndex(long index, iAgent*& agent, const char*& id) const;

    cPosition
    positionFor3DPoint(const long* point) const;

    cPosition
    positionFor3DPointF(const float* point) const;

    cPosition
    positionNear3DPoint(const long* point, long horizontalRange, long verticalRange) const;

    cPosition
    positionInSection(long sectionID, long x, long y) const;

    cPosition
    positionInSectionFor3DPoint(long sectionID, const long* point) const;

    cPosition
    positionInSectionFor3DPointF(long sectionID, const float* point) const;

    cPosition
    positionInSectionNear3DPoint(long sectionID, const long* point, long horizontalRange, long verticalRange) const;

    cPosition
    generateRandomPosition() const;

    cPosition
    generateRandomPositionLocally(const cPosition& centre, long range) const;

    cPosition
    generateRandomPositionInSection(long sectionID) const;

    long
    heightAtPosition(const cPosition& position) const;

    float
    heightAtPositionF(const cPosition& position) const;

    float
    heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const;

    long
    getCellForEndOfLine(const cPosition& start, long endX, long endY) const;

    bool
    positionIsValid(const cPosition& p) const;

    void
    burnContextIntoMesh(iCollisionContext* context);

    void
    saveGround(const char* format, bool includeMapping, iOutputStream* outputStream) const;

    void
    setTerrainCallBack(long terrainLayer, iTerrainCallBack* callBack);

    long
    addEndPoint(const cPosition& position);

    long
    addOffMeshConnection(long fromEndPoint, long toEndPoint, long penalty);

    long
    getNumberOfEndPoints() const;

    cPosition
    getEndPoint(long index) const;

    long
    getNumberOfOffMeshConnections() const;

    void
    getOffMeshConnectionInfo(long index, long& fromEndPoint, long& toEndPoint, long& penalty) const;

    void
    clearOffMeshConnectionsAndEndPoints();

    void
    generateCollisionPreprocessFor(iShape* shape, const char *const* attributes);

    void
    generatePathfindPreprocessFor(iShape* shape, const char *const* attributes);

    void
    releaseAllPreprocessFor(iShape* shape);

    void
    releasePathfindPreprocessFor(iShape* shape);

    void
    preprocessGenerationCompleted();

    bool
    shapeCanCollide(const iShape* shape) const;

    bool
    shapeCanPathfind(const iShape* shape) const;

    const char *const*
    getCollisionPreprocessAttributes(iShape* shape) const;

    const char *const*
    getPathfindPreprocessAttributes(iShape* shape) const;

    void
    saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const;

    void
    savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const;

    void
    loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, unsigned long bufferSize);

    void
    loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, unsigned long bufferSize);

    iAgent*
    placeAgent(iShape* shape, const cPosition& position) const;

    iCollisionContext*
    newContext() const;

    iObstacleSet*
    newObstacleSet() const;

    bool
    testPointCollision(iShape* shape, iCollisionContext* context, const cPosition& position) const;

    bool
    testLineCollision(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& end) const;

    bool
    testLineCollision_XY(iShape* shape, iCollisionContext* context, const cPosition& start, long x, long y, long& cell) const;

    iCollisionInfo*
    firstCollision(iShape* shape, iCollisionContext* context, const cPosition& start, long x, long y, long& cell) const;

    cPosition
    findClosestUnobstructedPosition(iShape* shape, iCollisionContext* context, const cPosition& position, long maximumDistance) const;

    void
    getAllAgentsOverlapped(iShape* shape, iCollisionContext* context, const cPosition& position, iAgent** resultsBuffer, long& numberOverlapped) const;

    iPath*
    findShortestPath(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal) const;

    iPath*
    findShortestPath_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack* queryCallBack) const;

    iPath*
    findPathAway(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, long distanceAway) const;

    iPath*
    findPathAway_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, long distanceAway, iQueryCallBack* queryCallBack) const;

    iPath*
    generateCurvedPath(iShape* shape, iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;

    iPath*
    constructPath(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength) const;

    iPath*
    constructPath_Reversed(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength) const;

    void
    savePath(iPath* path, iOutputStream* os) const;

    iPath*
    loadPath(const char* dataBuffer, unsigned long bufferSize) const;

    void
    renderLineOnGround(const cPosition& start, long endX, long endY, long originX, long originY, iRender3DLinesCallBack& callBack) const;

    iObstacleSet*
    newObstacleSet_WithAttributes(const char *const* attributes) const;

    cPosition
    positionInSectionInShape(long sectionID, long shapeOriginX, long shapeOriginY, long numberOfPoints, const long* coordinateData) const;

    iPath*
    generateCurvedPath_WithEndVector(iShape* shape, iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long endVectorX, long endVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
