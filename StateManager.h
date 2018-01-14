#pragma once
#include <d3d11_1.h>
#include"D3Def.h"
#include <map>
#include <vector>

using namespace std;

struct SamplerState
{
	ID3D11SamplerState* ptr;
	map<PiplineStage,int> bindingMap;
};

struct DepthStencilState
{
	ID3D11DepthStencilState* ptr;
};

struct RasterizerState
{
	ID3D11RasterizerState* ptr;
};

struct BlendState
{
	ID3D11BlendState* ptr;
};

class StateManager
{
	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;

	map<int, SamplerState> ssPool;
	map<int, DepthStencilState> dssPool;
	map<int, RasterizerState> rsPool;
	map<int, BlendState> bsPool;

	int GenerateSSID();
	int GenerateDSSID();
	int GenerateRSID();
	int GenerateBSID();

public:
	StateManager();
	StateManager(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~StateManager();
	int CreateState(D3D11_SAMPLER_DESC &samplerDesc);
	int CreateState(D3D11_RASTERIZER_DESC &rasterDesc);
	int CreateState(D3D11_BLEND_DESC &blendDesc);
	int CreateState(D3D11_DEPTH_STENCIL_DESC &depthStencilDesc);
	void ClearSamplerState(UINT id);
	void ClearRasterizerState(UINT id);
	void ClearBlendState(UINT id);
	void ClearDepthStencilState(UINT id);

	void ClearSamplerState();
	void ClearRasterizerState();
	void ClearBlendState();
	void ClearDepthStencilState();
	void Clear();

	bool SetSamplerState(UINT stages, UINT slot, int id);
	bool SetRasterState(int id);
	bool SetBlendState(int id, float factor[4],UINT mask);
	bool SetDepthStencilState(int id,UINT ref);
};