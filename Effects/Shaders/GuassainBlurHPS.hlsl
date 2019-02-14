#include "ConstantBuffers.hlsl"
#include "Input.hlsl"
#include "Texture.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	return  GuassainBlur(true, input.tex, 0);
	//return SampleTexture(0, SAMPLER_WARP, input.tex);
}

