
cbuffer cbPerFrame : register(b1)
{
	matrix	g_Projection					: packoffset(c0);
	matrix	g_ProjectionInv				: packoffset(c4);
	float3	g_CameraPos					: packoffset(c8);
	float		g_fAlphaTest					: packoffset(c8.w);
	uint2	g_ScreenDimensions		: packoffset(c9);
	uint		g_NumLights					: packoffset(c9.z);
	uint		g_MaxNumLightsPerTile	: packoffset(c9.w);
	uint3	g_VoxelDimention			: packoffset(c10);
	uint		pad1								: packoffset(c10.w);
	float3	g_VoxelSize					: packoffset(c11);
	//
	uint		pad2								: packoffset(c11.w);
	matrix	g_LightVP						: packoffset(c12);
	float2	g_PixelSize					: packoffset(c16);
	float2	pad3								: packoffset(c16.z);
};
