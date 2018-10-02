////////////////////////////////////////////////////////////////////////////////
// Filename: color.ps
////////////////////////////////////////////////////////////////////////////////
#include "../ConstantBuffers.hlsl"
#include "../LightFunc.hlsl"
#include "TypeDef.hlsl"
Texture2D shaderTexture: register(t0);
Texture2D normalTexture: register(t2);
SamplerState SampleType: register(s0);

struct Light
{
	float4 color;
	float3 direction;
	float angleCos;
};
StructuredBuffer<MaterialType> instanceMaterial : register(t6);
StructuredBuffer<Light> Lights: register(t7);
RWTexture3D<float4> voxels:register(u4);

bool IsInsideScene(const float3 pos)
{
	float3 bound = 0.5*g_VoxelDimention*g_VoxelSize;
	return abs(pos.x) < bound.x && abs(pos.y) < bound.y && abs(pos.z) < bound.z;
}

uint3 WorldToVoxelPosition(float3 pos)
{
	return (pos + 0.5*g_VoxelDimention.x*g_VoxelSize.x)/g_VoxelSize.x;

}

float4 main(PSinput input) : SV_TARGET
{
	if (!IsInsideScene(input.positionWorld.xyz))
	{
		discard;
	}

	float4 textureColor = float4(g_DiffuseColor, 1);
	float4 resut;
	float3 lightDir;
	float4 lcolor;
	float4 normalMap;
	float3 pixelNormal;
	// Sampling
	if (g_HasDiffuseMap)
	{
		textureColor = shaderTexture.Sample(SampleType, input.tex);
	}
	
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

	//float3 cameraDir = normalize(g_CameraPos - input.positionWorld.xyz);


	for (uint i = 0; i < g_NumLights; i++)
	{
		lightDir = normalize(Lights[i].direction - input.positionWorld.xyz );

		lcolor += Diffuse(Lights[i].color, lightDir, pixelNormal);// +SpecularBlinnPhong(Lights[i].color, lightDir, cameraDir, pixelNormal, 3, 4);
	}
	resut = float4(lcolor.xyz*textureColor.xyz, textureColor.a*g_AlphaFactor);


	uint3 vposition = WorldToVoxelPosition(input.positionWorld.xyz);

	voxels[vposition] = resut;
	discard;
	return resut;
}