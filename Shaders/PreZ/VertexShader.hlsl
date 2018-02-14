#include "../ConstantBuffers.hlsl"
#include "TypeDef.hlsl"

struct PerInstanceType
{
	matrix wVP;
	matrix world;
	float4 color;
	uint bindMatrixOffset;
};

StructuredBuffer<float4x4> boneMatrix : register(t5);
StructuredBuffer<PerInstanceType> instanceData : register(t6);
float4 BoneTransformPos(uniform float4 vertexPos, uniform float4 weight, uniform uint4 boneID)
{
	float4 result = float4(0, 0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		if (weight[i] > 0.001)
		{
			result += weight[i] * mul(vertexPos, boneMatrix[boneID[i]]);
		}
	}
	return result;
}

PSinput main(VSinput input)
{
    PSinput output;
    // Change the position vector to be 4 units for proper matrix calculations.
	float4 position = float4(input.position, 1.0f);
	output.position= float4(input.position, 1.0f);

	//Skinning:
	if (g_Flags & 0x01)
	{
		uint bindOffset = instanceData[input.instanceID].bindMatrixOffset;
		input.boneID += bindOffset;

		output.position = BoneTransformPos(position, input.boneWeight, input.boneID);
	}

	output.position = mul(output.position, instanceData[input.instanceID].wVP);
	output.tex = input.tex;
    return output;
}
