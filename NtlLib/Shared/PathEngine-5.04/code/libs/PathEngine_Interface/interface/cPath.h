//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"
#include "common/Containers/ReplacementList.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "common/STL/utility.h"
#include "platform_common/Mutex.h"

class cShape;
class cMesh;
class cCollisionContext;
class iCollisionInfo;
template <class T> class cPool;
class cMutex;
class iXMLOutputStream;
class iXMLInputStream;
class iAPIObjectWrapper;

class cPath : public iPath
{
    cMutex _mutex; // applied on external interface
    cMesh* _mesh;
    cList_NoSize<cPath*>::iterator _heldNode;

    long _size;
    long _startOffset;
    std::vector<cInternalPosition> _internalPositions;
    std::vector<cPosition> _points;
    std::vector<long>* _connectionIndices;

  // these are only kept valid when _size > 1
    long _length;
    cPosition _originalStart;
    float _distanceFromOriginalStart;

//    bool _isExactlyOnPath;
//    tPoint _approximationOnOtherSide; // valid if not exactly on path

    bool _toggleApproximationDirection;

    void init_common();

  // support for path pool

    long _poolIndex;
    friend class cPool<cPath>;
    void setPoolIndex(long value)
    {
        _poolIndex = value;
    }
    long getPoolIndex() const
    {
        return _poolIndex;
    }

    float getAdvanceInfo(float distance,
                         bool& reachesNextPoint,
                         bool& exactlyOnLine,
                         tPoint& point1, tPoint& point2
                         );
    void pop_front();
    void advancePoint0To(const cInternalPosition& p, float distanceAdvanced);

    void
    finaliseAdvance(
            const cInternalPosition& newPosition,
            float distance,
            float& precisionX, float& precisionY
            );
    void
    finaliseAdvance(
            const cPosition& newPosition,
            float distance,
            float& precisionX, float& precisionY
            );

public:

    iAPIObjectWrapper* _wrapper;

    cPath();
    ~cPath();

// initialisation (or reinitialisation)

    void attachToMesh(cMesh* mesh, cList_NoSize<cPath*>::iterator heldNode)
    {
        assertD(_mesh == 0);
        _mesh = mesh;
        assertD(_heldNode.isInvalid());
        _heldNode = heldNode;
    }

    void init(const cInternalPosition& singlePoint);

    // the vector is swapped in by these initialisers
    void init(std::vector<cInternalPosition>& points, std::vector<long>* connectionIndices);

    void init(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathSize);
    void init_Reversed(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathSize);

    void load(iXMLInputStream& is, const char *const* rootAttributes);

// iPath

    void destroy();
    long size() const;
    cPosition position(long pointIndex) const;
    long connectionIndex(long segmentIndex) const;
    const cPosition* getPositionArray() const;
    const long* getConnectionIndexArray() const;
    unsigned long getLength() const;
    iCollisionInfo* advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY);
    void renderOnGround(long originX, long originY, iRender3DLinesCallBack& callBack) const;
    void operator delete(void* p) {::operator delete(p);}

//=========

    bool operator==(cPath& rhs);
    bool operator!=(cPath& rhs)
    {
        return !((*this) == rhs);
    }

//    void append(const cInternalPosition& p);
//    void replaceEnd(long replaceFromIndex, const std::vector<cInternalPosition>& pathToAppend, unsigned long pathToAppendLength);
//    void replaceEnd(long replaceFromIndex, const std::list<std::pair<tFace, tPoint> >& pathToAppend);
//    void replaceStart(long replaceToIndex, const std::list<std::pair<tFace, tPoint> >& pathToPrepend);

    cInternalPosition internalPosition(long pointindex)
    {
        return _internalPositions[_startOffset+pointindex];
    }

//    long getIndexForEndUnwind(const tPoint& newEndPoint);
//    long getIndexForStartUnwind(const tPoint& newStartPoint);

    void curved(
            cShape* shape,
            cCollisionContext* context,
            long startVectorX, long startVectorY,
            long endVectorX, long endVectorY,
            long sectionLength,
            float turnRatio1, float turnRatio2,
            cPath* result
            );

    void getPrecisionXY(float& x, float& y);

    void convert(std::list<std::pair<tFace, tPoint> >& appendTo);

    void save(iXMLOutputStream& os);

    static long majorVersion();
    static long minorVersion();
};

inline cPath* FromI(iPath* ptr)
{
    return static_cast<cPath*>(ptr);
}

inline void Destroy(cPath* ptr)
{
    ptr->destroy();
}
