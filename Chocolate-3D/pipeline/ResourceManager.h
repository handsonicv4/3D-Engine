//=====================ResourceManager ==========================
//Author: Sentao
//============================================================

#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <unordered_map>
#include <vector>
#include"D3Def.h"
#include"Pipeline.h"
#include"IDContainer.h"
#include"Singleton.h"
using namespace std;

enum ResourceType
{
	Resource_Buffer = D3D11_RESOURCE_DIMENSION_BUFFER,
	Resource_Texture1D = D3D11_RESOURCE_DIMENSION_TEXTURE1D,
	Resource_Texture2D = D3D11_RESOURCE_DIMENSION_TEXTURE2D,
	Resource_Texture3D = D3D11_RESOURCE_DIMENSION_TEXTURE3D
};

enum AccessType
{
	Access_Default = D3D11_USAGE_DEFAULT,
	Access_Immutable = D3D11_USAGE_IMMUTABLE,
	Access_Dynamic = D3D11_USAGE_DYNAMIC,
	Access_Staging = D3D11_USAGE_STAGING
};

enum BindFlag
{
	Bind_Vertex_Buffer = D3D11_BIND_VERTEX_BUFFER,
	Bind_Index_Buffer = D3D11_BIND_INDEX_BUFFER,
	Bind_Constant_Buffer = D3D11_BIND_CONSTANT_BUFFER,
	Bind_Shader_Resource = D3D11_BIND_SHADER_RESOURCE,
	Bind_Stream_Out = D3D11_BIND_STREAM_OUTPUT,
	Bind_Render_Target = D3D11_BIND_RENDER_TARGET,
	Bind_Depth_Stencil = D3D11_BIND_DEPTH_STENCIL,
	Bind_Unordered_Access = D3D11_BIND_UNORDERED_ACCESS,
	Bind_Decoder = D3D11_BIND_DECODER,
	Bind_Video_Encoder = D3D11_BIND_VIDEO_ENCODER
};

struct ResourceDesc 
{
	string name;
	ResourceType type;
	AccessType access;
	UINT bindFlag;
	DXGI_FORMAT format;
	UINT size[3];
	UINT mipLevel;
	UINT elementStride;
	UINT miscFlag;
	UINT sampleCount;
	UINT sampleQuality;
	inline ResourceDesc()
	{
		type = Resource_Buffer;
		access = Access_Default;
		bindFlag = 0;
		mipLevel = 0;
		elementStride = 0;
		miscFlag = 0;
		sampleCount = 1;
		sampleQuality = 0;
		memset(size, 0, sizeof(size));
	}
};

class Resource
{
protected:
	friend class ResourceManager;
	static Resource* Create(ResourceDesc desc, void* pData = NULL, size_t dataSize = 0);
	static  Resource* GetBackBuffer();
	bool ResetData(const UINT value[4]);
	bool UpdateData(const void * pData, size_t size);
	bool GenerateMips();
	void Release();
	virtual ~Resource();

private:
	ID3D11Resource* ptr;
	ResourceDesc desc;
	unordered_map<D3D11_BIND_FLAG, IUnknown*> viewPool;
	static unordered_map<DXGI_FORMAT, UINT> FormatSizeTable;
	static Resource* pBackBuffer;
	static ResourceDesc GetDesc(ID3D11Resource* pD3DResource);
	static void InitFormatTable();
	bool CreateBuffer(void* pData = NULL, size_t dataSize = 0);
	bool CreateTexture2D(void* pData = NULL, size_t dataSize = 0);
	bool CreateTexture3D(void* pData = NULL, size_t dataSize = 0);
	bool GenerateViews();
	void ClearViews();
	Resource(ResourceDesc desc, void* pData = NULL, size_t dataSize = 0);
	Resource();
};

class ResourceManager : public	IDContainer<Resource*>
{
	SINGLETON(ResourceManager)
public:
	bool SetBinding(PipelineStage stage, BindFlag bindFlag, UINT startSlot, vector<int>& idList);
	bool SetBinding(PipelineStage stage, BindFlag bindFlag, UINT slot, int id);
	int Create(ResourceDesc desc, void* pData = NULL, size_t dataSize = 0);
	int CreateFromFile(const string& filePath);
	int GetBackBuffer();
	bool UpdateResourceData(UINT resourceID, const void* pData, UINT size);
	void CopyResourceData(UINT srcID, UINT dstID);
	bool GenerateMipMap(UINT id);
	void Reset(UINT id, const float value[4]);
	void Reset(UINT id, const UINT value[4]);
	void Reset(UINT id, UINT flag, float depth, UINT8 stencil);
private:
	void Release(Resource* &element) override;
	ID3D11DepthStencilView* currentDSV;
	vector<ID3D11RenderTargetView*> currentRTVs;
	int backBufferID;
	void Bind(UINT stages, D3D11_BIND_FLAG bindFlag, UINT startSlot, UINT numViews, void** ptr, UINT elementStride = 0, UINT offset = 0);
};
