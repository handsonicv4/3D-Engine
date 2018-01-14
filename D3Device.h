#pragma once
#include <d3d11_1.h>

class GraphicDevice
{
public:
	GraphicDevice();
	~GraphicDevice();

	int screenWidth, screenHeight;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	unsigned int numModes, numerator, denominator;
	int videoCardMemory;
	char videoCardDescription[128];
};
