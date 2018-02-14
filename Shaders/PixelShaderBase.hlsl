#include "ConstantBuffers.hlsl"
#include "LightFunc.hlsl"
#include "Input.hlsl"

struct Light
{
	float4 color;
	float3 direction;
	float angleCos;
};
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

Texture2D diffuseTexture: register(t0);
Texture2D normalTexture: register(t2);
SamplerState samplerState: register(s0);
SamplerState samplerStateC: register(s1);
StructuredBuffer<MaterialType> instanceMaterial : register(t6);
StructuredBuffer<Light> Lights: register(t7);

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
		float4 color = diffuseTexture.Sample(samplerState, input.tex);
		p.diffuseColor = color.rgb;
		p.opacity = color.a;
	}
	if (g_Flags & 0x04)
	{
		float4 normalMap = normalTexture.Sample(samplerState, input.tex);
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
		float4 color = diffuseTexture.Sample(samplerState, input.tex);
		p.diffuseColor = color.rgb;
		p.opacity = color.a;
	}
	if (0x04 & g_Flags & instanceMaterial[id].flags)
	{
		float4 normalMap = normalTexture.Sample(samplerState, input.tex);
		normalMap = (normalMap - 0.5f)*2.0f;
		p.normal = (normalMap.x*input.tangent) + (normalMap.y*input.bitangent) + (normalMap.z*input.normal);
		p.normal = normalize(p.normal);
	}
	return p;
}

float3 Diffuse(uniform Pixel p)
{
	//float3 lightDir = (float3)0;
	float3 result = (float3)0;
	for (uint i = 0; i < g_NumLights; i++)
	{
		//lightDir = normalize(Lights[i].direction - p.position);
		//result += Diffuse(Lights[i].color, lightDir, p.normal).rgb;
		result += PointLightDiffuse(Lights[i].color.rgb, Lights[i].direction, p.position, p.normal);
	}
	return result*p.diffusePower;
}

float3 Specular(uniform Pixel p)
{
	float3 lightDir = (float3)0;
	float3 result = (float3)0;
	float3 cameraDir = normalize(g_CameraPos - p.position);
	for (uint i = 0; i < g_NumLights; i++)
	{
		lightDir = normalize(Lights[i].direction - p.position);
		result += SpecularBlinnPhong(Lights[i].color, lightDir, cameraDir, p.normal, p.specularHardness).rgb;
	}
	return result*p.specularPower;
}

