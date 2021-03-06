#include "ConstantBuffers.hlsl"
#include "Input.hlsl"
#include "Skinning.hlsl"

PSinput main(VSinput input)
{
    PSinput output = (PSinput) 0;
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
	output.position = mul(output.position, instanceData[input.instanceID].wVP);


	output.normal = normalize( mul(output.normal, (float3x3)instanceData[input.instanceID].world));
	output.tangent= normalize(mul(output.tangent, (float3x3)instanceData[input.instanceID].world));
	output.bitangent = normalize(mul(output.bitangent, (float3x3)instanceData[input.instanceID].world));
	output.tex = input.tex;
	output.instanceID = input.instanceID;

	//output.positionLight = mul(output.positionWorld, g_LightVP);
    return output;
}
