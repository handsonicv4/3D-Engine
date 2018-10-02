#include "../ConstantBuffers.hlsl"
#include "../LightFunc.hlsl"
#include "TypeDef.hlsl"
#define MIP_OFFSET 0;
Texture2D shaderTexture: register(t0);
Texture2D normalTexture: register(t2);

SamplerState Sampler: register(s0);

struct Light
{
	float4 color;
	float3 direction;
	float angleCos;
};

StructuredBuffer<Light> Lights: register(t7);
Texture3D voxelTexture: register(t8);
float3 orthogonal(float3 vec) {

	vec = normalize(vec);

	float3 v = float3(1, 0, 0); // Pick any normalized vector.

	return abs(dot(vec, v)) > 0.99f ? cross(vec, float3(0, 1, 0)) : cross(vec, v);

}

uint3 WorldToVoxelPosition(float3 pos)
{
	return (pos + 0.5*g_VoxelDimention*g_VoxelSize) / g_VoxelSize;

}
float3 WorldToVoxelUVW(float3 pos)
{
	return (pos + 0.5*g_VoxelDimention*g_VoxelSize) / (g_VoxelDimention*g_VoxelSize);
}

//All parameter in UVW space
float3 TraceCone(float3 from, uniform float spread, uniform float initStride, uniform float maxDistance, in float3 direction)
{

	float4 result = (float4)0;
	float distance = initStride;
	while (distance < maxDistance && result.a < 1)
	{
		float3 samplePoint = from + distance*direction;
		float diameter = spread * distance;//Diameter in UVW space
		float mip =  log2(1 + diameter *g_VoxelDimention.x);
		float mip2 = (mip + 1)*(mip + 1);
		float4 color = voxelTexture.SampleLevel(Sampler, samplePoint, min(5.7, mip));
		
		result.rgb = result.a*result.rgb + (1 - result.a)*color.a*color.rgb* pow(2, 2 * mip);
		result.a += result.a + (1 - result.a)*color.a;
		distance += mip2 / g_VoxelDimention.x * 2;
	}
	return 0.075*result.rgb;
}

float3 TraceDiffuseCones(float3 from, uniform float3 normal, uniform float initStride, uniform float maxDistance )
{
	//const float seperation = 0.5;
	const float3 w = float3(1,0.7,0.7);

	const float3 N_OFFSET = normal / g_VoxelDimention * 3.7;
	from += N_OFFSET;
	//from -= direction / g_VoxelDimention * 2;
	const float3 base1 = normalize(orthogonal(normal));
	const float3 base2 = normalize(cross(base1, normal));
	const float3 base3 = normalize(base1 + base2);
	const float3 base4 = normalize(base1 - base2);
	
	const float3 cone1 = normalize(normal + base1);
	const float3 cone2 = normalize(normal + base2);
	const float3 cone3 = normalize(normal - base1);
	const float3 cone4 = normalize(normal - base2);
	const float3 cone5 = normalize(normal + base3);
	const float3 cone6 = normalize(normal + base4);
	const float3 cone7 = normalize(normal - base3);
	const float3 cone8 = normalize(normal - base4);

	float3 result = (float3)0;
	 
	result += w[0] * TraceCone(from- normal / g_VoxelDimention * 2, 0.325, initStride, maxDistance, normal);
																		
	result += w[1] * TraceCone(from - cone1 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone1);
	result += w[1] * TraceCone(from - cone2 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone2);
	result += w[1] * TraceCone(from - cone3 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone3);
	result += w[1] * TraceCone(from - cone4 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone4);
																
	result += w[2] * TraceCone(from - cone5 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone5);
	result += w[2] * TraceCone(from - cone6 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone6);
	result += w[2] * TraceCone(from - cone7 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone7);
	result += w[2] * TraceCone(from - cone8 / g_VoxelDimention * 2, 0.325, initStride, maxDistance, cone8);

	return 0.5*result;
}

float4 main(PSinput input) : SV_TARGET
{

	float4 textureColor = float4(g_DiffuseColor, 1);
	float4 resut;
	float3 lightDir;
	float4 lcolor;
	float4 normalMap;
	float3 pixelNormal;
	// Sampling
	if (g_HasDiffuseMap)
	{
		textureColor = shaderTexture.Sample(Sampler, input.tex);
	}
	pixelNormal = input.normal;
	if (g_HasNormalMap)
	{
		normalMap = normalTexture.Sample(Sampler, input.tex);
		normalMap = (normalMap - 0.5f)*2.0f;
		pixelNormal = (normalMap.x*input.tangent) + (normalMap.y*input.bitangent) + (normalMap.z*input.normal);
		pixelNormal = normalize(pixelNormal);
	}

	if (textureColor.a < 0.05)
		discard;

	float3 cameraDir = normalize(g_CameraPos - input.positionWorld.xyz);


	for (uint i = 0; i < g_NumLights; i++)
	{
		//lightDir = normalize(-Lights[i].direction);
		lightDir = normalize(Lights[i].direction - input.positionWorld.xyz);
		lcolor += Diffuse(Lights[i].color, lightDir, pixelNormal) + SpecularBlinnPhong(Lights[i].color, lightDir, cameraDir, pixelNormal, 5, 30);
	}

	float voxelStride = 1.0 / g_VoxelDimention.x;

	float3 pos = WorldToVoxelUVW(input.positionWorld.xyz);
	
	float3 ds = TraceDiffuseCones(pos, pixelNormal, voxelStride*3, 1.4);

	//float3 inSpecDir = reflect(-cameraDir, pixelNormal);

	//float3 sp = 3*TraceCone(pos, 0.3, voxelStride*3, 1.2, inSpecDir);

	//float3 refractDir = refract(-cameraDir, pixelNormal, 1.5);

	//float3 rs = 2*TraceCone(pos - input.normal / g_VoxelDimention * 5, 0.2, voxelStride * 4, 1.4, refractDir);

	resut = float4(g_AlphaFactor*(lcolor.rgb + ds)*textureColor.rgb, textureColor.a);

	return   resut;
}