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
#include "json11/json11.hpp"
using namespace std;

struct BindingRule
{
	PipelineStage stages;
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
	PassOperation(const json11::Json& obj, const unordered_map<string, int>& resourceMap);
};



class Pass
{
public:
	int vertexShaderID;
	int geometryShaderID;
	int pixelShaderID;
	int computShaderID;
	int depthStencilStateID;
	int rasterizorStateID;
	int blendStateID;
	int viewPortID;
	vector<BindingRule> samplerBinding;
	vector<BindingRule> resourceBinding;//Engine will unbind those resources from pipline after rendering
	D3D_PRIMITIVE_TOPOLOGY topology;
	vector<PassOperation> operations;

	Pass();
	Pass(const json11::Json& obj, const unordered_map<string, unordered_map<string, int>>& resourceMap);
private:
	void Load(const string& key, const int& id);
	void LoadSampler(int id, PipelineStage stage, int slot);
	void LoadResource(int id, PipelineStage stage, int slot, BindFlag bindFlag);
	void LoadTopology(const string& topology);
};

class Effect
{
public:
	unordered_map<string, unordered_map<string, int>> resourceMap;

	int inputLayout;
	vector<Pass> passes;

	unordered_map<string, vector<int>> renderers;

	static Effect* Create(const string & filePath);
	
	~Effect();
private:
	Effect() {};
	Effect(Effect const&) : Effect() {}
	Effect& operator= (Effect const&) { return *this; }
	static int CreateResource(const string& type, const string& filePath);
	static void DeleteResource(const string & type, const int& id);
};