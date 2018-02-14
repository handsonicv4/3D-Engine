
struct VSinput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BINORMAL;
	float2 tex : TEXCOORD0;
	uint4 boneID : BLENDINDICES;
	float4 boneWeight : BLENDWEIGHT;
	uint instanceID : SV_InstanceID;

};

struct PSinput
{
	float4 position : SV_POSITION;
	float4 positionWorld : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BINORMAL;
	float2 tex : TEXCOORD0;
	uint instanceMaterialID : TEXCOORD1;
};