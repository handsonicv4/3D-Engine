//=====================ResourceManager V 0.7========================
//Copy right belongs to Sentao Li
/*
=====================General description===========================
The purpose of ResourceManager is to provide a friendly and easy way to manage D3D pipline 
resources without compromise to much flexibility. ResourceManager can manage all pipline 
resources such as buffers & textures, as well as all views generated from them.
ResourceManager is ID based, which means  every time ResourceManager create a resource, 
it will return it's resource ID, user must keep the ID, since it's the only way to refer the resource.
You can create, delete resources or bind them to different pipline stages.
(Texture1D and texure3D are not supprted yet)
======================Create resources=============================
Use CreateBuffer or CreateTexture2D to create desired resources, or CreateBackBuffer to create
Back buffer resource. ResourceManager will automatically generate views according to the bindFlags
you passed in create function(ie. D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET 
will result Shader Resource View and Render Target View to create). If creation success it will return an
ID greater than or equal to 0, otherwise return -1;

int CreateBuffer(
							bindFlags,					//Create buffer with these bind flags(ie. D3D11_BIND_VERTEX_BUFFER)
							isDynamic,					//With CPU dynamic access or not
							bufferSize,					//Buffer size
							dataPointer,					//Initial data(can be NULL)
							dataSize,						//Initial data size
							elementStride				//If you are creating a Vertex or Index buffer, you MUST pass your PER-VERTEX/INDEX element stride or it will faill
																//If you are not creating a Vertex or Index buffer set to zero, or ResourceManager will create a structured buffer for you.
						)


int CreateTexture2D(
									bindFlags,			//Create buffer with these bind flags(ie. D3D11_BIND_VERTEX_BUFFER)
									format					//Pixel format
									isDynamic,			//With CPU dynamic access or not
									genMip,				//Should generate mipmap or not
									width,					//Texture width (pixel)
									height,					//Texture height (pixel)
									dataPointer,			//Initial data(can be NULL)
								)

======================Bind resources=============================
Use SetBinding to bind resources, if desired bindFlag is not exist, It will return false, If you are
binding  D3D11_BIND_VERTEX_BUFFER or D3D11_BIND_VERTEX_BUFFER, it will automatically 
bind the resource to Input_Assembler stage, other stage you passed in will be ignored, the same
rule can apply to Render Target and Depth Stencil flag. If the resource did not created with the 
bindFlag the method returns false and do nothing.

bool SetBinding(
							PiplineStage stage,						//Which pipline stage you want to bind to	
							D3D11_BIND_FLAG bindFlag,		//Which kind of view you want to bind for this resource
							UINT slot,										//Which slot you want to bind to
							int id											//Resource ID
							);

======================Update resources=============================
Use UpdateResourceData to update any resource, it will automatically chose update method according
to Dynamic flag.
=======================Read resources=============================
Use GetResourceData to retrieve data from resource, If the resource is not created with staging flag
The method will create a staging resource and copy data from original resource. So it is not recomended
to use this method to non-staging resource frequently

*/
//============================================================

#pragma once
#include <d3d11_1.h>
#include"D3Def.h"
#include <map>
#include <vector>
using namespace std;

//We use only one ResourceView(SRV, UAV) class to store all types of view with it's type recorded
struct ResourceView
{
	IUnknown* ptr;					//A pointer to D3D resource view
	D3D11_BIND_FLAG type;		//view type
	UINT stride;							//Vertex buffer or structured buffer will have this value
	//A view can bind to multiple stages and different slots of the pipline. And here is the map
	map<PiplineStage, int> bindingMap;
};

//We use only one ResourceData class to store all types of data(buffer & texture)
struct ResourceData
{
	ID3D11Resource* ptr = NULL;	//A pointer to D3D resource
	UINT stride;								//Vertex buffer or structured buffer will have this value
	UINT elementCount;				//Vertex buffer, Index buffer or structured buffer will have this value
	 //A texture or buffer may have multiple types of view (now we only support one for each type of view)
	//Also use map to accelerate access
	map<D3D11_BIND_FLAG, ResourceView> viewPool;
};

class ResourceManager
{
private:
	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;

	//Resource Pool
	map<UINT, ResourceData> resourcePool;
	map<DXGI_FORMAT, unsigned int> FormatSizeTable;
	//Binding Table
	int bindingTable[MAX_STAGE_NUMBER][MAX_BINDING_TYPE_NUMBER][MAX_SLOT_NUMBER];

	//Other globles
	D3D11_BOX box1D;

	//=================binding=======================
	UINT GetBindFlags(ID3D11Resource* resource);
	UINT GetIDByEnum(UINT enm);
	void Bind(UINT stages, D3D11_BIND_FLAG bindFlag, UINT startSlot, UINT numViews, void** ptr, UINT elementStride = 0, UINT offset = 0);
	vector<ID3D11RenderTargetView*> GetRenderTargets();
	bool SetBindingTable(PiplineStage stage, D3D11_BIND_FLAG bindFlag, UINT slot, int id);
	void UnbindView(ResourceView& view);

	//=================Creation======================
	int GenerateID();
	ID3D11UnorderedAccessView* CreateUAV(ID3D11Resource* resource);
	ID3D11ShaderResourceView* CreateSRV(ID3D11Resource* resource);
	D3D11_BUFFER_DESC GenerateBufferDesc(UINT  bindFlag, bool isDynamic, UINT size);
	D3D11_TEXTURE2D_DESC GenerateTexture2Desc(UINT  bindFlag, DXGI_FORMAT format, bool isDynamic, bool hasMip, UINT width, UINT height);
	ID3D11Buffer* CreateBuffer(D3D11_BUFFER_DESC& desc, void* pData, UINT dataSize);
	ID3D11Texture2D* CreateTexture2D(D3D11_TEXTURE2D_DESC& desc, void* pData = NULL);
	bool CreateViews(ResourceData &resource);
	void ClearViews(ResourceData& resource);

	//=================UpdateData===================
	bool UpdateResourceData(ID3D11Resource *resource, void* pData, UINT size);

public:
	ResourceManager();
	~ResourceManager();
	ResourceManager(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr);

	bool SetBinding(PiplineStage stage, D3D11_BIND_FLAG bindFlag, UINT startSlot, vector< int>& idList);
	bool SetBinding(PiplineStage stage, D3D11_BIND_FLAG bindFlag, UINT slot, int id);

	int CreateBuffer(UINT bindFlags, bool isDynamic, UINT bufferSize, void* pData = NULL, UINT dataSize = 0, UINT elementStride = 0);
	int CreateTexture2D(UINT  bindFlag, DXGI_FORMAT format, bool isDynamic, bool genMip, UINT width, UINT height, void* pData = NULL);
	int CreateBackBuffer(IDXGISwapChain* pSwapChain);

	bool UpdateResourceData(UINT resourceID, void* pData, UINT size);
	void CopyResourceData(UINT srcID, UINT dstID);
	vector<byte> GetResourceData(UINT srcID, UINT* xLength, UINT* yLength, UINT* zLength);

	void ResetRTV(UINT id, const float color[4]);
	void ResetDSV(UINT id, D3D11_CLEAR_FLAG flag, float depth, UINT8 stencil);

	void Clear(UINT id);
	void Clear();
};
