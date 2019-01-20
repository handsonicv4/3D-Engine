#include "PixelShaderBase.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	Pixel p = GetPixel(input);

	if (p.opacity < 0.05)
		discard;

	return (float4)0;
}