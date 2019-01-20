#include"GEngine.h"
#include"Simple_window.h"
#include"pipeline/DescFileLoader.h"
using namespace std;
#include "Shlwapi.h" //file name
#pragma comment(lib, "Shlwapi.lib" )
#include <codecvt>
#include <sstream>


bool v = false;
bool gi = true;
bool c = false;
int counter = 0;
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
			//engine.camera.SpinYaw(-0.6f*xPosRelative);
			//engine.camera.SpinPitch(-0.6f*yPosRelative );
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
			//engine.camera.LookAt(aiVector3D(0, 0, 1));
			instants[0]->useInstanceMaterial = !instants[0]->useInstanceMaterial;
		}
		if (wparam == 'Z')
		{
			if (instants[1]->material.opacity < 1)
			{
				instants[1]->material.opacity = 1;
			}
			else
			{
				instants[1]->material.opacity = 0.1;
			}
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

		if (wparam == '1')
			instants[0]->MoveFront(0.1);
		if (wparam == '2')
			instants[0]->MoveFront(-0.1);
		if (wparam == '3')
			instants[0]->MoveRight(0.1);
		if (wparam == '4')
			instants[0]->MoveRight(-0.1);
		if (wparam == '5')
			instants[0]->MoveUp(0.1);
		if (wparam == '6')
			instants[0]->MoveUp(-0.1);
		break;
	}
	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);;
	}
	}
}

ModelInstance* testSP;
int flag = 0;
float speed = 0.2;
void CALLBACK Timer15ms(HWND hWnd, UINT nMsg, UINT_PTR nTimerid, DWORD dwTime)
{
	//Camera Move
	float movingSpeed = 0.2;
	aiVector3D move;
	//k1->SpinYaw(0.8);
	//k3->SpinPitch(1.2);
	//dragon->SpinYaw(0.8);

	bool forward = true;
	aiQuaternion a;
	
	//k1->SetRotation(aiQuaternion(aiVector3D(0, 1, 0), 0.8));

	if (moveVec.Length() > 0.0001)
	{
		move = moveVec.Normalize();
		move = move*movingSpeed;
		engine.camera.MoveFront(move.z);
		engine.camera.MoveRight(move.x);
	}
	if (counter >= 80)
	{
		counter = 0;

		flag++;

		
		//v = !v;
		if (v)
		{
			//engine.camera.SetPosition(5, 5, -3);
		}
		else
		{
			//engine.camera.SetPosition(0, 0, -8);
		}
	}
	counter++;
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
		in->animationTime += 0.015;
	}
	testSP->animationTime += 0.015;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	char path[MAX_PATH + 1];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	string workingFolder = path;
	int upper = workingFolder.size() - 1;
	while (upper > 0 && workingFolder[upper] != '\\') upper--;
	workingFolder.resize(upper+1);
	//workingFolder += "\\";
	ArkWindow window("Engine");
	window.lpfnWndProc = WndProc;
	window.Show();
	engine.Init(window.hwnd, false);

	engine.LoadEffect(workingFolder + "Effects\\test.json");

	//--------------------------------------------------------------------------------

	engine.LoadPostMesh(workingFolder + "Models\\fullScreen.obj");

	engine.camera.SetPosition(0, 0, -8);
	engine.camera.FaceTo(aiVector3D(0, -0.2, 0.8));
	auto test = engine.LoadAsset(workingFolder + "Models\\TestModel.fbx");
	auto testi = engine.CreateInstance(test->defaultInstance);
	testi->animationID = 1;
	testi->animationTime = 0;
	testi->transform.SetScaling(0.7);
	testi->transform.SetPosition(0, 0, 3.5);
	//testi->components[1].materialInstance.diffuseBlendFactor = 1;
	testSP = testi;

	auto rect = engine.LoadAsset(workingFolder + "Models\\Rect.obj");
	auto back = engine.CreateInstance(rect->defaultInstance);
	back->transform.verticalLock = false;
	back->transform.SetScaling(9);
	back->transform.SetPosition(0,0,4.5);

	auto down = engine.CreateInstance(rect->defaultInstance);
	down->transform.verticalLock = false;
	down->transform.SetScaling(9);
	down->transform.SpinPitch(-90);
	down->transform.SetPosition(0, -4.5, 0);

	auto up = engine.CreateInstance(rect->defaultInstance);
	up->transform.verticalLock = false;
	up->transform.SetScaling(9);
	up->transform.SpinPitch(90);
	up->transform.SetPosition(0, 4.5, 0);

	auto right = engine.CreateInstance(rect->defaultInstance);
	right->transform.verticalLock = false;
	right->transform.SetScaling(9);
	right->transform.SpinYaw(-90);
	right->transform.SetPosition(4.5, 0, 0);
	right->components[0].materialInstance.specularPower = 2;
	right->components[0].materialInstance.specularBlendFactor = 1;
	right->components[0].materialInstance.specularColor[0] = 1;
	right->components[0].materialInstance.specularColor[1] = 1;
	right->components[0].materialInstance.specularColor[2] = 1;
	right->components[0].materialInstance.specularHardness = 30;
	right->components[0].materialInstance.diffusePower = 0.2;


	auto left = engine.CreateInstance(rect->defaultInstance);
	left->transform.verticalLock = false;
	left->transform.SetScaling(9);
	left->transform.SpinYaw(90);
	left->transform.SetPosition(-4.5, 0, 0);
	left->components[0].materialInstance.diffuseColor[0] = 0.8;
	left->components[0].materialInstance.diffuseColor[1] = 0.2;
	left->components[0].materialInstance.diffuseColor[2] = 0.2;


	auto dragon = engine.LoadAsset(workingFolder + "Models\\dragon.obj");
	auto dragon1 = engine.CreateInstance(dragon->defaultInstance);
	dragon1->transform.SetPosition(0, -4.5, -1);
	dragon1->transform.SetScaling(0.4);
	dragon1->components[0].materialInstance.diffusePower = 0.1;
	dragon1->components[0].materialInstance.refractiveIndex = 1.5;
	dragon1->components[0].materialInstance.opacity = 0.1;
	dragon1->components[0].materialInstance.specularPower = 1.3;
	dragon1->components[0].materialInstance.specularBlendFactor = 1;
	dragon1->components[0].materialInstance.specularColor[0] = 1;
	dragon1->components[0].materialInstance.specularColor[1] = 1;
	dragon1->components[0].materialInstance.specularColor[2] = 1;
	dragon1->components[0].materialInstance.specularHardness = 30;

	auto post = engine.LoadAsset(workingFolder + "Models\\fullScreen.obj");

	Light x;
	x.color[0] = 1.0f;
	x.color[1] = 1.0f;
	x.color[2] = 1.0f;
	x.color[3] = 1.0f;
	x.direction[0] = 0.0f;
	x.direction[1] = 4.0f;
	x.direction[2] = 0.0f;

	Camera lc;
	lc.verticalLock = false;
	lc.SetPosition(x.direction[0], x.direction[1] + 2, x.direction[2]);
	lc.FaceTo(aiVector3D(0, -1, 0));
	lc.fovY = 120;
	lc.screenAspect = 1;
	lc.zNear = 0.1f;
	lc.zFar = 1000;
	lc.UpdateProjectionMatrix();
	aiMatrix4x4 lvp = lc.GetProjectionMatrix()*lc.GetViewMatrix();
	memcpy(&x.vP, &lvp, sizeof(float[16]));

	engine.lightList.push_back(x);


	RAWINPUTDEVICE device;
	device.usUsagePage = 0x01;
	device.usUsage = 0x02;
	device.dwFlags = 0;
	device.hwndTarget = 0;
	RegisterRawInputDevices(&device, 1, sizeof device);

	MSG msg = { 0 };
	bool handled = false;
	//D3D11_PRIMITIVE_POINT

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



	auto cam = engine.camera;
	//Camera lc;
	//lc.verticalLock = false;
	//lc.SetPosition(engine.lightList[0].direction[0], engine.lightList[0].direction[1] + 2, engine.lightList[0].direction[2]);
	//lc.FaceTo(aiVector3D(0, -1, 0));
	//lc.fovY = 120;
	//lc.screenAspect = 1;
	//lc.zNear = 0.1f;
	//lc.zFar = 1000;
	
	lc.UpdateProjectionMatrix();
	
	engine.mainLight = lc;

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
		engine.UpdateFrameBuffer();
		engine.UpdateLightBuffer();

		engine.Render("voxel_cone_tracing");
		//engine.Render("direct_light");
		//engine.Render("depth");
		PipeLine::Swap();
		dragon1->transform.SpinYaw(2);
	}
}
