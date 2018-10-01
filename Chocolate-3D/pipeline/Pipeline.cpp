#include"Pipeline.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

GraphicDevice::GraphicDevice()
{

	HRESULT result;
	unsigned int i;
	int error;
	size_t stringLength;

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	// Store the dedicated video card memory in megabytes.
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
}

GraphicDevice::~GraphicDevice()
{
	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;
}

ID3D11Device* PipeLine::pDevice = NULL;
ID3D11DeviceContext* PipeLine::pContext = NULL;
IDXGISwapChain* PipeLine::pSwapChain = NULL;
HWND PipeLine::hwnd = 0;
int PipeLine::resolutionX = 0;
int PipeLine::resolutionY = 0;

InputLayout & PipeLine::InputLayout()
{
	return InputLayout::GetInstance();
}

VertexShader & PipeLine::VertexShader()
{
	return VertexShader::GetInstance();
}

PixelShader & PipeLine::PixelShader()
{
	return PixelShader::GetInstance();
}

GeometryShader & PipeLine::GeometryShader()
{
	return GeometryShader::GetInstance();
}

ComputeShader & PipeLine::ComputeShader()
{
	return ComputeShader::GetInstance();
}

DepthStencilState & PipeLine::DepthStencilState()
{
	return	DepthStencilState::GetInstance();
}

BlendState & PipeLine::BlendState()
{
	return BlendState::GetInstance();
}

RasterizorState & PipeLine::RasterizorState()
{
	return RasterizorState::GetInstance();
}

SamplerState & PipeLine::SamplerState()
{
	return SamplerState::GetInstance();
}

ViewPort & PipeLine::ViewPort()
{
	return ViewPort::GetInstance();
}

ResourceManager & PipeLine::Resources()
{
	return ResourceManager::GetInstance();
}

bool PipeLine::Init()
{
	Shutdown();
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	HRESULT result;
	result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &pDevice, NULL, &pContext);
	if (FAILED(result)) return false;
	return true;
}

bool PipeLine::Init(UINT resolutionX, UINT resolutionY, HWND hwnd, bool fullScreen) 
{
	if (!Init()) 
		return false;
	if (!CreateSwapChain(resolutionX, resolutionY, hwnd, fullScreen))
	{
		Shutdown();
		return false;
	}
	PipeLine::hwnd = hwnd;
	PipeLine::resolutionX = resolutionX;
	PipeLine::resolutionY = resolutionY;

	return true;
}

void PipeLine::Shutdown()
{
	if (!pDevice && !pContext && !pSwapChain) return;
	InputLayout().Clear();
	VertexShader().Clear();
	PixelShader().Clear();
	GeometryShader().Clear();
	ComputeShader().Clear();
	Resources().Clear();
	DepthStencilState().Clear();
	BlendState().Clear();
	RasterizorState().Clear();
	SamplerState().Clear();
	ViewPort().Clear();
	if(pSwapChain)
		pSwapChain->Release();
	if (pContext)
		pContext->Release();
	if(pDevice)
		pDevice->Release();
	pDevice = NULL;
	pContext = NULL;
	pSwapChain = NULL;
	hwnd = 0;
	resolutionX = 0;
	resolutionY = 0;
}

bool PipeLine::CreateSwapChain(UINT resolutionX, UINT resolutionY, HWND hwnd, bool fullScreen)
{
	if (pSwapChain)
	{
		pSwapChain->Release();
		pSwapChain = NULL;
	}

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Windowed = !fullScreen;

	// Set to a single back buffer.
	desc.BufferCount = 1;
	desc.BufferDesc.Width = resolutionX;
	desc.BufferDesc.Height = resolutionY;

	// Set regular 32-bit surface for the back buffer.
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the usage of the back buffer.
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	desc.OutputWindow = hwnd;

	// Turn multisampling off.
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// Set the scan line ordering and scaling to unspecified.
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	desc.Flags = 0;

	HRESULT result;
	IDXGIFactory *factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;

	result = factory->CreateSwapChain(pDevice, &desc, &pSwapChain);
	if (FAILED(result))
	{
		factory->Release();
		return false;
	}
	factory->Release();
	return true;
}

void PipeLine::SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type)
{
	pContext->IASetPrimitiveTopology(type);
}

void PipeLine::Draw(UINT indexCount, UINT instanceCount)
{
	pContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

void PipeLine::Compute(UINT threadCountX, UINT threadCountY, UINT threadCountZ)
{
	pContext->Dispatch(threadCountX, threadCountY, threadCountZ);
}

void PipeLine::Swap()
{
	PipeLine::pSwapChain->Present(0, 0);
}

PipeLine::~PipeLine()
{
}
