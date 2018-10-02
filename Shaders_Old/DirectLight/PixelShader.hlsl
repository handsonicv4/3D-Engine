#include "TypeDef.hlsl"
#include "../PixelShaderBase.hlsl"

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

	float3 color = DirectLight(p, true, true);
	float4 result = float4(color, p.opacity);
    return   result;
}