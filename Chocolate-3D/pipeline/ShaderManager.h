//-------------------------------Shader Manager-------------------------------
//Provide shader and Inputlayout management
//Author: Sentao
//--------------------------------------------------------------------------------

#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include "Pipeline.h"
#include "IDContainer.h"
#include "Singleton.h"
using namespace std;


class ShaderManager : public IDContainer<IUnknown*>
{
protected:
	void Release(IUnknown* & element) override;
	string version;
	int activeShaderID;

	static ID3DBlob * Compile(wstring fileName, string entryPoint, string target);
	static ID3DBlob * Compile(string fileName, string entryPoint, string target);
	virtual IUnknown* CreateFromFile(string fileName, string entryPoint) = 0;

public:
	ShaderManager();
	void Delete(UINT id) override;
	void Clear() override;
	int Create(string fileName, string entryPoint);
	int GetActiveShaderID() const;
	virtual void Activate(int id) = 0;
};



class InputLayout :public ShaderManager
{
	SINGLETON(InputLayout)
protected:
	ID3D11InputLayout* CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class VertexShader :public ShaderManager
{
	SINGLETON(VertexShader)
protected:
	ID3D11VertexShader* CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class PixelShader :public ShaderManager
{
	SINGLETON(PixelShader)
protected:
	ID3D11PixelShader* CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class ComputeShader :public ShaderManager
{
	SINGLETON(ComputeShader)
protected:
	ID3D11ComputeShader* CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

class GeometryShader :public ShaderManager
{
	SINGLETON(GeometryShader)
protected:
	ID3D11GeometryShader* CreateFromFile(string fileName, string entryPoint) override;
public:
	void Activate(int id) override;
};

