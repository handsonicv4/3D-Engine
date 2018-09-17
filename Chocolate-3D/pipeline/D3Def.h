#pragma once

#define MAX_SLOT_NUMBER 64

enum InputSlotDef
{
	Slot_Input_Position = 0,
	Slot_Input_Color = 1,
	Slot_Input_Normal = 2,
	Slot_Input_TexCoord = 3,
	Slot_Input_PSize = 4,
	Slot_Input_Binormal = 5,
	Slot_Input_Tangent = 6,
	Slot_Input_PositionT = 7,
	Slot_Input_BlendIndices = 8,
	Slot_Input_BlendWeight = 9,
	Slot_Input_InstanceID = 10,
	Slot_Input_Other = 15
};
enum ConstantBufferSlotDef
{
	Slot_CBuffer_Object = 0,
	Slot_CBuffer_Frame = 1,
	Slot_CBuffer_Mesh = 2,
};
enum TextureSlotDef
{
	Slot_Texture_Diffuse = 0,
	Slot_Texture_Ambient = 1,
	Slot_Texture_Normal = 2,
	Slot_Texture_Specular = 3,
	Slot_Texture_Depth = 4,
	Slot_Texture_AnimMatrix = 5,
	Slot_Texture_Instance = 6,
	Slot_Texture_Light = 7,
	Slot_Texture_Voxel = 8,
};
enum SamplerSlotDef
{
	Slot_Sampler_Default= 0,
	Slot_Sampler_Clamp = 1,
};
enum PiplineStage
{
	Stage_Input_Assembler = 0x1L,
	Stage_Vertex_Shader = 0x2L,
	Stage_Hull_Shader = 0x4L,
	Stage_Domain_Shader = 0x8L,
	Stage_Geometry_Shader = 0x10L,
	Stage_Stream_Out = 0x20L,
	Stage_Rasterizer_Stage = 0x40L,
	Stage_Pixel_Shader = 0x80L,
	Stage_Output_Merge = 0x100L,
	Stage_Compute_Shader=0x200L,
};
enum modelFlag
{
	Apply_Animation = 0x01,
	Apply_Texture_Diffuse = 0x02,
	Apply_Texture_Normal = 0x04,
	Apply_Texture_Specular = 0x08,
	Apply_Texture_Ambient = 0x10,
};

