#pragma once
#include <windows.h>
#include <vector>
#include <map>
using namespace std;

#include"ResourcePack.h"
#include"BufferStructure.h"
#include"pipeline/Pass.h"
#include"pipeline/Pipeline.h"
#include"asset/Model.h"

class GEngine
{
public:
	HWND hwnd;
	int resolutionX, resolutionY;
	Effect* effect;

	GEngine();
	~GEngine();

	Camera camera;
	vector<Light> lightList;

	bool Init(HWND window, bool fullscreen);
	bool LoadEffect(const string &file);
	void CloseEffect();
	void Shutdown();
	int LoadModel(Model &model);
	void UnloadModel(UINT modelID);
	void UpdateLight(vector<Light> lights);
	void Render();
	void Render(const Pass &pass);
	//void RenderOpac(Pass pass);
	//void RenderTransparent(Pass pass);
	int CreateSurfaceRec(float width, float hieght, float leftTopX, float leftTopY);
	bool Tiling();
	unsigned int depthStencilBufferID;
	float voxelSize[3];
	UINT voxelDimention[3];

	bool UpdateFrameBuffer();
	bool UpdateLightBuffer();
	map<int, ModelResource> modelList;
private:
	bool vsync_enabled;
	bool fullscreen;
	UINT oldResolutionX, oldResolutionY;
	float oldProjection[16];

	UINT numBonePerVertex;
	UINT numBonePerBatch;
	UINT maxInstances;

	//Light list
	UINT MaxLightNumber;
	UINT tileSize;
	UINT maxTileNumber;
	UINT tileBatchSize;

	//Buffers ID
	 int frameBufferID;
	 int objBufferID;
	 int animationMatrixBufferID;
	 int instanceBufferID;
	 int instanceMaterialID;
	 int lightBufferID;


	//Data for Buffers
	FrameBufferData frameData;
	ObjBufferData objData;
	vector<InstanceData> instanceData;
	vector<aiMatrix4x4> animationMatrix;// bone matrix;
	vector<InstanceMaterial> instanceMaterialData;// bone matrix;

	bool InitBuffers();
	void RenderMesh(MeshResource *pMesh);
	void RenderMaterial(MaterialResource *pMaterial);
	void RenderModel(ModelResource *pModel);
	void UpdateInstanceBuffer(Model *pModel, unsigned int meshID);

	bool UpdateObjBuffer();


};

