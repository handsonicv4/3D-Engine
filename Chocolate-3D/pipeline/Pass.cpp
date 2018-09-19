#include"Pass.h"
using namespace std;
BindingRule::BindingRule()
{
	slot = 0;
	resourceID = -1;
}

Pass::Pass()
{
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

