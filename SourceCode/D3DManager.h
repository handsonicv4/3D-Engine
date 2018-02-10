//Basic warp of D3D API
//Author Sentao
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
	ResourceManager resources;
	StateManager states;
	InputLayout inputLayout;
	VertexShader vertexShader;
	PixelShader pixelShader;
	GeometryShader geometryShader;
	ComputeShader computeShader;

	D3D11_VIEWPORT viewport;

	int backBufferID;

	virtual bool Init(HWND window, bool fullscreen);
	void Shutdown();
	void UpdateViewPort();
	void Draw(UINT indexCount, UINT instanceCount);
	void Compute(UINT threadCountX, UINT threadCountY, UINT threadCountZ);
	void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY typ);
	void Swap();
	~D3DManager();

private:
	
	GraphicDevice graphicDevice;
	IDXGIDevice *dXGIDevicePtr;
	IDXGISwapChain* swapChainPtr;

	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;



	DXGI_SWAP_CHAIN_DESC DefaultSwapChainDesc;


#pragma region SetDefaultDesc

	void SetDefaultSwapChainDesc(bool fullscreen);
	void SetDefaultSampler();
	void SetDefaultViewport();

#pragma endregion

};
