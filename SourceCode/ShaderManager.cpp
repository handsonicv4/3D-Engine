#include"ShaderManager.h"
#include <locale>
#include <codecvt>
#ifdef _DEBUG
DWORD shaderFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
DWORD shaderFlag = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // DEBUG



Pass::Pass(int vIndex, int pIndex)
{
	vSIndex = vIndex;
	pSIndex = pIndex;
}
Pass::Pass()
{
	vSIndex = 0;
	pSIndex = 0;
}

ShaderManager::ShaderManager()
{
	version = "5_0";
	activeVS = -1;
	activePS = -1;
	activeIL = -1;
	activeCS = -1;
	this->devicePtr = NULL;
	this->deviceContextPtr = NULL;
}
ShaderManager::ShaderManager(ID3D11Device * devicePtr, ID3D11DeviceContext * deviceContextPtr)
{
	version = "5_0";
	activeVS = -1;
	activePS = -1;
	activeIL = -1;
	this->devicePtr = devicePtr;
	this->deviceContextPtr = deviceContextPtr;
}
ShaderManager::~ShaderManager()
{
	Clear();
}

void ShaderManager::DeleteVertexShader(UINT id)
{
	if (id < MAX_SHADER_NUMBER && vertexShaderPool.count(id))
	{
		if(vertexShaderPool[id].ptr)
			vertexShaderPool[id].ptr->Release();
		vertexShaderPool.erase(id);
	}
}
void ShaderManager::DeletePixelShader(UINT id)
{
	if (id < MAX_SHADER_NUMBER && pixelShaderPool.count(id))
	{
		if (pixelShaderPool[id].ptr)
			pixelShaderPool[id].ptr->Release();
		pixelShaderPool.erase(id);
	}
}
void ShaderManager::DeleteComputeShader(UINT id)
{
	if (id < MAX_SHADER_NUMBER && computeShaderPool.count(id))
	{
		if (computeShaderPool[id].ptr)
			computeShaderPool[id].ptr->Release();
		computeShaderPool.erase(id);
	}
}

int ShaderManager::CreateVertexShader(string fileName, string entryPoint)
{
	int index = GenerateVSID();
	if (index < 0)
		return -1;
	HRESULT hr;
	VertexShader vs;
	string target = "vs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		return -1;
	}

	hr = devicePtr->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &vs.ptr);
	if (FAILED(hr))
	{
		MessageBoxA(0, "VertexShader create failed", 0, 0);
		compiledShader->Release();
		compiledShader = NULL;
		return -1;
	}

	//Find suitable inputlayout
	vs.inputLayoutID = GetInputLayout(compiledShader);
	if (vs.inputLayoutID == -1)
	{
		MessageBoxA(0, "InputLayout create failed", 0, 0);
		compiledShader->Release();
		compiledShader = NULL;
		return -1;
	}
	vertexShaderPool[index] = vs;

	compiledShader->Release();
	compiledShader = NULL;
	return index;
	
}
int ShaderManager::CreatePixelShader(string fileName, string entryPoint)
{
	int index = GeneratePSID();
	if (index < 0)
		return -1;
	PixelShader ps;
	string target = "ps_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		return -1;
	}

	HRESULT hr;
	
	hr=devicePtr->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &ps.ptr);
	if (FAILED(hr))
	{
		MessageBoxA(0, "PixelShader create failed", 0, 0);
		compiledShader->Release();
		compiledShader = NULL;
		return -1;
	}
	pixelShaderPool[index] = ps;
	compiledShader->Release();
	compiledShader = NULL;
	return index;
}
int ShaderManager::CreateComputeShader(string fileName, string entryPoint)
{
	int index = GenerateCSID();
	if (index < 0)
		return -1;
	ComputeShader cs;
	string target = "cs_" + version;
	ID3DBlob *compiledShader = Compile(fileName, entryPoint, target);
	if (!compiledShader)
	{
		return -1;
	}

	HRESULT hr = devicePtr->CreateComputeShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, &cs.ptr);
	if (FAILED(hr))
	{
		MessageBoxA(0, "ComputeShader create failed", 0, 0);
		compiledShader->Release();
		compiledShader = NULL;
		return -1;
	}
	computeShaderPool[index] = cs;
	compiledShader->Release();
	compiledShader = NULL;
	return index;
}

void ShaderManager::ActivateVertexShader(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->VSSetShader(NULL, NULL, 0);
		activeVS = -1;
		return;
	}

	else if (activeVS != id && vertexShaderPool.count(id))
	{
		deviceContextPtr->VSSetShader(vertexShaderPool[id].ptr, NULL, 0);
		activeVS = id;

		int iLID = vertexShaderPool[id].inputLayoutID;
		if (activeIL != iLID)
		{
			deviceContextPtr->IASetInputLayout(inputLayoutPool[iLID].ptr);
			activeIL = iLID;
		}

	}

}
void ShaderManager::ActivatePixelShader(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->PSSetShader(NULL, NULL, 0);
		activePS = -1;
		return;
	}
	else if (activePS != id && pixelShaderPool.count(id))
	{
		deviceContextPtr->PSSetShader(pixelShaderPool[id].ptr, NULL, 0);
		activePS = id;
	}
}
void ShaderManager::ActivateComputeShader(int id)
{
	if (id < 0)//Disable
	{
		deviceContextPtr->CSSetShader(NULL, NULL, 0);
		activeCS = -1;
		return;
	}
	if (activeCS != id && computeShaderPool.count(id))
	{
		deviceContextPtr->CSSetShader(computeShaderPool[id].ptr, NULL, 0);
		activeCS = id;
	}
}

void ShaderManager::ClearVertexShader()
{
	map<UINT, VertexShader>::iterator it = vertexShaderPool.begin();
	while (it != vertexShaderPool.end())
	{
		if (it->second.ptr)
		{
			it->second.ptr->Release();
			it->second.ptr = NULL;
		}
		it++;
	}
	vertexShaderPool.clear();
}
void ShaderManager::ClearPixelShader()
{
	map<UINT, PixelShader>::iterator it = pixelShaderPool.begin();
	while (it != pixelShaderPool.end())
	{
		if (it->second.ptr)
		{
			it->second.ptr->Release();
			it->second.ptr = NULL;
		}
		it++;
	}
	pixelShaderPool.clear();

}
void ShaderManager::ClearComputeShader()
{
	map<UINT, ComputeShader>::iterator it = computeShaderPool.begin();
	while (it != computeShaderPool.end())
	{
		if (it->second.ptr)
		{
			it->second.ptr->Release();
			it->second.ptr = NULL;
		}
		it++;
	}
	computeShaderPool.clear();
}
void ShaderManager::Clear()
{
	ClearVertexShader();
	ClearPixelShader();
	ClearComputeShader();
	inputLayoutPool.clear();
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
		return NULL;
	}

	if (FAILED(hr))
	{
		MessageBoxA(0, "VertexShader compile failed", 0, 0);
		if (compiledShader != NULL)
		{
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
int ShaderManager::GetInputLayout(ID3DBlob * compiledShader)
{
	map<UINT, InputLayout>::iterator it = inputLayoutPool.begin();
	HRESULT hr;
	InputLayout iL;
	//See if we can find a suitable layout from exist layout pool
	//By doing this we may wast some time at the initial phase
	//But we can avoid create new input-layout for every vertex shader and reduce the switch between layouts(which can be expensive at run time) 
	while (it != inputLayoutPool.end())
	{
		hr = devicePtr->CreateInputLayout(&it->second.desc[0], it->second.desc.size(), compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &iL.ptr);
		if (hr == S_OK)
		{
			iL.ptr->Release();
			return it->first;
		}
		else
		{
			if (iL.ptr)
			{
				iL.ptr->Release();
				iL.ptr = NULL;
			}
		}
		it++;
	}

	//If suitable inputlayout does not find, create one
	int index = GenerateILID();
	if (index == -1)
	{
		return -1;
	}
	ID3D11ShaderReflection *reflectPtr = NULL;

	hr = D3DReflect(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflectPtr);
	if (FAILED(hr))
	{
		MessageBoxA(0, "VertexShader reflect failed", 0, 0);
		return -1;
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
		//inputElementDescs[i].InputSlot = 0;
		//inputElementDescs[i].AlignedByteOffset = byteOffset;
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

	hr=devicePtr->CreateInputLayout(&inputElementDescs[0], shaderDesc.InputParameters, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &iL.ptr);
	if (FAILED(hr))
	{
		MessageBoxA(0, "InputLayout create failed", 0, 0);
		return -1;
	}
	iL.desc = inputElementDescs;
	inputLayoutPool[index] = iL;

	return index;
}

int ShaderManager::GenerateVSID()
{
	int i;
	for (i = 0; i < MAX_SHADER_NUMBER; i++)
	{
		if (vertexShaderPool.count(i) == 0)
			return i;
	}
	return -1;
}
int ShaderManager::GeneratePSID()
{
	int i;
	for (i = 0; i < MAX_SHADER_NUMBER; i++)
	{
		if (pixelShaderPool.count(i)==0)
			return i;
	}
	return -1;
}
int ShaderManager::GenerateILID()
{
	int i;
	for (i = 0; i < MAX_SHADER_NUMBER; i++)
	{
		if (inputLayoutPool.count(i) == 0)
			return i;
	}
	return -1;
}
int ShaderManager::GenerateCSID()
{
	int i;
	for (i = 0; i < MAX_SHADER_NUMBER; i++)
	{
		if (computeShaderPool.count(i) == 0)
			return i;
	}
	return -1;
}




