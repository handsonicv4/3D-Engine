//-------------------------------Shader Manager-------------------------------
//Provide shader and Inputlayout management
//Author: Sentao
//--------------------------------------------------------------------------------

#include"ShaderManager.h"
#include <locale>
#include <codecvt>
#include<assert.h>
#ifdef _DEBUG
DWORD shaderFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
DWORD shaderFlag = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // DEBUG

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib" )

//ShaderManager
ShaderManager::ShaderManager()
{
	version = "5_0";
	activeShaderID = INVALID;
}
void ShaderManager::Release(IUnknown *& element)
{
	element->Release();
	element = NULL;
}
ID3DBlob * ShaderManager::Compile(wstring fileName, string entryPoint, string target)
{
	ID3DBlob *compiledShader = NULL;
	ID3DBlob *compilationMsgs = NULL;
	HRESULT hr = D3DCompileFromFile(fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), shaderFlag, 0, &compiledShader, &compilationMsgs);

	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		compilationMsgs->Release();
		compilationMsgs = NULL;
		assert(0);
		return NULL;
	}

	if (FAILED(hr))
	{
		MessageBoxA(0, "VertexShader compile failed", 0, 0);
		if (compiledShader != NULL)
		{
			assert(0);
			compiledShader->Release();
			compiledShader = NULL;
		}
		return NULL;
	}
	return compiledShader;
}
ID3DBlob * ShaderManager::Compile(string fileName, string entryPoint, string target)
{
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	wstring wFileName = converter.from_bytes(fileName);
	return Compile(wFileName, entryPoint, target);
}
void ShaderManager::Delete(UINT id)
{
	if (!Exist(id)) return;
	if(id == activeShaderID) Activate(INVALID);
	IDContainer<IUnknown*>::Delete(id);
}
void ShaderManager::Clear()
{
	if(activeShaderID != INVALID) Activate(INVALID);
	IDContainer<IUnknown*>::Clear();
}
int ShaderManager::Create(string fileName, string entryPoint)
{
	int id = GenerateID();
	if (id < 0) return INVALID;

	IUnknown* shader = CreateFromFile(fileName, entryPoint);
	if (!shader) return INVALID;

	pool[id] = shader;
	return id;
}
int ShaderManager::GetActiveShaderID() const
{
	return activeShaderID;
}
//InputLayout
SINGLETON_C_D(InputLayout)
ID3D11InputLayout* InputLayout::CreateFromFile(string fileName, string entryPoint)
{
	ID3D11InputLayout *layout = NULL;
	HRESULT hr;
	string target = "vs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return layout;
	}

	int index = GenerateID();
	if (index == INVALID)
	{
		assert(0);
		return layout;
	}

	ID3D11ShaderReflection *reflectPtr = NULL;
	hr = D3DReflect(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflectPtr);
	if (FAILED(hr))
	{
		assert(0);
		return layout;
	}

	D3D11_SHADER_DESC shaderDesc;
	UINT byteOffset[16];
	ZeroMemory(byteOffset, 16 * sizeof(UINT));
	D3D11_INPUT_ELEMENT_DESC zeros;
	ZeroMemory(&zeros, sizeof(zeros));
	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;

	reflectPtr->GetDesc(&shaderDesc);
	inputElementDescs.resize(shaderDesc.InputParameters, zeros);

	D3D11_SIGNATURE_PARAMETER_DESC sPDesc;
	for (int i = 0; i != shaderDesc.InputParameters; ++i)
	{
		reflectPtr->GetInputParameterDesc(i, &sPDesc);
		inputElementDescs[i].SemanticName = sPDesc.SemanticName;
		inputElementDescs[i].SemanticIndex = sPDesc.SemanticIndex;
		inputElementDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDescs[i].InstanceDataStepRate = 0;

		if (!strcmp(sPDesc.SemanticName, "POSITION"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_Position;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "COLOR"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_Color;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "NORMAL"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_Normal;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "TEXCOORD"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_TexCoord;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "PSIZE"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_PSize;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "BINORMAL"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_Binormal;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "TANGENT"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_Tangent;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "POSITIONT"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_PositionT;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "BLENDINDICES"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_BlendIndices;
		}
		else 	if (!strcmp(sPDesc.SemanticName, "BLENDWEIGHT"))
		{
			inputElementDescs[i].InputSlot = Slot_Input_BlendWeight;
		}
		else if (!strcmp(sPDesc.SemanticName, "SV_InstanceID"))
		{
			inputElementDescs[i].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			inputElementDescs[i].InputSlot = Slot_Input_InstanceID;
		}
		else
		{
			inputElementDescs[i].InputSlot = Slot_Input_Other;
		}

		inputElementDescs[i].AlignedByteOffset = byteOffset[inputElementDescs[i].InputSlot];

		if (sPDesc.Mask == 1)
		{
			if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32_UINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32_SINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32_FLOAT;
			}
			byteOffset[inputElementDescs[i].InputSlot] += 4;
		}

		else if (sPDesc.Mask <= 3)
		{
			if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32_UINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32_SINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			byteOffset[inputElementDescs[i].InputSlot] += 8;
		}

		else if (sPDesc.Mask <= 7)
		{
			if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			byteOffset[inputElementDescs[i].InputSlot] += 12;
		}

		else if (sPDesc.Mask <= 15)
		{
			if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;
			}
			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}

			else if (sPDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				inputElementDescs[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			byteOffset[inputElementDescs[i].InputSlot] += 16;
		}
	}

	hr = PipeLine::pDevice->CreateInputLayout(&inputElementDescs[0], shaderDesc.InputParameters, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &layout);
	if (FAILED(hr))
	{
		assert(0);
		return layout;
	}

	reflectPtr->Release();
	compiledShader->Release();

	return layout;
}
void InputLayout::Activate(int id)
{
	if (id < 0)
	{
		PipeLine::pContext->IASetInputLayout(NULL);
		activeShaderID = INVALID;
	}
	else if (activeShaderID != id)
	{
		PipeLine::pContext->IASetInputLayout((ID3D11InputLayout*)pool[id]);
		activeShaderID = id;
	}
}

//VertexShader
SINGLETON_C_D(VertexShader)
ID3D11VertexShader* VertexShader::CreateFromFile(string fileName, string entryPoint)
{
	ID3D11VertexShader *vs = NULL;
	HRESULT hr;
	string target = "vs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return vs;
	}
	hr = PipeLine::pDevice->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &vs);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return vs;
}
void VertexShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		PipeLine::pContext->VSSetShader(NULL, NULL, 0);
		activeShaderID = INVALID;
		return;
	}

	else if (activeShaderID != id && pool.count(id))
	{
		PipeLine::pContext->VSSetShader((ID3D11VertexShader*)pool[id], NULL, 0);
		activeShaderID = id;
	}
}

//PixelShader
SINGLETON_C_D(PixelShader)
ID3D11PixelShader* PixelShader::CreateFromFile(string fileName, string entryPoint)
{

	ID3D11PixelShader* ps = NULL;
	string target = "ps_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader) return ps;
	HRESULT hr;
	hr = PipeLine::pDevice->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &ps);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return ps;
}
void PixelShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		PipeLine::pContext->PSSetShader(NULL, NULL, 0);
		activeShaderID = INVALID;
		return;
	}
	else if (activeShaderID != id && pool.count(id))
	{
		PipeLine::pContext->PSSetShader((ID3D11PixelShader*)pool[id], NULL, 0);
		activeShaderID = id;
	}
}

//ComputeShader
SINGLETON_C_D(ComputeShader)
ID3D11ComputeShader* ComputeShader::CreateFromFile(string fileName, string entryPoint)
{
	ID3D11ComputeShader* cs = NULL;
	string target = "cs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return cs;
	}
	HRESULT hr = PipeLine::pDevice->CreateComputeShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &cs);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return cs;
}
void ComputeShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		PipeLine::pContext->CSSetShader(NULL, NULL, 0);
		activeShaderID = INVALID;
		return;
	}

	if (activeShaderID != id && pool.count(id))
	{
		PipeLine::pContext->CSSetShader((ID3D11ComputeShader*)pool[id], NULL, 0);
		activeShaderID = id;
	}
}

//GeometryShader
SINGLETON_C_D(GeometryShader)
ID3D11GeometryShader* GeometryShader::CreateFromFile(string fileName, string entryPoint)
{
	ID3D11GeometryShader* gs = NULL;
	string target = "gs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return gs;
	}
	HRESULT hr = PipeLine::pDevice->CreateGeometryShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &gs);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return gs;
}
void GeometryShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		PipeLine::pContext->GSSetShader(NULL, NULL, 0);
		activeShaderID = INVALID;
		return;
	}
	else if (activeShaderID != id && pool.count(id))
	{
		PipeLine::pContext->GSSetShader((ID3D11GeometryShader*)pool[id], NULL, 0);
		activeShaderID = id;
	}
}
