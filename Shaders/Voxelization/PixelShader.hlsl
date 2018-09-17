#include "../PixelShaderBase.hlsl"
#include "TypeDef.hlsl"

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

	TextureColor color = GetTextureColor(input);
	LightColor light = DirectLight(input, true, false);

	uint3 vposition = WorldToVoxelPosition(input.positionWorld.xyz);
	float4 result= float4(color.diffuse*light.diffuse, color.alpha);
	voxels[vposition] = result;
	discard;
	return result;
}