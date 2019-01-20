/*-----------------------------------------------
*Pass is a simple struct to store all nessary 
*information (Pipline status) for a single Draw call
*Engine will use this information to configure graphich pipline 
*and make a draw call.
*-Author Sentao
*///---------------------------------------------

#include<windows.h>
#include <vector>
#include <unordered_map>
#include <d3d11_1.h>
#include "D3Def.h"
#include "ResourceManager.h"
#include "json11/json11.hpp"
using namespace std;

enum PassType
{
	Pass_Default,
	Pass_PostProcessing
};

class ResourcePort
{
public:
	PipelineStage stages;
	BindFlag flag;
	UINT slot;
	ResourcePort(const json11::Json& json);
	void Bind(int resID) const;
	void Unbind() const;
};

class SamplerPort
{
public:
	PipelineStage stages;
	UINT slot;
	SamplerPort(const json11::Json& json);
	void Bind(int resID) const;
	void Unbind() const;
};


class Pass
{
public:
	PassType type;
	int vertexShaderID;
	int geometryShaderID;
	int pixelShaderID;
	int computShaderID;
	int depthStencilStateID;
	int rasterizorStateID;
	int blendStateID;
	int viewPortID;
	vector<SamplerPort> samplerBinding;
	vector<ResourcePort> resourceBinding;//Engine will unbind those resources from pipline after rendering
	D3D_PRIMITIVE_TOPOLOGY topology;

	Pass();
	Pass(const json11::Json& obj, const unordered_map<string, unordered_map<string, int>>& resourceMap);

	void Bind(const vector<int> & resourceID, const vector<int> & samplerID) const;
	void Unbind() const;

private:
	void Load(const string& key, const int& id);
	void LoadTopology(const string& topology);
	static Pass* currentPass;
};


enum OperationType
{
	Operation_Pass,
	Operation_Post_Proc,
	Operation_Reset,
	Operation_GenMip,
	Operation_Copy
};

class Operation
{
public:
	OperationType type;
	virtual void Execute() = 0;
};

class PassOperation : public Operation
{
public:
	int passID;
	vector<int> passSamplerID;
	vector<int> passResourceID;
	PassOperation(const Pass* pPass);
	void Execute();
private:
	const Pass* pPass;
	
};

class ResetOperation : public Operation
{
public:
	UINT targetID;
	UINT value[4];
	ResetOperation();
	void Execute();
};

class GenMipOperation : public Operation
{
public:
	UINT targetID;
	GenMipOperation();
	void Execute();
};


class Effect
{
public:
	pair<size_t, size_t> resolution;
	vector<Pass> passes;
	unordered_map<string, vector<Operation*>> renderer;
	static Effect* Create(const string & filePath); //User should delete effect to prevent memory leak.
	void Apply();
	int inputLayout;
	~Effect();
private:
	static int CreateResource(const string& type, const string& filePath);
	static void DeleteResource(const string & type, const int& id);

	unordered_map<string, int> passNameTable;
	unordered_map<string, unordered_map<string, int>> resourceMap;
	vector<pair<SamplerPort,int>> staticSamplers;
	vector<pair<ResourcePort,int>> staticResources;

	PassOperation LoadPassConfig(const json11::Json & json) const;
	ResetOperation LoadResetOp(const json11::Json & json) const;
	GenMipOperation LoadGenMipOp(const json11::Json & json) const;

	vector<Operation*> LoadRenderer(const json11::Json & json);

	Effect() {};
	Effect(Effect const&) : Effect() {}
	Effect& operator= (Effect const&) { return *this; }
};
