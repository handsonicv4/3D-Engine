#include "PixelShaderBase.hlsl"
#define MIP_OFFSET 0;

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
	//from -= direction / g_VoxelDimention.x * 2;
	while (distance < maxDistance && result.a < 0.8)
	{
		float3 samplePoint = from + distance*direction;
		float diameter = spread * distance;//Diameter in UVW space
		float mip =  log2(1 + diameter *g_VoxelDimention.x);
		float mip2 = (mip + 1)*(mip + 1);
		float4 color = voxelTexture.SampleLevel(samplers[1], samplePoint, min(5.7, mip));
		result.rgb +=  0.04*((1 - result.a)* color.a* color.rgb*pow(2,mip+5));
		result.a += (1-result.a)*color.a;
		distance += diameter / (2.0 - spread);
	}
	return 0.9*result.rgb;
}
float3 TraceCone1(float3 from, uniform float spread, uniform float initStride, uniform float maxDistance, in float3 direction)
{
	float4 result = (float4)0;
	float distance = initStride;
	//from -= direction / g_VoxelDimention.x * 2;
	while (distance < maxDistance && result.a < 0.99)
	{
		float3 samplePoint = from + distance*direction;
		float diameter = spread * distance;//Diameter in UVW space
		float mip = log2(1 + diameter *g_VoxelDimention.x);
		float mip2 = (mip + 1)*(mip + 1);
		float4 color = voxelTexture.SampleLevel(samplers[1], samplePoint, min(5.7, mip));
		result.rgb += 0.04*((1 - result.a)* color.a* color.rgb*pow(2, mip + 4));
		result.a += (1 - result.a)*color.a;
		distance += diameter / (2.0 - spread);
	}
	return 0.9*result.rgb;
}

float3 TraceDiffuseCones(float3 from, uniform float3 normal, uniform float initStride, uniform float maxDistance )
{
	//const float seperation = 0.5;
	const float3 w = float3(1,0.6,0);

	const float3 N_OFFSET = normal / g_VoxelDimention * 1;

	from += N_OFFSET;
	//from -= direction / g_VoxelDimention * 2;
	const float3 base1 = normalize(orthogonal(normal));
	const float3 base2 = normalize(cross(base1, normal));
	//const float3 base3 = normalize(base1 + base2);
	//const float3 base4 = normalize(base1 - base2);
	
	const float3 cone1 = normalize(normal + base1);
	const float3 cone2 = normalize(normal + base2);
	const float3 cone3 = normalize(normal - base1);
	const float3 cone4 = normalize(normal - base2);
	//const float3 cone5 = normalize(normal + base3);
	//const float3 cone6 = normalize(normal + base4);
	//const float3 cone7 = normalize(normal - base3);
	//const float3 cone8 = normalize(normal - base4);

	float3 result = (float3)0;
	float spread = 0.925;
	result += w[0] * TraceCone(from, spread, initStride, maxDistance, normal);

	result += w[1] * TraceCone(from, spread, initStride, maxDistance, cone1);
	result += w[1] * TraceCone(from, spread, initStride, maxDistance, cone2);
	result += w[1] * TraceCone(from, spread, initStride, maxDistance, cone3);
	result += w[1] * TraceCone(from, spread, initStride, maxDistance, cone4);

	//result += w[2] * TraceCone(from, spread, initStride, maxDistance, cone5);
	//result += w[2] * TraceCone(from, spread, initStride, maxDistance, cone6);
	//result += w[2] * TraceCone(from, spread, initStride, maxDistance, cone7);
	//result += w[2] * TraceCone(from, spread, initStride, maxDistance, cone8);

	return 0.5*result;
}

float2 PositionToUV(float4 pos)
{
	return float2(pos.x/ g_ScreenDimensions.x, pos.y/ g_ScreenDimensions.y);
}

float4 main(PSinput input) : SV_TARGET
{

	Pixel p = GetPixel(input);

	if (p.opacity < 0.05)
		discard;

	float voxelStride = 1.0 / g_VoxelDimention.x;
	float3 pos = WorldToVoxelUVW(p.position);
	float3 cameraDir = normalize(g_CameraPos - p.position);

	float3 diffuse = (float3)0;
	float3 specular = (float3)0;
	float3 rs = (float3)0;

	//if (!IsInShadow(input.positionLight, 0.0001f))
	//{
		diffuse = Diffuse(p);
		specular = Specular(p);
	//}
	diffuse += p.diffusePower*TraceDiffuseCones(pos, p.normal, voxelStride*2, 1.4);

	if (p.specularPower > 1)
	{
		float3 inSpecDir = reflect(-cameraDir, p.normal);
		specular += p.specularPower*TraceCone(pos + input.normal / g_VoxelDimention * 3, 0.19, voxelStride * 2, 1.2, inSpecDir);
	}

	if (p.refractiveIndex > 1.1 && p.opacity<1)
	{
		float3 refractDir = refract(-cameraDir, p.normal, 0.7);
		rs = 6 * TraceCone1(pos - input.normal / g_VoxelDimention *2, 0.2, voxelStride*4, 1.4, refractDir);
	}
	float shade = shadowMap.Sample(samplers[1], PositionToUV(input.position)).r;
	
	float4 result = float4(shade*(p.opacity*diffuse*p.diffuseColor+specular*p.specularColor)+ (1-p.opacity)*rs + p.emissivity*p.emissiveColor, p.opacity);

	return   result;
}