#include "ConstantBuffers.hlsl"
#include "LightFunc.hlsl"
#include "Input.hlsl"
#include "Texture.hlsl"
#include "Skinning.hlsl"

struct Light
{
	float4 color;
	float3 direction;
	float angleCos;
	matrix lightVP;
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

Texture2D shadowMap: register(t10);
StructuredBuffer<Light> Lights: register(t7);




bool IsInShadow(uniform float4 pixelLightPos, uniform float bias)
{
	float2 projectTexCoord;
	bool result = false;
	projectTexCoord.x = pixelLightPos.x / pixelLightPos.w / 2.0f + 0.5f;
	projectTexCoord.y = -pixelLightPos.y / pixelLightPos.w / 2.0f + 0.5f;
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float depthValue = shadowMap.Sample(samplers[1], projectTexCoord).r;
		float pixel_light_z = pixelLightPos.z / pixelLightPos.w - bias;
		if (pixel_light_z > depthValue)
		{
			result = true;
		}
	}
	return result;
}

Pixel GetPixel(uniform PSinput input)
{
	Pixel p = (Pixel)0;
	uint flags = instanceData[input.instanceID].flags;
	p.opacity = 1;
	if (flags & 0x02)
	{
		float4 color = SampleTexture(TEXTURE_DIFFUSE, SAMPLER_WARP, input.tex);
		p.diffuseColor = color.rgb;
		p.opacity = color.a;
	}

	float weight = instanceData[input.instanceID].diffuseBlendFactor;
	float3 color = instanceData[input.instanceID].diffuseColor;
	p.diffuseColor = p.diffuseColor * (1 - weight) + color * weight;
	p.specularColor = instanceData[input.instanceID].specularColor;
	p.emissiveColor = instanceData[input.instanceID].emissiveColor;
	p.diffusePower = instanceData[input.instanceID].diffusePower;
	p.specularPower = instanceData[input.instanceID].specularPower;
	p.emissivity = instanceData[input.instanceID].emissivePower;
	p.specularHardness = instanceData[input.instanceID].specularHardness;
	p.opacity *= instanceData[input.instanceID].alphaFactor;
	p.refractiveIndex = instanceData[input.instanceID].refractiveIndex;
	p.position = input.positionWorld.xyz;
	p.normal = input.normal;

	if (flags & 0x04)
	{
		float4 normalMap = SampleTexture(TEXTURE_NORMAL, SAMPLER_WARP, input.tex);
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

