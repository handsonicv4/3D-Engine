#include "../ConstantBuffers.hlsl"
#include "TypeDef.hlsl"
Texture2D shaderTexture: register(t0);
SamplerState Sampler: register(s0);

float4 main(PSinput input) : SV_TARGET
{
	if (0x02 & g_Flags)
	{
		float4 textureColor = shaderTexture.Sample(Sampler, input.tex);
		if (textureColor.a < 0.05)
		{
			discard;
		}
	}
    return   float4(1,1,1,1);
}