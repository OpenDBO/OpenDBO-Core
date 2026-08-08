// Minimal d3dx9.h for x64 compatibility
// ID3DXEffect vtable matches June 2010 SDK exactly (79 methods in correct order)

#ifndef D3DX9_H
#define D3DX9_H

#include <d3d9.h>
#include "d3dx9math.h"

// D3DX types
typedef LPCSTR D3DXHANDLE;
typedef DWORD D3DXINCLUDE_TYPE;

#define D3DXINCLUDE_LOCAL 0
#define D3DXINCLUDE_SYSTEM 1

// Forward declarations for opaque types
struct ID3DXEffectPool { char _dummy; };
typedef struct ID3DXEffectPool *LPD3DXEFFECTPOOL;
struct ID3DXEffectStateManager { char _dummy; };
typedef struct ID3DXEffectStateManager *LPD3DXEFFECTSTATEMANAGER;

typedef struct _D3DXEFFECT_DESC { int _dummy; } D3DXEFFECT_DESC;
typedef struct _D3DXPARAMETER_DESC { int _dummy; } D3DXPARAMETER_DESC;
typedef struct _D3DXTECHNIQUE_DESC { int _dummy; } D3DXTECHNIQUE_DESC;
typedef struct _D3DXPASS_DESC { int _dummy; } D3DXPASS_DESC;
typedef struct _D3DXFUNCTION_DESC { int _dummy; } D3DXFUNCTION_DESC;

// ID3DXBuffer - COM interface
DECLARE_INTERFACE_(ID3DXBuffer, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID, LPVOID*) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    STDMETHOD_(LPVOID, GetBufferPointer)(THIS) PURE;
    STDMETHOD_(DWORD, GetBufferSize)(THIS) PURE;
};

// ID3DXInclude
DECLARE_INTERFACE(ID3DXInclude)
{
    STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE, LPCSTR, LPCVOID, LPCVOID*, UINT*) PURE;
    STDMETHOD(Close)(THIS_ LPCVOID) PURE;
};

// ID3DXEffect - COM interface with full correct vtable order
// All 79 methods listed in exact SDK vtable order
DECLARE_INTERFACE_(ID3DXEffect, IUnknown)
{
    // --- IUnknown (3) ---
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    // --- Descriptors (5) ---
    STDMETHOD(GetDesc)(THIS_ D3DXEFFECT_DESC *pDesc) PURE;
    STDMETHOD(GetParameterDesc)(THIS_ D3DXHANDLE hParameter, D3DXPARAMETER_DESC *pDesc) PURE;
    STDMETHOD(GetTechniqueDesc)(THIS_ D3DXHANDLE hTechnique, D3DXTECHNIQUE_DESC *pDesc) PURE;
    STDMETHOD(GetPassDesc)(THIS_ D3DXHANDLE hPass, D3DXPASS_DESC *pDesc) PURE;
    STDMETHOD(GetFunctionDesc)(THIS_ D3DXHANDLE hShader, D3DXFUNCTION_DESC *pDesc) PURE;
    // --- Handle Operations (12) ---
    STDMETHOD_(D3DXHANDLE, GetParameter)(THIS_ D3DXHANDLE hParameter, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetParameterByName)(THIS_ D3DXHANDLE hParameter, LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetParameterBySemantic)(THIS_ D3DXHANDLE hParameter, LPCSTR pSemantic) PURE;
    STDMETHOD_(D3DXHANDLE, GetParameterElement)(THIS_ D3DXHANDLE hParameter, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetTechnique)(THIS_ UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetTechniqueByName)(THIS_ LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetPass)(THIS_ D3DXHANDLE hTechnique, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetPassByName)(THIS_ D3DXHANDLE hTechnique, LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetFunction)(THIS_ UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetFunctionByName)(THIS_ LPCSTR pName) PURE;
    STDMETHOD_(D3DXHANDLE, GetAnnotation)(THIS_ D3DXHANDLE hObject, UINT Index) PURE;
    STDMETHOD_(D3DXHANDLE, GetAnnotationByName)(THIS_ D3DXHANDLE hObject, LPCSTR pName) PURE;
    // --- Get/Set Parameters (36) ---
    STDMETHOD(SetValue)(THIS_ D3DXHANDLE hParameter, LPCVOID pData, UINT Bytes) PURE;
    STDMETHOD(GetValue)(THIS_ D3DXHANDLE hParameter, LPVOID pData, UINT Bytes) PURE;
    STDMETHOD(SetBool)(THIS_ D3DXHANDLE hParameter, BOOL b) PURE;
    STDMETHOD(GetBool)(THIS_ D3DXHANDLE hParameter, BOOL *pb) PURE;
    STDMETHOD(SetBoolArray)(THIS_ D3DXHANDLE hParameter, CONST BOOL *pb, UINT Count) PURE;
    STDMETHOD(GetBoolArray)(THIS_ D3DXHANDLE hParameter, BOOL *pb, UINT Count) PURE;
    STDMETHOD(SetInt)(THIS_ D3DXHANDLE hParameter, INT n) PURE;
    STDMETHOD(GetInt)(THIS_ D3DXHANDLE hParameter, INT *pn) PURE;
    STDMETHOD(SetIntArray)(THIS_ D3DXHANDLE hParameter, CONST INT *pn, UINT Count) PURE;
    STDMETHOD(GetIntArray)(THIS_ D3DXHANDLE hParameter, INT *pn, UINT Count) PURE;
    STDMETHOD(SetFloat)(THIS_ D3DXHANDLE hParameter, FLOAT f) PURE;
    STDMETHOD(GetFloat)(THIS_ D3DXHANDLE hParameter, FLOAT *pf) PURE;
    STDMETHOD(SetFloatArray)(THIS_ D3DXHANDLE hParameter, CONST FLOAT *pf, UINT Count) PURE;
    STDMETHOD(GetFloatArray)(THIS_ D3DXHANDLE hParameter, FLOAT *pf, UINT Count) PURE;
    STDMETHOD(SetVector)(THIS_ D3DXHANDLE hParameter, CONST D3DXVECTOR4 *pVector) PURE;
    STDMETHOD(GetVector)(THIS_ D3DXHANDLE hParameter, D3DXVECTOR4 *pVector) PURE;
    STDMETHOD(SetVectorArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXVECTOR4 *pVector, UINT Count) PURE;
    STDMETHOD(GetVectorArray)(THIS_ D3DXHANDLE hParameter, D3DXVECTOR4 *pVector, UINT Count) PURE;
    STDMETHOD(SetMatrix)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX *pMatrix) PURE;
    STDMETHOD(GetMatrix)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX *pMatrix) PURE;
    STDMETHOD(SetMatrixArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX *pMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX *pMatrix, UINT Count) PURE;
    STDMETHOD(SetMatrixPointerArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX **ppMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixPointerArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX **ppMatrix, UINT Count) PURE;
    STDMETHOD(SetMatrixTranspose)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX *pMatrix) PURE;
    STDMETHOD(GetMatrixTranspose)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX *pMatrix) PURE;
    STDMETHOD(SetMatrixTransposeArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX *pMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixTransposeArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX *pMatrix, UINT Count) PURE;
    STDMETHOD(SetMatrixTransposePointerArray)(THIS_ D3DXHANDLE hParameter, CONST D3DXMATRIX **ppMatrix, UINT Count) PURE;
    STDMETHOD(GetMatrixTransposePointerArray)(THIS_ D3DXHANDLE hParameter, D3DXMATRIX **ppMatrix, UINT Count) PURE;
    STDMETHOD(SetString)(THIS_ D3DXHANDLE hParameter, LPCSTR pString) PURE;
    STDMETHOD(GetString)(THIS_ D3DXHANDLE hParameter, LPCSTR *ppString) PURE;
    STDMETHOD(SetTexture)(THIS_ D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture) PURE;
    STDMETHOD(GetTexture)(THIS_ D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 *ppTexture) PURE;
    STDMETHOD(GetPixelShader)(THIS_ D3DXHANDLE hParameter, LPDIRECT3DPIXELSHADER9 *ppPShader) PURE;
    STDMETHOD(GetVertexShader)(THIS_ D3DXHANDLE hParameter, LPDIRECT3DVERTEXSHADER9 *ppVShader) PURE;
    // --- Set Array Range (1) ---
    STDMETHOD(SetArrayRange)(THIS_ D3DXHANDLE hParameter, UINT uStart, UINT uEnd) PURE;
    // --- ID3DXEffect-specific methods (22) ---
    STDMETHOD(GetPool)(THIS_ LPD3DXEFFECTPOOL *ppPool) PURE;
    STDMETHOD(SetTechnique)(THIS_ D3DXHANDLE hTechnique) PURE;
    STDMETHOD_(D3DXHANDLE, GetCurrentTechnique)(THIS) PURE;
    STDMETHOD(ValidateTechnique)(THIS_ D3DXHANDLE hTechnique) PURE;
    STDMETHOD(FindNextValidTechnique)(THIS_ D3DXHANDLE hTechnique, D3DXHANDLE *pTechnique) PURE;
    STDMETHOD_(BOOL, IsParameterUsed)(THIS_ D3DXHANDLE hParameter, D3DXHANDLE hTechnique) PURE;
    STDMETHOD(Begin)(THIS_ UINT *pPasses, DWORD Flags) PURE;
    STDMETHOD(BeginPass)(THIS_ UINT Pass) PURE;
    STDMETHOD(CommitChanges)(THIS) PURE;
    STDMETHOD(EndPass)(THIS) PURE;
    STDMETHOD(End)(THIS) PURE;
    STDMETHOD(GetDevice)(THIS_ LPDIRECT3DDEVICE9 *ppDevice) PURE;
    STDMETHOD(OnLostDevice)(THIS) PURE;
    STDMETHOD(OnResetDevice)(THIS) PURE;
    STDMETHOD(SetStateManager)(THIS_ LPD3DXEFFECTSTATEMANAGER pManager) PURE;
    STDMETHOD(GetStateManager)(THIS_ LPD3DXEFFECTSTATEMANAGER *ppManager) PURE;
    STDMETHOD(BeginParameterBlock)(THIS) PURE;
    STDMETHOD_(D3DXHANDLE, EndParameterBlock)(THIS) PURE;
    STDMETHOD(ApplyParameterBlock)(THIS_ D3DXHANDLE hParameterBlock) PURE;
    STDMETHOD(DeleteParameterBlock)(THIS_ D3DXHANDLE hParameterBlock) PURE;
    STDMETHOD(CloneEffect)(THIS_ LPDIRECT3DDEVICE9 pDevice, struct ID3DXEffect **ppEffect) PURE;
    STDMETHOD(SetRawValue)(THIS_ D3DXHANDLE hParameter, LPCVOID pData, UINT ByteOffset, UINT Bytes) PURE;
};

typedef ID3DXBuffer *LPD3DXBUFFER;
typedef ID3DXEffect *LPD3DXEFFECT;
typedef ID3DXInclude *LPD3DXINCLUDE;

// Macro structures
typedef struct D3DXMACRO {
    LPCSTR Name;
    LPCSTR Definition;
} D3DXMACRO, *LPD3DXMACRO;

// D3DX Shader flags
#define D3DXSHADER_DEBUG                0x00000001
#define D3DXSHADER_SKIPVALIDATION       0x00000002
#define D3DXSHADER_SKIPOPTIMIZATION     0x00000004
#define D3DXSHADER_FORCE_VS_SOFTWARE    0x00000010
#define D3DXSHADER_FORCE_PS_SOFTWARE    0x00000020
#define D3DXSHADER_PARTIAL_PRECISION    0x00000040
#define D3DXSHADER_AVOID_FLOW_CONTROL   0x00000080
#define D3DXSHADER_PREFER_FLOW_CONTROL  0x00010000

// Effect compile flags
#define D3DXFX_NOT_CLONEABLE               (1 << 10)
#define D3DXFX_LARGEADDRESSAWARE           (1 << 17)

// D3DX function declarations - extern "C" to match d3dx9.lib import names
#ifdef __cplusplus
extern "C" {
#endif

HRESULT WINAPI D3DXAssembleShader(
    LPCSTR pSrcData, UINT SrcDataLen,
    CONST D3DXMACRO* pDefines, ID3DXInclude* pInclude,
    DWORD Flags, ID3DXBuffer** ppShader, ID3DXBuffer** ppErrorMsgs
);

HRESULT WINAPI D3DXCreateEffect(
    LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen,
    CONST D3DXMACRO* pDefines, ID3DXInclude* pInclude,
    DWORD Flags, LPVOID pPool, LPD3DXEFFECT* ppEffect, ID3DXBuffer** ppCompilationErrors
);

HRESULT WINAPI D3DXCreateEffectFromFileA(
    LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile,
    CONST D3DXMACRO* pDefines, ID3DXInclude* pInclude,
    DWORD Flags, LPVOID pPool, LPD3DXEFFECT* ppEffect, ID3DXBuffer** ppCompilationErrors
);

HRESULT WINAPI D3DXCreateEffectFromFileW(
    LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile,
    CONST D3DXMACRO* pDefines, ID3DXInclude* pInclude,
    DWORD Flags, LPVOID pPool, LPD3DXEFFECT* ppEffect, ID3DXBuffer** ppCompilationErrors
);

#ifdef __cplusplus
}
#endif

#ifdef UNICODE
#define D3DXCreateEffectFromFile D3DXCreateEffectFromFileW
#else
#define D3DXCreateEffectFromFile D3DXCreateEffectFromFileA
#endif

#endif // D3DX9_H
