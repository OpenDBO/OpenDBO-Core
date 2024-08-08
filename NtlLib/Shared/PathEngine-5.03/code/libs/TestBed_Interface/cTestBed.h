//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_TESTBED_INCLUDED
#define C_TESTBED_INCLUDED

#include "i_testbed.h"

#include "platform_common/LoadSaveBinary.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "DrawMesh.h"
#include "CameraControl.h"
#include "Camera.h"
#include "Draw3D.h"
#include "i_pathengine.h"
#include "ZoomExtents.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "platform_common/CoordinateRanges.h"
//#include "common/Containers/i_SwapOutSubscriber.h"
#include "LineCollidesWithTriangle.h"
#include "common/Containers/LazyPointerVector.h"

#include <stdio.h>

class cMeshRenderInfo;
class controlinterface;
class iGraphicsInterface;
class osinterface;
class cGraphicsInterfaceRecorder;
class cAgentRenderPreprocess;

class cTestBed : public iTestBed
{
    cLazyPointerVector<cGraphicsInterfaceRecorder> _expansionRecorders;
    cLazyPointerVector<cGraphicsInterfaceRecorder> _smallConvexRecorders;
    cLazyPointerVector<cAgentRenderPreprocess> _agentRenderPreprocess;

    controlinterface* cip;
    iGraphicsInterface* gip;
    osinterface* ip;

    cHandle<cMesh> _mesh;

    cMeshRenderInfo* _meshRenderInfo;
    cGraphicsInterfaceRecorder* _baseCircuitsRecorder;

    bool _pickedThisFrame;
    cPosition _pickPosition;

    long _pickScreenX;
    long _pickScreenY;

    bool _previousMouseValid;
    long _previousMouseX;
    long _previousMouseY;

    unsigned long _startHigh, _startLow;
    double _clocksBuffer[20];
    int _clocksBufferPos;

    long _currentY;

    cProfile* _profile;

    bool _cameraPositionSet;

    std::vector<nLineCollidesWithTriangle::cTriPreprocess> _losPreprocess;

public:

    //... make private again
    cTestBed(osinterface *ip);
    ~cTestBed();

// iTestBed

    const char *const* getVersionAttributes() const;

    long getInterfaceMajorVersion() const
    {
        return TESTBED_INTERFACE_MAJOR_VERSION;
    }
    long getInterfaceMinorVersion() const
    {
        return TESTBED_INTERFACE_MINOR_VERSION;
    }

    void reportError(const char *error_type, const char *error_string)
    {
        Error(error_type,error_string);
    }

    char* loadBinary(const char* filename, unsigned long& buffer_size)
    {
        return LoadBinary_AllocatedBuffer(filename,buffer_size);
    }
    void freeBuffer(char *buffer)
    {
        delete [] buffer;
    }

    void setMesh(iMesh *mesh);
    void releaseMesh();

    void zoomExtents();

    const char* receiveKeyMessage();
    bool getKeyState(const char *keystring);
    bool getLeftMouseState();
    bool getRightMouseState();

    void getMouseScreenPosition(long &screenx, long &screeny);
    void getMouseScreenDelta(long &screendx, long &screendy);

    cPosition positionAtMouse();

    void setColour(const char *colour);
    void setColourRGB(float r, float g, float b);

    void drawMesh();
    void drawMeshEdges();
    void drawMeshExpansion(iShape *shape);
    void drawAgent(iAgent *agent, long height);
    void drawShape(iShape *shape, const cPosition &position, long height);
    void drawAgentHeading(iAgent *agent, long size, long height, float heading);
    void drawPath(iPath *path);
    void drawPosition(const cPosition &position, long size);
    void drawLine(const cPosition &start, const cPosition &end);

    void drawSquareBounds(const cPosition &start, long distance);
    void drawAgentExpansion(iAgent *agent, iShape *expandedby);

    void printText8by16(long x, long y, const char *text);
    void printTextLine(long x, const char *text);
    
    void update();

    iMesh* getMesh()
    {
        return _mesh.get();
    }

    void drawBurntInObstacles();
    void drawSmallConvexExpansions(iShape *shape);

    void drawAgentsInContext(iCollisionContext* context, long height);

    void lookAt(const cPosition& target, float angle, float elevation, float distance);
    bool meshObstructsCamera();
    void setAdditiveBlending(float alpha);
    void setOpaque();

    void drawAgentWithPrecision(iAgent* agent, long height, float precisionX, float precisionY);
    void drawAgentHeadingWithPrecision(iAgent* agent, long size, long height, float heading, float precisionX, float precisionY);
    void lookAtWithPrecision(const cPosition& target, float precisionX, float precisionY, float angle, float elevation, float distance);

    long createSound(const char* fileName, long numberOfBuffers);
    void playSound(long index);

    long loadSplashImage(const char* fileName);
    void displaySplashImage(long x, long y, long imageIndex);
    void displaySplashImage_Centred(long imageIndex);

//    void supplyTerrainHeightField(long terrainLayer,
//                                  const float* heights,
//                                  long startX, long startY,
//                                  long gridSpacing, long gridWidth, long gridHeight);

    void operator delete(void* p) {::operator delete(p);}

//======================

    void update_DontBeginScene();
    void update_Redraw();
    void beginScene();

    iGraphicsInterface* getGIP()
    {
        return gip;
    }
    osinterface* getIP()
    {
        return ip;
    }
    controlinterface* getCIP()
    {
        return cip;
    }

    cMesh& refMesh()
    {
        assertD(_mesh);
        return *_mesh;
    }

    bool meshHasBeenSet()
    {
        return _mesh!=0;
    }

    void draw3DMeshInternalEdges();
    void drawIntegerCrossesAround(const cPosition& position, long radius, long cross_size);
    void draw3DFace(tFace_3D f);
    void draw3DEdge(tEdge_3D e);

};

inline cTestBed* FromI(iTestBed* ptr)
{
    return static_cast<cTestBed*>(ptr);
}

#endif
