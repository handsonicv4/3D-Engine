#pragma once

#define MAX_SHADER_NUMBER 256
#define MAX_SLOT_NUMBER 64
#define MAX_STAGE_NUMBER 10
#define MAX_BINDING_TYPE_NUMBER 10

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
	Slot_Texture_Instance=6,
	Slot_Texture_Light = 7
};
enum SamplerSlotDef
{
	Slot_Sampler_Default= 0,
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

enum BindFlag
{
	Bind_Vertex_Buffer = 0x1L,
	Bind_Index_Buffer = 0x2L,
	Bind_Constant_Buffer = 0x4L,
	Bind_Shader_Resource = 0x8L,
	Bind_Stream_Out = 0x10L,
	Bind_Render_Target = 0x20L,
	Bind_Depth_Stencil = 0x40L,
	Bind_Unordered_Access = 0x80L,
	Bind_Decoder = 0x200L,
	Bind_Video_Encoder = 0x400L
};

