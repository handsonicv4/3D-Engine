#include "ResourcePack.h"
#include "pipeline/Pipeline.h"

MeshResource::MeshResource()
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
	indexCount = 0;
	nodeID = -1;
}

void MeshResource::Render() const
{ 
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_Position, positionID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_Normal, normalID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_Tangent, tangentID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_Binormal, bitangentID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_TexCoord, texCoordID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_BlendIndices, boneIndexID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Vertex_Buffer, Slot_Input_BlendWeight, boneWeightID);
	PipeLine::Resources().SetBinding(Stage_Input_Assembler, Bind_Index_Buffer, 0, indiceID);
}

MaterialResource::MaterialResource()
{
	diffuseMap = -1;
	specularMap = -1;
	ambientMap = -1;
	normalMap = -1;
}

void MaterialResource::Render() const
{
	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Shader_Resource, Slot_Texture_Diffuse, diffuseMap);
	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Shader_Resource, Slot_Texture_Normal, normalMap);
	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Shader_Resource, Slot_Texture_Ambient, ambientMap);
	PipeLine::Resources().SetBinding(Stage_Pixel_Shader, Bind_Shader_Resource, Slot_Texture_Specular, specularMap);
}

ModelInstance::ModelInstance()
{
	visible = true;
	animationID = -1;
	animationTime = 0;
	pack = NULL;
	isDestroied = false;
}

void ModelInstance::ApplyAnimation()
{
	if (animationID >= pack->animationList.size() || animationID < 0 || animationTime < 0) return;
	Animation &animation = pack->animationList[animationID];
	NodeList &nodeList = pack->nodeList;
	double tick = animationTime * animation.ticksPerSecond;
	if (tick >= animation.duration)
		tick = fmod(tick, animation.duration);
	//Calculate Node Global Transform Matrix according to animation frame
	for (size_t i = 0; i < animation.nodeAnimationList.size(); i++)
	{
		NodeAnimation &nodeAnimation = animation.nodeAnimationList[i];
		aiMatrix4x4 &localTransformMatrix = nodeList[nodeAnimation.nodeID].localTransformMatrix;
		localTransformMatrix = nodeAnimation.Evaluate(tick).ToMatrix();
	}
	vector<aiMatrix4x4> nodeGlobals;
	nodeList.GetGlobalMatrix(nodeGlobals); 

	//Calculate bind matrix for each mesh instance
	for (GraphicInstance &unit : components)
	{
		MeshInstance &meshInstance = unit.meshInstance;
		MeshResource &mesh = *meshInstance.pResource;
		if (meshInstance.bindMatrix.size() != mesh.boneList.size())
			meshInstance.bindMatrix.resize(mesh.boneList.size());

		aiMatrix4x4 globalInverse = nodeGlobals[mesh.nodeID];
		globalInverse.Inverse();
		for (size_t i = 0; i < mesh.boneList.size(); i++)
		{
			const int &boneNodeID = mesh.boneList[i].nodeID;
			aiMatrix4x4 &boneNodeMatrix = nodeGlobals[boneNodeID];
			meshInstance.bindMatrix[i] = globalInverse * boneNodeMatrix*mesh.boneList[i].offset;
		}
	}
}

void ModelInstance::Destroy()
{
	isDestroied = true;
}

bool ModelInstance::IsDestroied()
{
	return isDestroied;
}

RenderPair::RenderPair()
{
	pMeshResource = NULL;
	pMaterialResource = NULL;
}

RenderPair::RenderPair(MeshResource * pMesh, MaterialResource * pMaterial)
{
	pMeshResource = pMesh;
	pMaterialResource = pMaterial;
}

bool RenderPair::operator==(const RenderPair & other) const
{
	return pMeshResource == other.pMeshResource && pMaterialResource == other.pMaterialResource;
}

void RenderPair::Render() const
{
	if (pMeshResource != NULL)
		pMeshResource->Render();
	if (pMaterialResource != NULL)
		pMaterialResource->Render();
}

MeshInstance::MeshInstance()
{
	pResource = NULL;
}

MeshInstance::MeshInstance(MeshResource * pMesh)
{
	pResource = pMesh;
	bindMatrix.resize(pResource->boneList.size());
}

MaterialInstance::MaterialInstance()
{
	diffusePower = 1;
	specularPower = 0;
	emissivePower = 0;
	specularHardness = 1;
	refractiveIndex = 1;
	opacity = 1;
	diffuseTextureOffset[0] = 0;
	diffuseTextureOffset[1] = 0;
	specularTextureOffset[0] = 0;
	specularTextureOffset[1] = 0;
	emissiveTextureOffset[0] = 0;
	emissiveTextureOffset[1] = 0;
	normalTextureOffset[0] = 0;
	normalTextureOffset[1] = 0;
	diffuseColor[0] = 1;
	diffuseColor[1] = 1;
	diffuseColor[2] = 1;
	specularColor[0] = 1;
	specularColor[1] = 1;
	specularColor[2] = 1;
	emissiveColor[0] = 1;
	emissiveColor[1] = 1;
	emissiveColor[2] = 1;
	diffuseBlendFactor = 0;
	specularBlendFactor = 0;
	emissiveBlendFactor = 0;
	normalTextureEnable = true;
}

AssetPack::AssetPack()
{
	defaultInstance.pack = this;
}
