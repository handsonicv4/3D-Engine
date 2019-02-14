#include"GEngine.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include "json11/json11.hpp"
#include "Pipeline/DescFileLoader.h"
using namespace std;
using namespace json11;
using namespace FileLoader;

float Saturate(float in)
{
	if (in < 0) return 0;
	if (in > 1) return 1;
	return in;
}

GEngine::GEngine()
{
	effect = NULL;
	numBonePerVertex = 4;
	numBonePerBatch = 1024;
	maxInstances = 256;

	vsync_enabled = false;
	fullscreen = false;
	MaxLightNumber = 1024;
	oldResolutionX = 0;
	oldResolutionY = 0;
	
	resolutionX = 1280;
	resolutionY = 720;

	maxTileNumber = 8160;//...
	tileSize = 16;
	tileBatchSize = 16;
	
	voxelDimention[0] = 64;
	voxelDimention[1] = 64;
	voxelDimention[2] = 64;

	voxelSize[0] = 10.0 / voxelDimention[0];
	voxelSize[1] = 10.0 / voxelDimention[1];
	voxelSize[2] = 10.0 / voxelDimention[2];

}

GEngine::~GEngine()
{
	Shutdown();
}

bool GEngine::Init(HWND window, bool fullscreen) 
{
	resolutionX = 1280;
	resolutionY = 720;
	hwnd = window;

	bool result = PipeLine::Init(resolutionX, resolutionY, window, fullscreen);

	if (!result)
		return false;

	result = InitBuffers();
	if (!result)
		return false;

	//int def = PipeLine::SamplerState().CreateFromFile("");
	//PipeLine::SamplerState().Apply(def, 0xffff, Slot_Sampler_Default);
	//int clamp = PipeLine::SamplerState().CreateFromFile("clamp");
	//PipeLine::SamplerState().Apply(clamp, 0xffff, Slot_Sampler_Clamp);
	PipeLine::SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool GEngine::InitBuffers()
{

	ResourceDesc descCB;
	descCB.type = Resource_Buffer;
	descCB.access = Access_Dynamic;
	descCB.bindFlag = Bind_Constant_Buffer;

	//Constant Buffer PerFrame
	descCB.name = "FrameBuffer";
	descCB.size[0] = sizeof(FrameBufferData);
	frameBufferID = PipeLine::Resources().Create(descCB);
	if (frameBufferID == -1)
		return false;

	ResourceDesc descSRV;
	descSRV.type = Resource_Buffer;
	descSRV.access = Access_Dynamic;
	descSRV.bindFlag = Bind_Shader_Resource;

	//BindMatrix Buffer 
	descSRV.name = "BindMatrixBuffer";
	descSRV.size[0] = numBonePerBatch * sizeof(float[16]);
	descSRV.elementStride = sizeof(float[16]);
	animationMatrixBufferID = PipeLine::Resources().Create(descSRV);
	if (animationMatrixBufferID == -1)
		return false;

	//Instance Buffer 
	descSRV.name = "Instance Buffer";
	descSRV.size[0] = maxInstances * sizeof(InstanceData);
	descSRV.elementStride = sizeof(InstanceData);
	instanceBufferID = PipeLine::Resources().Create(descSRV);
	if (instanceBufferID == -1)
		return false;

	//Light Buffer
	descSRV.name = "Light Buffer";
	descSRV.size[0] = MaxLightNumber * sizeof(Light);
	descSRV.elementStride = sizeof(Light);
	lightBufferID = PipeLine::Resources().Create(descSRV);
	if (lightBufferID == -1)
		return false;

	PipeLine::Resources().SetBinding(Stage_Vertex_Shader, Bind_Constant_Buffer, Slot_CBuffer_Frame, frameBufferID);
	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Constant_Buffer, Slot_CBuffer_Frame, frameBufferID);
	PipeLine::Resources().SetBinding(Stage_Compute_Shader, Bind_Constant_Buffer, Slot_CBuffer_Frame, frameBufferID);
	PipeLine::Resources().SetBinding(Stage_Geometry_Shader, Bind_Constant_Buffer, Slot_CBuffer_Frame, frameBufferID);

	PipeLine::Resources().SetBinding(Stage_Vertex_Shader, Bind_Shader_Resource, Slot_Texture_AnimMatrix, animationMatrixBufferID);

	PipeLine::Resources().SetBinding(Stage_Vertex_Shader, Bind_Shader_Resource, Slot_Texture_Instance, instanceBufferID);
	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Shader_Resource, Slot_Texture_Instance, instanceBufferID);

	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Shader_Resource, Slot_Texture_Light, lightBufferID);
	PipeLine::Resources().SetBinding(Stage_Vertex_Shader, Bind_Shader_Resource, Slot_Texture_Light, lightBufferID);

	return true;
}

bool GEngine::Tiling()
{
	//dataFrm.screenDimensions.x = resolutionX;
	//dataFrm.screenDimensions.y = resolutionY;
	//XMStoreFloat4x4(&(dataFrm.projectionInv), XMMatrixTranspose(XMMatrixInverse(NULL, camera.GetProjectionMatrix())));
	//Update some data
	//UpdateConstantBufferFrm();

	//DispatchNum and UAV Size
	UINT numTileX, numTileY, numBatchX, numBatchY;
	numTileX = resolutionX / tileSize + (resolutionX % tileSize ? 1 : 0);
	numTileY = resolutionY / tileSize + (resolutionY % tileSize ? 1 : 0);
	numBatchX = numTileX / tileBatchSize + (numTileX % tileBatchSize ? 1 : 0);
	numBatchY = numTileY / tileBatchSize + (numTileY % tileBatchSize ? 1 : 0);

	PipeLine::Compute(numBatchX, numBatchY, 1);

	return true;
}

void GEngine::Shutdown()
{
	PipeLine::Shutdown();
	return;
}

void GEngine::UpdateLight(vector<Light> lights)
{
	frameData.numLights = lights.size();
	lightList = lights;
}

void GEngine::UpdateBuckets()
{
	RenderPair key(NULL, NULL);
	instanceBuckets.clear();
	bindMatrixBuckets.clear();
	vector<ModelInstance*> destroied;
	for (ModelInstance* p : instances)
	{
		if (!p->visible) continue;
		if (p->IsDestroied())
		{
			destroied.push_back(p);
			continue;
		}
		for (GraphicInstance &unit : p->components)
		{
			key.pMeshResource = unit.meshInstance.pResource;
			key.pMaterialResource = unit.materialInstance.pResource;
			if (!instanceBuckets.count(key)) instanceBuckets[key] = vector<InstanceData>();
			if (!bindMatrixBuckets.count(key)) bindMatrixBuckets[key] = vector<aiMatrix4x4>();

			//Update InstanceData
			InstanceData iData;
			ZeroMemory(&iData, sizeof(iData));

			aiMatrix4x4 wvp = camera.GetProjectionMatrix()*camera.GetViewMatrix()*p->transform.transformMatrix;

			memcpy(iData.worldMatrix, &p->transform.transformMatrix, sizeof(float[16]));
			memcpy(&iData.wVP, &wvp, sizeof(float[16]));
			memcpy(&iData.diffuseColor, &unit.materialInstance.diffuseColor, sizeof(float[3]));
			memcpy(&iData.specularColor, &unit.materialInstance.specularColor, sizeof(float[3]));
			memcpy(&iData.emissiveColor, &unit.materialInstance.emissiveColor, sizeof(float[3]));
			memcpy(&iData.diffuseTextureOffset, &unit.materialInstance.diffuseTextureOffset, sizeof(float[2]));
			memcpy(&iData.specularTextureOffset, &unit.materialInstance.specularTextureOffset, sizeof(float[2]));
			memcpy(&iData.emissiveTextureOffset, &unit.materialInstance.emissiveTextureOffset, sizeof(float[2]));
			memcpy(&iData.normalTextureOffset, &unit.materialInstance.normalTextureOffset, sizeof(float[2]));
			iData.bindMatrixOffset = bindMatrixBuckets[key].size();
			iData.diffusePower = max(unit.materialInstance.diffusePower, 0);
			iData.specularPower = max(unit.materialInstance.specularPower, 0);
			iData.emissivePower = max(unit.materialInstance.emissivePower, 0);
			iData.alphaFactor = Saturate(unit.materialInstance.opacity);
			iData.diffuseBlendFactor = unit.materialInstance.pResource->diffuseMap == -1 ? 1 : Saturate(unit.materialInstance.diffuseBlendFactor);
			iData.specularBlendFactor = unit.materialInstance.pResource->specularMap == -1 ? 1 : Saturate(unit.materialInstance.specularBlendFactor);
			iData.emissiveBlendFactor = unit.materialInstance.pResource->ambientMap == -1 ? 1 : Saturate(unit.materialInstance.emissiveBlendFactor);
			iData.refractiveIndex = unit.materialInstance.refractiveIndex;
			iData.specularHardness = max(unit.materialInstance.specularHardness, 0);
			iData.flags |= (p->pack && p->pack->nodeList.size() && unit.meshInstance.pResource->boneIndexID != -1); // Animation flag
			iData.flags |= (iData.diffuseBlendFactor < 1) << 1;	//Diffuse texture flag
			iData.flags |= (iData.specularBlendFactor < 1) << 2;	 //Specular texture flag
			iData.flags |= (unit.materialInstance.pResource->normalMap != -1 && unit.materialInstance.normalTextureEnable) << 3; //Normal map flag
			iData.flags |= (iData.emissiveBlendFactor < 1) << 4; //Emissive texture flag

			instanceBuckets[key].push_back(move(iData));

			//Update bind matrix
			if(unit.meshInstance.bindMatrix.size() > 0)
				bindMatrixBuckets[key].insert(bindMatrixBuckets[key].end(), unit.meshInstance.bindMatrix.begin(), unit.meshInstance.bindMatrix.end());
		}
	}

	for (ModelInstance* p : destroied)
	{
		instances.erase(p);
		delete p;
	}
}

void GEngine::ApplyAnimation()
{
	for (ModelInstance* p : instances)
	{
		if (!p->IsDestroied() && p->visible)
		{
			p->ApplyAnimation();
		}
	}
}

void GEngine::Instancing(const RenderPair & rpair, const vector<InstanceData>& instanceData, const vector<aiMatrix4x4>& bindMatrix)
{
	if (rpair.pMeshResource == NULL || instanceData.size() == 0)
		return;

	//Bind mesh to pipeLine
	rpair.Render();

	//Update instance buffer and bindMatrix buffer
	if(instanceData.size() > 0)
		PipeLine::Resources().UpdateResourceData(instanceBufferID, &instanceData[0], sizeof(InstanceData)*instanceData.size());
	if(bindMatrix.size() > 0)
		PipeLine::Resources().UpdateResourceData(animationMatrixBufferID, &bindMatrix[0], sizeof(float[16])*bindMatrix.size());
	//Draw
	PipeLine::Draw(rpair.pMeshResource->indexCount, instanceData.size());
}

void GEngine::LoadPostMesh(string file)
{
	auto p = LoadAsset(file);
	postMesh = p->meshs[0];
}

bool GEngine::UpdateFrameBuffer()
{
	aiMatrix4x4 projection = camera.GetProjectionMatrix();
	aiMatrix4x4 projectionInv = projection;
	projectionInv.Inverse();

	memcpy(&frameData.projection, &projection, sizeof(float[16]));
	memcpy(&frameData.projectionInv, &projectionInv, sizeof(float[16]));
	memcpy(&frameData.voxelDimention, &voxelDimention, sizeof(float[3]));
	memcpy(&frameData.voxelSize, &voxelSize, sizeof(float[3]));
	//memcpy(&frameData.viewProjection, &viewProjection, sizeof(float[16]));

	aiVector3D position = camera.GetPosition();
	memcpy(frameData.cameraPos, &position, sizeof(float[3]));
	frameData.numLights = lightList.size();
	frameData.screenDimensions[0] = resolutionX;
	frameData.screenDimensions[1] = resolutionY;

	//maxNumLightsPerTile;
	return PipeLine::Resources().UpdateResourceData(frameBufferID, &frameData, sizeof(frameData));
}

bool GEngine::UpdateLightBuffer()
{
	return PipeLine::Resources().UpdateResourceData(lightBufferID, &lightList[0], sizeof(Light)*lightList.size());
}

ModelInstance * GEngine::CreateInstance(const ModelInstance &bluePrint)
{
	ModelInstance * instance = new ModelInstance(bluePrint);
	instances.insert(instance);
	return instance;
}


AssetPack * GEngine::LoadAsset(string file)
{
	AssetPack* assetPack = new AssetPack();
	Model model;
	model.LoadFileD3D(file);

	ResourceDesc descVB, descIB, descTX;
	descVB.name = "Vertex Buffer";
	descVB.type = Resource_Buffer;
	descVB.bindFlag = Bind_Vertex_Buffer;
	descVB.access = Access_Default;

	descIB.name = "Index Buffer";
	descIB.type = Resource_Buffer;
	descIB.bindFlag = Bind_Index_Buffer;
	descIB.access = Access_Default;
	descIB.elementStride = sizeof(UINT);

	descTX.name = "DiffuseMap";
	descTX.type = Resource_Texture2D;
	descTX.bindFlag = Bind_Shader_Resource | Bind_Render_Target;
	descTX.access = Access_Default;
	descTX.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descTX.mipLevel = 0;

	assetPack->meshs.resize(model.meshList.size());
	assetPack->animationList = model.animationList;
	assetPack->nodeList = model.nodeList;
	
	for (int i = 0; i < model.meshList.size(); i++)
	{
		MeshResource &dstMesh = assetPack->meshs[i];
		Mesh &srcMesh = model.meshList[i];

		dstMesh.boneList = srcMesh.boneList;
		dstMesh.nodeID = srcMesh.nodeID;

		unsigned int dataSize;
		void* dataPtr;
		if (!srcMesh.vertexPositions.empty())
		{
			dataSize = srcMesh.vertexPositions.size() * sizeof(float[3]);
			dataPtr = &srcMesh.vertexPositions[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = sizeof(float[3]);
			dstMesh.positionID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);
		}
		if (!srcMesh.vertexNormals.empty())
		{
			dataSize = srcMesh.vertexNormals.size() * sizeof(float[3]);
			dataPtr = &srcMesh.vertexNormals[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = sizeof(float[3]);
			dstMesh.normalID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);
		}
		if (!srcMesh.vertexTangent.empty() && !srcMesh.vertexBitangent.empty())
		{
			dataSize = srcMesh.vertexTangent.size() * sizeof(float[3]);
			dataPtr = &srcMesh.vertexTangent[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = sizeof(float[3]);
			dstMesh.tangentID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);

			dataSize = srcMesh.vertexBitangent.size() * sizeof(float[3]);
			dataPtr = &srcMesh.vertexBitangent[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = sizeof(float[3]);
			dstMesh.bitangentID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);
		}
		if (!srcMesh.vertexTexCoords.empty())
		{
			dataSize = srcMesh.vertexTexCoords.size() * sizeof(float[2]);
			dataPtr = &srcMesh.vertexTexCoords[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = sizeof(float[2]);
			dstMesh.texCoordID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);
		}
		if (model.hasAnimation)
		{
			dataSize = srcMesh.vertexBindID.size() * sizeof(UINT);
			dataPtr = &srcMesh.vertexBindID[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = numBonePerVertex * sizeof(UINT);
			dstMesh.boneIndexID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);

			dataSize = srcMesh.vertexBindWight.size() * sizeof(float);
			dataPtr = &srcMesh.vertexBindWight[0];
			descVB.size[0] = dataSize;
			descVB.elementStride = numBonePerVertex * sizeof(float);
			dstMesh.boneWeightID = PipeLine::Resources().Create(descVB, dataPtr, dataSize);
		}
		if (!srcMesh.indices.empty())
		{
			dataSize = srcMesh.indices.size() * sizeof(UINT);
			dataPtr = &srcMesh.indices[0];
			descIB.size[0] = dataSize;
			dstMesh.indiceID = PipeLine::Resources().Create(descIB, dataPtr, dataSize);
			dstMesh.indexCount = srcMesh.indices.size();
		}
	}
	
	assetPack->materials.resize(model.materialList.size());
	for (int i = 0; i < model.materialList.size(); i++)
	{
		MaterialResource &dstMaterial = assetPack->materials[i];
		Material &srcMaterial = model.materialList[i];

		if (srcMaterial.hasDiffuseMap)
		{
			descTX.size[0] = srcMaterial.diffuseMap.width;
			descTX.size[1] = srcMaterial.diffuseMap.height;
			dstMaterial.diffuseMap = PipeLine::Resources().Create(descTX, srcMaterial.diffuseMap.GetImageDataPtr());
		}

		if (srcMaterial.hasSpecularMap)
		{
			descTX.size[0] = srcMaterial.specularMap.width;
			descTX.size[1] = srcMaterial.specularMap.height;
			dstMaterial.specularMap = PipeLine::Resources().Create(descTX, srcMaterial.specularMap.GetImageDataPtr());
		}

		if (srcMaterial.hasNormalMap)
		{
			descTX.size[0] = srcMaterial.normalMap.width;
			descTX.size[1] = srcMaterial.normalMap.height;
			dstMaterial.normalMap = PipeLine::Resources().Create(descTX, srcMaterial.normalMap.GetImageDataPtr());
		}

		if (srcMaterial.hasAmbientMap)
		{
			descTX.size[0] = srcMaterial.ambientMap.width;
			descTX.size[1] = srcMaterial.ambientMap.height;
			dstMaterial.ambientMap = PipeLine::Resources().Create(descTX, srcMaterial.ambientMap.GetImageDataPtr());
		}
	}
	vector<GraphicInstance> &components = assetPack->defaultInstance.components;
	components.resize(model.meshList.size());
	for (int i = 0; i < model.meshList.size(); i++)
	{
		Mesh &srcMesh = model.meshList[i];
		Material &srcMaterial = model.materialList[srcMesh.materialID];
		components[i].meshInstance = MeshInstance(&assetPack->meshs[i]);
		components[i].materialInstance.pResource = &assetPack->materials[srcMesh.materialID];
		components[i].materialInstance.opacity = srcMaterial.opacity;
		components[i].materialInstance.diffusePower = srcMaterial.diffusePower;

		components[i].materialInstance.emissivePower = srcMaterial.emissivity;
		components[i].materialInstance.refractiveIndex = srcMaterial.refractiveIndex;
		components[i].materialInstance.specularPower = srcMaterial.specularPower;
		components[i].materialInstance.specularHardness = srcMaterial.specularHardness;
		memcpy(components[i].materialInstance.diffuseColor, srcMaterial.diffuse, sizeof(float[3]));
		memcpy(components[i].materialInstance.specularColor, srcMaterial.specular, sizeof(float[3]));
		memcpy(components[i].materialInstance.emissiveColor, srcMaterial.ambient, sizeof(float[3]));
	}

	return assetPack;
}

void GEngine::UnloadModel(UINT modelID)
{

}

void GEngine::Render(const PassOperation & cfg)
{
	//Render
	//const Pass & pass = effect->passes[cfg.passID];
	//for (auto& op : pass.operations)
	//{
	//	if (op.type == Pass_Operation_Draw)
	//	{
	//		pass.Bind(cfg);
	//		if (pass.type == Pass_Default)
	//		{
	//			for (auto& e : instanceBuckets)
	//			{
	//				Instancing(e.first, e.second, bindMatrixBuckets[e.first]);
	//			}
	//		}
	//		else if (pass.type == Pass_PostProcessing)
	//		{
	//			postMesh.Render();
	//			PipeLine::Draw(6,1);
	//		}
	//		pass.Unbind();
	//	}
	//	else if (op.type == Pass_Operation_Reset)
	//	{
	//		PipeLine::Resources().Reset(op.targetID, op.value);
	//	}
	//	else if (op.type == Pass_Operation_Generate_Mip)
	//	{
	//		PipeLine::Resources().GenerateMipMap(op.targetID);
	//	}
	//}

}

void GEngine::Render(const string &renderer)
{
	ApplyAnimation();
	UpdateBuckets();
	auto &operations = effect->renderer[renderer];
	for (auto& op : operations)
	{
		op->Execute();
		if (op->type == Operation_Pass)
		{
			for (auto& e : instanceBuckets)
			{
				Instancing(e.first, e.second, bindMatrixBuckets[e.first]);
			}
		}
		else if (op->type == Operation_Post_Proc)
		{
			postMesh.Render();
			PipeLine::Draw(6, 1);
		}
	}
}

bool GEngine::LoadEffect(const string & filePath)
{
	CloseEffect();
	effect = Effect::Create(filePath);
	effect->Apply();
	return true;
}

void GEngine::CloseEffect()
{
	if (effect)
	{
		delete effect;
		effect = NULL;
	}
}
//