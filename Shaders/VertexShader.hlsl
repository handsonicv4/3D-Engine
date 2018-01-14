#include "Common.hlsl"
/////////////
// GLOBALS //
/////////////


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BINORMAL;
	float2 tex : TEXCOORD0;

	uint4 boneID : BLENDINDICES;
	float4 boneWeight : BLENDWEIGHT;

	uint instanceID : SV_InstanceID;

};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BINORMAL;
	float2 tex : TEXCOORD0;
};

struct PerInstanceType
{
	matrix wVP;
	matrix world;
	float4 color;
	uint bindMatrixOffset;
};

StructuredBuffer<float4x4> boneMatrix : register(t5);
StructuredBuffer<PerInstanceType> instanceData : register(t6);

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    // Change the position vector to be 4 units for proper matrix calculations.
	float4 position = float4(input.position, 1.0f);
	output.position= float4(input.position, 1.0f);
	output.normal = input.normal;
	output.tangent = input.tangent;
	output.bitangent = input.bitangent;
	//Skinning:
	if (g_HasAnimation)
	{
		//1st bone
		uint bindOffset = instanceData[input.instanceID].bindMatrixOffset;
		output.position = input.boneWeight.x * mul(position, boneMatrix[bindOffset + input.boneID.x]);
		output.normal = input.boneWeight.x * normalize(mul(input.normal, (float3x3)boneMatrix[bindOffset + input.boneID.x]));
		if (g_HasNormalMap)
		{
			output.tangent = input.boneWeight.x * normalize(mul(input.tangent, (float3x3)boneMatrix[bindOffset + input.boneID.x]));
			output.bitangent = input.boneWeight.x * normalize(mul(input.bitangent, (float3x3)boneMatrix[bindOffset + input.boneID.x]));
		}
		//2nd bone
		if (input.boneWeight.y > 0.0001)
		{
			output.position += input.boneWeight.y * mul(position, boneMatrix[bindOffset + input.boneID.y]);
			output.normal += input.boneWeight.y * normalize(mul(input.normal, (float3x3)boneMatrix[bindOffset + input.boneID.y]));
			if (g_HasNormalMap)
			{
				output.tangent += input.boneWeight.y * normalize(mul(input.tangent, (float3x3)boneMatrix[bindOffset + input.boneID.y]));
				output.bitangent += input.boneWeight.y * normalize(mul(input.bitangent, (float3x3)boneMatrix[bindOffset + input.boneID.y]));
			}
		}

		//3rd bone
		if (input.boneWeight.z > 0.0001)
		{
			output.position += input.boneWeight.z * mul(position, boneMatrix[bindOffset + input.boneID.z]);
			output.normal += input.boneWeight.z * normalize(mul(input.normal, (float3x3)boneMatrix[bindOffset + input.boneID.z]));
			if (g_HasNormalMap)
			{
				output.tangent += input.boneWeight.z * normalize(mul(input.tangent, (float3x3)boneMatrix[bindOffset + input.boneID.z]));
				output.bitangent += input.boneWeight.z * normalize(mul(input.bitangent, (float3x3)boneMatrix[bindOffset + input.boneID.z]));
			}
		}

		//4th bone
		if (input.boneWeight.w > 0.0001)
		{
			output.position += input.boneWeight.w * mul(position, boneMatrix[bindOffset + input.boneID.w]);
			output.normal += input.boneWeight.w * normalize(mul(input.normal, (float3x3)boneMatrix[bindOffset + input.boneID.w]));
			if (g_HasNormalMap)
			{
				output.tangent += input.boneWeight.w * normalize(mul(input.tangent, (float3x3)boneMatrix[bindOffset + input.boneID.w]));
				output.bitangent += input.boneWeight.w * normalize(mul(input.bitangent, (float3x3)boneMatrix[bindOffset + input.boneID.w]));
			}
		}
	}


	output.position = mul(output.position, instanceData[input.instanceID].wVP);

	output.normal = normalize( mul(output.normal, (float3x3)instanceData[input.instanceID].world));
	output.tangent= normalize(mul(output.tangent, (float3x3)instanceData[input.instanceID].world));
	output.bitangent = normalize(mul(output.bitangent, (float3x3)instanceData[input.instanceID].world));
	output.tex = input.tex;
    
    return output;
}
