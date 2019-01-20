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
	PSinput output = (PSinput) 0;
	// Change the position vector to be 4 units for proper matrix calculations.
	float4 position = float4(input.position, 1.0f);
	output.position = float4(input.position, 1.0f);

	//Skinning:
	uint flags = instanceData[input.instanceID].flags;
	if (flags & 0x01)
	{
		uint bindOffset = instanceData[input.instanceID].bindMatrixOffset;
		input.boneID += bindOffset;
		output.position = BoneTransformPos(position, input.boneWeight, input.boneID);

	}
	output.positionWorld = mul(output.position, instanceData[input.instanceID].world);

	output.position = mul(output.positionWorld, Lights[0].lightVP);

	output.instanceID = input.instanceID;

	//output.positionLight = mul(output.positionWorld, Lights[0].lightVP);
	return output;
}
