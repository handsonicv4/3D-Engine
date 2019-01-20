#include "ConstantBuffers.hlsl"
#include "Input.hlsl"
#include "Texture.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	return  GuassainBlur(false, input.tex, 0);
}

