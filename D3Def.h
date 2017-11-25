#pragma once
//#include<Windows.h>


#define MAX_SHADER_NUMBER 256
#define MAX_SLOT_NUMBER 64
#define MAX_STAGE_NUMBER 10
#define MAX_BINDING_TYPE_NUMBER 10

enum InputSlotDef
{
	PositionSlot = 0,
	ColorSlot = 1,
	NormalSlot = 2,
	TexCoordSlot = 3,
	PSizeSlot = 4,
	BinormalSlot = 5,
	TangentSlot = 6,
	PositionTSlot = 7,
	BlendIndicesSlot = 8,
	BlendWeightSlot = 9,
	InstanceIDSlot = 10,
	OtherSlot = 15
	
};
enum ConstantBufferSlotDef
{

	Constant_Buffer_PerObject_Slot = 0,
	Constant_Buffer_PerFrame_Slot = 1,
	Constant_Buffer_PerMesh_Slot = 2,
};
enum TextureSlotDef
{
	Texture_Diffuse_Slot = 0,
	Texture_Ambient_Slot = 1,
	Texture_Normal_Slot = 2,
	Texture_Specular_Slot = 3,
	Texture_Depth_Slot = 4,
	BindMatrix_Slot = 5,
	Instance_Slot=6,
	Texture_Light_Array_Slot = 7
};
enum SamplerSlotDef
{
	Sampler_Dfault_Slot= 0,
};
enum PiplineStage
{
	Input_Assembler = 0x1L,
	Vertex_Shader = 0x2L,
	Hull_Shader = 0x4L,
	Domain_Shader = 0x8L,
	Geometry_Shader = 0x10L,
	Stream_Out = 0x20L,
	Rasterizer_Stage = 0x40L,
	Pixel_Shader = 0x80L,
	Output_Merge = 0x100L,
	Compute_Shader=0x200L,
};


