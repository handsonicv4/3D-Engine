#include "PixelShaderBase.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	Pixel p;
	if (input.instanceMaterialID > 0)
	{
		p = GetInstancePixel(input);
	}
	else
	{
		p = GetPixel(input);
	}
	if (p.opacity < 0.05)
		discard;

	return (float4)0;
}