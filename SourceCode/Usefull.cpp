#pragma once
#include"Usefull.h"
void Message(LPCSTR title, int in)
{
	char c[256];
	sprintf_s(c, "%d", in);
	MessageBoxA(0, c, title, 0);
}

void Message(LPCSTR title, float in)
{
	char c[256];
	sprintf_s(c, "%f", in);
	MessageBoxA(0, c, title, 0);
}

string GetFileExtention(string fileName)
{
	int index = fileName.length() - 2;
	string result;
	for (; index >= 0; index--)
	{
		if (fileName[index] == '.')
		{
			result = fileName.substr(index + 1, fileName.length() - index - 1);
			for (size_t i = 0; i < result.length(); i++)
			{
				if (result[i] >= 'A' && result[i] <= 'Z')
					result[i] = 32 + result[i];
			}

			return result;
		}
		if (fileName[index] == '\\' || fileName[index] == '/' || fileName[index] == ':')
			return "";
	}
	return "";
}

string GetFolderPath(string filePath)
{
	for (int i = filePath.length() - 1; i >= 0; i--)
	{
		if (filePath[i] == '\\')
		{
			return filePath.substr(0, i + 1);
		}
	}
	return "";
}

string GetFileName(string filePath)
{
	for (int i = filePath.length() - 1; i >= 0; i--)
	{
		if (filePath[i] == '\\')
		{
			return filePath.substr(i + 1, filePath.length() - 1 - i);
		}
	}
	return "";
}