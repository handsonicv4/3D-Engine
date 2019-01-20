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

	float pixelSize[2];
	float pad3[2];
};

struct InstanceData
{
	float	wVP[16];

	float worldMatrix[16];

	//Texture & Color blend:
	float diffuseColor[3];
	float diffuseBlendFactor;   //The weight of diffuseColor VS. diffuse texture (1: all from diffuseColor, 0: all from texture)

	float specularColor[3];
	float specularBlendFactor;   //The weight of specularColor VS. specular texture

	float emissiveColor[3];
	float emissiveBlendFactor;  //The weight of emissiveColor VS. emissive texture

	//Texture Offset (UV offset)
	float diffuseTextureOffset[2];
	float specularTextureOffset[2];

	float emissiveTextureOffset[2];
	float normalTextureOffset[2];

	UINT flags;
	float alphaFactor;
	float refractiveIndex;
	float specularHardness;

	float diffusePower;
	float specularPower;
	float emissivePower;
	unsigned int bindMatrixOffset;
};

struct Light
{
	float color[4];
	float direction[3];//sport, directional
	float angleCos;
	float vP[16];
};
