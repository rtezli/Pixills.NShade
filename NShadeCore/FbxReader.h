#pragma once

#include "common.h"

namespace nshade
{
	class FbxReader
	{
	public:
		FbxReader();
		~FbxReader();
	public:
		static HRESULT Read(CHAR *pFileName, vector<NVertex> *pVertices, vector<UINT> *pIndices);
	private:
		static HRESULT	 TraverseChildren(FbxNode *pNode, vector<FbxNode*> *pMesh);
		static HRESULT   TraverseAndStoreFbxNode(vector<FbxNode*> *pNodes, FbxAxisSystem *pAxisSystem, vector<NVertex> *pVertices, vector<UINT> *pIndices);
		static XMFLOAT3  ConvertFbxVector4ToXMFLOAT3(FbxVector4 *pCoordinate, FbxAxisSystem *pAxisSystem, FLOAT scale);
	};
}

