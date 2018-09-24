#pragma once
#include"ResourceManager.h"
using namespace std;

namespace FileLoader {

	ResourceType parseResourceType(const string& str);

	AccessType parseAccessType(const string& str);

	BindFlag parseBindFlag(const string& str);

	PipelineStage parsePipelineStage(const string& str);

	DXGI_FORMAT parseFormat(const string& str);

	D3D11_COMPARISON_FUNC parseComparisionFunc(const string& str);

	D3D11_STENCIL_OP parseStencilOp(const string& str);

	D3D11_BLEND parseBlend(const string& str);

	D3D11_BLEND_OP parseBlendOp(const string& str);

	D3D11_FILL_MODE parseFillMode(const string& str);

	D3D11_CULL_MODE parseCullMode(const string& str);

	D3D11_FILTER parseFilter(const string& str);

	D3D11_TEXTURE_ADDRESS_MODE parseTextureAddressMode(const string& str);

	D3D11_PRIMITIVE_TOPOLOGY parseTopology(const string& str);

	ResourceDesc LoadResourceDesc(const string &filePath);

	DepthStencilDesc LoadDepthStencilDesc(const string & filePath);

	BlendDesc LoadBlendDesc(const string & filePath);

	RasterizerDesc LoadRasterizerDesc(const string & filePath);

	SamplerDesc LoadSamplerDesc(const string & filePath);

	ViewPortDesc LoadViewPort(const string & filePath);
}
