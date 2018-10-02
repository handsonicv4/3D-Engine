#include"../ConstantBuffers.hlsl"

//------------------------------
#ifndef TILE_SIZE
//#pragma message( "TILESIZE undefined. Default to 16.")
#define TILE_SIZE (uint)16 // should be defined by the application.
#endif

#ifndef THREADS_NUM
#define THREADS_NUM 16
#endif

RWStructuredBuffer<Frustum> out_Frustums : register(u0);

struct ComputeShaderInput
{
	uint3 groupID : SV_GroupID; // 3D index of the thread group in the dispatch.
	uint3 groupThreadID : SV_GroupThreadID; // 3D index of local thread ID in a thread group.
	uint3 dispatchThreadID  : SV_DispatchThreadID;  // 3D index of global thread ID in the dispatch.
	uint  groupIndex  : SV_GroupIndex;  // Flattened local index of the thread within a thread group.
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



[numthreads(THREADS_NUM, THREADS_NUM, 1)]
void main(ComputeShaderInput IN)
{
	// View space eye position is always at the origin.
	uint3 threadsDimensions = uint3(g_ScreenDimensions.x / TILE_SIZE, g_ScreenDimensions.y / TILE_SIZE, 1);
	if (g_ScreenDimensions.x%TILE_SIZE != 0)
		threadsDimensions.x += 1;
	if (g_ScreenDimensions.y%TILE_SIZE != 0)
		threadsDimensions.y += 1;
	const float3 eyePos = float3(0, 0, 0);

	uint2 screenSpace[4];
	//diagonal
	// Top left point
	screenSpace[0] = IN.dispatchThreadID.xy * TILE_SIZE;
	// Bottom right point
	screenSpace[3] = (IN.dispatchThreadID.xy + uint2(1,1)) * TILE_SIZE;
	//shrink to fit screen boundary
	if (screenSpace[3].x >= g_ScreenDimensions.x)
		screenSpace[3].x = g_ScreenDimensions.x - 1;
	if (screenSpace[3].y >= g_ScreenDimensions.y)
		screenSpace[3].y = g_ScreenDimensions.y - 1;

	//other diagonal
	// Top right point
	screenSpace[1] = uint2(screenSpace[3].x, screenSpace[0].y);
	// Bottom left point
	screenSpace[2] = uint2(screenSpace[0].x, screenSpace[3].y);


	float3 viewSpace[4];
	// Now convert the screen space points to view space
	for (int i = 0; i < 4; i++)
	{

		viewSpace[i] = ScreenToView(float4(screenSpace[i],1.0f,1.0f)).xyz;
	}

	Frustum frustum;
	frustum = ComputeFrustum(eyePos, viewSpace[0], viewSpace[1], viewSpace[2], viewSpace[3]);

	if (IN.dispatchThreadID.x < threadsDimensions.x && IN.dispatchThreadID.y < threadsDimensions.y)
	{
		uint index = IN.dispatchThreadID.x + (IN.dispatchThreadID.y * threadsDimensions.x);

		out_Frustums[index] = frustum;
	}
}