#define TEXTURE_NUM 3
#define TEXTURE_DIFFUSE 0
#define TEXTURE_NORMAL 2

#define SAMPLER_NUM 3
#define SAMPLER_WARP 0
#define SAMPLER_CLAMP 1

Texture2D textures[TEXTURE_NUM]: register(t0);
SamplerState samplers[SAMPLER_NUM]: register(s0);

float4 SampleTexture(uniform uint textureID, uniform uint samplerID, uniform float2  uv)
{
	return textures[textureID].Sample(samplers[samplerID], uv);
}

float4 SampleTexture(uniform uint textureID, uniform uint samplerID, uniform float2  uv, uniform float mip)
{
	return textures[textureID].SampleLevel(samplers[samplerID], uv, mip);
}
//7 kernal
float4 GuassainBlur(bool isHorizontal, float2 texCoord, uint textureID)
{
	const int step = 2;
	const float weights[step] = { 0.44908, 0.05092 };
	const float offset[step] = { 0.53805, 2.06278 };

	float4 result = (float4) 0;
	float2 pixelOffset = 2.0f / g_ScreenDimensions;
	if (isHorizontal) pixelOffset.y = 0;
	else pixelOffset.x = 0;
	for (int i = 0; i < step; i++)
	{
		float2 texOffset = dot(offset[i] , pixelOffset);
		float4 color = SampleTexture(textureID, SAMPLER_WARP, texCoord + texOffset,1) + SampleTexture(textureID, SAMPLER_WARP, texCoord - texOffset,1);
		result += weights[i] * color;
	}
	return result;
}