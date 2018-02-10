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

//ShaderManager
ShaderManager::ShaderManager()
{
	devicePtr = NULL;
	deviceContextPtr = NULL;
	version = "5_0";
	activeShaderID = -1;
}
ShaderManager::ShaderManager(ID3D11Device * devicePtr, ID3D11DeviceContext * deviceContextPtr)
{
	version = "5_0";
	activeShaderID = -1;
	Init(devicePtr, deviceContextPtr);
}
void ShaderManager::Init(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	devicePtr = pDevice;
	deviceContextPtr = pDeviceContext;
}
ShaderManager::~ShaderManager()
{
	Clear();
}

int ShaderManager::GenerateID()
{
	int i;
	for (i = 0; i < MAX_SHADER_NUMBER; i++)
	{
		if (shaderPool.count(i) == 0)
			return i;
	}
	return -1;
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
	if (id < MAX_SHADER_NUMBER && shaderPool.count(id))
	{
		if (shaderPool[id].ptr)
			shaderPool[id].ptr->Release();
		shaderPool.erase(id);
	}
}
void ShaderManager::Clear()
{
	map<UINT, ShaderType>::iterator it = shaderPool.begin();
	while (it != shaderPool.end())
	{
		if (it->second.ptr)
		{
			it->second.ptr->Release();
			it->second.ptr = NULL;
		}
		it++;
	}
	shaderPool.clear();
}
int ShaderManager::Create(string fileName, string entryPoint)
{
	int id = GenerateID();
	if (id < 0)
		return -1;

	ShaderType shader = CreateFromFile(fileName, entryPoint);
	if (!shader.ptr)
		return -1;

	shaderPool[id] = shader;
	return id;
}

//InputLayout
ShaderType InputLayout::CreateFromFile(string fileName, string entryPoint)
{
	ShaderType layout;
	ID3D11InputLayout **pptr = (ID3D11InputLayout **)&layout.ptr;
	HRESULT hr;
	string target = "vs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return layout;
	}

	int index = GenerateID();
	if (index == -1)
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

	hr = devicePtr->CreateInputLayout(&inputElementDescs[0], shaderDesc.InputParameters, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), pptr);
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
		deviceContextPtr->IASetInputLayout(NULL);
	}
	else if (activeShaderID != id)
	{
		deviceContextPtr->IASetInputLayout((ID3D11InputLayout*)shaderPool[id].ptr);
		activeShaderID = id;
	}
}

//VertexShader
ShaderType VertexShader::CreateFromFile(string fileName, string entryPoint)
{
	ShaderType vs;
	ID3D11VertexShader **pptr = (ID3D11VertexShader **)&vs.ptr;
	HRESULT hr;
	string target = "vs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return vs;
	}
	hr = devicePtr->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, pptr);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return vs;
}
void VertexShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->VSSetShader(NULL, NULL, 0);
		activeShaderID = -1;
		return;
	}

	else if (activeShaderID != id && shaderPool.count(id))
	{
		deviceContextPtr->VSSetShader((ID3D11VertexShader*)shaderPool[id].ptr, NULL, 0);
		activeShaderID = id;
	}
}

//PixelShader
ShaderType PixelShader::CreateFromFile(string fileName, string entryPoint)
{
	ShaderType ps;
	ID3D11PixelShader ** pptr = (ID3D11PixelShader **)&ps.ptr;
	string target = "ps_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		return ps;
	}
	HRESULT hr;
	hr = devicePtr->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, pptr);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return ps;
}
void PixelShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->PSSetShader(NULL, NULL, 0);
		activeShaderID = -1;
		return;
	}
	else if (activeShaderID != id && shaderPool.count(id))
	{
		deviceContextPtr->PSSetShader((ID3D11PixelShader*)shaderPool[id].ptr, NULL, 0);
		activeShaderID = id;
	}
}

//ComputeShader
ShaderType ComputeShader::CreateFromFile(string fileName, string entryPoint)
{
	ShaderType cs;
	ID3D11ComputeShader** pptr = (ID3D11ComputeShader**)&cs.ptr;
	string target = "cs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return cs;
	}
	HRESULT hr = devicePtr->CreateComputeShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, pptr);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return cs;
}
void ComputeShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->CSSetShader(NULL, NULL, 0);
		activeShaderID = -1;
		return;
	}

	if (activeShaderID != id && shaderPool.count(id))
	{
		deviceContextPtr->CSSetShader((ID3D11ComputeShader*)shaderPool[id].ptr, NULL, 0);
		activeShaderID = id;
	}
}

//GeometryShader
ShaderType GeometryShader::CreateFromFile(string fileName, string entryPoint)
{
	ShaderType gs;
	ID3D11GeometryShader** pptr = (ID3D11GeometryShader**)&gs.ptr;
	string target = "gs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		assert(0);
		return gs;
	}
	HRESULT hr = devicePtr->CreateGeometryShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, pptr);
	compiledShader->Release();
	compiledShader = NULL;
	assert(SUCCEEDED(hr));
	return gs;
}
void GeometryShader::Activate(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->GSSetShader(NULL, NULL, 0);
		activeShaderID = -1;
		return;
	}
	else if (activeShaderID != id && shaderPool.count(id))
	{
		deviceContextPtr->GSSetShader((ID3D11GeometryShader*)shaderPool[id].ptr, NULL, 0);
		activeShaderID = id;
	}
}
