//-------------------------------Shader Manager-------------------------------
//Provide shader and Inputlayout management
//Author: Sentao
//--------------------------------------------------------------------------------

#pragma once
#include <d3d11_1.h>
#include <D3Dcompiler.h>
#include"D3Def.h"
#include <map>
#include <vector>
using namespace std;
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib" )

struct ShaderType
{
	IUnknown *ptr = NULL;
};

class ShaderManager
{
protected:
	map<UINT, ShaderType> shaderPool;
	string version;
	int activeShaderID;
	int GenerateID();

	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;
	static ID3DBlob * Compile(wstring fileName, string entryPoint, string target);
	static ID3DBlob * Compile(string fileName, string entryPoint, string target);
	virtual ShaderType CreateFromFile(string fileName, string entryPoint) = 0;

public:
	ShaderManager();
	void Init(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr);
	ShaderManager(ID3D11Device* devicePtr, ID3D11DeviceContext* deviceContextPtr);
	virtual ~ShaderManager();
	void Delete(UINT id);
	void Clear();
	int Create(string fileName, string entryPoint);
	virtual void Activate(int id) = 0;
};

class InputLayout :public ShaderManager
{
protected:
	ShaderType CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class VertexShader :public ShaderManager
{
protected:
	ShaderType CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class PixelShader :public ShaderManager
{
protected:
	ShaderType CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class ComputeShader :public ShaderManager
{
protected:
	ShaderType CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class GeometryShader :public ShaderManager
{
protected:
	ShaderType CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};