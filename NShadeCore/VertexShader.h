#pragma once

#include "common.h"
#include "shader.h"

class VertexShader //: public Shader
{
public:
	VertexShader(DeviceResources* pResources);
	~VertexShader();
public:
	void				Render(ID3D11Buffer* constBuffer);
	void				Setup(vector<nshade::Vertex>* vertices, vector<unsigned int>* indices);
	ID3D11InputLayout*	const GetInputLayout(){ return PInputLayout; }
protected:
	void						Load(char* file);
	void						Compile(char* file, ShaderVersion version);
	DeviceResources*			PResources;
	unsigned int				ByteWidth;
	ID3D11InputLayout*			PInputLayout;
	ID3D11Buffer*				PVertexBuffer;
	ID3D11Buffer*				PIndexBuffer;
	ID3D11VertexShader*			PVertexShader;
	D3D11_INPUT_ELEMENT_DESC*	PInputDescription;
	FileBytes*					ByteCode;
};
