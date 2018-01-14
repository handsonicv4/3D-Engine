#pragma once
#include<string>
#include"Texture.h"
using namespace std;

class Material
{
public:
	Material();
	string name;
	float shininess;
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float opacity;
	string textureFilePath;
	bool hasDiffuseMap;
	bool hasSpecularMap;
	bool hasAmbientMap;
	bool hasNormalMap;
	TextureData diffuseMap;
	TextureData specularMap;
	TextureData ambientMap;
	TextureData normalMap;
	void Clear();

	int pixelResourceID=-1;

protected:
	 
};

