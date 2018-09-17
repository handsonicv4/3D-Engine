//-------------------------------------------------------------------------
//-------------------State Manager Class for DirectX11------------------
//Provide Blend, DepthStencil, Rasterizor, Sampler State Management
//Author: Sentao
//-------------------------------------------------------------------------

#include"StateManager.h"
#include"Pipeline.h"


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

int DepthStencilState::CreateFromFile(const string & file)
{
	DepthStencilDesc desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.StencilEnable = false;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	if (file == "r")
	{
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	else if (file == "d")
	{
		desc.DepthEnable = false;
	}
	
	return Create(desc);
}

void DepthStencilState::Apply(int id, UINT depthStencilRef)
{
	if (Exist(id))
	{
		PipeLine::pContext->OMSetDepthStencilState(pool[id], depthStencilRef);
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

int BlendState::CreateFromFile(const string & file)
{
	BlendDesc desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	return Create(desc);
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

int RasterizorState::CreateFromFile(const string & file)
{
	RasterizerDesc desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_BACK;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;
	return Create(desc);
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

int SamplerState::CreateFromFile(const string & file)
{
	SamplerDesc desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	if (file == "clamp")
	{
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	return Create(desc);
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

int ViewPort::CreateFromFile(const string & file)
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(viewPort));
	viewPort.Width = (float)1280;
	viewPort.Height = (float)800;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	return Create(viewPort);
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