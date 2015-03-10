#pragma once
#pragma warning( disable : 4996 )

#include "common.h"
#include "shader.h"

class VertexShader
{
public:
	VertexShader(DeviceResources* pResources);
public:
	ID3D11InputLayout*			const GetInputLayout(){ return _InputLayout; }
	UINT						const GetInputSize(){ return _InputDataSize; }
	ID3D11VertexShader*			const Shader(){ return _VertexShader; }
	ID3D11Buffer*				const GetVertexBuffer(){ return _VertexBuffer.get(); }
	ID3D11Buffer*				const GetIndexBuffer(){ return _IndexBuffer.get(); }
	CHAR*						const GetExtraData(){ return _ExtraInputData; }
	UINT						const GetExtraDataSize(){ return _ExtraInputDataSize; }

	VOID						CreateBuffers(vector<NVertex> *vertices, vector<UINT> *indices);
	VOID						AppendExtraData(CHAR *data, UINT size);
	//VOID						SetExtraDataSize(UINT size);
protected:
	VOID						Load(CHAR *file);
	VOID						Compile(CHAR *file, ShaderVersion version);
	
	CHAR*						_InputData;
	UINT						_InputDataSize;
	
	/* Extra input data to be added (excluding vertex data) like light, shadow data etc. */
	CHAR*						_ExtraInputData;
	UINT						_ExtraInputDataSize;

	DeviceResources*			_Resources;
	ID3D11InputLayout*			_InputLayout;
	ID3D11VertexShader*			_VertexShader;
	D3D11_INPUT_ELEMENT_DESC*	_InputDescription;
	FileBytes*					_ByteCode;
	shared_ptr<ID3D11Buffer>	_VertexBuffer;
	shared_ptr<ID3D11Buffer>	_IndexBuffer;
};
#pragma warning( restore : 4996 )