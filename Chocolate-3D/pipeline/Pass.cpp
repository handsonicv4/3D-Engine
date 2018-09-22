#include"Pass.h"
using namespace std;
BindingRule::BindingRule()
{
	slot = 0;
	resourceID = -1;
}

PassOperation::PassOperation()
{
	type = Pass_Operation_Draw;
	memset(threadSize, 0, sizeof(threadSize));
	resourceID = 0;
	memset(value, 0, sizeof(value));
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

