#include"Pass.h"
using namespace std;
BindingRule::BindingRule()
{
	slot = 0;
	resourceID = -1;
}

Pass::Pass()
{

	//Depth Stencil Defualt Value
	ZeroMemory(&depthStencil, sizeof(depthStencil));
	depthStencil.DepthEnable = true;
	depthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencil.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencil.StencilEnable = false;
	depthStencil.StencilReadMask = 0xFF;
	depthStencil.StencilWriteMask = 0xFF;
	depthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Rasterizer Defualt Value
	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.AntialiasedLineEnable = false;
	rasterizer.CullMode = D3D11_CULL_BACK;
	rasterizer.DepthBias = 0;
	rasterizer.DepthBiasClamp = 0.0f;
	rasterizer.DepthClipEnable = true;
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.FrontCounterClockwise = false;
	rasterizer.MultisampleEnable = false;
	rasterizer.ScissorEnable = false;
	rasterizer.SlopeScaledDepthBias = 0.0f;

	//Blend Defualt Value
	ZeroMemory(&blend, sizeof(blend));
	blend.RenderTarget[0].BlendEnable = false;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//View Port Defualt Value
	ZeroMemory(&viewPort, sizeof(viewPort));
	viewPort.Width = (float)1280;
	viewPort.Height = (float)800;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	//No Defualt Shaders
	vertexShaderID = -1;
	geometryShaderID = -1;
	pixelShaderID = -1;
	computShaderID = -1;
	depthStencilStateID = -1;
	rasterizorStateID = -1;
	bendStateID = -1;
	viewPortID = -1;

	//Defualt Topology
	topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

