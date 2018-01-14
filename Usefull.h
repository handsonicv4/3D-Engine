#pragma once
#include<string>
#include<windows.h>
using namespace std;

void Message(LPCSTR title, int in);

void Message(LPCSTR title, float in);

string GetFileExtention(string fileName);

string GetFolderPath(string filePath);

string GetFileName(string filePath);