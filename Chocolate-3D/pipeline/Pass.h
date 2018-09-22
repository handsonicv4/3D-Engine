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

enum PassOperationType
{
	Pass_Operation_Draw,
	Pass_Operation_Compute,
	Pass_Operation_Generate_Mip,
	Pass_Operation_Reset,
	Pass_Operation_Copy
};

struct PassOperation
{
	PassOperationType type;
	UINT threadSize[3];
	UINT resourceID;
	UINT value[4];
	PassOperation();
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
	vector<PassOperation> operations;

	Pass();
};

class Effect
{
public:
//Minimum render unit : a single Mesh batch ?  
//What if the material is different ? pixel shader and depth/blend state can ne different!
//What about meshes use the same material ?
//Different Mesh can not do skined instancing!
//Then the same mesh with the same material?
//What is the meaning of "the same material" ? do they have to have the same texture/diffuse color?
//No, as long as they can use the same pipeline state.
//The answer ?  Same mesh instances with the same renderer.

//what we got: renderer(Contracing.Opac, Contracing.Refrect, Contracing.Transparent)
//						 instance.mesh.material->renderer(Opac, Refrect, Transparent)
//						 materialBuffer[] : diffuse, specular, emission, texturesSlots!
//						 instanceBuffer[]: position, bindMatrixOffset, materialID
//renderer have orders
//for(model) for(mesh) for(instences with materials with the same renderer)//renderer switch redundency
//unordered_map<renderer, vector<material>> 
//for(rander) for(model) for(mesh) render all instance; O(randerer * meshs);
//SM: [shadow]
//Opac: [pre_z, direct_light];
//Trans: [trans_light]

	vector<int> vertexShaders;
	vector<int> geometryShaders;
	vector<int> pixelShaders;
	vector<int> computShaders;
	vector<int> depthStencilStates;
	vector<int> rasterizorStates;
	vector<int> bendStates;
	vector<int> samplerStates;
	vector<int> viewPorts;
	vector<int> resources;

	vector<Pass> passes;

	unordered_map<string, vector<int>> renderers;
};