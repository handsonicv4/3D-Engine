#include"Common.hlsl"

//------------------------------
#ifndef THREADS_NUM
#define THREADS_NUM 16
#endif

RWStructuredBuffer<Frustum> out_Frustums : register(u0);

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