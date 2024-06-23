//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include <d3dx8.h>
#include <windows.h>
#include "common/STL/vector.h"

class cVertexBufferManager;

enum eRenderState
{
    RS_INVALID,
    RS_LINE,
    RS_TEXT,
    RS_POLYGON,
    RS_BLENDED_POLYGON,
};

class cD3D
{
    long _sizeX, _sizeY;
    LPDIRECT3D8             _pD3D;
    LPDIRECT3DDEVICE8       _pd3dDevice;
    LPDIRECT3DVERTEXBUFFER8 _pVB;
    LPDIRECT3DTEXTURE8      _pFontTexture;
    LPDIRECT3DTEXTURE8      _pTexture;
    LPDIRECT3DTEXTURE8      _pTexture2;
    LPDIRECT3DTEXTURE8      _pTexture3;
    D3DLIGHT8               _light;
    D3DMATERIAL8            _mtrl;
    D3DXMATRIX              _matProj;
    D3DXMATRIX              _matProj_Biased;
    D3DXMATRIX              _matView;
    D3DXMATRIX              _matTextView;
    cVertexBufferManager*   _vertexBufferManager;
    float _r, _g, _b, _a;
    float _ambientPercent;
    eRenderState _renderState;
    float _zBias;
    bool _in2D;
    float _storedZBias;
    D3DXVECTOR3 _storedLightDirection;
    IDirect3DVertexBuffer8* _currentstreamsource;
    std::vector<LPDIRECT3DTEXTURE8> _loadedImages;
    std::vector<D3DXIMAGE_INFO> _imageInfos;

    void setAmbient();

public:

    static cD3D& refInstance();

    static LPDIRECT3DDEVICE8 device()
    {
        return refInstance()._pd3dDevice;
    }
    static cVertexBufferManager& refBufferManager()
    {
        return *refInstance()._vertexBufferManager;
    }

    long getSizeX() const
    {
        return _sizeX;
    }
    long getSizeY() const
    {
        return _sizeY;
    }

    HRESULT init(HWND hWnd, long sizeX, long sizeY);

    void setupMatrices();
    void cleanup();
    void render();
    void beginScene();
    void endScene();
    void present();

    void setLightColour(float r, float g, float b);
    void setAlpha(float a);
    float getCurrentAlpha() const;

    void setRenderState(eRenderState state);
    void setLoadedImageRenderState(long index);
    void setCamera2D(long scale, long x, long y, bool zIsIgnored);
    void setCamera3D(const float* target, float angle1, float angle2, float distance);

    void setZBias(float value);
    float getZBias();

    void setStreamSource(IDirect3DVertexBuffer8* source, UINT size);
    void clearStreamSource();

    void into2D();
    void outOf2D();

    long loadImage(const char* fileName);
    long getImageWidth(long index) const
    {
        return _imageInfos[index].Width;
    }
    long getImageHeight(long index) const
    {
        return _imageInfos[index].Height;
    }
};
