#include"ResourceManager.h"

ResourceManager::ResourceManager()
{
	memset(bindingTable, -1, sizeof(bindingTable));

	FormatSizeTable[DXGI_FORMAT_R32G32B32A32_TYPELESS] = 128;
	FormatSizeTable[DXGI_FORMAT_R32G32B32A32_FLOAT] = 128;
	FormatSizeTable[DXGI_FORMAT_R32G32B32A32_UINT] = 128;
	FormatSizeTable[DXGI_FORMAT_R32G32B32A32_SINT] = 128;

	FormatSizeTable[DXGI_FORMAT_R32G32B32_TYPELESS] = 96;
	FormatSizeTable[DXGI_FORMAT_R32G32B32_FLOAT] = 96;
	FormatSizeTable[DXGI_FORMAT_R32G32B32_UINT] = 96;
	FormatSizeTable[DXGI_FORMAT_R32G32B32_SINT] = 96;

	FormatSizeTable[DXGI_FORMAT_R16G16B16A16_TYPELESS] = 64;
	FormatSizeTable[DXGI_FORMAT_R16G16B16A16_FLOAT] = 64;
	FormatSizeTable[DXGI_FORMAT_R16G16B16A16_UNORM] = 64;
	FormatSizeTable[DXGI_FORMAT_R16G16B16A16_UINT] = 64;
	FormatSizeTable[DXGI_FORMAT_R16G16B16A16_SNORM] = 64;
	FormatSizeTable[DXGI_FORMAT_R16G16B16A16_SINT] = 64;

	FormatSizeTable[DXGI_FORMAT_R32G32_TYPELESS] = 64;
	FormatSizeTable[DXGI_FORMAT_R32G32_FLOAT] = 64;
	FormatSizeTable[DXGI_FORMAT_R32G32_UINT] = 64;
	FormatSizeTable[DXGI_FORMAT_R32G32_SINT] = 64;

	FormatSizeTable[DXGI_FORMAT_R32G8X24_TYPELESS] = 64;
	FormatSizeTable[DXGI_FORMAT_D32_FLOAT_S8X24_UINT] = 64;
	FormatSizeTable[DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS] = 64;
	FormatSizeTable[DXGI_FORMAT_X32_TYPELESS_G8X24_UINT] = 64;

	FormatSizeTable[DXGI_FORMAT_R10G10B10A2_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_R10G10B10A2_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_R10G10B10A2_UINT] = 32;
	FormatSizeTable[DXGI_FORMAT_R11G11B10_FLOAT] = 32;

	FormatSizeTable[DXGI_FORMAT_R8G8B8A8_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_R8G8B8A8_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_R8G8B8A8_UNORM_SRGB] = 32;
	FormatSizeTable[DXGI_FORMAT_R8G8B8A8_UINT] = 32;
	FormatSizeTable[DXGI_FORMAT_R8G8B8A8_SNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_R8G8B8A8_SINT] = 32;

	FormatSizeTable[DXGI_FORMAT_R16G16_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_R16G16_FLOAT] = 32;
	FormatSizeTable[DXGI_FORMAT_R16G16_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_R16G16_UINT] = 32;
	FormatSizeTable[DXGI_FORMAT_R16G16_SNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_R16G16_SINT] = 32;

	FormatSizeTable[DXGI_FORMAT_R32_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_D32_FLOAT] = 32;
	FormatSizeTable[DXGI_FORMAT_R32_FLOAT] = 32;
	FormatSizeTable[DXGI_FORMAT_R32_UINT] = 32;
	FormatSizeTable[DXGI_FORMAT_R32_SINT] = 32;

	FormatSizeTable[DXGI_FORMAT_R24G8_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_D24_UNORM_S8_UINT] = 32;
	FormatSizeTable[DXGI_FORMAT_R24_UNORM_X8_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_X24_TYPELESS_G8_UINT] = 32;

	FormatSizeTable[DXGI_FORMAT_R8G8_TYPELESS] = 16;
	FormatSizeTable[DXGI_FORMAT_R8G8_UNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_R8G8_UINT] = 16;
	FormatSizeTable[DXGI_FORMAT_R8G8_SNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_R8G8_SINT] = 16;

	FormatSizeTable[DXGI_FORMAT_R16_TYPELESS] = 16;
	FormatSizeTable[DXGI_FORMAT_R16_FLOAT] = 16;
	FormatSizeTable[DXGI_FORMAT_D16_UNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_R16_UNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_R16_UINT] = 16;
	FormatSizeTable[DXGI_FORMAT_R16_SNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_R16_SINT] = 16;

	FormatSizeTable[DXGI_FORMAT_R8_TYPELESS] = 8;
	FormatSizeTable[DXGI_FORMAT_R8_UNORM] = 8;
	FormatSizeTable[DXGI_FORMAT_R8_UINT] = 8;
	FormatSizeTable[DXGI_FORMAT_R8_SNORM] = 8;
	FormatSizeTable[DXGI_FORMAT_R8_SINT] = 8;
	FormatSizeTable[DXGI_FORMAT_A8_UNORM] = 8;

	FormatSizeTable[DXGI_FORMAT_R1_UNORM] = 1;

	FormatSizeTable[DXGI_FORMAT_R9G9B9E5_SHAREDEXP] = 32;
	FormatSizeTable[DXGI_FORMAT_R8G8_B8G8_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_G8R8_G8B8_UNORM] = 32;

	FormatSizeTable[DXGI_FORMAT_BC1_TYPELESS] = 16;
	FormatSizeTable[DXGI_FORMAT_BC1_UNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_BC1_UNORM_SRGB] = 16;

	FormatSizeTable[DXGI_FORMAT_BC2_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_BC2_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_BC2_UNORM_SRGB] = 32;

	FormatSizeTable[DXGI_FORMAT_BC3_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_BC3_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_BC3_UNORM_SRGB] = 32;

	FormatSizeTable[DXGI_FORMAT_BC4_TYPELESS] = 16;
	FormatSizeTable[DXGI_FORMAT_BC4_UNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_BC4_SNORM] = 16;

	FormatSizeTable[DXGI_FORMAT_B5G6R5_UNORM] = 16;
	FormatSizeTable[DXGI_FORMAT_B5G5R5A1_UNORM] = 16;

	FormatSizeTable[DXGI_FORMAT_B8G8R8A8_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_B8G8R8X8_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_B8G8R8A8_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_B8G8R8A8_UNORM_SRGB] = 32;
	FormatSizeTable[DXGI_FORMAT_B8G8R8X8_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_B8G8R8X8_UNORM_SRGB] = 32;

	FormatSizeTable[DXGI_FORMAT_BC6H_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_BC6H_UF16] = 32;
	FormatSizeTable[DXGI_FORMAT_BC6H_SF16] = 32;

	FormatSizeTable[DXGI_FORMAT_BC7_TYPELESS] = 32;
	FormatSizeTable[DXGI_FORMAT_BC7_UNORM] = 32;
	FormatSizeTable[DXGI_FORMAT_BC7_UNORM_SRGB] = 32;

	FormatSizeTable[DXGI_FORMAT_B4G4R4A4_UNORM] = 16;


	box1D.left = 0;
	box1D.right = 0;
	box1D.top = 0;
	box1D.bottom = 1;
	box1D.front = 0;
	box1D.back = 1;
}
ResourceManager::~ResourceManager()
{
	Clear();
}
ResourceManager::ResourceManager(ID3D11Device * devicePtr, ID3D11DeviceContext * deviceContextPtr) :ResourceManager()
{
	this->devicePtr = devicePtr;
	this->deviceContextPtr = deviceContextPtr;
}
UINT ResourceManager::GetBindFlags(ID3D11Resource * resource)
{
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);
	bool hasUAV = false, hasSRV = false;
	if (type == D3D11_RESOURCE_DIMENSION_BUFFER)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		ID3D11Buffer * pBuffer = (ID3D11Buffer *)resource;
		pBuffer->GetDesc(&bufferDesc);
		return bufferDesc.BindFlags;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture2D* pTexture = (ID3D11Texture2D*)resource;
		pTexture->GetDesc(&textureDesc);
		return textureDesc.BindFlags;
	}
	return D3D11_BIND_FLAG();
}
UINT ResourceManager::GetIDByEnum(UINT enm)
{
	UINT id = 0;
	while (enm > 1)
	{
		enm = enm >> 1;
		id++;
	}
	return id;
}
void ResourceManager::Bind(UINT stages, D3D11_BIND_FLAG bindFlag, UINT startSlot, UINT numViews, void ** ptr, UINT elementStride, UINT offset)
{
	if (bindFlag == D3D11_BIND_VERTEX_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		deviceContextPtr->IASetVertexBuffers(startSlot, numViews, buffer, &elementStride, &offset);
	}
	else if (bindFlag == D3D11_BIND_INDEX_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		deviceContextPtr->IASetIndexBuffer(*buffer, DXGI_FORMAT_R32_UINT, offset);
	}
	else if (bindFlag == D3D11_BIND_STREAM_OUTPUT)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		deviceContextPtr->SOSetTargets(numViews, buffer, &offset);
	}
	else if (bindFlag == D3D11_BIND_UNORDERED_ACCESS)
	{
		ID3D11UnorderedAccessView** uav = (ID3D11UnorderedAccessView**)ptr;
		deviceContextPtr->CSSetUnorderedAccessViews(startSlot, numViews, uav, NULL);
	}
	else if (bindFlag == D3D11_BIND_DEPTH_STENCIL)
	{
		ID3D11DepthStencilView** dsv = (ID3D11DepthStencilView**)ptr;
		//Need to keep render targets unchanged
		vector<ID3D11RenderTargetView*> oldrtvs = GetRenderTargets();
		ID3D11RenderTargetView **rtvptr = NULL;
		if (oldrtvs.size())
		{
			rtvptr = &oldrtvs[0];
		}
		deviceContextPtr->OMSetRenderTargets(oldrtvs.size(), rtvptr, *dsv);
	}
	else if (bindFlag == D3D11_BIND_RENDER_TARGET)
	{
		ID3D11DepthStencilView* dsv = NULL;
		ID3D11RenderTargetView** rtv = (ID3D11RenderTargetView**)ptr;
		UINT stageID = GetIDByEnum(Output_Merge);
		UINT bindID = GetIDByEnum(D3D11_BIND_DEPTH_STENCIL);
		int dsvID = bindingTable[stageID][bindID][0];
		if (dsvID != -1)
		{
			dsv = (ID3D11DepthStencilView*)resourcePool[dsvID].viewPool[D3D11_BIND_DEPTH_STENCIL].ptr;
		}
		deviceContextPtr->OMSetRenderTargets(numViews, rtv, dsv);
	}
	else if (bindFlag == D3D11_BIND_CONSTANT_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		if (stages & Vertex_Shader)
		{
			deviceContextPtr->VSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Hull_Shader)
		{
			deviceContextPtr->HSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Domain_Shader)
		{
			deviceContextPtr->DSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Geometry_Shader)
		{
			deviceContextPtr->GSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Pixel_Shader)
		{
			deviceContextPtr->PSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Compute_Shader)
		{
			deviceContextPtr->CSSetConstantBuffers(startSlot, numViews, buffer);
		}
	}
	else if (bindFlag == D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView** srv = (ID3D11ShaderResourceView**)ptr;
		if (stages & Vertex_Shader)
		{
			deviceContextPtr->VSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Hull_Shader)
		{
			deviceContextPtr->HSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Domain_Shader)
		{
			deviceContextPtr->DSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Geometry_Shader)
		{
			deviceContextPtr->GSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Pixel_Shader)
		{
			deviceContextPtr->PSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Compute_Shader)
		{
			deviceContextPtr->CSSetShaderResources(startSlot, numViews, srv);
		}
	}
}
vector<ID3D11RenderTargetView*> ResourceManager::GetRenderTargets()
{
	UINT stageID = GetIDByEnum(Output_Merge);
	UINT bindID = GetIDByEnum(D3D11_BIND_RENDER_TARGET);
	int* table = bindingTable[stageID][bindID];
	vector<ID3D11RenderTargetView*> rtvs;
	rtvs.reserve(MAX_SLOT_NUMBER);
	for (int i = 0; table[i] != -1 && i < MAX_SLOT_NUMBER; i++)
	{
		ResourceData &data = resourcePool[table[i]];
		ID3D11RenderTargetView* ptr = (ID3D11RenderTargetView*)data.viewPool[D3D11_BIND_RENDER_TARGET].ptr;
		rtvs.push_back(ptr);
	}
	return rtvs;
}
bool ResourceManager::SetBindingTable(PiplineStage stage, D3D11_BIND_FLAG bindFlag, UINT slot, int id)
{
	UINT stageID = GetIDByEnum(stage);
	UINT bindID = GetIDByEnum(bindFlag);
	int oldID = bindingTable[stageID][bindID][slot];
	if (oldID == id)
	{
		return false;//Already binded (avoid rebind)
	}
	if (oldID != -1)//Clear old resource binding map
	{
		resourcePool[oldID].viewPool[bindFlag].bindingMap[stage] = -1;
	}
	if (id != -1)//Set new resource binding map
	{
		resourcePool[id].viewPool[bindFlag].bindingMap[stage] = slot;
	}
	//Set global binding table
	bindingTable[stageID][bindID][slot] = id;
	return true;
}
void ResourceManager::UnbindView(ResourceView & view)
{
	map<PiplineStage, int> temp = view.bindingMap;//copy
	map<PiplineStage, int>::iterator it = temp.begin();
	while (it != view.bindingMap.end())
	{
		if (it->second>-1)
			SetBinding(it->first, view.type, it->second, -1);
		it++;
	}
}
int ResourceManager::GenerateID()
{
	int i;
	for (i = 0; i < INT_MAX; i++)
	{
		if (resourcePool.count(i) == 0)
			return i;
	}
	return -1;
}
ID3D11ShaderResourceView * ResourceManager::CreateSRV(ID3D11Resource * resource)
{
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	if (type == D3D11_RESOURCE_DIMENSION_BUFFER)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		ID3D11Buffer * pBuffer = (ID3D11Buffer *)resource;
		pBuffer->GetDesc(&bufferDesc);
		if (!(bufferDesc.BindFlags&D3D11_BIND_SHADER_RESOURCE))
		{
			return NULL;
		}
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		//Structured buffer only!!(Temporary)
		if (bufferDesc.MiscFlags&D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.BufferEx.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
		}
		else
		{
			//Structured buffer only!!
			return NULL;
		}
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture2D* pTexture = (ID3D11Texture2D*)resource;
		pTexture->GetDesc(&textureDesc);
		if (!(textureDesc.BindFlags&D3D11_BIND_SHADER_RESOURCE))
		{
			return NULL;
		}
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
	}

	ID3D11ShaderResourceView * pSRV;
	HRESULT hr = devicePtr->CreateShaderResourceView(resource, &srvDesc, &pSRV);
	if (FAILED(hr))
	{
		return nullptr;
	}

	return pSRV;
}
ID3D11UnorderedAccessView * ResourceManager::CreateUAV(ID3D11Resource * resource)
{

	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	if (type == D3D11_RESOURCE_DIMENSION_BUFFER)
	{

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		ID3D11Buffer * pBuffer = (ID3D11Buffer *)resource;
		pBuffer->GetDesc(&bufferDesc);
		if (!(bufferDesc.BindFlags&D3D11_BIND_UNORDERED_ACCESS))
		{
			return NULL;
		}
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		//Structured buffer only!!
		if (bufferDesc.MiscFlags&D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
		}
		else
		{
			//Structured buffer only!!
			return NULL;
		}
	}

	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture2D* pTexture = (ID3D11Texture2D*)resource;
		pTexture->GetDesc(&textureDesc);
		if (!(textureDesc.BindFlags&D3D11_BIND_UNORDERED_ACCESS))
		{
			return NULL;
		}
		uavDesc.Format = textureDesc.Format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
	}

	ID3D11UnorderedAccessView * pUAV;
	HRESULT hr = devicePtr->CreateUnorderedAccessView(resource, &uavDesc, &pUAV);
	if (FAILED(hr))
	{
		return nullptr;
	}

	return pUAV;
}
D3D11_BUFFER_DESC ResourceManager::GenerateBufferDesc(UINT bindFlag, bool isDynamic, UINT size)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = bindFlag;
	desc.ByteWidth = size;
	if (!isDynamic)//Default
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	return desc;
}
D3D11_TEXTURE2D_DESC ResourceManager::GenerateTexture2Desc(UINT  bindFlag, DXGI_FORMAT format, bool isDynamic, bool  hasMip, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Height = height;
	desc.Width = width;
	desc.BindFlags = bindFlag;

	if (hasMip)
	{
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 0;
	}
	else
	{
		desc.MiscFlags = 0;
		desc.MipLevels = 1;
	}
	if (!isDynamic)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	return desc;
}
ID3D11Buffer* ResourceManager::CreateBuffer(D3D11_BUFFER_DESC & desc, void * pData, UINT  dataSize)
{
	if (pData && (dataSize > desc.ByteWidth))
	{
		return NULL;
	}
	HRESULT hr;
	ID3D11Buffer* buffer;
	if (pData&&dataSize)
	{
		D3D11_SUBRESOURCE_DATA sbData;
		sbData.pSysMem = pData;
		sbData.SysMemPitch = dataSize;
		sbData.SysMemSlicePitch = dataSize;//No need
		hr = devicePtr->CreateBuffer(&desc, &sbData, &buffer);
	}
	else
	{
		hr = devicePtr->CreateBuffer(&desc, NULL, &buffer);
	}
	if (FAILED(hr))
	{
		return NULL;
	}
	return buffer;
}
ID3D11Texture2D * ResourceManager::CreateTexture2D(D3D11_TEXTURE2D_DESC & desc, void * pData)
{
	UINT pixelByte = FormatSizeTable[desc.Format]/8;
	if (pixelByte <= 0)
		return NULL;
	HRESULT hr;
	ID3D11Texture2D* Texture;
	hr = devicePtr->CreateTexture2D(&desc, NULL, &Texture);
	if (FAILED(hr))
	{
		return NULL;
	}

	if (pData)
	{
		deviceContextPtr->UpdateSubresource(Texture, 0, NULL, pData, desc.Width* pixelByte, 0);
	}
	return Texture;
}
bool ResourceManager::CreateViews(ResourceData &resource)
{
	UINT flags = GetBindFlags(resource.ptr);
	ResourceView rview;
	rview.stride = resource.stride;
	//Views
	if (flags&D3D11_BIND_UNORDERED_ACCESS)
	{
		ID3D11UnorderedAccessView* uav;
		uav = CreateUAV(resource.ptr);
		if (!uav)
		{
			return false;
		}
		rview.ptr = uav;
		rview.type = D3D11_BIND_UNORDERED_ACCESS;
		resource.viewPool[D3D11_BIND_UNORDERED_ACCESS] = rview;
	}
	if (flags&D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView* srv;
		srv = CreateSRV(resource.ptr);
		if (!srv)
		{
			ClearViews(resource);
			return false;
		}
		rview.ptr = srv;
		rview.type = D3D11_BIND_SHADER_RESOURCE;
		resource.viewPool[D3D11_BIND_SHADER_RESOURCE] = rview;
	}
	if (flags&D3D11_BIND_RENDER_TARGET)
	{
		ID3D11RenderTargetView* rtv;
		HRESULT hr;
		hr = devicePtr->CreateRenderTargetView(resource.ptr, NULL, &rtv);
		if (FAILED(hr))
		{
			ClearViews(resource);
			return false;
		}
		rview.ptr = rtv;
		rview.type = D3D11_BIND_RENDER_TARGET;
		resource.viewPool[D3D11_BIND_RENDER_TARGET] = rview;
	}
	if (flags&D3D11_BIND_DEPTH_STENCIL)
	{
		ID3D11DepthStencilView* dsv;
		HRESULT hr;
		hr = devicePtr->CreateDepthStencilView(resource.ptr, NULL, &dsv);
		if (FAILED(hr))
		{
			ClearViews(resource);
			return false;
		}
		rview.ptr = dsv;
		rview.type = D3D11_BIND_DEPTH_STENCIL;
		resource.viewPool[D3D11_BIND_DEPTH_STENCIL] = rview;
	}
	//Buffers
	if (flags&D3D11_BIND_CONSTANT_BUFFER)
	{
		rview.ptr = resource.ptr;
		rview.type = D3D11_BIND_CONSTANT_BUFFER;
		resource.viewPool[D3D11_BIND_CONSTANT_BUFFER] = rview;
	}
	if (flags&D3D11_BIND_INDEX_BUFFER)
	{
		rview.ptr = resource.ptr;
		rview.type = D3D11_BIND_INDEX_BUFFER;
		resource.viewPool[D3D11_BIND_INDEX_BUFFER] = rview;
	}
	if (flags&D3D11_BIND_VERTEX_BUFFER)
	{
		rview.ptr = resource.ptr;
		rview.type = D3D11_BIND_VERTEX_BUFFER;
		resource.viewPool[D3D11_BIND_VERTEX_BUFFER] = rview;
	}
	if (flags&D3D11_BIND_STREAM_OUTPUT)
	{
		rview.ptr = resource.ptr;
		rview.type = D3D11_BIND_STREAM_OUTPUT;
		resource.viewPool[D3D11_BIND_STREAM_OUTPUT] = rview;
	}
	return true;
}
void ResourceManager::ClearViews(ResourceData & resource)
{
	map<D3D11_BIND_FLAG, ResourceView>::iterator it = resource.viewPool.begin();
	while (it != resource.viewPool.end())
	{
		UnbindView(it->second);
		if (it->first == D3D11_BIND_SHADER_RESOURCE ||
			it->first == D3D11_BIND_UNORDERED_ACCESS ||
			it->first == D3D11_BIND_RENDER_TARGET ||
			it->first == D3D11_BIND_DEPTH_STENCIL
			)
		{
			((IUnknown*)(it->second.ptr))->Release();
		}

		it++;
	}
}
bool ResourceManager::UpdateResourceData(ID3D11Resource *resource, void * pData, UINT size)
{
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);

	D3D11_USAGE usage;
	UINT cpuAccess;
	UINT rowPitch = 0;
	if (type == D3D11_RESOURCE_DIMENSION_BUFFER)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		ID3D11Buffer * pBuffer = (ID3D11Buffer *)resource;
		pBuffer->GetDesc(&bufferDesc);
		usage = bufferDesc.Usage;
		cpuAccess = bufferDesc.CPUAccessFlags;
		rowPitch = 0;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture2D* pTexture = (ID3D11Texture2D*)resource;
		pTexture->GetDesc(&textureDesc);
		usage = textureDesc.Usage;
		cpuAccess = textureDesc.CPUAccessFlags;
		rowPitch = textureDesc.Width*(FormatSizeTable[textureDesc.Format] / 8);
		if (rowPitch <= 0)
			return false;
	}

	if ((usage == D3D11_USAGE_DYNAMIC) && (cpuAccess&D3D11_CPU_ACCESS_WRITE))
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		result = deviceContextPtr->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}
		memcpy(mappedResource.pData, pData, size);
		deviceContextPtr->Unmap(resource, 0);
	}
	else if (usage == D3D11_USAGE_DEFAULT)
	{
		box1D.right = size;
		deviceContextPtr->UpdateSubresource(resource, 0, &box1D, pData, rowPitch, 0);
	}
	return true;
}
bool ResourceManager::SetBinding(PiplineStage stage, D3D11_BIND_FLAG bindFlag, UINT startSlot, vector< int>& idList)
{
	if (startSlot < 0 || startSlot >= MAX_SLOT_NUMBER || !idList.size())
	{
		return false;
	}
	if (bindFlag == D3D11_BIND_DEPTH_STENCIL)
	{
		stage = Output_Merge;
		startSlot = 0;
		if (idList.size() > 1)
			return false;
	}
	else if (bindFlag == D3D11_BIND_RENDER_TARGET)
	{
		stage = Output_Merge;
		startSlot = 0;
	}
	else if (bindFlag == D3D11_BIND_INDEX_BUFFER)
	{
		stage = Input_Assembler;
		startSlot = 0;
		if (idList.size() > 1)
			return false;
	}
	else if (bindFlag == D3D11_BIND_VERTEX_BUFFER)
	{
		stage = Input_Assembler;
	}
	else if (bindFlag == D3D11_BIND_STREAM_OUTPUT)
	{
		stage = Stream_Out;
		startSlot = 0;
	}
	vector<void*> newView;
	newView.resize(idList.size());
	for (int i = 0; i < idList.size(); i++)
	{
		int &id = idList[i];
		if (id == -1)
		{
			newView[i] = NULL;
		}
		else if (resourcePool.count(id) && resourcePool[id].viewPool.count(bindFlag))
		{
			newView[i] = resourcePool[id].viewPool[bindFlag].ptr;
		}
		else
		{
			return false;
		}
	}
	unsigned	int &stride = resourcePool[idList[0]].stride;
	bool change = false;

	if (bindFlag == D3D11_BIND_RENDER_TARGET || bindFlag == D3D11_BIND_STREAM_OUTPUT)
	{
		//Set OM stage will lose all binding resource
		for (int i = 0; i < MAX_SLOT_NUMBER; i++)
		{
			SetBindingTable(stage, bindFlag, i, -1);
		}
		change = true;
	}
	for (int i = 0; i < idList.size(); i++)
	{
		change = SetBindingTable(stage, bindFlag, startSlot + i, idList[i]) || change;
	}


	if (change)
	{
		Bind(stage, bindFlag, startSlot, newView.size(), &newView[0], stride);
	}
}
bool ResourceManager::SetBinding(PiplineStage stage, D3D11_BIND_FLAG bindFlag, UINT slot, int id)
{
	vector<int> temp;
	temp.push_back(id);
	//Bind(stage, bindFlag, slot, 1, (void**)&resourcePool[id].viewPool[bindFlag].ptr, resourcePool[id].stride);
	return  SetBinding(stage, bindFlag, slot, temp);
}
int ResourceManager::CreateBuffer(UINT  bindFlags, bool isDynamic, UINT bufferSize, void * pData, UINT dataSize, UINT elementStride)
{
	if (bindFlags&D3D11_BIND_VERTEX_BUFFER)
	{//Vertex buffer must have stride
		if (!elementStride)
			return -1;
	}
	int id = GenerateID();
	if (id == -1)
	{
		return -1;
	}
	ResourceData rdata;
	//HRESULT hr;
	D3D11_BUFFER_DESC desc = GenerateBufferDesc(bindFlags, isDynamic, bufferSize);
	if (elementStride && !(bindFlags&D3D11_BIND_VERTEX_BUFFER) && !(bindFlags&D3D11_BIND_INDEX_BUFFER))//Structured buffer detected
	{
		desc.ByteWidth = bufferSize;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = elementStride;
	}

	ID3D11Buffer* buffer = CreateBuffer(desc, pData, dataSize);
	if (!buffer)
	{
		return -1;
	}

	rdata.ptr = buffer;
	rdata.stride = elementStride;
	if (elementStride)
	{
		rdata.elementCount = dataSize / elementStride;
	}
	else
	{
		rdata.elementCount = 0;
	}
	if (!CreateViews(rdata))
	{
		rdata.ptr->Release();
		return -1;
	}
	resourcePool[id] = rdata;
	return id;
}
int ResourceManager::CreateTexture2D(UINT bindFlag, DXGI_FORMAT format,  bool isDynamic,bool genMip, UINT width, UINT height, void * pData)
{
	int id = GenerateID();
	if (id == -1)
	{
		return -1;
	}
	ResourceData rdata;
	D3D11_TEXTURE2D_DESC desc = GenerateTexture2Desc(bindFlag, format, isDynamic, genMip, width, height);
	ID3D11Texture2D* texture = CreateTexture2D(desc, pData);
	rdata.ptr = texture;
	rdata.stride = 0;
	rdata.elementCount = 0;
	if (!CreateViews(rdata))
	{
		rdata.ptr->Release();
		return -1;
	}
	if (rdata.viewPool.count(D3D11_BIND_SHADER_RESOURCE) && rdata.viewPool.count(D3D11_BIND_RENDER_TARGET) && genMip)
	{
		ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)rdata.viewPool[D3D11_BIND_SHADER_RESOURCE].ptr;
		deviceContextPtr->GenerateMips(srv);
	}
	resourcePool[id] = rdata;
	return id;
}
int ResourceManager::CreateBackBuffer(IDXGISwapChain * pSwapChain)
{
	int id = GenerateID();
	if (id == -1)
	{
		return -1;
	}
	ID3D11Buffer* backBufferPtr;
	HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(hr))
	{
		return -1;
	}
	ResourceData data;
	data.ptr = backBufferPtr;
	data.elementCount = 0;
	data.stride = 0;
	if (!CreateViews(data))
	{
		data.ptr->Release();
		return -1;
	}
	resourcePool[id] = data;
	return id;
}
bool ResourceManager::UpdateResourceData(UINT resourceID, void * pData, UINT size)
{
	if (!resourcePool.count(resourceID))
		return false;

	return UpdateResourceData(resourcePool[resourceID].ptr, pData, size);
}
void ResourceManager::CopyResourceData(UINT srcID, UINT dstID)
{
	if (!resourcePool.count(srcID) || !resourcePool.count(dstID))
		return;
	deviceContextPtr->CopyResource(resourcePool[dstID].ptr, resourcePool[srcID].ptr);
}
vector<byte> ResourceManager::GetResourceData(UINT srcID, UINT * xLength, UINT * yLength, UINT * zLength)
{
	if (!resourcePool.count(srcID))
		return vector<byte>();
	ResourceData &resource = resourcePool[srcID];
	UINT x, y, z;
	x = y = z = 1;
	HRESULT hr;
	vector<byte> data;
	bool staging = false;
	ID3D11Resource* stagingRC = NULL;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	D3D11_RESOURCE_DIMENSION type;
	resource.ptr->GetType(&type);
	//Buffer type
	if (type == D3D11_RESOURCE_DIMENSION_BUFFER)
	{
		ID3D11Buffer* buffer = (ID3D11Buffer*)resource.ptr;
		D3D11_BUFFER_DESC desc;
		buffer->GetDesc(&desc);
		x = desc.ByteWidth;
		//If the buffer can be readed by CPU
		if (desc.Usage == D3D11_USAGE_STAGING&&(desc.CPUAccessFlags&D3D11_CPU_ACCESS_READ))
		{
			stagingRC = buffer;
		}
		else
		{
			//If not, we create a staging buffer and copy data to it
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			hr = devicePtr->CreateBuffer(&desc, NULL, (ID3D11Buffer**)&stagingRC);
			if (FAILED(hr))
			{
				return data;
			}
			deviceContextPtr->CopyResource(stagingRC, buffer);
		}

		hr = deviceContextPtr->Map(stagingRC, 0, D3D11_MAP_READ, 0, &MappedResource);
		if (FAILED(hr))
		{
			return data;
		}
		x = MappedResource.RowPitch;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE1D)
	{
		ID3D11Texture1D* texture = (ID3D11Texture1D*)resource.ptr;
		D3D11_TEXTURE1D_DESC desc;
		texture->GetDesc(&desc);
		if (desc.Usage == D3D11_USAGE_STAGING && (desc.CPUAccessFlags&D3D11_CPU_ACCESS_READ))
		{
			stagingRC = texture;
		}
		else
		{
			//If not, we create a staging texture and copy data to it
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			hr = devicePtr->CreateTexture1D(&desc, NULL, (ID3D11Texture1D**)&stagingRC);
			if (FAILED(hr))
			{
				return data;
			}
			deviceContextPtr->CopyResource(stagingRC, texture);
		}
		hr = deviceContextPtr->Map(stagingRC, 0, D3D11_MAP_READ, 0, &MappedResource);
		if (FAILED(hr))
		{
			return data;
		}
		x = MappedResource.RowPitch;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		ID3D11Texture2D* texture = (ID3D11Texture2D*)resource.ptr;
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
		UINT pixelByte = FormatSizeTable[desc.Format] / 8;
		//If texture support staging we use it directly
		if (desc.Usage == D3D11_USAGE_STAGING && (desc.CPUAccessFlags&D3D11_CPU_ACCESS_READ))
		{
			stagingRC = texture;
		}
		else
		{
			//If not, we create a staging texture and copy data to it
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			hr = devicePtr->CreateTexture2D(&desc, NULL, (ID3D11Texture2D**)&stagingRC);
			if (FAILED(hr))
			{
				return data;
			}
			deviceContextPtr->CopyResource(stagingRC, texture);
		}
		hr = deviceContextPtr->Map(stagingRC, 0, D3D11_MAP_READ, 0, &MappedResource);
		if (FAILED(hr))
		{
			return data;
		}
		x = MappedResource.RowPitch;
		y = desc.Height*pixelByte;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
	{
		ID3D11Texture3D* texture = (ID3D11Texture3D*)resource.ptr;
		D3D11_TEXTURE3D_DESC desc;
		texture->GetDesc(&desc);
		UINT pixelByte = FormatSizeTable[desc.Format] / 8;
		//If texture support staging we use it directly
		if (desc.Usage == D3D11_USAGE_STAGING && (desc.CPUAccessFlags&D3D11_CPU_ACCESS_READ))
		{
			stagingRC = texture;
		}
		else
		{
			//If not, we create a staging texture and copy data to it
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			hr = devicePtr->CreateTexture3D(&desc, NULL, (ID3D11Texture3D**)&stagingRC);
			if (FAILED(hr))
			{
				return data;
			}
			deviceContextPtr->CopyResource(stagingRC, texture);
		}
		hr = deviceContextPtr->Map(stagingRC, 0, D3D11_MAP_READ, 0, &MappedResource);
		if (FAILED(hr))
		{
			return data;
		}
		x = MappedResource.RowPitch;
		y = MappedResource.DepthPitch;
		z = desc.Depth*pixelByte;
	}

	//Prepare out put data vector
	data.resize(x*y*z);
	byte* srcData = (byte*)MappedResource.pData;
	memcpy(&data[0], srcData, data.size());
	deviceContextPtr->Unmap(stagingRC, 0);
	//If we created a staging resource, release it
	if (stagingRC != resource.ptr)
	{
		stagingRC->Release();
	}
	//Out put data dimension info
	if (xLength != NULL)
		*xLength = x;
	if(yLength != NULL)
		*yLength = y;
	if (zLength != NULL)
		*zLength = z;

	return data;
}
void ResourceManager::ResetRTV(UINT id, const float color[4])
{
	if (!resourcePool.count(id) || !resourcePool[id].viewPool.count(D3D11_BIND_RENDER_TARGET))
	{
		return;
	}
	ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)resourcePool[id].viewPool[D3D11_BIND_RENDER_TARGET].ptr;
	deviceContextPtr->ClearRenderTargetView(rtv, color);
}
void ResourceManager::ResetDSV(UINT id, D3D11_CLEAR_FLAG flag, float depth, UINT8 stencil)
{
	if (!resourcePool.count(id) || !resourcePool[id].viewPool.count(D3D11_BIND_DEPTH_STENCIL))
	{
		return;
	}
	ID3D11DepthStencilView* dsv = (ID3D11DepthStencilView*)resourcePool[id].viewPool[D3D11_BIND_DEPTH_STENCIL].ptr;
	deviceContextPtr->ClearDepthStencilView(dsv, flag, depth, stencil);
}
void ResourceManager::Clear(UINT id)
{
	if (!resourcePool.count(id))
	{
		return;
	}
	ClearViews(resourcePool[id]);
	resourcePool[id].ptr->Release();
	resourcePool.erase(id);
}
void ResourceManager::Clear()
{
	map<UINT, ResourceData>::iterator it = resourcePool.begin();
	while (it != resourcePool.end())
	{
		ClearViews(it->second);
		it->second.ptr->Release();
	}
	resourcePool.clear();
	memset(bindingTable, -1, sizeof(bindingTable));
}