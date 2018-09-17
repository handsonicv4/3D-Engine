#pragma once
#include"Texture.h"
#include"Usefull.h"
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include"stb-master/stb_image.h"

TextureData::TextureData()
{
	data = NULL;
	supportedType.push_back("png");
	supportedType.push_back("jpg");
	supportedType.push_back("tga");
	supportedType.push_back("bmp");
	supportedType.push_back("ppm");
	supportedType.push_back("gif");
	supportedType.push_back("psd");
	supportedType.push_back("hdr");
	supportedType.push_back("pic");
	supportedType.push_back("pgm");
	Clear();
}
TextureData::~TextureData()
{
	Clear();
}
void TextureData::Clear()
{

	if (data)
	{
		stbi_image_free(data);
		data = NULL;
	}

	width = 0;
	height = 0;
	imageSize = 0;

}
bool TextureData::LoadFromFile(string filePath)
{
	Clear();

	data = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);
	if (!data&&GetFileExtention(filePath) == "")//Some model files do not include file extention in texture file name
	{
		string tryPath;
		for (size_t i = 0; i < supportedType.size(); i++)
		{
			tryPath = filePath + "." + supportedType[i];
			data = stbi_load(tryPath.c_str(), &width, &height, &bpp, 4);
			if (data)
				break;
		}
	}

	if (!data)
		return false;
	bpp = 32;
	imageSize = height*width*(bpp / 8);//iByte
	return true;
}
unsigned char * TextureData::GetImageDataPtr()
{
	return data;
}