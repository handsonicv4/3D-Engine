#pragma once
#include <vector>
#include <windows.h>
#include"asset/Model.h"
using namespace std;

//A pre-combined mesh resource in graphics memory, shared by all it's instance
//Typically immutable
class MeshResource
{
public:
	int positionID;
	int normalID;
	int tangentID;
	int bitangentID;
	int colorID;
	int texCoordID;
	int indiceID;
	int boneIndexID;
	int boneWeightID;
	UINT indexCount;
	vector<BindingBone> boneList;
	int nodeID;
	MeshResource();
	void Render() const;
};


//A pre-combined textures resource in graphics memory, shared by all it's instance
//Typically immutable
class MaterialResource
{
public:
	int diffuseMap;
	int specularMap;
	int ambientMap;
	int normalMap;
	MaterialResource();
	void Render() const;
};

//RenderPair: used as keys to classify instances according to meshs and materials, accelerate instancing.
class RenderPair
{
public:
	MeshResource* pMeshResource;
	MaterialResource* pMaterialResource;
	RenderPair();
	RenderPair(MeshResource* pMesh, MaterialResource* pMaterial);
	bool operator==(const RenderPair &other) const;
	void Render() const;
};

//Hash function for renderPair
namespace std {
	template<>
	struct hash<RenderPair>
	{
		std::size_t operator()(const RenderPair &rp) const;
	};

	inline std::size_t hash<RenderPair>::operator()(const RenderPair & rp) const
	{
		return std::hash<MeshResource*>()(rp.pMeshResource) ^ std::hash<MaterialResource*>()(rp.pMaterialResource);
	}

}

//Represent each mesh instance, give flexibility for instancing
class MeshInstance
{
public:
	MeshResource* pResource;
	vector<aiMatrix4x4> bindMatrix;
	MeshInstance();
	MeshInstance(MeshResource* pMesh);
};

class MaterialInstance
{
public:
	MaterialResource* pResource;
	float diffusePower;
	float specularPower;
	float emissivePower;
	float specularHardness;
	float refractiveIndex;
	float opacity;
	float diffuseTextureOffset[2];
	float specularTextureOffset[2];
	float emissiveTextureOffset[2];
	float normalTextureOffset[2];
	float diffuseColor[3];
	float specularColor[3];
	float emissiveColor[3];
	float diffuseBlendFactor;
	float specularBlendFactor;
	float emissiveBlendFactor;
	bool normalTextureEnable;
	MaterialInstance();
};

class GraphicInstance
{
public:
	MeshInstance meshInstance;
	MaterialInstance materialInstance;
};

class AssetPack;

class ModelInstance
{
public:
	bool visible;
	vector<GraphicInstance> components;
	Transform transform;
	int animationID;
	float animationTime;
	AssetPack* pack;
	ModelInstance();
	void ApplyAnimation();
	void Destroy();
	bool IsDestroied();
private:
	bool isDestroied;
};

//A combined Graphics ResourcePack typically created from model files
class AssetPack
{
public:
	vector<MeshResource> meshs;
	vector<MaterialResource> materials;
	NodeList nodeList;
	vector<Animation> animationList;
	ModelInstance defaultInstance;
	AssetPack();
};