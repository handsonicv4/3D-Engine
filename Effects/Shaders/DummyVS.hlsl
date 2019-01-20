#include "ConstantBuffers.hlsl"
#include "Input.hlsl"

PSinput main(VSinput input)
{
	PSinput output = (PSinput)0;
	output.position = float4(input.position, 1);
	output.tex = input.tex;
	output.normal = input.normal;
	output.tangent = input.tangent;
	output.bitangent = input.bitangent;
	output.instanceID = input.instanceID;
	return output;
}