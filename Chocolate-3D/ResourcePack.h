#pragma once
#include <vector>
#include <windows.h>
#include"asset/Model.h"
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

	float diffusePower;
	float specularHardness;
	float specularPower;
	float emissivity;
	float refractiveIndex;
	float diffuseColor[3];
	float ambientColor[3];
	float specularColor[3];
	float opacity;
};
inline MaterialResource::MaterialResource()
{
	diffuseMap = -1;
	specularMap = -1;
	ambientMap = -1;
	normalMap = -1;

	diffusePower=1;
	specularHardness=1;
	specularPower = 1;
	emissivity = 0;
	refractiveIndex = 1;
	diffuseColor[0] = 1;
	diffuseColor[1] = 1;
	diffuseColor[2] = 1;
	ambientColor[0] = 1;
	ambientColor[1] = 1;
	ambientColor[2] = 1;
	specularColor[0] = 1;
	specularColor[1] = 1;
	specularColor[2] = 1;
	opacity = 1;
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