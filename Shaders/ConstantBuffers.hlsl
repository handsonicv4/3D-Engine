cbuffer cbPerObject : register(b0)
{
	uint g_Flags							: packoffset(c0.x);
	float g_AlphaFactor				: packoffset(c0.y);
	float g_DiffusePower			: packoffset(c0.z);
	float g_SpecularHardness	: packoffset(c0.w);
	float3 g_DiffuseColor			: packoffset(c1);
	float g_SpecularPower			: packoffset(c1.w);
	float3 g_SpecularColor		: packoffset(c2);
	float g_Emissivity					: packoffset(c2.w);
	float3 g_AmbientColor		: packoffset(c3.x);
	float g_RefractiveIndex		: packoffset(c3.w);
}

cbuffer cbPerFrame : register(b1)
{
	matrix	g_Projection						: packoffset(c0);
	matrix	g_ProjectionInv				: packoffset(c4);
	float3	g_CameraPos					: packoffset(c8);
	float		g_fAlphaTest					: packoffset(c8.w);
	uint2	g_ScreenDimensions		: packoffset(c9);
	uint		g_NumLights					: packoffset(c9.z);
	uint		g_MaxNumLightsPerTile	: packoffset(c9.w);
	uint3	g_VoxelDimention			: packoffset(c10);
	uint		pad1								: packoffset(c10.w);
	float3	g_VoxelSize						: packoffset(c11);
};
