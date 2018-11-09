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