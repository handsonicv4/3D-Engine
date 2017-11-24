//=====================ShaderManager V 0.7========================
//Copy right belongs to Sentao Li

/*======================General description=========================
Manage D3D shaders and inputlayout (Just support vertex shader, pixel shader and compute shader for now)

*/
#pragma once
#include <d3d11_1.h>
#include"D3Def.h"
#include <map>
#include <vector>
using namespace std;


struct InputLayout
{
	ID3D11InputLayout* ptr = NULL;
	vector<D3D11_INPUT_ELEMENT_DESC> desc;
};

struct VertexShader
{
	ID3D11VertexShader* ptr = NULL;
	int inputLayoutID=-1;
};

struct PixelShader
{
	ID3D11PixelShader* ptr = NULL;
};

struct ComputeShader
{
	ID3D11ComputeShader* ptr = NULL;
};

class ShaderManager
{
	friend class D3D;
public:
	ShaderManager();
	ShaderManager(ID3D11Device* devicePtr,	ID3D11DeviceContext* deviceContextPtr);
	~ShaderManager();
	void DeleteVertexShader(UINT id);
	void DeletePixelShader(UINT id);
	void DeleteComputeShader(UINT id);
	void ClearVertexShader();
	void ClearPixelShader();
	void ClearComputeShader();
	void Clear();
	int CreateVertexShader(string fileName, string entryPoint);
	int CreatePixelShader(string fileName, string entryPoint);
	int CreateComputeShader(string fileName, string entryPoint);
	void ActivateVertexShader(int id);
	void ActivatePixelShader(int id);
	void ActivateComputeShader(int id);
private:
	string version;
	//string uniformEntryPoint;
	int activeVS;
	int activePS;
	int activeIL;
	int activeCS;
	ID3D11Device* devicePtr;
	ID3D11DeviceContext* deviceContextPtr;
	map<UINT, VertexShader> vertexShaderPool;
	map<UINT, PixelShader> pixelShaderPool;
	map<UINT, InputLayout> inputLayoutPool;
	map<UINT, ComputeShader> computeShaderPool;

	int GetInputLayout(ID3DBlob* compiledShader);
	ID3DBlob * Compile(wstring fileName, string entryPoint, string target);
	ID3DBlob * Compile(string fileName, string entryPoint, string target);
	int GenerateVSID();
	int GeneratePSID();
	int GenerateILID();
	int GenerateCSID();
};

class Pass
{
public:
	int vSIndex;
	int pSIndex;
	Pass(int vIndex, int pIndex);
	Pass();
};

