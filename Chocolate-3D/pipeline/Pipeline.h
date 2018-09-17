#pragma once
#include<windows.h>
#include <d3d11_1.h>
#include "ShaderManager.h"
#include "StateManager.h"
#include "ResourceManager.h"
using namespace std;


class GraphicDevice
{
public:
	GraphicDevice();
	~GraphicDevice();

	int screenWidth, screenHeight;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	unsigned int numModes, numerator, denominator;
	int videoCardMemory;
	char videoCardDescription[128];
};


class PipeLine
{
	friend class InputLayout;
	friend class VertexShader;
	friend class PixelShader;
	friend class GeometryShader;
	friend class ComputeShader;
	friend class ResourceManager;
	friend class DepthStencilState;
	friend class BlendState;
	friend class RasterizorState;
	friend class SamplerState;
	friend class ViewPort;
	friend class Resource;
public:

	static InputLayout& InputLayout();
	static VertexShader& VertexShader();
	static PixelShader& PixelShader();
	static GeometryShader& GeometryShader();
	static ComputeShader& ComputeShader();

	static DepthStencilState& DepthStencilState();
	static BlendState& BlendState();
	static RasterizorState& RasterizorState();
	static SamplerState& SamplerState();
	static ViewPort& ViewPort();

	static ResourceManager& Resources();

	static bool Init(UINT resolutionX, UINT resolutionY, HWND hwnd, bool fullScreen);
	static void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type);
	static void Draw(UINT indexCount, UINT instanceCount);
	static void Compute(UINT threadCountX, UINT threadCountY, UINT threadCountZ);
	static void Swap();
	static void Shutdown();
private:
	PipeLine();
	~PipeLine();
	PipeLine(PipeLine const&);
	PipeLine& operator=(PipeLine const&) { return *this; };
	static ID3D11Device* pDevice;
	static ID3D11DeviceContext* pContext;
	static IDXGISwapChain* pSwapChain;
	static HWND hwnd;
	static int resolutionX;
	static int resolutionY;
	static bool CreateSwapChain(UINT resolutionX, UINT resolutionY, HWND hwnd, bool fullScreen);
};