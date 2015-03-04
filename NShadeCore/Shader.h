#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include "common.h"
#include "d3dcompiler.h"

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

struct ShaderDescription
{
	ShaderType Type;
	ShaderVersion Version;
	LPCSTR FilePath;
};

class Shader
{
public:
	Shader(DeviceResources* pResources);
	~Shader();
public:
	virtual void Render(ID3D11Buffer* constBuffer) = 0;
	virtual void Load(LPCWSTR file) = 0;
	virtual void Compile(LPCWSTR file) = 0;
protected:
	DeviceResources*					PResources;
	ShaderType							Type;
	ShaderVersion						Version;
	LPCWSTR								ShaderFilePath;
	FileBytes*							ByteCode;
};

