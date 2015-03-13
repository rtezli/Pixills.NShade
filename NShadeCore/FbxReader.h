#pragma once

#include "common.h"

namespace nshade
{
	EXTERN class API FbxReader
	{
	public:
		static HRESULT Read(CHAR* fileName, vector<NVertex>* vertices, vector<UINT>* indices);
	private:
		static HRESULT	 TraverseChildren(FbxNode* node, vector<FbxNode*>* mesh);
		static HRESULT   TraverseAndStoreFbxNode(vector<FbxNode*>* nodes, FbxAxisSystem* axisSystem, vector<NVertex>* vertices, vector<UINT>* indices);
		static XMFLOAT3  ConvertFbxVector4ToXMFLOAT3(FbxVector4* coordinate, FbxAxisSystem* axisSystem, FLOAT scale);
	};
}

