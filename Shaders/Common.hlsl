#ifndef TILE_SIZE
//#pragma message( "TILESIZE undefined. Default to 16.")
#define TILE_SIZE (uint)16 // should be defined by the application.
#endif

struct ComputeShaderInput
{
	uint3 groupID : SV_GroupID; // 3D index of the thread group in the dispatch.
	uint3 groupThreadID : SV_GroupThreadID; // 3D index of local thread ID in a thread group.
	uint3 dispatchThreadID  : SV_DispatchThreadID;  // 3D index of global thread ID in the dispatch.
	uint  groupIndex  : SV_GroupIndex;  // Flattened local index of the thread within a thread group.
};

cbuffer cbPerObject : register(b0)
{
	bool g_HasAmbientMap		: packoffset(c0.x);
	bool g_HasDiffuseMap		: packoffset(c0.y);
	bool g_HasNormalMap		: packoffset(c0.z);
	bool g_HasSpecularMap		: packoffset(c0.w);

	bool g_HasAnimation			: packoffset(c1.x);
	bool g_Transparent				: packoffset(c1.y);
}

cbuffer cbPerFrame : register(b1)
{
	matrix	g_Projection						: packoffset(c0);
	matrix	g_ProjectionInv				: packoffset(c4);
	float3	g_CameraPos					: packoffset(c8);
	float		g_fAlphaTest					: packoffset(c8.w);//........
	uint2	g_ScreenDimensions		: packoffset(c9);
	uint		g_NumLights					: packoffset(c9.z);
	uint		g_MaxNumLightsPerTile	: packoffset(c9.w);
};

struct Plane
{
	float3 N;   // Plane normal.
	float  d;   // Distance to origin.
};
struct Frustum
{
	Plane planes[4];   // top, right, bottom,left   frustum planes.
};
Plane ComputePlane(float3 p0, float3 p1, float3 p2)
{
	Plane plane;

	float3 v0 = p1 - p0;
	float3 v2 = p2 - p0;

	plane.N = normalize(cross(v0, v2));

	// Compute the distance to the origin using p0.
	plane.d = dot(plane.N, p0);

	return plane;
}
Frustum ComputeFrustum(float3 head, float3 bottom1, float3 bottom2, float3 bottom3, float3 bottom4)
{
	Frustum frustum;
	frustum.planes[0] = ComputePlane(head, bottom1, bottom2);
	frustum.planes[1] = ComputePlane(head, bottom2, bottom3);
	frustum.planes[2] = ComputePlane(head, bottom3, bottom4);
	frustum.planes[3] = ComputePlane(head, bottom4, bottom1);
	return frustum;
}

float4 ClipToView(float4 clip)
{
	// View space position.
	float4 view = mul(clip, g_ProjectionInv);
	// Perspective projection.
	view = view / view.w;
	return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView(float4 screen)
{
	// Convert to normalized texture coordinates
	float2 nScreen = screen.xy / g_ScreenDimensions;

	// Convert to clip space
	float4 clip = float4(float2(nScreen.x, 1.0f - nScreen.y) * 2.0f - 1.0f, screen.z, screen.w);

	return ClipToView(clip);
}

float4 Diffuse(float4 lightColor, float3 lightVec, float3 normal)
{
	float NdotL = max(dot(normal, lightVec), 0);
	return lightColor * NdotL;
}

float4 Specular(float4 lightColor,  float3 lightVec, float specularPower, float3 viewVec,float3 normal)
{
	float3 reflectionVec = normalize(reflect(-lightVec, normal));
	float RdotV = max(dot(reflectionVec, viewVec), 0);

	return lightColor * pow(RdotV, specularPower);
}