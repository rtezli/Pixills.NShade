#pragma once
#pragma warning( disable : 4996 )

#include "common.h"
#include "shader.h"

class VertexShader //: public Shader
{
public:
	VertexShader(DeviceResources* pResources);
public:
	ID3D11InputLayout*	const GetInputLayout(){ return PInputLayout; }
	UINT				const GetInputSize(){ return ByteWidth; }
	ID3D11VertexShader*	const Shader(){ return PVertexShader; }
	ID3D11Buffer*		const GetVertexBuffer(){ return PVertexBuffer.get(); }
	ID3D11Buffer*		const GetIndexBuffer(){ return PIndexBuffer.get(); }

	VOID				CreateBuffers(vector<NVertex>* vertices, vector<UINT> *indices);
protected:
	VOID						Load(CHAR *file);
	VOID						Compile(CHAR *file, ShaderVersion version);
	UINT						ByteWidth;

	DeviceResources*			PResources;
	ID3D11InputLayout*			PInputLayout;
	ID3D11VertexShader*			PVertexShader;
	D3D11_INPUT_ELEMENT_DESC*	PInputDescription;
	FileBytes*					PByteCode;
	shared_ptr<ID3D11Buffer>	PVertexBuffer;
	shared_ptr<ID3D11Buffer>	PIndexBuffer;
};
#pragma warning( restore : 4996 )