#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "vector"
#include "d3dcompiler.h"
#include "res.h"
#include "file.h"
#include "export.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "domainshader.h"
#include "hullshader.h"
#include "geometryshader.h"
#include "computeshader.h"

using namespace std;

#ifndef PS_PROFILE
#define PS_PROFILE {"ps_5_0" }
#endif

#ifndef VS_PROFILE
#define VS_PROFILE {"vs_5_0" }
#endif

#ifndef GS_PROFILE
#define GS_PROFILE { "ps_5_0" }
#endif

#ifndef HS_PROFILE
#define HS_PROFILE { "hs_5_0" }
#endif

#ifndef DS_PROFILE
#define DS_PROFILE { "ds_5_0" }
#endif

enum ShaderVersion : char
{
    V0 = 0,
    V1 = 0,
    V2 = 0,
    V3 = 0,
    V4 = 0,
    V5 = 0
};

enum ShaderType : char
{
    VS = 1,
    PS = 2,
    GS = 3,
    HS = 4,
    CS = 5,
    DS = 6
};

struct Shaders
{
    PixelShader     *PixelShader;
    VertexShader    *VertexShader;
    HullShader      *HullShader;
    DomainShader    *DomainShader;
    GeometryShader  *GeometryShader;
    ComputeShader   *ComputeShader;
};

EXTERN class API Shader
{
public:
    HRESULT	SetVertexShader(wchar_t *compiledShaderFile);
    HRESULT	CompileVertexShader(wchar_t *shaderSource);

    HRESULT SetHullShader(wchar_t *compiledShaderFile);
    HRESULT CompileHullShader(wchar_t *shaderSource);

    HRESULT SetDomainShader(wchar_t *compiledShaderFile);
    HRESULT CompileDomainShader(wchar_t *shaderSource);

    HRESULT SetGeometryShader(wchar_t *compiledShaderFile);
    HRESULT CompileGeometryShader(wchar_t *shaderSource);

    HRESULT SetPixelShader(wchar_t *compiledShaderFile);
    HRESULT CompilePixelShader(wchar_t *shaderSource);
private:
    HRESULT CompileShader(wchar_t *compiledShaderFile, ID3DBlob *blob, LPCSTR shaderProfile);
private:
    vector<D3D11_INPUT_ELEMENT_DESC>    _inputDescription;
    ShaderType                          _type;
    ID3D11PixelShader*                  _pixelShader;
    ID3D11VertexShader*                 _vertexShader;
    ID3D11HullShader*                   _hullShader;
    ID3D11DomainShader*                 _domainShader;
    ID3D11GeometryShader*               _geometryShader;
    ID3D11ComputeShader*                _computeShader;
};