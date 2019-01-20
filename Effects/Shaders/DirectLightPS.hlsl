#include "PixelShaderBase.hlsl"

float4 main(PSinput input) : SV_TARGET
{
	Pixel p = GetPixel(input);

	if (p.opacity < 0.05)
		discard;


	float3 color = p.emissivity*p.emissiveColor;
	//float bias = 0.0001f;
	//bool shadow = IsInShadow(input.positionLight, bias);
	//if (shadow)
	//{
	//	return float4(color, p.opacity);
	//}

	color += Diffuse(p)*p.diffuseColor+ Specular(p)*p.specularColor;
	float4 result = float4(color, p.opacity);
    return result;
}