#pragma once

#include "includes.h"

class FbxParser
{
public:
	FbxParser();
	~FbxParser();
public:
	static HRESULT Read(char* fileName, vector<nshade::Vertex>* vertices, vector<unsigned int>* indices);
private:
	static HRESULT	 TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh);
	static HRESULT   TraverseAndStoreFbxNode(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem, vector<nshade::Vertex>* vertices, vector<unsigned int>* indices);
	static XMFLOAT3  ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, float scale);
};

