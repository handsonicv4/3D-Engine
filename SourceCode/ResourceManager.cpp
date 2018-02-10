//=====================ResourceManager ==========================
//Author: Sentao
//===========================================================
#include"ResourceManager.h"

ResourceManager::ResourceManager()
{
	currentDSV = NULL;
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
D3D11_BIND_FLAG ResourceManager::GetBindFlag(BindFlag flag)
{
	return (D3D11_BIND_FLAG)flag;
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
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
	{
		D3D11_TEXTURE3D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture3D* pTexture = (ID3D11Texture3D*)resource;
		pTexture->GetDesc(&textureDesc);
		return textureDesc.BindFlags;
	}
	return 0;
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
		if (stages & Stage_Compute_Shader)
		{
			deviceContextPtr->CSSetUnorderedAccessViews(startSlot, numViews, uav, NULL);
		}
		if (stages&Stage_Output_Merge)
		{
			deviceContextPtr->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, NULL, NULL, startSlot, numViews, uav, NULL);
		}
	}
	else if (bindFlag == D3D11_BIND_DEPTH_STENCIL)
	{
		ID3D11DepthStencilView** dsv = (ID3D11DepthStencilView**)ptr;

		//Keep render targets unchanged
		ID3D11RenderTargetView **rtvptr = NULL;
		if (currentRTVs.size())
		{
			rtvptr = &currentRTVs[0];
		}
		//Record new DSV
		currentDSV = *dsv;
		deviceContextPtr->OMSetRenderTargetsAndUnorderedAccessViews(currentRTVs.size(), rtvptr, *dsv, 4, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
	}
	else if (bindFlag == D3D11_BIND_RENDER_TARGET)
	{
		numViews = numViews > 8 ? 8 : numViews;
		ID3D11DepthStencilView* dsv = NULL;
		ID3D11RenderTargetView** rtv = (ID3D11RenderTargetView**)ptr;
		//Keep current DSV
		if (currentDSV)
		{
				dsv = currentDSV;
		}
		//Record new RTV
		currentRTVs.clear();
		for (int i = 0; i < numViews; i++)
		{
			currentRTVs.push_back(rtv[i]);
		}
		deviceContextPtr->OMSetRenderTargetsAndUnorderedAccessViews(numViews, rtv, dsv, 4, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
	}
	else if (bindFlag == D3D11_BIND_CONSTANT_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		if (stages & Stage_Vertex_Shader)
		{
			deviceContextPtr->VSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Hull_Shader)
		{
			deviceContextPtr->HSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Domain_Shader)
		{
			deviceContextPtr->DSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Geometry_Shader)
		{
			deviceContextPtr->GSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Pixel_Shader)
		{
			deviceContextPtr->PSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Compute_Shader)
		{
			deviceContextPtr->CSSetConstantBuffers(startSlot, numViews, buffer);
		}
	}
	else if (bindFlag == D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView** srv = (ID3D11ShaderResourceView**)ptr;
		if (stages & Stage_Vertex_Shader)
		{
			deviceContextPtr->VSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Hull_Shader)
		{
			deviceContextPtr->HSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Domain_Shader)
		{
			deviceContextPtr->DSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Geometry_Shader)
		{
			deviceContextPtr->GSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Pixel_Shader)
		{
			deviceContextPtr->PSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Compute_Shader)
		{
			deviceContextPtr->CSSetShaderResources(startSlot, numViews, srv);
		}
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
D3D11_TEXTURE3D_DESC ResourceManager::GenerateTexture3Desc(UINT bindFlag, DXGI_FORMAT format, bool isDynamic, bool hasMip, UINT width, UINT height, UINT depth)
{
	D3D11_TEXTURE3D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.MipLevels = 0;
	desc.Format = format;
	desc.Height = height;
	desc.Width = width;
	desc.Depth = depth;
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
ID3D11Texture3D * ResourceManager::CreateTexture3D(D3D11_TEXTURE3D_DESC & desc, void * pData)
{
	UINT pixelByte = FormatSizeTable[desc.Format] / 8;
	if (pixelByte <= 0)
		return NULL;
	HRESULT hr;
	ID3D11Texture3D* texture;
	hr = devicePtr->CreateTexture3D(&desc, NULL, &texture);
	if (FAILED(hr))
	{
		return NULL;
	}

	if (pData)
	{
		UINT rowpitch = desc.Width* pixelByte;
		UINT depthpitch = rowpitch*desc.Height;
		deviceContextPtr->UpdateSubresource(texture, 0, NULL, pData, rowpitch, depthpitch);
	}
	return texture;

}
bool ResourceManager::CreateViews(ResourceData &resource)
{
	UINT flags = GetBindFlags(resource.ptr);
	ResourceView rview;
	rview.stride = resource.stride;
	HRESULT hr;
	//Views
	if (flags&D3D11_BIND_UNORDERED_ACCESS)
	{
		ID3D11UnorderedAccessView* uav=NULL;
		hr=devicePtr->CreateUnorderedAccessView(resource.ptr, NULL, &uav);
		if (FAILED(hr))
		{
			ClearViews(resource);
			return false;
		}
		rview.ptr = uav;
		rview.type = D3D11_BIND_UNORDERED_ACCESS;
		resource.viewPool[D3D11_BIND_UNORDERED_ACCESS] = rview;
	}
	if (flags&D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView* srv= NULL;
		hr = devicePtr->CreateShaderResourceView(resource.ptr, NULL, &srv);
		if (FAILED(hr))
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
		ID3D11DepthStencilView* dsv=NULL;
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
bool ResourceManager::SetBinding(PiplineStage stage, BindFlag bindFlag, UINT startSlot, vector< int>& idList)
{
	D3D11_BIND_FLAG d3dbindFlag = GetBindFlag(bindFlag);
	if (startSlot < 0 || startSlot >= MAX_SLOT_NUMBER || !idList.size())
	{
		return false;
	}
	if (d3dbindFlag == D3D11_BIND_DEPTH_STENCIL)
	{
		stage = Stage_Output_Merge;
		startSlot = 0;
		if (idList.size() > 1)
			return false;
	}
	else if (d3dbindFlag == D3D11_BIND_RENDER_TARGET)
	{
		stage = Stage_Output_Merge;
		startSlot = 0;
	}
	else if (d3dbindFlag == D3D11_BIND_INDEX_BUFFER)
	{
		stage = Stage_Input_Assembler;
		startSlot = 0;
		if (idList.size() > 1)
			return false;
	}
	else if (d3dbindFlag == D3D11_BIND_VERTEX_BUFFER)
	{
		stage = Stage_Input_Assembler;
	}
	else if (d3dbindFlag == D3D11_BIND_STREAM_OUTPUT)
	{
		stage = Stage_Stream_Out;
		startSlot = 0;
	}


	vector<void*> newView;
	newView.resize(idList.size());
	int stride=-1;
	for (int i = 0; i < idList.size(); i++)
	{
		int &id = idList[i];
		if (id == -1)
		{
			newView[i] = NULL;
		}
		else if (resourcePool.count(id) && resourcePool[id].viewPool.count(d3dbindFlag))
		{
			newView[i] = resourcePool[id].viewPool[d3dbindFlag].ptr;
			stride = resourcePool[id].stride;
		}
		else
		{
			return false;
		}
	}

	Bind(stage, d3dbindFlag, startSlot, newView.size(), &newView[0], stride);

	return true;

}
bool ResourceManager::SetBinding(PiplineStage stage, BindFlag bindFlag, UINT slot, int id)
{
	vector<int> temp;
	temp.push_back(id);

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
int ResourceManager::CreateTexture3D(UINT bindFlag, DXGI_FORMAT format, bool isDynamic, bool genMip, UINT width, UINT height, UINT depth, void * pData)
{
	int id = GenerateID();
	if (id == -1)
	{
		return -1;
	}
	ResourceData rdata;
	D3D11_TEXTURE3D_DESC desc = GenerateTexture3Desc(bindFlag, format, isDynamic, genMip, width, height, depth);
	ID3D11Texture3D* texture = CreateTexture3D(desc, pData);
	if (!texture)
	{
		return -1;
	}
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
		UINT pixelByte = FormatSizeTable[desc.Format] / 8;
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
			//desc.MipLevels = 1;
			hr = devicePtr->CreateTexture3D(&desc, NULL, (ID3D11Texture3D**)&stagingRC);
			if (FAILED(hr))
			{
				return data;
			}
			deviceContextPtr->GenerateMips((ID3D11ShaderResourceView*)resource.viewPool[D3D11_BIND_SHADER_RESOURCE].ptr);
			deviceContextPtr->CopyResource(stagingRC, texture);
		}
		hr = deviceContextPtr->Map(stagingRC, 4, D3D11_MAP_READ, 0, &MappedResource);
		if (FAILED(hr))
		{
			return data;
		}
		x = MappedResource.RowPitch;
		y = MappedResource.DepthPitch/ MappedResource.RowPitch *pixelByte;
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
void ResourceManager::ResetUAV(UINT id, const float value[4])
{
	if (resourcePool.count(id)&& resourcePool[id].viewPool.count(D3D11_BIND_UNORDERED_ACCESS))
	{
		deviceContextPtr->ClearUnorderedAccessViewFloat((ID3D11UnorderedAccessView*)resourcePool[id].viewPool[D3D11_BIND_UNORDERED_ACCESS].ptr, value);
	}
}
void ResourceManager::ResetUAV(UINT id, const UINT value[4])
{
	if (resourcePool.count(id) && resourcePool[id].viewPool.count(D3D11_BIND_UNORDERED_ACCESS))
	{
		deviceContextPtr->ClearUnorderedAccessViewUint((ID3D11UnorderedAccessView*)resourcePool[id].viewPool[D3D11_BIND_UNORDERED_ACCESS].ptr, value);
	}
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
bool ResourceManager::GenerateMipMap(UINT id)
{
	if (resourcePool.count(id)&& resourcePool[id].viewPool.count(D3D11_BIND_SHADER_RESOURCE))
	{
		ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)resourcePool[id].viewPool[D3D11_BIND_SHADER_RESOURCE].ptr;
		deviceContextPtr->GenerateMips(srv);
	}
	return false;
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
		it++;
	}
	resourcePool.clear();

}