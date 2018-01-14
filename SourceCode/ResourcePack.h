#pragma once
#include <vector>
#include <windows.h>
#include"Model.h"
using namespace std;

class MeshResource
{
public:
	MeshResource();
	int positionID;
	int normalID;
	int tangentID;
	int bitangentID;
	int colorID;
	int texCoordID;
	int indiceID;
	int boneIndexID;
	int boneWeightID;

	UINT vertexCount;
	UINT indexCount;
	int materialID;
};

inline MeshResource::MeshResource()
{
	positionID = -1;
	normalID = -1;
	tangentID = -1;
	bitangentID = -1;
	colorID = -1;
	texCoordID = -1;
	indiceID = -1;
	boneIndexID = -1;
	boneWeightID = -1;
	vertexCount = 0;
	indexCount = 0;
	materialID = -1;
}

class MaterialResource
{
public:
	MaterialResource();
	int diffuseMap;
	int specularMap;
	int ambientMap;
	int normalMap;
};
inline MaterialResource::MaterialResource()
{
	diffuseMap = -1;
	specularMap = -1;
	ambientMap = -1;
	normalMap = -1;
}

class ModelResource
{
public:
	ModelResource();
	vector<MeshResource> meshes;
	vector<MaterialResource> materials;
	UINT animationCount;
	bool hasAnimation;
	Model* model;
};
inline ModelResource::ModelResource()
{
	animationCount = 0;
	hasAnimation = false;
	model = NULL;
}