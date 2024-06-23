//**********************************************************************
//
// Copyright (c) 2006
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TESTBED_INTERFACE_HAS_BEEN_INCLUDED
#define TESTBED_INTERFACE_HAS_BEEN_INCLUDED

const long TESTBED_INTERFACE_MAJOR_VERSION = 2;
const long TESTBED_INTERFACE_MINOR_VERSION = 1;

#ifndef CPOSITION_HAS_BEEN_INCLUDED
#define CPOSITION_HAS_BEEN_INCLUDED
class cPosition
{
public:
    cPosition() :cell(-1) {}
    long x,y,cell;
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
class iPath;
class iPathEngine;
class iCollisionContext;

class iTestBed
{
    void operator delete(void* void_pointer) {}
public:
    virtual const char *const* getVersionAttributes() const=0;
    virtual long getInterfaceMajorVersion() const=0;
    virtual long getInterfaceMinorVersion() const=0;

    virtual void reportError(const char* error_type, const char* error_string)=0;

    virtual char* loadBinary(const char* filename, unsigned long& buffer_size)=0;
    virtual void freeBuffer(char* buffer)=0;

    virtual void setMesh(iMesh* mesh)=0;
    virtual void releaseMesh()=0;
    virtual iMesh* getMesh()=0;

    virtual const char* receiveKeyMessage()=0;
    virtual bool getKeyState(const char* keystring)=0;
    virtual bool getLeftMouseState()=0;
    virtual bool getRightMouseState()=0;
    virtual void getMouseScreenPosition(long& screenx, long& screeny)=0;
    virtual void getMouseScreenDelta(long& screendx, long& screendy)=0;

    virtual cPosition positionAtMouse()=0;

    virtual void setColour(const char* colour)=0;
    virtual void setColourRGB(float r, float g, float b)=0;
    virtual void setAdditiveBlending(float alpha)=0;
    virtual void setOpaque()=0;

    virtual void zoomExtents()=0;
    virtual void lookAt(const cPosition& target, float angle, float elevation, float distance)=0;
    virtual void lookAtWithPrecision(const cPosition& target, float precisionX, float precisionY, float angle, float elevation, float distance)=0;
    virtual bool meshObstructsCamera()=0;

    virtual void drawMesh()=0;
    virtual void drawMeshEdges()=0;
    virtual void drawBurntInObstacles()=0;
    virtual void drawMeshExpansion(iShape* shape)=0;
    virtual void drawSmallConvexExpansions(iShape* shape)=0;
    virtual void drawAgent(iAgent* agent, long height)=0;
    virtual void drawAgentWithPrecision(iAgent* agent, long height, float precisionX, float precisionY)=0;
    virtual void drawAgentHeading(iAgent* agent, long size, long height, float heading)=0;
    virtual void drawAgentHeadingWithPrecision(iAgent* agent, long size, long height, float heading, float precisionX, float precisionY)=0;
    virtual void drawAgentExpansion(iAgent* agent, iShape* expandedby)=0;
    virtual void drawAgentsInContext(iCollisionContext* context, long height)=0;
    virtual void drawShape(iShape* shape, const cPosition& position, long height)=0;
    virtual void drawPath(iPath* path)=0;
    virtual void drawPosition(const cPosition& position, long size)=0;
    virtual void drawLine(const cPosition& start, const cPosition& end)=0;

    virtual void drawSquareBounds(const cPosition& start, long distance)=0;

    virtual void printText8by16(long x, long y, const char* text)=0;
    virtual void printTextLine(long x, const char* text)=0;

    virtual void update()=0;

    virtual long createSound(const char* fileName, long numberOfBuffers) = 0;
    virtual void playSound(long index) = 0;

    virtual long loadSplashImage(const char* fileName) = 0;
    virtual void displaySplashImage(long x, long y, long imageIndex) = 0;
    virtual void displaySplashImage_Centred(long imageIndex) = 0;

    virtual void drawRangeBounds(long minX, long minY, long maxX, long maxY) = 0;
};

#endif // Include guard
