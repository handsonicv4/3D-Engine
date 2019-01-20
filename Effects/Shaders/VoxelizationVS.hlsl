#include "ConstantBuffers.hlsl"
#include "Input.hlsl"
#include "Skinning.hlsl"

struct Light
{
	float4 color;
	float3 direction;
	float angleCos;
	matrix lightVP;
};

//bind
StructuredBuffer<Light> Lights: register(t7);

PSinput main(VSinput input)
{
	PSinput output;
    // Change the position vector to be 4 units for proper matrix calculations.
	float4 position = float4(input.position, 1.0f);
	output.position= float4(input.position, 1.0f);
	output.normal = input.normal;
	output.tangent = input.tangent;
	output.bitangent = input.bitangent;
	//Skinning:
	uint flags = instanceData[input.instanceID].flags;
	if (flags & 0x01)
	{
		uint bindOffset = instanceData[input.instanceID].bindMatrixOffset;
		input.boneID += bindOffset;

		output.position = BoneTransformPos(position, input.boneWeight, input.boneID);
		output.normal = BoneTransformNorm(input.normal, input.boneWeight, input.boneID);

		if (flags & 0x04)
		{
			output.tangent = BoneTransformNorm(input.tangent, input.boneWeight, input.boneID);
			output.bitangent = BoneTransformNorm(input.bitangent, input.boneWeight, input.boneID);
		}
	}
	output.positionWorld = mul(output.position, instanceData[input.instanceID].world);

	float4 hVoxScale = float4( 0.5*g_VoxelDimention*g_VoxelSize,1);
	output.position = output.positionWorld / 5.0;

	output.normal = normalize( mul(output.normal, (float3x3)instanceData[input.instanceID].world));
	output.tangent= normalize(mul(output.tangent, (float3x3)instanceData[input.instanceID].world));
	output.bitangent = normalize(mul(output.bitangent, (float3x3)instanceData[input.instanceID].world));
	output.tex = input.tex;
	output.instanceID = input.instanceID;

	output.positionLight = mul(output.positionWorld, Lights[0].lightVP);
    return output;
}