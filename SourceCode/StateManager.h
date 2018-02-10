//-------------------------------------------------------------------------
//-------------------State Manager Class for DirectX11------------------
//Provide Blend, DepthStencil, Rasterizor, Sampler State Management
//Author: Sentao
//-------------------------------------------------------------------------

#pragma once
#include <d3d11_1.h>
#include"D3Def.h"
#include <map>
#include <vector>
using namespace std;

class StateManager
{
private:
	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;
	float blendFactor[4];
	UINT blendSampleMask;
	UINT depthStemcilRef;

public:
	D3D11_DEPTH_STENCIL_DESC depthStencil;
	D3D11_RASTERIZER_DESC rasterizor;
	D3D11_BLEND_DESC blend;
	D3D11_SAMPLER_DESC sampler;

	StateManager();
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void UpdateDepthStencilState();
	void UpdateRasterizorState();
	void UpdateBlendState();
	void UpdateSamplerState(UINT stage, UINT slot);
};