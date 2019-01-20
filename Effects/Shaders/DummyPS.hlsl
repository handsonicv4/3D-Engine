#include "PixelShaderBase.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	return  SampleTexture(TEXTURE_DIFFUSE, SAMPLER_WARP, input.tex);
	//return  float4(1, 1, 1, 1);
}