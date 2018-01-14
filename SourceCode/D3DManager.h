#pragma once
#include<windows.h>
#include <d3d11_1.h>
#include"D3Device.h"
#include"ShaderManager.h"
#include"ResourceManager.h"
#include"StateManager.h"
using namespace std;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class D3DManager
{
public:
	HWND hwnd;
	int resolutionX, resolutionY;
	ShaderManager shaders;
	ResourceManager resources;
	StateManager states;
	int backBufferID;
	int DefaultDepthStateID;
	int DisableDepthStateID;
	int RWDepthStateID;
	int DefaultRasterStateID;
	int DefaultBlendStateID;
	int DisableBlendStateID;
	int DefaultSamplerStateID;

	virtual bool Init(HWND window, bool fullscreen);
	void Shutdown();

	void Draw(UINT indexCount, UINT instanceCount);
	void Compute(UINT threadCountX, UINT threadCountY, UINT threadCountZ);
	void Swap();
	~D3DManager();

private:
	
	GraphicDevice graphicDevice;
	IDXGIDevice *dXGIDevicePtr;
	IDXGISwapChain* swapChainPtr;

	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;

	D3D11_VIEWPORT defaultViewport;
	DXGI_SWAP_CHAIN_DESC DefaultSwapChainDesc;
	D3D11_DEPTH_STENCIL_DESC DefaultDepthStencilDesc;
	D3D11_RASTERIZER_DESC defaultRasterDesc;
	D3D11_BLEND_DESC defaultBlendDesc;
	D3D11_SAMPLER_DESC defaultSamplerDesc;

#pragma region SetDefaultDesc

	void SetDefaultSwapChainDesc(bool fullscreen);
	void SetDefaultDepthStencilDesc();
	void SetDefaultRasterDesc();
	void SetDefaultSamplerDesc();
	void SetDefaultBlendDesc();
	void SetDefaultViewport();

#pragma endregion

};
