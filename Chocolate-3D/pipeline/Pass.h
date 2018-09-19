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

struct Operation
{
	OperationType type;
	UINT threadSize[3];
	UINT resourceID;
	UINT value[4];
	Operation();
};

enum OperationType
{
	Draw,
	Compute,
	Generate_Mip,
	Reset,
	Copy
};

struct Pass
{
	int vertexShaderID;
	int geometryShaderID;
	int pixelShaderID;
	int computShaderID;
	int depthStencilStateID;
	int rasterizorStateID;
	int bendStateID;
	int viewPortID;
	vector<BindingRule> samplerBinding;
	vector<BindingRule> resourceBinding;//Engine will unbind those resources from pipline after rendering
	D3D_PRIMITIVE_TOPOLOGY topology;
	vector<Operation> operations;

	Pass();
};