#include "Input.hlsl"
#include "Texture.hlsl"
#include "ConstantBuffers.hlsl"

struct MaterialType
{
	float diffusePower;
	float specularPower;
	float specularHardness;
	float emissivity;
	float refractiveIndex;
	float3 diffuseColor;
	float3 emissiveColor;
	float3 specularColor;
	float opacity;
	uint flags;
};
struct Pixel
{
	float diffusePower;
	float specularPower;
	float specularHardness;
	float emissivity;
	float refractiveIndex;
	float3 diffuseColor;
	float3 emissiveColor;
	float3 specularColor;
	float opacity;
	float3 normal;
	float3 position;
};

StructuredBuffer<MaterialType> instanceMaterial : register(t6);

Pixel GetPixel(uniform PSinput input)
{
	Pixel p = (Pixel)0;
	p.diffuseColor = g_DiffuseColor;
	p.emissiveColor = g_AmbientColor;
	p.specularColor = g_SpecularColor;
	p.diffusePower = g_DiffusePower;
	p.specularPower = g_SpecularPower;
	p.specularHardness = g_SpecularHardness;
	p.emissivity = g_Emissivity;
	p.opacity = g_AlphaFactor;
	p.refractiveIndex = g_RefractiveIndex;
	p.position = input.positionWorld.xyz;
	p.normal = input.normal;
	if (g_Flags & 0x02)
	{
		float4 color = SampleTexture(TEXTURE_DIFFUSE, SAMPLER_WARP, input.tex);
		p.diffuseColor = color.rgb;
		p.opacity = color.a;
	}
	if (g_Flags & 0x04)
	{
		float4 normalMap = SampleTexture(TEXTURE_NORMAL, SAMPLER_WARP, input.tex);
		normalMap = (normalMap - 0.5f)*2.0f;
		p.normal = (normalMap.x*input.tangent) + (normalMap.y*input.bitangent) + (normalMap.z*input.normal);
		p.normal = normalize(p.normal);
	}
	return p;
}

Pixel GetInstancePixel(uniform PSinput input)
{
	uint id = input.instanceMaterialID - 1;
	Pixel p = (Pixel)0;
	p.diffuseColor = instanceMaterial[id].diffuseColor;
	p.emissiveColor = instanceMaterial[id].emissiveColor;
	p.specularColor = instanceMaterial[id].specularColor;
	p.diffusePower = instanceMaterial[id].diffusePower;
	p.specularPower = instanceMaterial[id].specularPower;
	p.specularHardness = instanceMaterial[id].specularHardness;
	p.emissivity = instanceMaterial[id].emissivity;
	p.opacity = instanceMaterial[id].opacity;
	p.refractiveIndex = instanceMaterial[id].refractiveIndex;
	p.position = input.positionWorld.xyz;
	p.normal = input.normal;
	if (0x02 & g_Flags & instanceMaterial[id].flags)
	{
		float4 color = SampleTexture(TEXTURE_DIFFUSE, SAMPLER_WARP, input.tex);
		p.diffuseColor = color.rgb;
		p.opacity = color.a;
	}
	if (0x04 & g_Flags & instanceMaterial[id].flags)
	{
		float4 normalMap = SampleTexture(TEXTURE_NORMAL, SAMPLER_WARP, input.tex);
		normalMap = (normalMap - 0.5f)*2.0f;
		p.normal = (normalMap.x*input.tangent) + (normalMap.y*input.bitangent) + (normalMap.z*input.normal);
		p.normal = normalize(p.normal);
	}
	return p;
}
