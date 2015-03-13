#pragma once
#pragma warning( disable : 4996 )

#include "common.h"
#include "material.h"
#include "phongvertexshader.h"

EXTERN class API Model
{
public:
	Model(DeviceResources *resources);

	HRESULT AssignMaterial(Material *material);
	HRESULT LoadModelFromFBXFile(CHAR *fileName);
	HRESULT LoadModelFromOBJFile(CHAR *fileName, BOOL isRightHand);
	HRESULT CreateCube(FLOAT size, XMFLOAT3 *position);
	HRESULT CreateHorizontalPlane(FLOAT size, XMFLOAT3 *position);
	HRESULT	Initialize();

	static NVertex Cube[];
	static NVertex Sphere[];
private:
	HRESULT						TraverseAndStoreFbxNode1(vector<FbxNode*> *nodes, FbxAxisSystem *axisSystem);
	HRESULT						TraverseAndStoreFbxNode2(vector<FbxNode*> *nodes, FbxAxisSystem *axisSystem);
	HRESULT						TraverseChildren(FbxNode *node, vector<FbxNode*> *mesh);
	HRESULT						FillVertexAndIndexBuffer(vector<UINT> *modelIndexes, vector<NVertex> *modelVertices);
	XMFLOAT3					ConvertFbxVector4ToXMFLOAT3(FbxVector4 *coordinate, FbxAxisSystem *axisSystem, FLOAT scale);
	DeviceResources*			DeviceResource(){ return _deviceResources.get(); }
	HRESULT						InitializeVertexBuffer();
	HRESULT						InitializeIndexBuffer(INT indeces[]);
	HRESULT						InitializeConstantBuffer();
	HRESULT						SetTopology(CHAR verticesPerFace);
private:
	unsigned short				_indexCount = 0;
	D3D11_BUFFER_DESC			_bufferDesc;
	D3D11_SUBRESOURCE_DATA		_initData;
	shared_ptr<DeviceResources>	_deviceResources;
	Material*					_material;
	vector<UINT>*				_indices;
	vector<NVertex>*			_vertices;
	D3D11_PRIMITIVE_TOPOLOGY	_topology;
};

#pragma warning( restore : 4996 )