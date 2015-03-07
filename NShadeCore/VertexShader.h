#pragma once
#pragma warning( disable : 4996 )

#include "common.h"
#include "shader.h"

class VertexShader //: public Shader
{
public:
	VertexShader(DeviceResources* pResources);
	~VertexShader();
public:
	ID3D11InputLayout*	const GetInputLayout(){ return PInputLayout; }
	unsigned int		const GetInputSize(){ return ByteWidth; }
	ID3D11VertexShader*	const Shader(){ return PVertexShader; }
	ID3D11Buffer*		const GetVertexBuffer(){ return PVertexBuffer.get(); }
	ID3D11Buffer*		const GetIndexBuffer(){ return PIndexBuffer.get(); }

	void				CreateBuffers(vector<nshade::Vertex>* vertices, vector<unsigned int>* indices);
protected:
	void						Load(char* file);
	void						Compile(char* file, ShaderVersion version);
	unsigned int				ByteWidth;

	DeviceResources*			PResources;
	ID3D11InputLayout*			PInputLayout;
	ID3D11VertexShader*			PVertexShader;
	D3D11_INPUT_ELEMENT_DESC*	PInputDescription;
	FileBytes*					PByteCode;
	shared_ptr<ID3D11Buffer>	PVertexBuffer;
	shared_ptr<ID3D11Buffer>	PIndexBuffer;
};
#pragma warning( restore : 4996 )