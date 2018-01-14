#pragma once
#include"Material.h"

Material::Material()
{
	name = "";
	shininess = 0;
	memset(diffuse, 0, sizeof(diffuse));
	memset(ambient, 0, sizeof(ambient));
	memset(specular, 0, sizeof(specular));
	opacity = 1;
	textureFilePath = "";
	hasDiffuseMap = false;
	hasSpecularMap = false;
	hasAmbientMap = false;
	hasNormalMap = false;
}

void Material::Clear()
{
	diffuseMap.Clear();
	specularMap.Clear();
	ambientMap.Clear();
	normalMap.Clear();
}
