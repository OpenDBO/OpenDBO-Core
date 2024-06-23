//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iAction.h"
#include <d3dx8.h>
#include <string>

class iRenderGeometry : public iAction
{
public:

    ~iRenderGeometry() {}

    virtual void render(bool blended) = 0;
    void perform(cActionContext& context)
    {
        if(context._blendingOn == context._renderBlended)
        {
            render(context._blendingOn);
        }
    }
    bool isRenderGeometry()
    {
        return true;
    }
};

class cLine : public iRenderGeometry
{
    long _indexInManagedBuffer;
    IDirect3DVertexBuffer8* _vertexBuffer;
    long _vertexIndexInVertexBuffer;

public:

    cLine(const float* pointArray);
    ~cLine();
    void render(bool blended);
};


class cPolygon : public iRenderGeometry
{
    long _points;
    long _indexInManagedBuffer;
    IDirect3DVertexBuffer8* _vertexBuffer;
    long _vertexIndexInVertexBuffer;

public:

    cPolygon(long points, const float* pointArray);
    ~cPolygon();

    void render(bool blended);
};

class cText : public iRenderGeometry
{
    long _x, _y;
    std::string _str;

public:

    cText(long x, long y, const char* str);
    void render(bool blended);
};

class cSplashImage : public iRenderGeometry
{
    long _x, _y;
    long _width, _height;
    long _loadedImageIndex;

public:

    cSplashImage(long x, long y, long width, long height, long loadedImageIndex);
    void render(bool blended);
};
