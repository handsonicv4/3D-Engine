////////////////////////////////////////////////////////////////////////////////
// Filename: color.ps
////////////////////////////////////////////////////////////////////////////////
#include "Common.hlsl"
Texture2D shaderTexture: register(t0);
Texture2D normalTexture: register(t2);
SamplerState SampleType: register(s0);

struct Light
{
	float4 color;
	float3 direction;
	float angleCos;
};

StructuredBuffer<Light> Lights: register(t7);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BINORMAL;
	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
     float4 textureColor;
	 float4 resut;
	 float3 lig;
	 float4 lcolor;
	 float4 normalMap;
	 float3 pixelNormal;
    // Sampling
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	pixelNormal = input.normal;
	if (g_HasNormalMap)
	{
		normalMap = normalTexture.Sample(SampleType, input.tex);
		normalMap = (normalMap - 0.5f)*2.0f;
		pixelNormal = (normalMap.x*input.tangent) + (normalMap.y*input.bitangent) + (normalMap.z*input.normal);
		pixelNormal = normalize(pixelNormal);
	}

	if (textureColor.a < 0.05)
		discard;
	float3 V = normalize(g_CameraPos - input.position.xyz);


	for (uint i = 0;i < g_NumLights;i++)
	{
		lig = normalize(-Lights[i].direction);
	
		lcolor += Diffuse(Lights[i].color, lig, pixelNormal)+Specular(Lights[i].color, lig, 0.7, V, pixelNormal);
	}

	lcolor.a = 1;

	resut = float4( lcolor.xyz*textureColor.xyz, textureColor.a);
	
    return   resut;
}