//-------------------------------------------------------------------------
//-------------------State Manager Class for DirectX11------------------
//Provide Blend, DepthStencil, Rasterizor, Sampler and ViewPort Management
//Author: Sentao
//-------------------------------------------------------------------------

#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include "IDContainer.h"
#include "Singleton.h"
using namespace std;

typedef D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;

typedef D3D11_BLEND_DESC BlendDesc;

typedef D3D11_RASTERIZER_DESC RasterizerDesc;

typedef D3D11_SAMPLER_DESC SamplerDesc;

class DepthStencilState : public IDContainer<ID3D11DepthStencilState*>
{
	SINGLETON(DepthStencilState)
protected:
	void Release(ID3D11DepthStencilState* &element) override;
public:
	int Create(DepthStencilDesc desc);
	int CreateFromFile(const string& file);
	void Apply(int id, UINT depthStencilRef);
};


class BlendState : public IDContainer<ID3D11BlendState*>
{
	SINGLETON(BlendState)
protected:
	void Release(ID3D11BlendState* &element) override;
public:
	int Create(BlendDesc desc);
	int CreateFromFile(const string& file);
	void Apply(int id, float blendFactor[4], UINT blendSampleMask);
};

class RasterizorState : public IDContainer<ID3D11RasterizerState*>
{
	SINGLETON(RasterizorState)
protected:
	void Release(ID3D11RasterizerState* &element) override;
public:
	int Create(RasterizerDesc desc);
	int CreateFromFile(const string& file);
	void Apply(int id);
};

class SamplerState : public IDContainer<ID3D11SamplerState*>
{
	SINGLETON(SamplerState)
protected:
	void Release(ID3D11SamplerState* &element) override;
public:
	int Create(SamplerDesc desc);
	int CreateFromFile(const string& file);
	void Apply(int id, UINT stage, UINT slot);
};

class ViewPort : public IDContainer<D3D11_VIEWPORT>
{
	SINGLETON(ViewPort)
public:
	int Create(const D3D11_VIEWPORT& desc);
	int CreateFromFile(const string& file);
	void Apply(vector<int> id);
	void Apply(int id);
};