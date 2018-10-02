#include "../ConstantBuffers.hlsl"
#include "TypeDef.hlsl"
/////////////
// GLOBALS //
/////////////
struct PerInstanceType
{
	matrix wVP;
	matrix world;
	float4 color;
	uint bindMatrixOffset;
};

StructuredBuffer<PerInstanceType> instanceData : register(t6);
//////////////
// TYPEDEFS //
//////////////


struct VGSinput
{
    float4 position : SV_POSITION;
	float3 tex : TEXCOORD0;
	matrix wvp : TEXCOORD1;
};
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VGSinput main(VSinput input)
{
	VGSinput output;
	float4 position = float4(input.position, 1.0f);
	//output.position = mul(position, instanceData[input.instanceID].wVP);
	output.position = position;
	output.wvp = instanceData[input.instanceID].wVP;
	output.tex = input.position/g_VoxelSize;//Voxel Space Position
	return output;
}
