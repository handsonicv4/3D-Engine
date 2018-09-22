//=====================ResourceManager ==========================
//Author: Sentao
//===========================================================
#include"ResourceManager.h"
#include"DescFileLoader.h"
using namespace FileLoader;

unordered_map<DXGI_FORMAT, UINT> Resource::FormatSizeTable;

Resource* Resource::pBackBuffer = NULL;

void Resource::InitFormatTable()
{
	if (FormatSizeTable.size() == 0)
	{
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
	}
}

Resource::Resource()
{
	InitFormatTable();
	ptr = NULL;
}

Resource::Resource(ResourceDesc desc, void* pData, size_t dataSize)
{
	InitFormatTable();
	this->desc = desc;
	ptr = NULL;
	if (desc.type == Resource_Buffer) 
	{
		if (!CreateBuffer(pData, dataSize)) return;
	}
	else if (desc.type == Resource_Texture2D)
	{
		if (!CreateTexture2D(pData, dataSize)) return;
	}
	else if (desc.type == Resource_Texture3D) {
		if (!CreateTexture3D(pData, dataSize)) return;
	}
	if (!GenerateViews()) {
		Release();
		return;
	}
	GenerateMips();
}

void Resource:: Release() 
{
	ClearViews();
	if (ptr) 
	{
		ptr->Release();
		ptr = NULL;
	}
}

Resource::~Resource() 
{
	Release();
}

void Resource::ClearViews()
{
	unordered_map<D3D11_BIND_FLAG, IUnknown*>::iterator it = viewPool.begin();
	while (it != viewPool.end())
	{
		if (it->first == D3D11_BIND_SHADER_RESOURCE ||
			it->first == D3D11_BIND_UNORDERED_ACCESS ||
			it->first == D3D11_BIND_RENDER_TARGET ||
			it->first == D3D11_BIND_DEPTH_STENCIL
			)
		{
			it->second->Release();
		}
		it->second = NULL;
		it++;
	}
	viewPool.clear();
}

bool Resource::GenerateViews()
{
	HRESULT hr;
	//Views
	if (desc.bindFlag&D3D11_BIND_UNORDERED_ACCESS)
	{
		ID3D11UnorderedAccessView* uav = NULL;
		hr = PipeLine::pDevice->CreateUnorderedAccessView(ptr, NULL, &uav);
		if (FAILED(hr))
		{
			ClearViews();
			return false;
		}
		viewPool[D3D11_BIND_UNORDERED_ACCESS] = uav;
	}
	if (desc.bindFlag&D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView* srv = NULL;
		hr = PipeLine::pDevice->CreateShaderResourceView(ptr, NULL, &srv);
		if (FAILED(hr))
		{
			ClearViews();
			return false;
		}
		viewPool[D3D11_BIND_SHADER_RESOURCE] = srv;
	}
	if (desc.bindFlag&D3D11_BIND_RENDER_TARGET)
	{
		ID3D11RenderTargetView* rtv;
		hr = PipeLine::pDevice->CreateRenderTargetView(ptr, NULL, &rtv);
		if (FAILED(hr))
		{
			ClearViews();
			return false;
		}
		viewPool[D3D11_BIND_RENDER_TARGET] = rtv;
	}
	if (desc.bindFlag&D3D11_BIND_DEPTH_STENCIL)
	{
		ID3D11DepthStencilView* dsv = NULL;
		hr = PipeLine::pDevice->CreateDepthStencilView(ptr, NULL, &dsv);
		if (FAILED(hr))
		{
			ClearViews();
			return false;
		}
		viewPool[D3D11_BIND_DEPTH_STENCIL] = dsv;
	}
	//Buffers
	if (desc.bindFlag&D3D11_BIND_CONSTANT_BUFFER)
	{
		viewPool[D3D11_BIND_CONSTANT_BUFFER] = ptr;
	}
	if (desc.bindFlag&D3D11_BIND_INDEX_BUFFER)
	{
		viewPool[D3D11_BIND_INDEX_BUFFER] = ptr;
	}
	if (desc.bindFlag&D3D11_BIND_VERTEX_BUFFER)
	{
		viewPool[D3D11_BIND_VERTEX_BUFFER] = ptr;
	}
	if (desc.bindFlag&D3D11_BIND_STREAM_OUTPUT)
	{
		viewPool[D3D11_BIND_STREAM_OUTPUT] = ptr;
	}
	return true;
}

bool Resource::ResetData(const UINT value[4]) 
{
	if (viewPool.count(D3D11_BIND_UNORDERED_ACCESS))
	{
		PipeLine::pContext->ClearUnorderedAccessViewUint((ID3D11UnorderedAccessView*)viewPool[D3D11_BIND_UNORDERED_ACCESS], value);
	}
	else if (viewPool.count(D3D11_BIND_RENDER_TARGET))
	{
		ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)viewPool[D3D11_BIND_RENDER_TARGET];
		PipeLine::pContext->ClearRenderTargetView(rtv, (float*)value);
	}
	else 	if (viewPool.count(D3D11_BIND_DEPTH_STENCIL))
	{
		ID3D11DepthStencilView* dsv = (ID3D11DepthStencilView*)viewPool[D3D11_BIND_DEPTH_STENCIL];
		float a = *((float*)&value[1]);
		PipeLine::pContext->ClearDepthStencilView(dsv, value[0], a, 0);
	}
	else 
	{
		return false;
	}
	return true;
}

bool Resource::UpdateData(void * pData, size_t size) 
{
	UINT rowPitch = 0;
	if(desc.type==Resource_Texture2D)
		rowPitch = desc.size[1]*(FormatSizeTable[desc.format] / 8);
	if (rowPitch < 0)
		return false;
	if (desc.access == Access_Dynamic)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		result = PipeLine::pContext->Map(ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}
		memcpy(mappedResource.pData, pData, size);
		PipeLine::pContext->Unmap(ptr, 0);
	}
	else if (desc.access == Access_Default)
	{
		static D3D11_BOX box1D;
		box1D.left = 0;
		box1D.right = 0;
		box1D.top = 0;
		box1D.bottom = 1;
		box1D.front = 0;
		box1D.back = 1;
		box1D.right = size;
		PipeLine::pContext->UpdateSubresource(ptr, 0, NULL, pData, rowPitch, 0);
	}
	return true;
}

bool Resource::GenerateMips()
{
	if (desc.mipLevel != 1 && (desc.bindFlag&Bind_Shader_Resource)&&(desc.bindFlag&Bind_Render_Target))
	{
		ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)viewPool[D3D11_BIND_SHADER_RESOURCE];
		PipeLine::pContext->GenerateMips(srv);
		return true;
	}
	return false;
}

bool Resource::CreateBuffer(void* pData, size_t dataSize)
{
	if (ptr)  Release();
	if (pData && (dataSize > desc.size[0]))
	{
		return false;
	}
	//----------------------Create description--------------------------
	desc.mipLevel = 1;
	desc.size[1] = desc.size[2] = 0;
	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));
	bufDesc.BindFlags = desc.bindFlag;
	bufDesc.ByteWidth = desc.size[0];
	if (desc.access == Access_Dynamic)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (desc.access == Access_Staging)
	{
		bufDesc.Usage = D3D11_USAGE_STAGING;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	}
	else if (desc.access == Access_Immutable)
	{
		bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufDesc.CPUAccessFlags = 0;
	}
	else
	{
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.CPUAccessFlags = 0;
	}
	if (desc.elementStride != 0 && !(desc.bindFlag & Bind_Vertex_Buffer) && !(desc.bindFlag & Bind_Index_Buffer))
	{
		bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufDesc.StructureByteStride = desc.elementStride;

	}
	//------------------------------Create Resource-----------------------
	HRESULT hr;
	ID3D11Buffer* buffer;
	if (pData&&dataSize)
	{
		D3D11_SUBRESOURCE_DATA sbData;
		sbData.pSysMem = pData;
		sbData.SysMemPitch = dataSize;
		sbData.SysMemSlicePitch = dataSize;//No need
		hr = PipeLine::pDevice->CreateBuffer(&bufDesc, &sbData, &buffer);
	}
	else
	{
		hr = PipeLine::pDevice->CreateBuffer(&bufDesc, NULL, &buffer);
	}
	if (FAILED(hr)) return false;

	ptr = buffer;
	return true;
}

bool Resource::CreateTexture2D(void * pData, size_t dataSize)
{
	if (ptr)  Release();
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.MipLevels = desc.mipLevel;
	texDesc.ArraySize = 1;
	texDesc.Format = desc.format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Width = desc.size[0];
	texDesc.Height = desc.size[1];
	texDesc.BindFlags = desc.bindFlag;

	if (desc.mipLevel!=1)
	{
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (desc.access == Access_Dynamic)
	{
		texDesc.Usage = D3D11_USAGE_DYNAMIC;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (desc.access == Access_Staging)
	{
		texDesc.Usage = D3D11_USAGE_STAGING;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	}
	//else if (desc.access == Immutable)
	//{
	//	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	texDesc.CPUAccessFlags = 0;
	//}
	else
	{
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.CPUAccessFlags = 0;
	}
	
	UINT pixelByte = FormatSizeTable[texDesc.Format] / 8;
	if (pixelByte <= 0) return false;
	HRESULT hr;
	ID3D11Texture2D* Texture;
	hr = PipeLine::pDevice->CreateTexture2D(&texDesc, NULL, &Texture);
	if (FAILED(hr)) return false;

	if (pData)
	{
		PipeLine::pContext->UpdateSubresource(Texture, 0, NULL, pData, texDesc.Width* pixelByte, 0);
		
	}
	ptr = Texture;
	return true;
}

bool Resource::CreateTexture3D(void * pData, size_t dataSize)
{
	if (ptr)  Release();
	D3D11_TEXTURE3D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.MipLevels = 0;
	texDesc.Format = desc.format;
	texDesc.Height = desc.size[0];
	texDesc.Width = desc.size[1];
	texDesc.Depth = desc.size[2];
	texDesc.BindFlags = desc.bindFlag;

	if (desc.mipLevel != 1)
	{
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (desc.access == Access_Dynamic)
	{
		texDesc.Usage = D3D11_USAGE_DYNAMIC;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (desc.access == Access_Staging)
	{
		texDesc.Usage = D3D11_USAGE_STAGING;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	}
	//else if (desc.access == Immutable)
	//{
	//	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	texDesc.CPUAccessFlags = 0;
	//}
	else
	{
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.CPUAccessFlags = 0;
	}

	UINT pixelByte = FormatSizeTable[texDesc.Format] / 8;
	if (pixelByte <= 0) return false;
	HRESULT hr;
	ID3D11Texture3D* texture;
	hr = PipeLine::pDevice->CreateTexture3D(&texDesc, NULL, &texture);
	if (FAILED(hr)) return false;

	if (pData)
	{
		UINT rowpitch = texDesc.Width* pixelByte;
		UINT depthpitch = rowpitch*texDesc.Height;
		PipeLine::pContext->UpdateSubresource(texture, 0, NULL, pData, rowpitch, depthpitch);
	}
	ptr = texture;
	return true;
}

Resource * Resource::Create(ResourceDesc desc, void * pData, size_t dataSize)
{
	Resource* r = new Resource(desc, pData, dataSize);
	if (r->ptr == NULL) {
		delete r;
		r = NULL;
	}
	return r;
}

ResourceDesc Resource::GetDesc(ID3D11Resource * pD3DResource)
{
	ResourceDesc desc;
	D3D11_RESOURCE_DIMENSION type;
	pD3DResource->GetType(&type);
	bool hasUAV = false, hasSRV = false;
	if (type == D3D11_RESOURCE_DIMENSION_BUFFER)
	{
		desc.type = Resource_Buffer;
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		ID3D11Buffer * pBuffer = (ID3D11Buffer *)pD3DResource;
		pBuffer->GetDesc(&bufferDesc);
		desc.bindFlag = bufferDesc.BindFlags;
		desc.access = (AccessType)bufferDesc.Usage;
		desc.size[0] = bufferDesc.ByteWidth;
		desc.elementStride = bufferDesc.StructureByteStride;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		desc.type = Resource_Texture2D;
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture2D* pTexture = (ID3D11Texture2D*)pD3DResource;
		pTexture->GetDesc(&textureDesc);
		desc.bindFlag = textureDesc.BindFlags;
		desc.access = (AccessType)textureDesc.Usage;
		desc.size[0] = textureDesc.Width;
		desc.size[1] = textureDesc.Height;
		desc.format = textureDesc.Format;
		desc.mipLevel = textureDesc.MipLevels;
	}
	else if (type == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
	{
		desc.type = Resource_Texture3D;
		D3D11_TEXTURE3D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ID3D11Texture3D* pTexture = (ID3D11Texture3D*)pD3DResource;
		pTexture->GetDesc(&textureDesc);
		desc.bindFlag = textureDesc.BindFlags;
		desc.access = (AccessType)textureDesc.Usage;
		desc.size[0] = textureDesc.Width;
		desc.size[1] = textureDesc.Height;
		desc.size[2] = textureDesc.Depth;
		desc.format = textureDesc.Format;
		desc.mipLevel = textureDesc.MipLevels;
	}
	return desc;
}

Resource * Resource::GetBackBuffer()
{
	if (pBackBuffer) return pBackBuffer;
	pBackBuffer = new Resource();
	HRESULT hr = PipeLine::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(pBackBuffer->ptr));
	if (FAILED(hr)) return NULL;
	pBackBuffer->desc = GetDesc(pBackBuffer->ptr);
	if (!pBackBuffer->GenerateViews()) {
		delete pBackBuffer;
		pBackBuffer = NULL;
		//**
	}
	return pBackBuffer;
}


ResourceManager::ResourceManager()
{
	currentDSV = NULL;
	backBufferID = INVALID;
}

ResourceManager::~ResourceManager() {}

void ResourceManager::Release(Resource *& element)
{
	delete element;
	element = NULL;
}

void ResourceManager::Bind(UINT stages, D3D11_BIND_FLAG bindFlag, UINT startSlot, UINT numViews, void ** ptr, UINT elementStride, UINT offset)
{
	if (bindFlag == D3D11_BIND_VERTEX_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		PipeLine::pContext->IASetVertexBuffers(startSlot, numViews, buffer, &elementStride, &offset);
	}
	else if (bindFlag == D3D11_BIND_INDEX_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		PipeLine::pContext->IASetIndexBuffer(*buffer, DXGI_FORMAT_R32_UINT, offset);
	}
	else if (bindFlag == D3D11_BIND_STREAM_OUTPUT)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		PipeLine::pContext->SOSetTargets(numViews, buffer, &offset);
	}
	else if (bindFlag == D3D11_BIND_UNORDERED_ACCESS)
	{
		ID3D11UnorderedAccessView** uav = (ID3D11UnorderedAccessView**)ptr;
		if (stages & Stage_Compute_Shader)
		{
			PipeLine::pContext->CSSetUnorderedAccessViews(startSlot, numViews, uav, NULL);
		}
		if (stages&Stage_Output_Merge)
		{
			PipeLine::pContext->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, NULL, NULL, startSlot, numViews, uav, NULL);
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
		PipeLine::pContext->OMSetRenderTargetsAndUnorderedAccessViews(currentRTVs.size(), rtvptr, *dsv, 4, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
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
		for (UINT i = 0; i < numViews; i++)
		{
			currentRTVs.push_back(rtv[i]);
		}
		PipeLine::pContext->OMSetRenderTargetsAndUnorderedAccessViews(numViews, rtv, dsv, 4, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
	}
	else if (bindFlag == D3D11_BIND_CONSTANT_BUFFER)
	{
		ID3D11Buffer** buffer = (ID3D11Buffer**)ptr;
		if (stages & Stage_Vertex_Shader)
		{
			PipeLine::pContext->VSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Hull_Shader)
		{
			PipeLine::pContext->HSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Domain_Shader)
		{
			PipeLine::pContext->DSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Geometry_Shader)
		{
			PipeLine::pContext->GSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Pixel_Shader)
		{
			PipeLine::pContext->PSSetConstantBuffers(startSlot, numViews, buffer);
		}
		if (stages & Stage_Compute_Shader)
		{
			PipeLine::pContext->CSSetConstantBuffers(startSlot, numViews, buffer);
		}
	}
	else if (bindFlag == D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView** srv = (ID3D11ShaderResourceView**)ptr;
		if (stages & Stage_Vertex_Shader)
		{
			PipeLine::pContext->VSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Hull_Shader)
		{
			PipeLine::pContext->HSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Domain_Shader)
		{
			PipeLine::pContext->DSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Geometry_Shader)
		{
			PipeLine::pContext->GSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Pixel_Shader)
		{
			PipeLine::pContext->PSSetShaderResources(startSlot, numViews, srv);
		}
		if (stages & Stage_Compute_Shader)
		{
			PipeLine::pContext->CSSetShaderResources(startSlot, numViews, srv);
		}
	}
}

bool ResourceManager::SetBinding(PiplineStage stage, BindFlag bindFlag, UINT startSlot, vector< int>& idList)
{
	D3D11_BIND_FLAG d3dbindFlag = (D3D11_BIND_FLAG)bindFlag;
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
	for (UINT i = 0; i < idList.size(); i++)
	{
		int &id = idList[i];
		if (id == INVALID)
		{
			newView[i] = NULL;
		}
		else if (Exist(id) && pool[id]->viewPool.count(d3dbindFlag))
		{
			newView[i] = pool[id]->viewPool[d3dbindFlag];
			stride = pool[id]->desc.elementStride;
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

int ResourceManager::Create(ResourceDesc desc, void * pData, size_t dataSize)
{
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	Resource* r = Resource::Create(desc, pData, dataSize);
	if (!r) return INVALID;
	pool[id] = r;
	return id;
}

int ResourceManager::CreateFromFile(const string & filePath)
{
	return Create(LoadResourceDesc(filePath));
}

int ResourceManager::GetBackBuffer()
{
	if (backBufferID != INVALID) return backBufferID;
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	Resource* r = Resource::GetBackBuffer();
	if (!r) return INVALID;
	pool[id] = r;
	backBufferID = id;
	return backBufferID;
}

bool ResourceManager::UpdateResourceData(UINT resourceID, void * pData, UINT size)
{
	if (!Exist(resourceID)) return false;
	return pool[resourceID]->UpdateData(pData, size);
}

void ResourceManager::CopyResourceData(UINT srcID, UINT dstID)
{
	if (!Exist(srcID) || !Exist(dstID))
		return;
	//deviceContextPtr->CopyResource(resourcePool[dstID].ptr, resourcePool[srcID].ptr);
}

void ResourceManager::Reset(UINT id, const float value[4])
{
	if (Exist(id)) pool[id]->ResetData((const UINT*)value);
}

void ResourceManager::Reset(UINT id, const UINT value[4])
{
	if (Exist(id)) pool[id]->ResetData(value);
}

void ResourceManager::Reset(UINT id, UINT flag, float depth, UINT8 stencil)
{
	if (Exist(id))
	{
		UINT data[4];
		data[0] = flag;
		memcpy(&data[1], &depth, sizeof(float));
		pool[id]->ResetData(data);
	}
}

bool ResourceManager::GenerateMipMap(UINT id)
{
	//**
	if (Exist(id))
	{
		return pool[id]->GenerateMips();
	}
	return false;
}
