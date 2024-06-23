//**********************************************************************
//
// Copyright (c) 2002-2006
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_INTERFACE_HAS_BEEN_INCLUDED
#define PATHENGINE_INTERFACE_HAS_BEEN_INCLUDED

const long PATHENGINE_INTERFACE_MAJOR_VERSION = 2;
const long PATHENGINE_INTERFACE_MINOR_VERSION = 46;

#ifndef CPOSITION_HAS_BEEN_INCLUDED
#define CPOSITION_HAS_BEEN_INCLUDED
class cPosition
{
public:
    cPosition() : cell(-1) {}
    long x, y, cell;
    bool operator==(const cPosition& rhs) const
    {
        if(cell == -1)
            return cell == rhs.cell;
        return cell == rhs.cell && x == rhs.x && y == rhs.y;
    }
    bool operator!=(const cPosition& rhs) const
    {
        return !((*this) == rhs);
    }
};
#endif

class iShape;
class iMesh;
class iAgent;
class iCollisionContext;
class iObstacleSet;
class iPath;
class iCollisionInfo;
class iErrorHandler;
class iResourceLoader;
class iOutputStream;
class iTerrainCallBack;
class iRender3DLinesCallBack;
class iFaceVertexMesh;
class iAnchorsAndPinnedShapes;
class iQueryCallBack;
class iContentChunk;
class iMeshFederation;
class iMeshFederationTileGeneratedCallBack;

class iPathEngine
{
    void operator delete(void* voidPointer) {}
public:
    virtual const char *const* getVersionAttributes() const = 0;
    virtual long getInterfaceMajorVersion() const = 0;
    virtual long getInterfaceMinorVersion() const = 0;
    virtual void getReleaseNumbers(long& majorReleaseNumber, long& minorReleaseNumber, long& internalReleaseNumber) const = 0;
    virtual void setErrorHandler(iErrorHandler* newHandler) = 0;
    virtual iErrorHandler* getErrorHandler() = 0;
    virtual void setPathPoolParameters(long pathsPooled, long pointsPerPath) = 0;
    virtual bool shapeIsValid(long numberOfPoints, const long* coordinateData) = 0;
    virtual iShape* newShape(long numberOfPoints, const long* coordinateData) = 0;
    virtual iMesh* loadMeshFromBuffer(const char* format, const char* dataBuffer, unsigned long bufferSize, const char *const* options) = 0;
    virtual iMesh* buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char *const* options) = 0;
    virtual void saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char* format, iOutputStream* os) = 0;
    virtual unsigned long totalMemoryAllocated() = 0;
    virtual unsigned long maximumMemoryAllocated() = 0;
    virtual void resetMaximumMemoryAllocated() = 0;
    virtual void setRandomSeed(unsigned long value) = 0;
    virtual bool collisionPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize) = 0;
    virtual bool pathfindPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize) = 0;
    virtual void deleteAllObjects() = 0;
    virtual iContentChunk* newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes) = 0;
    virtual iContentChunk* loadContentChunk(const char* dataBuffer, unsigned long bufferSize) = 0;
    virtual iMeshFederation* buildMeshFederation_FromWorldMesh(const iMesh* worldMesh, long tileSize, long overlap, iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack) = 0;
    virtual iMeshFederation* loadFederation(const char* format, const char* dataBuffer, unsigned long bufferSize) = 0;
};

class iShape
{
public:
    virtual void destroy() = 0;
    virtual long size() const = 0;
    virtual void vertex(long index, long& x, long& y) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iShape* afterCast = static_cast<iShape*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iMesh
{
public:
    virtual void destroy() = 0;
    virtual long getNumberOf3DFaces() const = 0;
    virtual long get3DFaceAtPosition(const cPosition& position) const = 0;
    virtual void get3DFaceVertex(long faceIndex, long vertexIndexInFace, long& x, long& y, float& z) const = 0;
    virtual long get3DFaceConnection(long faceIndex, long edgeIndexInFace) const = 0;
    virtual void get3DFaceNormal(long faceIndex, float& resultX, float& resultY, float& resultZ) const = 0;
    virtual long get3DFaceAttribute(long faceIndex, long attributeIndex) const = 0;
    virtual long getNumberOfSections() const = 0;
    virtual long getSurfaceType(const cPosition& position) const = 0;
    virtual long getSectionID(const cPosition& position) const = 0;
    virtual void storeAnchor(const char* id, const cPosition& position, long orientation) = 0;
    virtual cPosition retrieveAnchor(const char* id, long& orientation) const = 0;
    virtual long getNumberOfAnchors() const = 0;
    virtual void retrieveAnchorByIndex(long index, cPosition& position, long& orientation, const char*& id) const = 0;
    virtual void storeNamedObstacle(const char* id, iAgent* agent) = 0;
    virtual iAgent* retrieveNamedObstacle(const char* id) const = 0;
    virtual long getNumberOfNamedObstacles() const = 0;
    virtual void retrieveNamedObstacleByIndex(long index, iAgent*& agent, const char*& id) const = 0;
    virtual cPosition positionFor3DPoint(const long* point) const = 0;
    virtual cPosition positionFor3DPointF(const float* point) const = 0;
    virtual cPosition positionNear3DPoint(const long* point, long horizontalRange, long verticalRange) const = 0;
    virtual cPosition positionInSection(long sectionID, long x, long y) const = 0;
    virtual cPosition positionInSectionFor3DPoint(long sectionID, const long* point) const = 0;
    virtual cPosition positionInSectionFor3DPointF(long sectionID, const float* point) const = 0;
    virtual cPosition positionInSectionNear3DPoint(long sectionID, const long* point, long horizontalRange, long verticalRange) const = 0;
    virtual cPosition generateRandomPosition() const = 0;
    virtual cPosition generateRandomPositionLocally(const cPosition& centre, long range) const = 0;
    virtual cPosition generateRandomPositionInSection(long sectionID) const = 0;
    virtual long heightAtPosition(const cPosition& position) const = 0;
    virtual float heightAtPositionF(const cPosition& position) const = 0;
    virtual float heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const = 0;
    virtual long getCellForEndOfLine(const cPosition& start, long endX, long endY) const = 0;
    virtual bool positionIsValid(const cPosition& p) const = 0;
    virtual void burnContextIntoMesh(iCollisionContext* context) = 0;
    virtual void saveGround(const char* format, bool includeMapping, iOutputStream* outputStream) const = 0;
    virtual void setTerrainCallBack(long terrainLayer, iTerrainCallBack* callBack) = 0;
    virtual long addEndPoint(const cPosition& position) = 0;
    virtual long addOffMeshConnection(long fromEndPoint, long toEndPoint, long penalty) = 0;
    virtual long getNumberOfEndPoints() const = 0;
    virtual cPosition getEndPoint(long index) const = 0;
    virtual long getNumberOfOffMeshConnections() const = 0;
    virtual void getOffMeshConnectionInfo(long index, long& fromEndPoint, long& toEndPoint, long& penalty) const = 0;
    virtual void clearOffMeshConnectionsAndEndPoints() = 0;
    virtual void generateCollisionPreprocessFor(iShape* shape, const char *const* attributes) = 0;
    virtual void generatePathfindPreprocessFor(iShape* shape, const char *const* attributes) = 0;
    virtual void releaseAllPreprocessFor(iShape* shape) = 0;
    virtual void releasePathfindPreprocessFor(iShape* shape) = 0;
    virtual void preprocessGenerationCompleted() = 0;
    virtual bool shapeCanCollide(const iShape* shape) const = 0;
    virtual bool shapeCanPathfind(const iShape* shape) const = 0;
    virtual const char *const* getCollisionPreprocessAttributes(iShape* shape) const = 0;
    virtual const char *const* getPathfindPreprocessAttributes(iShape* shape) const = 0;
    virtual void saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const = 0;
    virtual void savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const = 0;
    virtual void loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, unsigned long bufferSize) = 0;
    virtual void loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, unsigned long bufferSize) = 0;
    virtual iAgent* placeAgent(iShape* shape, const cPosition& position) const = 0;
    virtual iCollisionContext* newContext() const = 0;
    virtual iObstacleSet* newObstacleSet() const = 0;
    virtual bool testPointCollision(iShape* shape, iCollisionContext* context, const cPosition& position) const = 0;
    virtual bool testLineCollision(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& end) const = 0;
    virtual bool testLineCollision_XY(iShape* shape, iCollisionContext* context, const cPosition& start, long x, long y, long& cell) const = 0;
    virtual iCollisionInfo* firstCollision(iShape* shape, iCollisionContext* context, const cPosition& start, long x, long y, long& cell) const = 0;
    virtual cPosition findClosestUnobstructedPosition(iShape* shape, iCollisionContext* context, const cPosition& position, long maximumDistance) const = 0;
    virtual void getAllAgentsOverlapped(iShape* shape, iCollisionContext* context, const cPosition& position, iAgent** resultsBuffer, long& numberOverlapped) const = 0;
    virtual iPath* findShortestPath(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal) const = 0;
    virtual iPath* findShortestPath_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack* queryCallBack) const = 0;
    virtual iPath* findPathAway(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, long distanceAway) const = 0;
    virtual iPath* findPathAway_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, long distanceAway, iQueryCallBack* queryCallBack) const = 0;
    virtual iPath* generateCurvedPath(iShape* shape, iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long sectionLength, float turnRatio1, float turnRatio2) const = 0;
    virtual iPath* constructPath(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength) const = 0;
    virtual iPath* constructPath_Reversed(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength) const = 0;
    virtual void savePath(iPath* path, iOutputStream* os) const = 0;
    virtual iPath* loadPath(const char* dataBuffer, unsigned long bufferSize) const = 0;
    virtual void renderLineOnGround(const cPosition& start, long endX, long endY, long originX, long originY, iRender3DLinesCallBack& callBack) const = 0;
    virtual iObstacleSet* newObstacleSet_WithAttributes(const char *const* attributes) const = 0;
    virtual cPosition positionInSectionInShape(long sectionID, long shapeOriginX, long shapeOriginY, long numberOfPoints, const long* coordinateData) const = 0;
    virtual iPath* generateCurvedPath_WithEndVector(iShape* shape, iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long endVectorX, long endVectorY, long sectionLength, float turnRatio1, float turnRatio2) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iMesh* afterCast = static_cast<iMesh*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iAgent
{
public:
    virtual void destroy() = 0;
    virtual iMesh* getMesh() const = 0;
    virtual iShape* getShape() const = 0;
    virtual void setUserData(void* voidPointer) = 0;
    virtual void* getUserData() const = 0;
    virtual cPosition getPosition() const = 0;
    virtual void moveTo(const cPosition& newPosition) = 0;
    virtual bool canCollide() const = 0;
    virtual bool canPathfind() const = 0;
    virtual bool testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const = 0;
    virtual bool testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const = 0;
    virtual bool testCollisionTo_XY(iCollisionContext* context, long x, long y, long& cell) const = 0;
    virtual iCollisionInfo* firstCollisionTo(iCollisionContext* context, long x, long y, long& cell) const = 0;
    virtual cPosition findClosestUnobstructedPosition(iCollisionContext* context, long maximumDistance) const = 0;
    virtual void getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, long& numberOverlapped) const = 0;
    virtual bool testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const = 0;
    virtual iPath* findShortestPathTo(iCollisionContext* context, const cPosition& target) const = 0;
    virtual iPath* findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const = 0;
    virtual iPath* findPathAway(iCollisionContext* context, const cPosition& awayFrom, long distanceAway) const = 0;
    virtual iPath* findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, long distanceAway, iQueryCallBack* queryCallBack) const = 0;
    virtual iCollisionInfo* advanceAlongPath(iPath* path, float distance, iCollisionContext* context) = 0;
    virtual iCollisionInfo* advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY) = 0;
    virtual iPath* generateCurvedPath(iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long sectionLength, float turnRatio1, float turnRatio2) const = 0;
    virtual void setTraverseCost(float cost) = 0;
    virtual void setTraverseCostDirection(long directionVectorX, long directionVectorY) = 0;
    virtual float getTraverseCost() const = 0;
    virtual void getTraverseCostDirection(long& directionVectorX, long& directionVectorY) const = 0;
    virtual iPath* generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long endVectorX, long endVectorY, long sectionLength, float turnRatio1, float turnRatio2) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iAgent* afterCast = static_cast<iAgent*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iCollisionContext
{
public:
    virtual void destroy() = 0;
    virtual void addAgent(iAgent* agent) = 0;
    virtual void removeAgent(iAgent* agent) = 0;
    virtual long getNumberOfAgents() const = 0;
    virtual iAgent* getAgent(long index) const = 0;
    virtual bool includes(iAgent* agent) const = 0;
    virtual void addObstacleSet(iObstacleSet* context) = 0;
    virtual void removeObstacleSet(iObstacleSet* context) = 0;
    virtual long getNumberOfObstacleSets() const = 0;
    virtual iObstacleSet* getObstacleSet(long index) const = 0;
    virtual void updateCollisionPreprocessFor(iShape* shape) const = 0;
    virtual void updatePathfindingPreprocessFor(iShape* shape) const = 0;
    virtual void setSurfaceTypeTraverseCost(long surfaceType, float cost) = 0;
    virtual void setSurfaceTypeTraverseCostDirection(long surfaceType, long directionVectorX, long directionVectorY) = 0;
    virtual void setQueryBounds(long startX, long startY, long endX, long endY) = 0;
    virtual void clearQueryBounds() = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iCollisionContext* afterCast = static_cast<iCollisionContext*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iObstacleSet
{
public:
    virtual void destroy() = 0;
    virtual void addAgent(iAgent* agent) = 0;
    virtual void removeAgent(iAgent* agent) = 0;
    virtual long getNumberOfAgents() const = 0;
    virtual iAgent* getAgent(long index) const = 0;
    virtual bool includes(iAgent* agent) const = 0;
    virtual long size() const = 0;
    virtual bool empty() const = 0;
    virtual void updateCollisionPreprocessFor(iShape* shape) const = 0;
    virtual void updatePathfindingPreprocessFor(iShape* shape) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iObstacleSet* afterCast = static_cast<iObstacleSet*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iPath
{
public:
    virtual void destroy() = 0;
    virtual long size() const = 0;
    virtual cPosition position(long pointIndex) const = 0;
    virtual long connectionIndex(long segmentIndex) const = 0;
    virtual const cPosition* getPositionArray() const = 0;
    virtual const long* getConnectionIndexArray() const = 0;
    virtual unsigned long getLength() const = 0;
    virtual iCollisionInfo* advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY) = 0;
    virtual void renderOnGround(long originX, long originY, iRender3DLinesCallBack& callBack) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iPath* afterCast = static_cast<iPath*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iCollisionInfo
{
public:
    virtual void destroy() = 0;
    virtual void getCollidingLine(long* coords) const = 0;
    virtual iAgent* getCollidingAgent() const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iCollisionInfo* afterCast = static_cast<iCollisionInfo*>(voidPointer);
            afterCast->destroy();
        }
    }
};

#ifndef I_ERRORHANDLER_HAS_BEEN_INCLUDED
#define I_ERRORHANDLER_HAS_BEEN_INCLUDED
class iErrorHandler
{
public:
    virtual ~iErrorHandler() {}
    enum eAction
    {
        CONTINUE,
        ABORT,
        BREAK,
        CONTINUE_IGNORE_IN_FUTURE,
    };
    virtual eAction handle(const char* type, const char* description, const char *const* attributes) = 0;
};
#endif

#ifndef I_OUTPUTSTREAM_HAS_BEEN_INCLUDED
#define I_OUTPUTSTREAM_HAS_BEEN_INCLUDED
class iOutputStream
{
public:
    virtual ~iOutputStream() {}
    virtual void put(const char* data, unsigned long dataSize) = 0;
};
class cNullOutputStream : public iOutputStream
{
public:
    void put(const char* data, unsigned long dataSize) {}
};
#endif

class iTerrainCallBack
{
public:
    virtual ~iTerrainCallBack() {}
    virtual float getHeightAt(long x, long y) = 0;
    virtual float getHeightAtWithPrecision(long x, long y, float precisionX, float precisionY) = 0;
};

class iRender3DLinesCallBack
{
public:
    virtual ~iRender3DLinesCallBack() {}
    virtual void startVertex(float x, float y, float z) = 0;
    virtual void vertex(float x, float y, float z) = 0;
};

static const long PE_FaceAttribute_SurfaceType = 0;
static const long PE_FaceAttribute_SectionID = 1;
static const long PE_FaceAttribute_MarksPortal = 2;
static const long PE_FaceAttribute_MarksExternalShape = 3;
static const long PE_FaceAttribute_MarksConnectingEdge = 4;
static const long PE_FaceAttribute_UserData = 5;
static const long PE_FaceAttribute_MarksTerrainPortal = 6;
static const long PE_FaceAttribute_Top = 6;
class iFaceVertexMesh
{
public:
    virtual ~iFaceVertexMesh() {}
    virtual long faces() const = 0;
    virtual long vertices() const = 0;
    virtual long vertexIndex(long face, long vertexInFace) const = 0;
    virtual long vertexX(long vertex) const = 0;
    virtual long vertexY(long vertex) const = 0;
    virtual float vertexZ(long vertex) const = 0;
    virtual long faceAttribute(long face, long attributeIndex) const = 0;
};

static const long PE_AnchorAttribute_ResolvesToTerrain = 0;
static const long PE_AnchorAttribute_Top = 0;
static const long PE_PinnedShapeAttribute_Type = 0;
static const long PE_PinnedShapeAttribute_Top = 0;
static const long PE_PinnedShapeType_BurntIn = 0;
static const long PE_PinnedShapeType_NamedObstacle = 1;
static const long PE_PinnedShapeType_Top = 1;
class iAnchorsAndPinnedShapes
{
public:
    virtual ~iAnchorsAndPinnedShapes() {}

    virtual long numberOfAnchors() const = 0;
    virtual const char* anchor_Name(long anchorIndex) const = 0;
    virtual void anchor_HorizontalPosition(long anchorIndex, long& x, long& y) const = 0;
    virtual void anchor_VerticalRange(long anchorIndex, float& zStart, float& zEnd) const = 0;
    virtual long anchor_Orientation(long anchorIndex) const = 0;
    virtual long anchor_Attribute(long anchorIndex, long attributeIndex) const = 0;

    virtual long numberOfPinnedShapes() const = 0;
    virtual const char* pinnedShape_Name(long shapeIndex) const = 0;
    virtual long pinnedShape_AnchorIndex(long shapeIndex) const = 0;
    virtual long pinnedShape_NumberOfVertices(long shapeIndex) const = 0;
    virtual void pinnedShape_Vertex(long shapeIndex, long vertexIndex, long& x, long& y) const = 0;
    virtual long pinnedShape_Attribute(long shapeIndex, long attributeIndex) const = 0;
};

class iQueryCallBack
{
public:
    virtual ~iQueryCallBack() {}
    virtual long desiredCallBackFrequency() = 0;
    virtual bool abort() = 0;
};

class iContentChunk
{
public:
    virtual void destroy() = 0;
    virtual bool preValidate(long maximumScale, long translationRange) = 0;
    virtual void save(iOutputStream* outputStream) const = 0;
    virtual const iFaceVertexMesh* instantiate(long rotation, long scale, long translationX, long translationY, float translationZ) = 0;
    virtual const iFaceVertexMesh* instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ) = 0;
    virtual void deleteAllInstances() = 0;
    virtual void addAnchorsAndShapes(iMesh* mesh, long rotation, long scale, long translationX, long translationY, float translationZ, const char* idPrefix) = 0;
    virtual void addAnchorsAndShapes_FloatRotateAndScale(iMesh* mesh, float rotation, float scale, long translationX, long translationY, float translationZ, const char* idPrefix) = 0;
    virtual bool hasGroundComponent() const = 0;
    virtual bool hasAnchorsAndShapesComponent() const = 0;
    virtual const iAnchorsAndPinnedShapes* instantiateAnchorsAndShapes(long rotation, long scale, long translationX, long translationY, float translationZ) = 0;
    virtual const iAnchorsAndPinnedShapes* instantiateAnchorsAndShapes_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ) = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iContentChunk* afterCast = static_cast<iContentChunk*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iMeshFederation
{
public:
    virtual void destroy() = 0;
    virtual void save(const char* format, iOutputStream* outputStream) const = 0;
    virtual long size() const = 0;
    virtual bool coordinatesAreInsideRange(long x, long y) const = 0;
    virtual long tileForQuery(long queryStartX, long queryStartY) const = 0;
    virtual void getTileCentre(long tileIndex, long& centreX, long& centreY) const = 0;
    virtual void getHandledRegion_World(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const = 0;
    virtual void getRepresentedRegion_World(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const = 0;
    virtual void getHandledRegion_Local(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const = 0;
    virtual void getRepresentedRegion_Local(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const = 0;
    virtual cPosition translatePosition(long fromTileIndex, const iMesh* fromMesh, long toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iMeshFederation* afterCast = static_cast<iMeshFederation*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iMeshFederationTileGeneratedCallBack
{
public:
    virtual ~iMeshFederationTileGeneratedCallBack() {}
    virtual void tileGenerated(long tileIndex, iMesh* tileMesh) = 0;
};

#endif // ends file include guard
