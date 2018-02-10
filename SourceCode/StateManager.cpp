//-------------------------------------------------------------------------
//-------------------State Manager Class for DirectX11------------------
//Provide Blend, DepthStencil, Rasterizor, Sampler State Management
//Author: Sentao
//-------------------------------------------------------------------------

#include"StateManager.h"

StateManager::StateManager()
{
	ZeroMemory(&depthStencil, sizeof(depthStencil));
	ZeroMemory(&rasterizor, sizeof(rasterizor));
	ZeroMemory(&blend, sizeof(blend));
	ZeroMemory(&sampler, sizeof(sampler));
	blendFactor[0] = 1;
	blendFactor[1] = 1;
	blendFactor[2] = 1;
	blendFactor[3] = 1;
	blendSampleMask = 0xffffffff;
	depthStemcilRef = 1;
	devicePtr = NULL;
	deviceContextPtr = NULL;
}

void StateManager::Init(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	devicePtr = pDevice;
	deviceContextPtr = pDeviceContext;
}

void StateManager::UpdateDepthStencilState()
{
	ID3D11DepthStencilState* ds;
	devicePtr->CreateDepthStencilState(&depthStencil, &ds);
	deviceContextPtr->OMSetDepthStencilState(ds, depthStemcilRef);
	ds->Release();
}

void StateManager::UpdateRasterizorState()
{
	ID3D11RasterizerState* rs;
	devicePtr->CreateRasterizerState(&rasterizor, &rs);
	deviceContextPtr->RSSetState(rs);
	rs->Release();
}

void StateManager::UpdateBlendState()
{
	ID3D11BlendState* bs;
	devicePtr->CreateBlendState(&blend, &bs);
	deviceContextPtr->OMSetBlendState(bs, blendFactor, blendSampleMask);
	bs->Release();
}

void StateManager::UpdateSamplerState(UINT stage, UINT slot)
{
	ID3D11SamplerState* ss;
	devicePtr->CreateSamplerState(&sampler, &ss);
	if (stage&Stage_Vertex_Shader)
	{
		deviceContextPtr->VSSetSamplers(slot, 1, &ss);
	}
	if (stage&Stage_Pixel_Shader)
	{
		deviceContextPtr->PSSetSamplers(slot, 1, &ss);
	}
	if (stage&Stage_Compute_Shader)
	{
		deviceContextPtr->CSSetSamplers(slot, 1, &ss);
	}
	if (stage&Stage_Domain_Shader)
	{
		deviceContextPtr->DSSetSamplers(slot, 1, &ss);
	}
	if (stage&Stage_Hull_Shader)
	{
		deviceContextPtr->HSSetSamplers(slot, 1, &ss);
	}
	if (stage&Stage_Geometry_Shader)
	{
		deviceContextPtr->GSSetSamplers(slot, 1, &ss);
	}

	ss->Release();
}