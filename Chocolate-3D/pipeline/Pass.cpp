#include"Pass.h"
#include <fstream>
#include <sstream>
#include"Pipeline.h"
#include"DescFileLoader.h"
#include <unordered_map>
using namespace std;
using namespace json11;

PassOperationType parsePassOperationType(const string& str)
{
	static unordered_map<string, PassOperationType> passOperationType;
	if (!passOperationType.size())
	{
		passOperationType["draw"] = Pass_Operation_Draw;
		passOperationType["compute"] = Pass_Operation_Compute;
		passOperationType["generate_mip"] = Pass_Operation_Generate_Mip;
		passOperationType["reset"] = Pass_Operation_Reset;
		passOperationType["copy"] = Pass_Operation_Copy;
	}
	if (!passOperationType.count(str)) throw exception(("Parse PassOperationType Error : " + str).c_str());
	return passOperationType[str];
}

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

PassOperation::PassOperation(const json11::Json& obj, const unordered_map<string,int>& subresource):PassOperation()
{
	if(!obj.is_object()) throw exception("Parse PassOperation Error");
	type = parsePassOperationType(obj["type"].string_value());
	if (type == Pass_Operation_Compute)
	{
		if (!obj["threads"].is_array() || obj["threads"].array_items().size() != 3)
			throw exception("Parse PassOperation Error");
		const auto& arr = obj["threads"].array_items();
		for (int i = 0; i < 3; i++)
		{
			if(!arr[i].is_number()) throw exception("Parse PassOperation Error");
			threadSize[i] = arr[i].int_value();
		}
	}
	else if (type == Pass_Operation_Reset)
	{
		if (!obj["target"].is_string() || !subresource.count(obj["target"].string_value()))
			throw exception("Parse PassOperation Error");
		string key = obj["target"].string_value();
		resourceID = subresource.find(key)->second;

		if (!obj["value_type"].is_string() || !obj["value"].is_array())
			throw exception("Parse PassOperation Error");
		const auto& arr = obj["value"].array_items();
		if (obj["value_type"].string_value() == "float")
		{
			if(arr.size() != 4) throw exception("Parse PassOperation Error");
			for (int i = 0; i < 4 ; i++)
			{
				float v = arr[i].number_value();
				value[i] = *((UINT*)&v);
			}
		}
		else if (obj["value_type"].string_value() == "uint")
		{
			if (arr.size() != 4) throw exception("Parse PassOperation Error");
			for (int i = 0; i < 4 ; i++)
			{
				UINT v = arr[i].int_value();
				value[i] = v;
			}
		}
		else if (obj["value_type"].string_value() == "depth")
		{
			if (arr.size() != 1) throw exception("Parse PassOperation Error");
			value[0] = 1;
			float v = arr[0].number_value();
			value[1] = *((UINT*)&v);
		}
		else if (obj["value_type"].string_value() == "stencil")
		{
			if (arr.size() != 1) throw exception("Parse PassOperation Error");
			value[0] = 2;
			value[2] = arr[0].int_value();
		}
		else if (obj["value_type"].string_value() == "depth_stencil") {
			if (arr.size() != 2) throw exception("Parse PassOperation Error");
			value[0] = 3;
			float v = arr[0].number_value();
			value[1] = *((UINT*)&v);
			value[2] = arr[1].int_value();
		}
		else throw exception("Parse PassOperation Error");
	}
	else if (type == Pass_Operation_Generate_Mip)
	{
		if (!obj["target"].is_string() || !subresource.count(obj["target"].string_value()))
			throw exception("Parse PassOperation Error");
		string key = obj["target"].string_value();
		resourceID = subresource.find(key)->second;

	}
	else if (type == Pass_Operation_Draw)
	{

	}
	else throw exception("Parse PassOperation Error");
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
	blendStateID = -1;
	viewPortID = -1;

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

void Pass::LoadSampler(int id, PipelineStage stage, int slot)
{
	BindingRule rule;
	rule.resourceID = id;
	rule.stages = stage;
	rule.slot = slot;
	samplerBinding.push_back(move(rule));
}

void Pass::LoadResource(int id, PipelineStage stage, int slot, BindFlag bindFlag)
{
	BindingRule rule;
	rule.resourceID = id;
	rule.stages = stage;
	rule.slot = slot;
	rule.flag = bindFlag;
	resourceBinding.push_back(move(rule));
}

void Pass::LoadTopology(const string & topology)
{
	this->topology = FileLoader::parseTopology(topology);
}

Pass::Pass(const json11::Json & jpass, const unordered_map<string, unordered_map<string, int>>& resourceMap):Pass()
{
	if (!jpass.is_object())
		throw exception("Pass creation faild");
	auto& passData = jpass.object_items();
	for (auto& item : passData)
	{
		if (resourceMap.count(item.first))
		{
			const auto& subRes = resourceMap.find(item.first)->second;
			if (item.first == "sampler_state")
			{
				if (!item.second.is_array()) throw exception("Pass creation faild");
				for (auto& bind : item.second.array_items())
				{
					if (!subRes.count(bind["name"].string_value()) || !bind["slot"].is_number())
						throw exception("Pass creation faild");
					LoadSampler(subRes.find(bind["name"].string_value())->second, FileLoader::parsePipelineStage(bind["stage"].string_value()), bind["slot"].int_value());
				}
			}
			else if (item.first == "resource")
			{
				if (!item.second.is_array()) throw exception("Pass creation faild");
				for (auto& bind : item.second.array_items())
				{
					if (!subRes.count(bind["name"].string_value()) || !bind["slot"].is_number())
						throw exception("Pass creation faild");
					LoadResource(subRes.find(bind["name"].string_value())->second, FileLoader::parsePipelineStage(bind["stage"].string_value()), bind["slot"].int_value(), FileLoader::parseBindFlag(bind["binding_flag"].string_value()));
				}
			}
			else
			{
				if (!item.second.is_string() || !subRes.count(item.second.string_value()))
					throw exception("Pass creation faild");
				Load(item.first, subRes.find(item.second.string_value())->second);
			}
		}
		else if (item.first == "operation")
		{
			if (!item.second.is_array())
				throw exception("Pass creation faild");
			for (const auto& op : item.second.array_items())
			{
				operations.push_back(PassOperation(op, resourceMap.find("resource")->second));
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
}


Effect* Effect::Create(const string & filePath)
{
	Effect* effect = new Effect();
	try {
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

		if (!json["resource"].is_object()) throw exception(("Error: can not read \"resource\". " + filePath).c_str());
		if (!json["pass"].is_object()) throw exception(("Error: can not read \"pass\". " + filePath).c_str());
		if (!json["renderer"].is_object()) throw exception(("Error: can not read \"renderer\". " + filePath).c_str());

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

		//Load Pass:
		auto& jpass = json["pass"].object_items();
		unordered_map<string, int> passName;
		for (auto& e : jpass)
		{
			passName[e.first] = effect->passes.size();
			effect->passes.push_back(Pass(e.second, effect->resourceMap));
		}

		//Load renderer
		auto& jrenderer = json["renderer"].object_items();
		for (auto& e : jrenderer)
		{
			if (!e.second.is_array()) throw exception(("Error: can not parse renderer: " + e.first + ", in " + filePath).c_str());
			for (auto& pname : e.second.array_items())
			{
				if(!passName.count(pname.string_value()))  throw exception(("Error: can not parse renderer: " + e.first + ", in " + filePath).c_str());
				effect->renderers[e.first].push_back(passName[pname.string_value()]);
			}
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
	resourceMap.clear();
	passes.clear();
	renderers.clear();
}
