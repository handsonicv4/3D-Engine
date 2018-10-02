#include "../ConstantBuffers.hlsl"
#include "TypeDef.hlsl"

Texture3D voxels :register(t0);

struct VGSinput
{
    float4 position : SV_POSITION;
	float3 tex : TEXCOORD0;
	matrix wvp : TEXCOORD1;
};

struct VPSinput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

[maxvertexcount(1)]
void main(point VGSinput input[1], inout PointStream<VPSinput> outStream)
{

	VPSinput output = (VPSinput)0;

	float4 color = voxels.Load(uint4(input[0].tex, 0));
	if (color.w>0)
	{
		output.position = input[0].position;
		output.color = color;
		outStream.Append(output);
	}
	outStream.RestartStrip();
}
