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
Instance *k1;
Instance *k3;
Instance *dragon;
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


int flag = 0;
float speed = 0.2;
void CALLBACK Timer15ms(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	//Camera Move
	float movingSpeed = 0.2;
	aiVector3D move;
	k1->SpinYaw(0.8);
	k3->SpinPitch(1.2);
	dragon->SpinYaw(0.8);

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


	//Pass visualization;
	//visualization.vertexShaderID = engine.vertexShader.Create(workingFolder + "Shaders\\voxelization\\vis.hlsl", "main");
	//visualization.geometryShaderID = engine.geometryShader.Create(workingFolder + "Shaders\\voxelization\\vgs.hlsl", "main");
	//visualization.pixelShaderID = engine.pixelShader.Create(workingFolder + "Shaders\\voxelization\\vps.hlsl", "main");
	//visualization.rasterizer.CullMode = D3D11_CULL_NONE;
	//visualization.blend.RenderTarget[0].BlendEnable = true;
	//visualization.viewPort.Height = (float)engine.resolutionY;
	//visualization.viewPort.Width = (float)engine.resolutionX;
	//visualization.topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	//ResourceBind voxelView;
	//voxelView.flag = Bind_Shader_Resource;
	//voxelView.stages = Stage_Geometry_Shader;
	//voxelView.slot = 0;
	//voxelView.resourceID = vtex;
	//visualization.bindingTable.push_back(voxelView);



	vector<aiVector3D> vo;
	vo.resize(64 * 64 * 64);
	vector<unsigned int> ind;
	ind.resize(64 * 64 * 64);
	UINT index = 0;
	float stride = 10.0 / 64;
	for (unsigned int i = 0; i < 64; i++)
	{
		for (unsigned int j = 0; j < 64; j++)
		{
			for (unsigned int z = 0; z < 64; z++)
			{
				vo[index].x = (i+0.5)*stride;
				vo[index].y = (j + 0.5)*stride;
				vo[index].z = (z + 0.5)*stride;

				ind[index] = index;
				index++;
			}
		}
	}

	Model vox;
	Mesh temp;
	temp.vertexPositions = vo;
	temp.indices = ind;

	vox.meshList.push_back(temp);
	int vid= engine.LoadModel(vox);
	Instance *pv = vox.CreateInstance();
	pv->visable = false;
	//--------------------------------------------------------------------------------

	engine.camera.SetPosition(0, 0, -8);
	engine.camera.FaceTo(aiVector3D(0, -0.2, 0.8));


	Light x;
	x.color[0] = 1.0f;
	x.color[1] = 1.0f;
	x.color[2] = 1.0f;
	x.color[3] = 1.0f;
	x.direction[0] = 0.0f;
	x.direction[1] = 4.0f;
	x.direction[2] = 0.0f;
	engine.lightList.push_back(x);

	Model amodel, bmodel, dmodel;
	//bmodel.LoadFileD3D(workingFolder + "Models\\bob_lamp_update.md5mesh");
	//amodel.LoadFileD3D("E:\\Wolf\\Wolf.fbx");
	amodel.LoadFileD3D(workingFolder+"Models\\TestModel.fbx");
	dmodel.LoadFileD3D(workingFolder+"Models\\dragon.obj");

	//bmodel.LoadFileD3D("E:\\3d121007ms\\魔兽世界模型の arcticcondor\\Creature\\Arcticcondor\\arcticcondor.obj");

	int mid = engine.LoadModel(amodel);
	engine.LoadModel(bmodel);
	engine.LoadModel(dmodel);

	Instance* dr = dmodel.CreateInstance();
	dr->SetScaling(0.3);
	dr->useInstanceMaterial = true;
	dr->SetPosition(0, -4.5, -1.5);
	dr->material.refractiveIndex = 1.5;
	dr->material.textureEnableFlags = 0x00;
	dr->material.specularHardness = 30;
	dr->material.specularPower = 3;
	dr->material.opacity = 0.1;
	dragon = dr;

	//amodel.Purge();
	//Instance *in1 = bmodel.CreateInstance();
	//in1->SetScaling(0.7);
	//in1->SpinPitch(-89);
	//in1->SetPosition(1,-4.5,-1);
	//in1->useInstanceMaterial = true;
	//in1->material.opacity = 0.1;
	//in1->material.refractiveIndex = 1.5;
	//in1->material.textureEnableFlags = 0x00;
	//in1->material.specularHardness = 30;
	//in1->material.specularPower = 3;

	Instance *in = amodel.CreateInstance();
	in->animationID = 0;
	in->animationTime = 0;
	//in->SpinPitch(-89);
	//in->SpinYaw(180);
	in->SetScaling(0.7);
	in->SetPosition(0,0, 4.5);
	in->useInstanceMaterial = false;
	in->material.textureEnableFlags = 0x04;
	in->material.diffuseColor[0] = 0.5;
	in->material.diffuseColor[1] = 1;
	in->material.diffuseColor[2] = 0.5;
	in->material.diffusePower = 0.1;
	in->material.specularHardness = 35;
	in->material.specularPower = 3;
	in->material.emissivity = 1;
	in->material.opacity = 1;
	in->material.refractiveIndex = 1;
	in->material.emissiveColor[0] = 0.9;
	in->material.emissiveColor[1] = 0.9;
	in->material.emissiveColor[2] = 0.9;

	instants.push_back(in);
	//instants.push_back(in1);

	Model rect,box;
	rect.LoadFileD3D(workingFolder + "Models\\Rect.obj");
	engine.LoadModel(rect);
	Instance *r1 = rect.CreateInstance();
	Instance *r2 = rect.CreateInstance();
	Instance *r3 = rect.CreateInstance();
	Instance *r4 = rect.CreateInstance();
	Instance *r5 = rect.CreateInstance();
	r1->SetScaling(9);
	r2->SetScaling(9);
	r3->SetScaling(9);
	r4->SetScaling(9);
	r5->SetScaling(9);
	r1->verticalLock = false;
	r2->verticalLock = false;
	r3->verticalLock = false;
	r4->verticalLock = false;
	r5->verticalLock = false;

	r1->SetPosition(0, 0, 4.5);
	r2->SpinPitch(-90);
	r2->SetPosition(0, -4.5, 0);
	r3->SpinYaw(90);
	r3->SetPosition(-4.5, 0, 0);
	r4->SpinYaw(-90);
	r4->SetPosition(4.5, 0, 0);
	r5->SpinPitch(90);
	r5->SetPosition(0, 4.5, 0);
	r3->useInstanceMaterial = true;
	r3->material.diffuseColor[0] = 1;
	r3->material.diffuseColor[1] = 0.4;
	r3->material.diffuseColor[2] = 0.4;
	r4->useInstanceMaterial = true;
	r4->material.diffuseColor[0] = 0.4;
	r4->material.diffuseColor[1] = 0.4;
	r4->material.diffuseColor[2] = 1;
	r4->material.specularHardness = 30;
	r4->material.specularPower = 2;
	r4->material.diffusePower = 0.2;

	box.LoadFileD3D(workingFolder + "Models\\Cube.obj");
	engine.LoadModel(box);
	Instance *k = box.CreateInstance();
	Instance *k2 = box.CreateInstance();
	k3 = box.CreateInstance();
	k1 = box.CreateInstance();
	//Instance *k4 = box.CreateInstance();

	k->SetPosition(-3.5, -3.5, 3.5);
	k->SetScaling(2);
	k->useInstanceMaterial = true;
	k->material.diffuseColor[0] = 0.7;
	k->material.diffuseColor[1] = 0.3;
	k->material.diffuseColor[2] = 0;
	k->material.specularPower = 1;
	k->material.specularHardness = 5;

	k2->SetPosition(2.0, -3.5, 3.5);
	k2->SetScaling(1.3);
	k2->useInstanceMaterial = true;
	k2->material.diffusePower = 0.1;
	k2->material.emissivity = 0.9;
	k2->material.emissiveColor[0] = 0.4;
	k2->material.emissiveColor[1] = 0.9;
	k2->material.emissiveColor[2] = 0.4;

	k3->SetPosition(-2, 0, -0.5);
	k3->SetScaling(1.5);
	k3->verticalLock = false;
	k3->SpinRoll(45);
	k3->SpinPitch(45);
	k3->useInstanceMaterial = true;
	k3->material.specularPower = 4;
	k3->material.diffusePower = 0.15;
	k3->material.specularHardness = 30;


	k1->SetPosition(-3.2, -3.5, -2);
	k1->SetScaling(1.3);
	k1->verticalLock = false;
	k1->SpinRoll(45);
	k1->SpinPitch(45);
	k1->useInstanceMaterial = true;
	k1->material.diffusePower = 0.1;
	k1->material.emissivity = 0.9;
	k1->material.emissiveColor[0] = 0.6;
	k1->material.emissiveColor[1] = 0.6;
	k1->material.emissiveColor[2] = 0.9;
	k1->material.specularPower = 1;
	k1->material.specularHardness = 10;


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
	Camera lc;
	lc.verticalLock = false;
	lc.SetPosition(engine.lightList[0].direction[0], engine.lightList[0].direction[1] + 2, engine.lightList[0].direction[2]);
	lc.FaceTo(aiVector3D(0, -1, 0));
	lc.fovY = 120;
	lc.screenAspect = 1;
	lc.zNear = 0.1f;
	lc.zFar = 1000;
	
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
		engine.camera = lc;
		engine.UpdateFrameBuffer();
		engine.UpdateLightBuffer();
		engine.Render("depth");
		engine.camera = cam;
		engine.UpdateFrameBuffer();
		engine.Render("voxel_cone_tracing");

		PipeLine::Swap();
	}
}
