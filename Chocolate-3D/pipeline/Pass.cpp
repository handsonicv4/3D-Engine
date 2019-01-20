#include"Pass.h"
#include <fstream>
#include <sstream>
#include"Pipeline.h"
#include"DescFileLoader.h"
#include <unordered_map>
using namespace std;
using namespace json11;

Pass* Pass::currentPass = NULL;

Pass::Pass()
{
	//No Defualt Shaders
	vertexShaderID = -1;
	geometryShaderID = -1;
	pixelShaderID = -1;
	computShaderID = -1;
	depthStencilStateID = -1;
	rasterizorStateID = -1;
	blendStateID = -1;
	viewPortID = -1;
	type = Pass_Default;
	//Defualt Topology
	topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void Pass::Load(const string& key, const int& id)
{
	if (key == "vertex_shader") vertexShaderID = id;
	else if (key == "pixel_shader") pixelShaderID = id;
	else if (key == "geometry_shader") geometryShaderID = id;
	else if (key == "depth_stencil_state") depthStencilStateID = id;
	else if (key == "blend_state") blendStateID = id;
	else if (key == "rasterizer_state") rasterizorStateID = id;
	else if (key == "view_port") viewPortID = id;
	else throw exception(("Can not parse : " + key).c_str());
}


void Pass::LoadTopology(const string & topology)
{
	this->topology = FileLoader::parseTopology(topology);
}

void Pass::Bind(const vector<int> & resourceID, const vector<int> & samplerID) const
{
	if(resourceID.size() != resourceBinding.size() ||
		samplerID.size() != samplerBinding.size())
		throw exception("Pass Cfg mismach!");

	if(currentPass != NULL) currentPass->Unbind();

	PipeLine::DepthStencilState().Apply(depthStencilStateID, 1);
	float bf[] = { 1,1,1,1 };
	PipeLine::BlendState().Apply(blendStateID, bf, 0xffffffff);
	PipeLine::RasterizorState().Apply(rasterizorStateID);

	PipeLine::ViewPort().Apply(viewPortID);

	PipeLine::VertexShader().Activate(vertexShaderID);
	PipeLine::PixelShader().Activate(pixelShaderID);
	PipeLine::GeometryShader().Activate(geometryShaderID);

	PipeLine::SetPrimitiveType(topology);
	//Set resource binding
	for (size_t i = 0; i < resourceBinding.size(); i++)
	{
		resourceBinding[i].Bind(resourceID[i]);
	}
	//Set sampler binding
	for (size_t i = 0; i < samplerBinding.size(); i++)
	{
		samplerBinding[i].Bind(samplerID[i]);
	}
	currentPass = const_cast<Pass*>(this);
}

void Pass::Unbind() const
{
	for (int i = 0; i < resourceBinding.size(); i++)
	{
		const auto &bind = resourceBinding[i];
		PipeLine::Resources().SetBinding(bind.stages, bind.flag, bind.slot, -1);
	}

	if (currentPass == this)
		currentPass == NULL;
}

Pass::Pass(const json11::Json & jpass, const unordered_map<string, unordered_map<string, int>>& resourceMap):Pass()
{
	if (!jpass.is_object())
		throw exception("Pass creation faild");
	auto& passData = jpass.object_items();
	for (auto& item : passData)
	{
		if (item.first == "type" && item.second.is_string())
		{
			if(item.second.string_value() == "post")
				type = Pass_PostProcessing;
		}
		else if (resourceMap.count(item.first))
		{
			const auto& subRes = resourceMap.find(item.first)->second;
			if (item.first == "sampler_state")
			{
				if (!item.second.is_array()) throw exception("Pass creation faild");
				for (auto& bind : item.second.array_items())
				{
					samplerBinding.push_back(move(SamplerPort(bind)));
				}
			}
			else if (item.first == "resource")
			{
				if (!item.second.is_array()) throw exception("Pass creation faild");
				for (auto& bind : item.second.array_items())
				{
					resourceBinding.push_back(move(ResourcePort(bind)));
				}
			}
			else
			{
				if (!item.second.is_string() || !subRes.count(item.second.string_value()))
					throw exception("Pass creation faild");
				Load(item.first, subRes.find(item.second.string_value())->second);
			}
		}

	}
}


int Effect::CreateResource(const string& type, const string& filePath)
{
	if(type == "vertex_shader") return PipeLine::VertexShader().Create(filePath, "main");
	else if (type == "pixel_shader") return PipeLine::PixelShader().Create(filePath, "main");
	else if (type == "geometry_shader") return PipeLine::GeometryShader().Create(filePath, "main");
	else if (type == "depth_stencil_state") return PipeLine::DepthStencilState().CreateFromFile(filePath);
	else if (type == "blend_state") return PipeLine::BlendState().CreateFromFile(filePath);
	else if (type == "rasterizer_state") return PipeLine::RasterizorState().CreateFromFile(filePath);
	else if (type == "sampler_state") return PipeLine::SamplerState().CreateFromFile(filePath);
	else if (type == "view_port") return PipeLine::ViewPort().CreateFromFile(filePath);
	else if (type == "resource") return PipeLine::Resources().CreateFromFile(filePath);
	else return -1;
}

void Effect::DeleteResource(const string& type, const int& id)
{
	if (type == "vertex_shader")  PipeLine::VertexShader().Delete(id);
	else if (type == "pixel_shader")  PipeLine::PixelShader().Delete(id);
	else if (type == "geometry_shader")  PipeLine::GeometryShader().Delete(id);
	else if (type == "depth_stencil_state")  PipeLine::DepthStencilState().Delete(id);
	else if (type == "blend_state")  PipeLine::BlendState().Delete(id);
	else if (type == "rasterizer_state")  PipeLine::RasterizorState().Delete(id);
	else if (type == "sampler_state")  PipeLine::SamplerState().Delete(id);
	else if (type == "view_port")  PipeLine::ViewPort().Delete(id);
	else if (type == "resource")  PipeLine::Resources().Delete(id);
	else throw exception("Unknow Resource Type");
}

PassOperation Effect::LoadPassConfig(const json11::Json & json) const
{
	if (!json.is_object() || 
		!json["pass"].is_string() || 
		!passNameTable.count(json["pass"].string_value()) ||
		(!json["sampler"].is_null() && !json["sampler"].is_array()) ||
		(!json["resource"].is_null() && !json["resource"].is_array())
		) 
		throw exception("LoadPassConfig error");

	
	int passID = passNameTable.at(json["pass"].string_value());
	PassOperation cfg(&passes[passID]);

	if (!json["sampler"].is_null())
	{
		for (const auto& s : json["sampler"].array_items())
		{
			if(!s.is_string() || !resourceMap.at("sampler").count(s.string_value())) 	throw exception("LoadPassConfig error");
			cfg.passSamplerID.push_back(resourceMap.at("sampler").at(s.string_value()));
		}
	}

	if (!json["resource"].is_null())
	{
		for (const auto& s : json["resource"].array_items())
		{
			if (!s.is_string() || !resourceMap.at("resource").count(s.string_value())) 	throw exception("LoadPassConfig Error");
			cfg.passResourceID.push_back(resourceMap.at("resource").at(s.string_value()));
		}
	}
	
	return move(cfg);
}

ResetOperation Effect::LoadResetOp(const json11::Json & json) const
{
	if (!json.is_object() || !json["reset"].is_string() || !json["value_type"].is_string() || !json["value"].is_array()) throw exception("Parse Operation Error");
	string key = json["reset"].string_value();
	if (!resourceMap.at("resource").count(key))  throw exception("Parse Operation Error");

	ResetOperation op;
	op.targetID = resourceMap.at("resource").at(key);

	const auto& arr = json["value"].array_items();
	if (json["value_type"].string_value() == "float")
	{
		if (arr.size() != 4) throw exception("Parse ResetOperation Error");
		for (int i = 0; i < 4; i++)
		{
			float v = arr[i].number_value();
			op.value[i] = *((UINT*)&v);
		}
	}
	else if (json["value_type"].string_value() == "uint")
	{
		if (arr.size() != 4) throw exception("Parse ResetOperation Error");
		for (int i = 0; i < 4; i++)
		{
			UINT v = arr[i].int_value();
			op.value[i] = v;
		}
	}
	else if (json["value_type"].string_value() == "depth")
	{
		if (arr.size() != 1) throw exception("Parse ResetOperation Error");
		op.value[0] = 1;
		float v = arr[0].number_value();
		op.value[1] = *((UINT*)&v);
	}
	else if (json["value_type"].string_value() == "stencil")
	{
		if (arr.size() != 1) throw exception("Parse ResetOperation Error");
		op.value[0] = 2;
		op.value[2] = arr[0].int_value();
	}
	else if (json["value_type"].string_value() == "depth_stencil") {
		if (arr.size() != 2) throw exception("Parse ResetOperation Error");
		op.value[0] = 3;
		float v = arr[0].number_value();
		op.value[1] = *((UINT*)&v);
		op.value[2] = arr[1].int_value();
	}
	else throw exception("Parse ResetOperation Error");

	return op;
}

GenMipOperation Effect::LoadGenMipOp(const json11::Json & json) const
{
	if(!json.is_object() || !json["gen_mip"].is_string() || !resourceMap.at("resource").count(json["gen_mip"].string_value()))  throw exception("Load GenMip Error");
	GenMipOperation op;
	op.targetID = resourceMap.at("resource").at(json["gen_mip"].string_value());
	return op;
}


vector<Operation*> Effect::LoadRenderer(const json11::Json & json)
{
	if(!json.is_array()) throw exception("Load Renderer Error");
	vector<Operation*> result;
	for (const auto& e : json.array_items())
	{
		if (!e.is_object()) 
			throw exception("Load Operation Error");
		if (e["pass"].is_string())
		{
			result.push_back(new PassOperation(LoadPassConfig(e)));
		}
		else if (e["reset"].is_string())
		{
			result.push_back(new ResetOperation(LoadResetOp(e)));
		}
		else if (e["gen_mip"].is_string())
		{
			result.push_back(new GenMipOperation(LoadGenMipOp(e)));
		}
		else  throw exception("Load Operation Error");
	}
	return move(result);
}

void Effect::Apply()
{
	PipeLine::InputLayout().Activate(inputLayout);
	for (const auto& sampler : staticSamplers)
	{
		sampler.first.Bind(sampler.second);
	}
	for (const auto& resource : staticResources)
	{
		resource.first.Bind(resource.second);
	}
}

Effect* Effect::Create(const string & filePath)
{
	Effect* effect = new Effect();
	try 
	{
		ifstream file(filePath);
		if (!file)
		{
			throw ("Can't find/open file: " + filePath);
		}
		stringstream ss;
		ss << file.rdbuf();
		file.close();
		string input = ss.str();

		string err;
		Json json = Json::parse(input, err, json11::COMMENTS);
		if (err != "") throw exception(("Json Format Error: " + filePath + ". " + err).c_str());
		string workingFolder = filePath;
		int upper = workingFolder.size() - 1;
		while (upper > 0 && workingFolder[upper] != '\\') upper--;
		workingFolder.resize(upper + 1);

		//Check
		if (!json["resource"].is_object()) throw exception(("Error: can not read \"resource\". " + filePath).c_str());
		if (!json["pass"].is_object()) throw exception(("Error: can not read \"pass\". " + filePath).c_str());
		if (!json["renderer"].is_object()) throw exception(("Error: can not read \"renderer\". " + filePath).c_str());
		if (!json["config"].is_object()) throw exception(("Error: can not read \"config\". " + filePath).c_str());
		if (!json["static_sampler"].is_null() && !json["static_sampler"].is_array()) throw exception(("Error: can not read \"static_sampler\". " + filePath).c_str());
		if (!json["static_resource"].is_null() && !json["static_resource"].is_array()) throw exception(("Error: can not read \"static_resource\". " + filePath).c_str());

		//Load InputLayout:
		if (!json["config"]["input_layout"].is_string()) throw exception(("Error: can not read \"input_layout\". " + filePath).c_str());
		effect->inputLayout = PipeLine::InputLayout().Create(workingFolder + json["config"]["input_layout"].string_value(),"main");
		if(effect->inputLayout < 0) throw exception(("Error: can not create \"input_layout\" : " + workingFolder + json["config"]["input_layout"].string_value()).c_str());

		//Load Resources:
		auto& resource = json["resource"].object_items();
		effect->resourceMap["resource"]["back_buffer"] = PipeLine::Resources().GetBackBuffer();
		for (auto& e : resource)//for each kind of resources;
		{
			if (!e.second.is_object()) throw exception(("Error: can not parse " + e.first + filePath).c_str());

			auto& item = e.second.object_items();

			for (auto& res : item)//for each resource;
			{
				if (!res.second.is_string()) throw exception(("Error: can not parse " + res.first + ", in " + filePath).c_str());
				int id = CreateResource(e.first, workingFolder + res.second.string_value());
				if (id < 0) throw exception((effect, e.first + " Creation Failed : Name: " + workingFolder + res.second.string_value()).c_str());
				effect->resourceMap[e.first][res.first] = id;
			}
		}

		//Load Static Samplers
		if (json["static_sampler"].is_array())
		{
			for (auto& s : json["static_sampler"].array_items())
			{
				if(!s["name"].is_string() || !effect->resourceMap["sampler_state"].count(s["name"].string_value())) throw exception(("Error: can not parse static_sampler, in " + filePath).c_str());
				effect->staticSamplers.push_back(make_pair(SamplerPort(s), effect->resourceMap["sampler_state"][s["name"].string_value()]));
			}
		}

		//Load Static Resources
		if (json["static_resource"].is_array())
		{
			for (auto& s : json["static_resource"].array_items())
			{
				if (!s["name"].is_string() || !effect->resourceMap["resource"].count(s["name"].string_value())) throw exception(("Error: can not parse static_resource, in " + filePath).c_str());

				effect->staticResources.push_back(make_pair(ResourcePort(s), effect->resourceMap["resource"][s["name"].string_value()]));
			}
		}

		//Load Pass:
		auto& jpass = json["pass"].object_items();
		for (auto& e : jpass)
		{
			effect->passNameTable[e.first] = effect->passes.size();
			effect->passes.push_back(move(Pass(e.second, effect->resourceMap)));
		}

		//Load renderer
		auto& jrenderer = json["renderer"].object_items();
		for (auto& e : jrenderer)
		{
			if (!e.second.is_array()) throw exception(("Error: can not parse renderer: " + e.first + ", in " + filePath).c_str());
			effect->renderer[e.first] = effect->LoadRenderer(e.second);
		}
	}
	catch (exception ex)
	{
		delete effect;
		throw ex;
	}
	return effect;
}

Effect::~Effect()
{
	PipeLine::InputLayout().Delete(inputLayout);
	for (auto& resType : resourceMap)
	{
		for (auto& res : resType.second)
		{
			DeleteResource(resType.first, res.second);
		}
		resType.second.clear();
	}

	for (auto& vec : renderer)
	{
		for (auto p : vec.second)
		{
			delete p;
		}
	}
}

PassOperation::PassOperation(const Pass* p) : pPass(p)
{
	if (p->type == Pass_PostProcessing)
		type = Operation_Post_Proc;
	else
		type = Operation_Pass;
}

void PassOperation::Execute()
{
	pPass->Bind(passResourceID, passSamplerID);

}

ResetOperation::ResetOperation()
{
	type = Operation_Reset;
	targetID = -1;
	targetID = 0;
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
	value[3] = 0;
}

void ResetOperation::Execute()
{
	PipeLine::Resources().Reset(targetID, value);
}

GenMipOperation::GenMipOperation()
{
	type = Operation_GenMip;
	targetID = -1;
}

void GenMipOperation::Execute()
{
	PipeLine::Resources().GenerateMipMap(targetID);
}

ResourcePort::ResourcePort(const json11::Json & json)
{
	if (!json.is_object() ||
		!json["slot"].is_number() ||
		 json["slot"].int_value() <0 ||
		!json["stage"].is_string() ||
		!json["binding_flag"].is_string()
		)
		throw exception("ResourceBinding creation faild");

	stages = FileLoader::parsePipelineStage(json["stage"].string_value());
	flag = FileLoader::parseBindFlag(json["binding_flag"].string_value());
	slot = json["slot"].int_value();
}

void ResourcePort::Bind(int resID) const
{
	PipeLine::Resources().SetBinding(stages, flag, slot, resID);
}

void ResourcePort::Unbind() const
{
	PipeLine::Resources().SetBinding(stages, flag, slot, -1);
}

SamplerPort::SamplerPort(const json11::Json & json)
{
	if (!json.is_object() ||
		!json["slot"].is_number() ||
		json["slot"].int_value() < 0 ||
		!json["stage"].is_string()
		)
		throw exception("ResourceBinding creation faild");

	stages = FileLoader::parsePipelineStage(json["stage"].string_value());
	slot = json["slot"].int_value();
}

void SamplerPort::Bind(int resID) const
{
	PipeLine::SamplerState().Apply(resID, stages, slot);
}

void SamplerPort::Unbind() const
{
	PipeLine::SamplerState().Apply(-1, stages, slot);
}


