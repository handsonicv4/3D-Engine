#pragma once
#include<unordered_map>
#include<d3d11_1.h>
#include"ResourceManager.h"
using namespace std;

namespace FileLoader {

	ResourceDesc LoadResourceDesc(const string &filePath);

	DepthStencilDesc LoadDepthStencilDesc(const string & filePath);

	BlendDesc LoadBlendDesc(const string & filePath);

	RasterizerDesc LoadRasterizerDesc(const string & filePath);

	SamplerDesc LoadSamplerDesc(const string & filePath);

	ViewPortDesc LoadViewPort(const string & filePath);
}
