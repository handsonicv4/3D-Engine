#include"D3DManager.h"
using namespace std;

bool D3DManager::Init(HWND window, bool fullscreen)
{

	resolutionX = 1280;
	resolutionY = 720;
	hwnd = window;
	SetDefaultSwapChainDesc(fullscreen);
	SetDefaultDepthStencilDesc();
	SetDefaultRasterDesc();
	SetDefaultSamplerDesc();
	SetDefaultBlendDesc();
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

	shaders = ShaderManager(devicePtr, deviceContextPtr);
	resources = ResourceManager(devicePtr, deviceContextPtr);
	states = StateManager(devicePtr, deviceContextPtr);
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

	//depthStencilBufferID = resources.CreateTexture2D(D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT, false, false, resolutionX, resolutionY);
	//ts = resources.SetBinding(Output_Merge, D3D11_BIND_DEPTH_STENCIL, 0, depthStencilBufferID);

	DefaultDepthStateID = states.CreateState(DefaultDepthStencilDesc);
	states.SetDepthStencilState(DefaultDepthStateID, 1);
	//states.ClearDepthStencilState(depthStencilStateID);

	DefaultRasterStateID = states.CreateState(defaultRasterDesc);
	states.SetRasterState(DefaultRasterStateID);
	//states.ClearRasterizerState(rasterizerStateID);

	DefaultBlendStateID = states.CreateState(defaultBlendDesc);
	float factor[] = { 0,0,0,0 };
	states.SetBlendState(DefaultBlendStateID, factor, 0xffffffff);
	//states.ClearBlendState(blendStateID);

	DefaultSamplerStateID = states.CreateState(defaultSamplerDesc);
	states.SetSamplerState(Stage_Pixel_Shader, 0, DefaultSamplerStateID);
	//states.ClearSamplerState(samplerStateID);

	deviceContextPtr->RSSetViewports(1, &defaultViewport);
	deviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void D3DManager::Shutdown()
{
	shaders.Clear();
	resources.Clear();
	states.Clear();

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

void D3DManager::SetDefaultDepthStencilDesc()
{
	// Initialize the description of the stencil state.
	ZeroMemory(&DefaultDepthStencilDesc, sizeof(DefaultDepthStencilDesc));

	// Set up the description of the stencil state.
	DefaultDepthStencilDesc.DepthEnable = true;
	DefaultDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DefaultDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	DefaultDepthStencilDesc.StencilEnable = true;
	DefaultDepthStencilDesc.StencilReadMask = 0xFF;
	DefaultDepthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	DefaultDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DefaultDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DefaultDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DefaultDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	DefaultDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DefaultDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DefaultDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DefaultDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}

void D3DManager::SetDefaultRasterDesc()
{
	ZeroMemory(&defaultRasterDesc, sizeof(defaultRasterDesc));
	defaultRasterDesc.AntialiasedLineEnable = false;
	defaultRasterDesc.CullMode = D3D11_CULL_BACK;
	defaultRasterDesc.DepthBias = 0;
	defaultRasterDesc.DepthBiasClamp = 0.0f;
	defaultRasterDesc.DepthClipEnable = true;
	defaultRasterDesc.FillMode = D3D11_FILL_SOLID;
	defaultRasterDesc.FrontCounterClockwise = false;
	defaultRasterDesc.MultisampleEnable = false;
	defaultRasterDesc.ScissorEnable = false;
	defaultRasterDesc.SlopeScaledDepthBias = 0.0f;
}

void D3DManager::SetDefaultViewport()
{
	// Setup the viewport for rendering.
	ZeroMemory(&defaultViewport, sizeof(defaultViewport));
	defaultViewport.Width = (float)resolutionX;
	defaultViewport.Height = (float)resolutionY;
	defaultViewport.MinDepth = 0.0f;
	defaultViewport.MaxDepth = 1.0f;
	defaultViewport.TopLeftX = 0.0f;
	defaultViewport.TopLeftY = 0.0f;
}

void D3DManager::SetDefaultSamplerDesc()
{
	// Create a texture sampler state description.
	ZeroMemory(&defaultSamplerDesc, sizeof(defaultSamplerDesc));
	defaultSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	defaultSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	defaultSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	defaultSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	defaultSamplerDesc.MipLODBias = 0.0f;
	defaultSamplerDesc.MaxAnisotropy = 1;
	defaultSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	defaultSamplerDesc.BorderColor[0] = 0;
	defaultSamplerDesc.BorderColor[1] = 0;
	defaultSamplerDesc.BorderColor[2] = 0;
	defaultSamplerDesc.BorderColor[3] = 0;
	defaultSamplerDesc.MinLOD = 0;
	defaultSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
}

void D3DManager::SetDefaultBlendDesc()
{
	// Create an alpha enabled blend state description.
	ZeroMemory(&defaultBlendDesc, sizeof(defaultBlendDesc));
	defaultBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	defaultBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	defaultBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	defaultBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	defaultBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	defaultBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	defaultBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	defaultBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
}

D3DManager::~D3DManager()
{
	Shutdown();
}