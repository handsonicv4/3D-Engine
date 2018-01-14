#pragma once
#include<vector>
//Need STB_image lib

using namespace std;

class TextureData
{
public:

	int width;
	int height;
	int bpp;
	int imageSize;
	vector<string> supportedType;
	unsigned char* GetImageDataPtr();

	bool LoadFromFile(string filePath);
	void Clear();

	TextureData();
	~TextureData();
protected:

	unsigned char *data;
};

