/*-----------------------------------------------
*Pass is a simple struct to store all nessary 
*information (Pipline status) for a single Draw call
*Engine will use this information to configure graphich pipline 
*and make a draw call.
*-Author Sentao
*///---------------------------------------------

#include<windows.h>
#include <vector>
#include <d3d11_1.h>
#include "D3Def.h"
#include "ResourceManager.h"
using namespace std;

struct BindingRule
{
	PiplineStage stages;
	BindFlag flag;
	UINT slot;
	UINT resourceID;
	BindingRule();
};

struct Pass
{
	D3D11_VIEWPORT viewPort;
	D3D11_DEPTH_STENCIL_DESC depthStencil;
	D3D11_RASTERIZER_DESC rasterizer;
	D3D11_BLEND_DESC blend;

	int vertexShaderID;
	int geometryShaderID;
	int pixelShaderID;
	int computShaderID;
	int depthStencilStateID;
	int rasterizorStateID;
	int bendStateID;
	int viewPortID;
	vector<BindingRule> samplerBinding;
	D3D_PRIMITIVE_TOPOLOGY topology;

	//Engine will unbind those resources from pipline after rendering
	vector<BindingRule> resourceBinding;

	Pass();
};