#pragma once
#include"Material.h"

Material::Material()
{
	name = "";
	shininess = 0;
	diffuse[0] = 1;
	diffuse[1] = 1;
	diffuse[2] = 1;
	ambient[0] = 1;
	ambient[1] = 1;
	ambient[2] = 1;
	specular[0] = 1;
	specular[1] = 1;
	specular[2] = 1;
	opacity = 1;
	textureFilePath = "";
	hasDiffuseMap = false;
	hasSpecularMap = false;
	hasAmbientMap = false;
	hasNormalMap = false;
	diffusePower=1;
	specularHardness=1;
	specularPower=0;
	emissivity=0;
	refractiveIndex=1;
}

void Material::Clear()
{
	diffuseMap.Clear();
	specularMap.Clear();
	ambientMap.Clear();
	normalMap.Clear();
}
