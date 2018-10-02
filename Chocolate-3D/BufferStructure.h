//GPU realated buffer and structure definaiton
//-Author Sentao
#pragma once
typedef unsigned int UINT;

struct FrameBufferData
{
	float	projection[16];

	float	projectionInv[16];

	float	cameraPos[3];
	float alphaTest;

	UINT screenDimensions[2];
	UINT numLights;
	UINT maxNumLightsPerTile;

	UINT voxelDimention[3];
	float pad1;

	float voxelSize[3];
	float pad2;

	float	lightVP[16];
};

struct ObjBufferData
{
	UINT flags;
	float alphaFactor;
	float diffusePower;
	float specularHardness;

	float diffuseColor[3];
	float specularPower;

	float specularColor[3];
	float emissivity;

	float ambientColor[3];
	float refractiveIndex;


};

struct InstanceData
{
	float	wVP[16];
	float world[16];
	float color[4];
	unsigned int bindMatrixOffset;
	unsigned int instanceMaterialID;
};

struct Light
{
	float color[4];
	float direction[3];//sport, directional
	float angleCos;
	
	//float range;//sport, point
};
