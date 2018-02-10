//Basic warp of D3D API
//Author Sentao
#include"D3DManager.h"
using namespace std;

bool D3DManager::Init(HWND window, bool fullscreen)
{

	resolutionX = 1280;
	resolutionY = 720;
	hwnd = window;
	SetDefaultSwapChainDesc(fullscreen);

	SetDefaultViewport();
	
	HRESULT result;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &devicePtr, NULL, &deviceContextPtr);
	if (FAILED(result))
	{
		return false;
	}

	vertexShader.Init(devicePtr, deviceContextPtr);
	inputLayout.Init(devicePtr, deviceContextPtr);
	pixelShader.Init(devicePtr, deviceContextPtr);
	geometryShader.Init(devicePtr, deviceContextPtr);
	computeShader.Init(devicePtr, deviceContextPtr);
	resources = ResourceManager(devicePtr, deviceContextPtr);
	states.Init(devicePtr, deviceContextPtr);
	result = devicePtr->QueryInterface(__uuidof(IDXGIDevice), (void **)&dXGIDevicePtr);
	if (FAILED(result))
	{
		return false;
	}

	result = graphicDevice.factory->CreateSwapChain(devicePtr, &DefaultSwapChainDesc, &swapChainPtr);
	if (FAILED(result))
	{
		return false;
	}
	backBufferID = resources.CreateBackBuffer(swapChainPtr);
	bool ts = resources.SetBinding(Stage_Output_Merge, Bind_Render_Target, 0, backBufferID);


	SetDefaultSampler();
	UpdateViewPort();

	SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void D3DManager::SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY typ)
{
	deviceContextPtr->IASetPrimitiveTopology(typ);
}

void D3DManager::UpdateViewPort()
{
	deviceContextPtr->RSSetViewports(1, &viewport);
}

void D3DManager::Shutdown()
{
	vertexShader.Clear();
	pixelShader.Clear();
	inputLayout.Clear();
	resources.Clear();


	if (swapChainPtr)
	{
		swapChainPtr->SetFullscreenState(false, NULL);
		swapChainPtr->Release();
		swapChainPtr = NULL;
	}

	if (deviceContextPtr)
	{
		deviceContextPtr->Release();
		deviceContextPtr = NULL;
	}

	if (devicePtr)
	{
		devicePtr->Release();
		devicePtr = NULL;
	}

	if (dXGIDevicePtr)
	{
		dXGIDevicePtr->Release();
		dXGIDevicePtr = NULL;
	}

}

void D3DManager::Draw(UINT indexCount, UINT instanceCount)
{
	deviceContextPtr->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

void D3DManager::Compute(UINT threadCountX, UINT threadCountY, UINT threadCountZ)
{
	deviceContextPtr->Dispatch(threadCountX, threadCountY, threadCountZ);
}

void D3DManager::Swap()
{
	swapChainPtr->Present(0, 0);
}

void D3DManager::SetDefaultSwapChainDesc(bool fullscreen)
{
	ZeroMemory(&DefaultSwapChainDesc, sizeof(DefaultSwapChainDesc));


	DefaultSwapChainDesc.Windowed = !fullscreen;

	// Set to a single back buffer.
	DefaultSwapChainDesc.BufferCount = 1;
	DefaultSwapChainDesc.BufferDesc.Width = resolutionX;
	DefaultSwapChainDesc.BufferDesc.Height = resolutionY;

	// Set regular 32-bit surface for the back buffer.
	DefaultSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the usage of the back buffer.
	DefaultSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	DefaultSwapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	DefaultSwapChainDesc.SampleDesc.Count = 1;
	DefaultSwapChainDesc.SampleDesc.Quality = 0;

	// Set the scan line ordering and scaling to unspecified.
	DefaultSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	DefaultSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	DefaultSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	DefaultSwapChainDesc.Flags = 0;
}

void D3DManager::SetDefaultViewport()
{
	// Setup the viewport for rendering.
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = (float)resolutionX;
	viewport.Height = (float)resolutionY;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
}

void D3DManager::SetDefaultSampler()
{
	states.sampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	states.sampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	states.sampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	states.sampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	states.sampler.MipLODBias = 0.0f;
	states.sampler.MaxAnisotropy = 1;
	states.sampler.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	states.sampler.BorderColor[0] = 0;
	states.sampler.BorderColor[1] = 0;
	states.sampler.BorderColor[2] = 0;
	states.sampler.BorderColor[3] = 0;
	states.sampler.MinLOD = 0;
	states.sampler.MaxLOD = D3D11_FLOAT32_MAX;
	states.UpdateSamplerState(0xffff, Slot_Sampler_Default);

	states.sampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	states.sampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	states.sampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	states.UpdateSamplerState(0xffff, Slot_Sampler_Clamp);
}

D3DManager::~D3DManager()
{
	Shutdown();
}