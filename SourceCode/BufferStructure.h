#pragma once

struct FrameBufferData
{
	float	projection[16];
	float	projectionInv[16];
	float	cameraPos[3];
	float alphaTest;
	UINT screenDimensions[2];
	UINT numLights;
	UINT maxNumLightsPerTile;
};

struct ObjBufferData
{
	UINT HasAmbientMap;
	UINT HasDiffuseMap;
	UINT HasNormalMap;
	UINT HasSpecularMap;

	UINT HasAnimation;
	UINT Transparent;
	float pad[2];
};

struct InstanceData
{
	float	wVP[16];
	float world[16];
	float color[4];
	unsigned int bindMatrixOffset;
};

struct Light
{
	float color[4];
	float direction[3];//sport, directional
	float angleCos;
	//float range;//sport, point
};

struct Plane
{
	float normal[3];
	float dist;
};

struct Frustum
{
	Plane planes[4];   // top, right, bottom, left  frustum planes.
};