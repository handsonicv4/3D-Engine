#include "PixelShaderBase.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	return SampleTexture(0, SAMPLER_WARP, input.tex) * SampleTexture(1, SAMPLER_WARP, input.tex);
}