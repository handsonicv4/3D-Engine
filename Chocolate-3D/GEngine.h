#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <unordered_set>
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
	Camera mainLight;
	vector<Light> lightList;

	bool Init(HWND window, bool fullscreen);
	bool LoadEffect(const string &file);
	void CloseEffect();
	void Shutdown();
	AssetPack* LoadAsset(string file);
	void UnloadModel(UINT modelID);
	void UpdateLight(vector<Light> lights);

	void Render(const PassOperation &cfg);

	void Render(const string &renderer);
	
	bool Tiling();
	unsigned int depthStencilBufferID;
	float voxelSize[3];
	UINT voxelDimention[3];

	bool UpdateFrameBuffer();
	bool UpdateLightBuffer();
	unordered_set<AssetPack*> resourcePacks;
	unordered_set<ModelInstance*> instances;
	ModelInstance* CreateInstance(const ModelInstance &bluePrint);
	void Instancing(const RenderPair &rpair, const vector<InstanceData> &instanceData, const vector<aiMatrix4x4> &bindMatrix);
	void LoadPostMesh(string file);
private:
	MeshResource postMesh;
	
	void UpdateBuckets();
	void ApplyAnimation();
	unordered_map<RenderPair, vector<InstanceData>> instanceBuckets;
	unordered_map<RenderPair, vector<aiMatrix4x4>> bindMatrixBuckets;
	
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
	 int animationMatrixBufferID;
	 int instanceBufferID;
	 int lightBufferID;

	//Data for Buffers
	FrameBufferData frameData;

	bool InitBuffers();
};

