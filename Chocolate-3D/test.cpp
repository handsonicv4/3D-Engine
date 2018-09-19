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
void CALLBACK Timer15ms(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	//Camera Move
	float movingSpeed = 0.2;
	aiVector3D move;
	k1->SpinYaw(0.8);
	k3->SpinPitch(1.2);

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

	int il = PipeLine::InputLayout().Create(workingFolder + "Shaders\\DirectLightVS.hlsl", "main");
	PipeLine::InputLayout().Activate(il);

	DepthStencilDesc dsd;
	dsd = FileLoader::LoadDepthStencilDesc(workingFolder + "Effects\\States\\DSS_Default.json");
	int dssDefault = PipeLine::DepthStencilState().Create(dsd);
	dsd = FileLoader::LoadDepthStencilDesc(workingFolder + "Effects\\States\\DSS_Read.json");
	int dssRead = PipeLine::DepthStencilState().Create(dsd);
	dsd = FileLoader::LoadDepthStencilDesc(workingFolder + "Effects\\States\\DSS_Disable.json");
	int dssDisable = PipeLine::DepthStencilState().Create(dsd);

	BlendDesc bd;
	bd = FileLoader::LoadBlendDesc(workingFolder + "Effects\\States\\BS_Disable.json");
	int bsDisable = PipeLine::BlendState().Create(bd);

	RasterizerDesc rd = FileLoader::LoadRasterizerDesc(workingFolder + "Effects\\States\\RS_Default.json");
	int rsDefault = PipeLine::RasterizorState().Create(rd);
	rd = FileLoader::LoadRasterizerDesc(workingFolder + "Effects\\States\\RS_No_Cull.json");
	int rsNoCull = PipeLine::RasterizorState().Create(rd);

	ViewPortDesc vpd = FileLoader::LoadViewPort(workingFolder + "Effects\\States\\VP_Default.json");
	int vpDefault = PipeLine::ViewPort().Create(vpd);
	vpd = FileLoader::LoadViewPort(workingFolder + "Effects\\States\\VP_64.json");
	int vp64 = PipeLine::ViewPort().Create(vpd);

	SamplerDesc sd = FileLoader::LoadSamplerDesc(workingFolder + "Effects\\States\\SS_Default.json");
	int def = PipeLine::SamplerState().Create(sd);
	PipeLine::SamplerState().Apply(def, 0xffff, Slot_Sampler_Default);
	sd = FileLoader::LoadSamplerDesc(workingFolder + "Effects\\States\\SS_Clamp.json");
	int clamp = PipeLine::SamplerState().Create(sd);
	PipeLine::SamplerState().Apply(clamp, 0xffff, Slot_Sampler_Clamp);

	ResourceDesc voxelDesc = FileLoader::LoadResourceDesc(workingFolder + "Effects\\Resources\\voxel.json");
	int vtex = PipeLine::Resources().Create(voxelDesc);

	BindingRule voxelTexture;
	voxelTexture.resourceID = vtex;
	float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	PipeLine::Resources().Reset(voxelTexture.resourceID, black);
	voxelTexture.flag = Bind_Unordered_Access;
	voxelTexture.stages = Stage_Output_Merge;
	voxelTexture.slot = 4;

	Pass directLight;
	directLight.vertexShaderID = PipeLine::VertexShader().Create(workingFolder + "Shaders\\DirectLightVS.hlsl", "main");
	directLight.pixelShaderID = PipeLine::PixelShader().Create(workingFolder + "Shaders\\DirectLightPS.hlsl", "main");
	directLight.bendStateID = bsDisable;
	directLight.depthStencilStateID = dssRead;
	directLight.rasterizorStateID = rsDefault;
	directLight.viewPortID = vpDefault;

	Pass preZ;
	preZ.vertexShaderID= PipeLine::VertexShader().Create(workingFolder + "Shaders\\PreZ\\VertexShader.hlsl", "main");
	preZ.pixelShaderID = PipeLine::PixelShader().Create(workingFolder + "Shaders\\PreZ\\PixelShader.hlsl", "main");
	preZ.bendStateID = bsDisable;
	preZ.depthStencilStateID = dssDefault;
	preZ.rasterizorStateID = rsDefault;
	preZ.viewPortID = vpDefault;

	Pass voxelization;
	voxelization.vertexShaderID = PipeLine::VertexShader().Create(workingFolder + "Shaders\\VoxelizationVS.hlsl", "main");
	voxelization.pixelShaderID= PipeLine::PixelShader().Create(workingFolder + "Shaders\\VoxelizationPS.hlsl", "main");
	voxelization.geometryShaderID= PipeLine::GeometryShader().Create(workingFolder + "Shaders\\VoxelizationGS.hlsl", "main");
	voxelization.resourceBinding.push_back(voxelTexture);
	voxelization.bendStateID = bsDisable;
	voxelization.depthStencilStateID = dssDisable;
	voxelization.rasterizorStateID = rsNoCull;
	voxelization.viewPortID = vp64;

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

	BindingRule voxelT;
	voxelT.flag = Bind_Shader_Resource;
	voxelT.stages = Stage_Pixel_Shader;
	voxelT.slot = Slot_Texture_Voxel;
	voxelT.resourceID = vtex;

	Pass coneTracing;
	coneTracing.vertexShaderID = directLight.vertexShaderID;
	coneTracing.pixelShaderID= PipeLine::PixelShader().Create(workingFolder + "Shaders\\\ConeTracingPS.hlsl", "main");
	coneTracing.resourceBinding.push_back(voxelT);
	coneTracing.bendStateID = bsDisable;
	coneTracing.depthStencilStateID = dssRead;
	coneTracing.rasterizorStateID = rsDefault;
	coneTracing.viewPortID = vpDefault;



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

	Model amodel, bmodel;
	bmodel.LoadFileD3D("E:\\bob_lamp_update\\bob_lamp_update.md5mesh");
	//amodel.LoadFileD3D("E:\\Wolf\\Wolf.fbx");
	amodel.LoadFileD3D(workingFolder+"Models\\TestModel.fbx");

	//bmodel.LoadFileD3D("E:\\3d121007ms\\ħ������ģ�ͤ� arcticcondor\\Creature\\Arcticcondor\\arcticcondor.obj");

	int mid = engine.LoadModel(amodel);
	engine.LoadModel(bmodel);
	//amodel.Purge();
	Instance *in1 = bmodel.CreateInstance();
	in1->SetScaling(0.7);
	in1->SpinPitch(-89);
	in1->SetPosition(1,-4.5,-1);
	in1->useInstanceMaterial = true;
	in1->material.opacity = 0.1;
	in1->material.refractiveIndex = 1.5;
	in1->material.textureEnableFlags = 0x00;
	in1->material.specularHardness = 30;
	in1->material.specularPower = 3;
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
	instants.push_back(in1);

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

	k2->SetPosition(2.0, -3.5, 3.5);
	k2->SetScaling(2);
	k2->useInstanceMaterial = true;
	k2->material.diffusePower = 0.1;

	k2->material.emissivity = 0.9;
	k2->material.emissiveColor[0] = 0.4;
	k2->material.emissiveColor[1] = 0.9;
	k2->material.emissiveColor[2] = 0.4;

	k3->SetPosition(-2, 0, -2);
	k3->SetScaling(2);
	k3->verticalLock = false;
	k3->SpinRoll(45);
	k3->SpinPitch(45);
	k3->useInstanceMaterial = true;
	k3->material.specularPower = 4;
	k3->material.diffusePower = 0.15;
	k3->material.specularHardness = 30;


	k1->SetPosition(2, 0.5, -1);
	k1->SetScaling(1.5);
	k1->verticalLock = false;
	k1->SpinRoll(45);
	k1->SpinPitch(45);
	k1->useInstanceMaterial = true;
	k1->material.opacity = 0.1;
	k1->material.refractiveIndex = 1.5;
	k1->material.specularPower = 2;
	k1->material.specularHardness = 30;
	k1->material.specularColor[0] = 0.2;
	k1->material.specularColor[1] = 0.2;
	k1->material.specularColor[2] = 0.8;

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

		float color[] = { 0,0,0.5,0 };
		PipeLine::Resources().Reset(PipeLine::Resources().GetBackBuffer(), color);
		PipeLine::Resources().ResetDSV(engine.depthStencilBufferID, D3D11_CLEAR_DEPTH, 1.0f, 0);
		engine.UpdateFrameBuffer();
		engine.UpdateLightBuffer();


		if (gi)
		{
			in->visable = true;
			//in2->visable = true;
			//pW1->visable = true;
			//pW2->visable = true;
			//pR->visable = true;
			//pB->visable = true;
			pv->visable = false;

			PipeLine::Resources().Reset(vtex, black);
			engine.Render(voxelization);
			PipeLine::Resources().GenerateMipMap(vtex);

			engine.Render(preZ);

			if (false)
			{
				PipeLine::Resources().ResetDSV(engine.depthStencilBufferID, D3D11_CLEAR_DEPTH, 1.0f, 0);
				in->visable = false;
				//in2->visable = false;
				//pW1->visable = false;
				//pW2->visable = false;
				//pR->visable = false;
				//pB->visable = false;
				pv->visable = true;
				//engine.Render(visualization);
			}
			else
			{
				engine.Render(coneTracing);
			}
		}
		else
		{
			engine.Render(preZ);
			engine.Render(directLight);
		}
		PipeLine::Swap();
	}
}
