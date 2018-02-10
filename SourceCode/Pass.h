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
using namespace std;

struct ResourceBind
{
	PiplineStage stages;
	BindFlag flag;
	UINT slot;
	UINT resourceID;
	ResourceBind();
};

struct Pass
{
	D3D11_VIEWPORT viewPort;
	D3D11_DEPTH_STENCIL_DESC depthStencil;
	D3D11_RASTERIZER_DESC rasterizer;
	D3D11_BLEND_DESC blend;
	UINT vertexShaderID;
	UINT geometryShaderID;
	UINT pixelShaderID;
	D3D_PRIMITIVE_TOPOLOGY topology;

	//Engine will unbind those resources from pipline after rendering
	vector<ResourceBind> bindingTable;

	Pass();
};