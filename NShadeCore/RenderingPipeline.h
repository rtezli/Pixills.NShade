#pragma once

#include "includes.h"

class RenderingPipeline;

class RenderingPipelineStep10
{
public:
	RenderingPipelineStep10();
	~RenderingPipelineStep10();
public:
	static RenderingPipeline* CreateForTarget(LPWSTR shaderByteCodeFile, ID3D11Buffer* constanBuffer);
};

class RenderingPipelineStep9
{
public:
	RenderingPipelineStep9();
	~RenderingPipelineStep9();
public:
	static RenderingPipelineStep10* CreateMerger();
};

class RenderingPipelineStep8
{
public:
	RenderingPipelineStep8();
	~RenderingPipelineStep8();
public:
	static RenderingPipelineStep9* LoadPixielShader(LPWSTR shaderByteCodeFile, ID3D11Buffer* constanBuffer);
	static RenderingPipelineStep9* CompilePixielShader(LPWSTR shaderSourceCode, ID3D11Buffer* constanBuffer);
};

class RenderingPipelineStep7
{
public:
	RenderingPipelineStep7();
	~RenderingPipelineStep7();
public:
	static RenderingPipelineStep8* CreateRaseterizerr();
	static RenderingPipelineStep8* Skip();
};

class RenderingPipelineStep6
{
public:
	RenderingPipelineStep6();
	~RenderingPipelineStep6();
public:
	static RenderingPipelineStep7* CreateGeometryShader(ID3D11Buffer* constanBuffer);
	static RenderingPipelineStep7* Skip();
	static RenderingPipelineStep6* WriteToBuffer(ID3D11Buffer* constanBuffer);
};

class RenderingPipelineStep5
{
public:
	RenderingPipelineStep5();
	~RenderingPipelineStep5();
public:
	static RenderingPipelineStep6* CreateDomainShader(ID3D11Buffer* constanBuffer);
	static RenderingPipelineStep6* Skip();
};


class RenderingPipelineStep4
{
public:
	RenderingPipelineStep4();
	~RenderingPipelineStep4();
public:
	static RenderingPipelineStep5* CreateTesselator();
	static RenderingPipelineStep5* Skip();
};

class RenderingPipelineStep3
{
public:
	RenderingPipelineStep3();
	~RenderingPipelineStep3();
public:
	static RenderingPipelineStep4* LoadHullhaders(ID3D11Buffer* constanBuffer);
	static RenderingPipelineStep4* Skip();
};


class RenderingPipelineStep2
{
public:
	RenderingPipelineStep2();
	~RenderingPipelineStep2();
public:
	static RenderingPipelineStep3* LoadVertexShader(LPWSTR shaderByteCodeFile, ID3D11Buffer* constanBuffer);
	static RenderingPipelineStep3* CompileVertexShader(LPWSTR shaderSourceCode, ID3D11Buffer* constanBuffer);
};

class RenderingPipelineStep1
{
public:
	RenderingPipelineStep1();
	~RenderingPipelineStep1();
public:
	static RenderingPipelineStep2* AssembleInput();
};

class RenderingPipeline
{
public:
	RenderingPipeline();
	~RenderingPipeline();
public:
	static RenderingPipelineStep1* LoadScene(vector<int> indexBuffer, vector<VertexPositionColor> vertexBuffer);
};