#include"StateManager.h"

int StateManager::GenerateSSID()
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (!ssPool.count(i))
		{
			return i;
		}
	}
	return -1;
}

int StateManager::GenerateDSSID()
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (!dssPool.count(i))
		{
			return i;
		}
	}
	return -1;
}

int StateManager::GenerateRSID()
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (!rsPool.count(i))
		{
			return i;
		}
	}
	return -1;
}

int StateManager::GenerateBSID()
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (!bsPool.count(i))
		{
			return i;
		}
	}
	return -1;
}

StateManager::StateManager()
{

}

StateManager::StateManager(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	devicePtr = pDevice;
	deviceContextPtr = pDeviceContext;
}

StateManager::~StateManager()
{
	ClearSamplerState();
	ClearRasterizerState();
	ClearBlendState();
	ClearDepthStencilState();
}

int StateManager::CreateState(D3D11_SAMPLER_DESC & samplerDesc)
{
	int id = GenerateSSID();
	if (id < 0)
	{
		return -1;
	}
	ID3D11SamplerState* sampler;
	HRESULT hr = devicePtr->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr))
		return -1;
	SamplerState samplerdata;
	samplerdata.ptr = sampler;
	ssPool[id] = samplerdata;
	return id;
}

int StateManager::CreateState(D3D11_RASTERIZER_DESC & rasterDesc)
{
	int id = GenerateRSID();
	if (id < 0)
	{
		return -1;
	}
	ID3D11RasterizerState* raster;
	HRESULT hr = devicePtr->CreateRasterizerState(&rasterDesc, &raster);
	if (FAILED(hr))
		return -1;
	RasterizerState rasterdata;
	rasterdata.ptr = raster;
	rsPool[id] = rasterdata;
	return id;
}

int StateManager::CreateState(D3D11_BLEND_DESC & blendDesc)
{
	int id = GenerateBSID();
	if (id < 0)
	{
		return -1;
	}
	ID3D11BlendState* blender;
	HRESULT hr = devicePtr->CreateBlendState(&blendDesc, &blender);
	if (FAILED(hr))
		return -1;
	BlendState blenderdata;
	blenderdata.ptr = blender;
	bsPool[id] = blenderdata;
	return id;
}

int StateManager::CreateState(D3D11_DEPTH_STENCIL_DESC & depthStencilDesc)
{
	int id = GenerateDSSID();
	if (id < 0)
	{
		return -1;
	}
	ID3D11DepthStencilState* dss;
	HRESULT hr = devicePtr->CreateDepthStencilState(&depthStencilDesc, &dss);
	if (FAILED(hr))
		return -1;
	DepthStencilState dssdata;
	dssdata.ptr = dss;
	dssPool[id] = dssdata;
	return id;
}

void StateManager::ClearSamplerState(UINT id)
{
	if (!ssPool.count(id))
		return;
	ssPool[id].ptr->Release();
	ssPool.erase(id);
}

void StateManager::ClearRasterizerState(UINT id)
{
	if (!rsPool.count(id))
		return;
	rsPool[id].ptr->Release();
	rsPool.erase(id);
}

void StateManager::ClearBlendState(UINT id)
{
	if (!bsPool.count(id))
		return;
	bsPool[id].ptr->Release();
	bsPool.erase(id);
}

void StateManager::ClearDepthStencilState(UINT id)
{
	if (!dssPool.count(id))
		return;
	dssPool[id].ptr->Release();
	dssPool.erase(id);
}

void StateManager::ClearSamplerState()
{
	map<int, SamplerState>::iterator it = ssPool.begin();
	while (it != ssPool.end())
	{
		it->second.ptr->Release();
		it++;
	}
	ssPool.clear();
}

void StateManager::ClearRasterizerState()
{
	map<int, RasterizerState>::iterator it = rsPool.begin();
	while (it != rsPool.end())
	{
		it->second.ptr->Release();
		it++;
	}
	rsPool.clear();
}

void StateManager::ClearBlendState()
{
	map<int, BlendState>::iterator it = bsPool.begin();
	while (it != bsPool.end())
	{
		it->second.ptr->Release();
		it++;
	}
	bsPool.clear();
}

void StateManager::ClearDepthStencilState()
{
	map<int, DepthStencilState>::iterator it = dssPool.begin();
	while (it != dssPool.end())
	{
		it->second.ptr->Release();
		it++;
	}
	dssPool.clear();
}

void StateManager::Clear()
{
	ClearSamplerState();
	ClearRasterizerState();
	ClearBlendState();
	ClearDepthStencilState();
}

bool StateManager::SetSamplerState(UINT stages, UINT slot, int id)
{
	ID3D11SamplerState* ptr=NULL;
	if (id >= 0)
	{
		if (!ssPool.count(id))
			return false;
		else
			ptr = ssPool[id].ptr;
	}
	if (stages & Stage_Vertex_Shader)
	{
			deviceContextPtr->VSSetSamplers(slot, 1, &ptr);
	}
	if (stages & Stage_Hull_Shader)
	{
			deviceContextPtr->HSSetSamplers(slot, 1, &ptr);
	}
	if (stages & Stage_Domain_Shader)
	{
		deviceContextPtr->DSSetSamplers(slot, 1, &ptr);
	}
	if (stages & Stage_Geometry_Shader)
	{
			deviceContextPtr->GSSetSamplers(slot, 1, &ptr);
	}
	if (stages & Stage_Pixel_Shader)
	{
			deviceContextPtr->PSSetSamplers(slot, 1, &ptr);
	}
	if (stages & Stage_Compute_Shader)
	{
			deviceContextPtr->CSSetSamplers(slot, 1, &ptr);
	}
	return true;
}

bool StateManager::SetRasterState(int id)
{
	ID3D11RasterizerState* ptr = NULL;
	if (id >= 0)
	{
		if (!rsPool.count(id))
			return false;
		else
			ptr = rsPool[id].ptr;
	}
	deviceContextPtr->RSSetState(ptr);
	return true;
}

bool StateManager::SetBlendState(int id, float factor[4], UINT mask)
{
	ID3D11BlendState* ptr = NULL;
	if (id >= 0)
	{
		if (!bsPool.count(id))
			return false;
		else
			ptr = bsPool[id].ptr;
	}
	deviceContextPtr->OMSetBlendState(ptr, factor, mask);
	return true;
}

bool StateManager::SetDepthStencilState(int id,UINT ref)
{
	ID3D11DepthStencilState* ptr = NULL;
	if (id >= 0)
	{
		if (!dssPool.count(id))
			return false;
		else
			ptr = dssPool[id].ptr;
	}
	deviceContextPtr->OMSetDepthStencilState(ptr,ref);
	return true;
}