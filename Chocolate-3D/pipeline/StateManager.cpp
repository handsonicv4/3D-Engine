//-------------------------------------------------------------------------
//-------------------State Manager Class for DirectX11------------------
//Provide Blend, DepthStencil, Rasterizor, Sampler State Management
//Author: Sentao
//-------------------------------------------------------------------------

#include"StateManager.h"
#include"DescFileLoader.h"
#include"Pipeline.h"
using namespace FileLoader;

DepthStencilState::DepthStencilState()
{
}
DepthStencilState::~DepthStencilState() {}
void DepthStencilState::Release(ID3D11DepthStencilState *& element) 
{
	element->Release();
	element = NULL;
}

int DepthStencilState::Create(DepthStencilDesc desc)
{
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	ID3D11DepthStencilState* ds;
	HRESULT hr = PipeLine::pDevice->CreateDepthStencilState(&desc, &ds);
	if (FAILED(hr)) return INVALID;
	pool[id] = ds;
	return id;
}

int DepthStencilState::CreateFromFile(const string & filePath)
{
	return Create(LoadDepthStencilDesc(filePath));
}

void DepthStencilState::Apply(int id, UINT stencilRef)
{
	if (Exist(id))
	{
		PipeLine::pContext->OMSetDepthStencilState(pool[id], stencilRef);
	}
}

BlendState::BlendState() 
{
}
BlendState::~BlendState() {}
void BlendState::Release(ID3D11BlendState *& element)
{
	element->Release();
	element = NULL;
}

int BlendState::Create(BlendDesc desc)
{
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	ID3D11BlendState* bs;
	HRESULT hr = PipeLine::pDevice->CreateBlendState(&desc, &bs);
	if (FAILED(hr)) return INVALID;
	pool[id] = bs;
	return id;
}

int BlendState::CreateFromFile(const string & filePath)
{
	return Create(LoadBlendDesc(filePath));
}

void BlendState::Apply(int id, float blendFactor[4], UINT blendSampleMask)
{
	if (Exist(id))
	{
		PipeLine::pContext->OMSetBlendState(pool[id], blendFactor, blendSampleMask);
	}
}

RasterizorState::RasterizorState() 
{
}
RasterizorState::~RasterizorState() {}
void RasterizorState::Release(ID3D11RasterizerState *& element)
{
	element->Release();
	element = NULL;
}

int RasterizorState::Create(RasterizerDesc desc)
{
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	ID3D11RasterizerState* rs;
	HRESULT hr = PipeLine::pDevice->CreateRasterizerState(&desc, &rs);
	if (FAILED(hr)) return INVALID;
	pool[id] = rs;
	return id;
}

int RasterizorState::CreateFromFile(const string & filePath)
{
	return Create(LoadRasterizerDesc(filePath));
}

void RasterizorState::Apply(int id)
{
	if (Exist(id))
	{
		PipeLine::pContext->RSSetState(pool[id]);
	}
}

SamplerState::SamplerState() {}
SamplerState::~SamplerState() {}
void SamplerState::Release(ID3D11SamplerState *& element)
{
	element->Release();
	element = NULL;
}

int SamplerState::Create(SamplerDesc desc)
{
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	ID3D11SamplerState* ss;
	HRESULT hr = PipeLine::pDevice->CreateSamplerState(&desc, &ss);
	if (FAILED(hr)) return INVALID;
	pool[id] = ss;
	return id;
}

int SamplerState::CreateFromFile(const string & filePath)
{

	return Create(LoadSamplerDesc(filePath));
}

void SamplerState::Apply(int id, UINT stage, UINT slot)
{
	if (!Exist(id)) return;
	if (stage&Stage_Vertex_Shader)
	{
		PipeLine::pContext->VSSetSamplers(slot, 1, &pool[id]);
	}
	if (stage&Stage_Pixel_Shader)
	{
		PipeLine::pContext->PSSetSamplers(slot, 1, &pool[id]);
	}
	if (stage&Stage_Compute_Shader)
	{
		PipeLine::pContext->CSSetSamplers(slot, 1, &pool[id]);
	}
	if (stage&Stage_Domain_Shader)
	{
		PipeLine::pContext->DSSetSamplers(slot, 1, &pool[id]);
	}
	if (stage&Stage_Hull_Shader)
	{
		PipeLine::pContext->HSSetSamplers(slot, 1, &pool[id]);
	}
	if (stage&Stage_Geometry_Shader)
	{
		PipeLine::pContext->GSSetSamplers(slot, 1, &pool[id]);
	}
}


ViewPort::ViewPort() {}
ViewPort::~ViewPort() {};
int ViewPort::Create(const D3D11_VIEWPORT &desc)
{
	int id = GenerateID();
	if (id == INVALID) return INVALID;
	pool[id] = desc;
	return id;
}

int ViewPort::CreateFromFile(const string & filePath)
{
	return Create(LoadViewPort(filePath));
}

void ViewPort::Apply(vector<int> id)
{
	if (id.size() == 1)
	{
		PipeLine::pContext->RSSetViewports(1, &pool[id[0]]);
		return;
	}
	vector<D3D11_VIEWPORT> res;
	res.reserve(id.size());
	for (int i : id) 
	{
		res.push_back(pool[i]);
	}
	PipeLine::pContext->RSSetViewports(res.size(), &res[0]);
}

void ViewPort::Apply(int id)
{
	PipeLine::pContext->RSSetViewports(1, &pool[id]);
}