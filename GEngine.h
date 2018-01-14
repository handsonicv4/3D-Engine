#pragma once
#include<windows.h>
#include <vector>
#include <map>
using namespace std;

#include"D3DManager.h"
#include"ResourcePack.h"
#include"Model.h"
#include"BufferStructure.h"

class GEngine :public D3DManager
{
public:
	GEngine();
	~GEngine();

	Camera camera;
	vector<Light> lightList;

	bool Init(HWND window, bool fullscreen);
	void Shutdown();
	int LoadModel(Model &model);
	void UnloadModel(UINT modelID);
	void UpdateLight(vector<Light> lights);
	void Frame();
	int CreateSurfaceRec(float width, float hieght, float leftTopX, float leftTopY);
	bool Tiling();
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
	unsigned int frameBufferID;
	unsigned int objBufferID;
	unsigned int animationMatrixBufferID;
	unsigned int instanceBufferID;
	unsigned int depthStencilBufferID;
	unsigned int lightBufferID;

	//Models
	map<int, ModelResource> modelList;

	//Data for Buffers
	FrameBufferData frameData;
	ObjBufferData objData;
	vector<InstanceData> instanceData;
	vector<aiMatrix4x4> animationMatrix;// bone matrix;

	bool InitBuffers();
	void RenderMesh(MeshResource *pMesh);
	void RenderMaterial(MaterialResource *pMaterial);
	void RenderModel(ModelResource *pModel);
	void UpdateInstanceBuffer(Model *pModel, unsigned int meshID);
	bool UpdateFrameBuffer();
	bool UpdateObjBuffer();
	bool UpdateLightBuffer();

};

