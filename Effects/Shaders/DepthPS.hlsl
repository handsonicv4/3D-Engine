#include "PixelShaderBase.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	float4 result = (float4) 0;
	result.x = input.position.z;
	result.y = input.position.z;
	result.z = input.position.z;
	result.w = 1.0;
    return  result;
}