#include"GEngine.h"
#include"Ark.h"
//#include"Timer.h"
using namespace std;
#include "Shlwapi.h" //file name
#pragma comment(lib, "Shlwapi.lib" )

GEngine engine;
aiVector3D moveVec(0, 0, 0);
vector<Instance*> instants;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_INPUT:
	{
		UINT dwSize = 40;
		static BYTE lpb[40];

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;
			engine.camera.SpinYaw(-0.6f*xPosRelative);
			engine.camera.SpinPitch(-0.6f*yPosRelative );
		}
		break;
	}
	case WM_KEYDOWN:
	{

		if (wparam == 'W')
			moveVec.z = 0.2;
		if (wparam == 'S')
			moveVec.z = -0.2;
		if (wparam == 'A')
			moveVec.x = -0.2;
		if (wparam == 'D')
			moveVec.x = 0.2;
		if (wparam == 'X')
		{
			engine.camera.LookAt(aiVector3D(0, 0, 1));
		}
		break;
	}
	case WM_KEYUP:
	{
		if (wparam == VK_ESCAPE)
		{
			engine.Shutdown();
			exit(0);
		}
		if (wparam == 'W')
			moveVec.z = 0;
		if (wparam == 'S')
			moveVec.z = 0;
		if (wparam == 'A')
			moveVec.x = 0;
		if (wparam == 'D')
			moveVec.x = 0;
		break;
	}
	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);;
	}
	}
}

void CALLBACK Timer15ms(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	//Camera Move
	float movingSpeed = 0.2;
	aiVector3D move;
	if (moveVec.Length() > 0.0001)
	{
		move = moveVec.Normalize();
		move = move*movingSpeed;
		engine.camera.MoveFront(move.z);
		engine.camera.MoveRight(move.x);
	}

	//Animation
	for (int i = 0; i < instants.size(); i++)
	{
		Instance *in =instants[i];
		if (in->animationTime * 25 > in->GetAnimationDuration())
		{
			if (in->animationID + 1 < in->GetAnimationNumber())
			{
				in->animationID++;
			}
			else
			{
				in->animationID = 0;
			}
			in->animationTime = 0;
		}
		in->animationTime += 0.007;
	}

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	char path[MAX_PATH + 1];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	string workingFolder = path;
	workingFolder += "\\";
	ArkWindow window(L"Engine");
	window.lpfnWndProc = WndProc;
	window.Show();
	
	engine.Init(window.hwnd, false);
	
	int defaultVS = engine.shaders.CreateVertexShader(workingFolder+"Shaders\\VertexShader.hlsl", "main");
	int defaultPS = engine.shaders.CreatePixelShader(workingFolder + "Shaders\\PixelShader.hlsl", "main");

	engine.shaders.ActivateVertexShader(defaultVS);
	engine.shaders.ActivatePixelShader(defaultPS);

	int cs = engine.shaders.CreateComputeShader(workingFolder + "Shaders\\TileCompute.hlsl", "main");
	engine.shaders.ActivateComputeShader(cs);

	engine.camera.SetPosition(0, 0, -10);
	engine.camera.FaceTo(aiVector3D(0, 0, 1));

	Light x;
	x.color[0] = 0.9f;
	x.color[1] = 0.9f;
	x.color[2] = 0.9f;
	x.color[3] = 1.0f;
	x.direction[0] = 0.2f;
	x.direction[1] = -0.0f;
	x.direction[2] = 0.8f;
	engine.lightList.push_back(x);

	Model amodel;
	amodel.LoadFileD3D(workingFolder+"Models\\TestModel.fbx");
	//bmodel.LoadFileD3D("E:\\3d121007ms\\魔兽世界模型の arcticcondor\\Creature\\Arcticcondor\\arcticcondor.obj");

	int mid = engine.LoadModel(amodel);
	amodel.Purge();

	Instance *in = amodel.CreateInstance();
	in->animationID = 0;
	in->animationTime = 0;

	instants.push_back(in);
	//int tilebuffer=d.resources.CreateBuffer(D3D11_BIND_UNORDERED_ACCESS, false, sizeof(Frustum) * d.maxTileNumber, NULL, NULL, sizeof(Frustum));
	//d.resources.SetBinding(Stage_Compute_Shader, Bind_Unordered_Access, 0, tilebuffer);
	//d.Tiling();

	//Timer timer;
	//timer.Initialize();
	//wchar_t wstr[256];

	RAWINPUTDEVICE device;
	device.usUsagePage = 0x01;
	device.usUsage = 0x02;
	device.dwFlags = 0;
	device.hwndTarget = 0;
	RegisterRawInputDevices(&device, 1, sizeof device);

	MSG msg = { 0 };
	bool handled = false;


	double tick = 0;
	//SetCursorPos(0, 0);
	RECT wRect;
	int centerX, centerY;
	GetClientRect(window.hwnd, &wRect);
	centerX = (wRect.left + wRect.right) / 2;
	centerY = (wRect.top + wRect.bottom) / 2;

	POINT p2 = { centerX ,centerY};
	ClientToScreen(window.hwnd, &p2);
	SetCursorPos(p2.x, p2.y);
	ShowCursor(false);
	int time = SetTimer(window.hwnd, 1, 15, Timer15ms);
	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE | PM_NOYIELD))
		{
			handled = false;

			if (!handled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		//_itow_s(timer.GetTimeGap(), wstr, 128, 10);
		//da.QuickDrawText(wstr);

		engine.Frame();
		//vector<byte> by = d.resourceList.GetResourceData(tilebuffer, NULL, NULL, NULL);
	}
}
