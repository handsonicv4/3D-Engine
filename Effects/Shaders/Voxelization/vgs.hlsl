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

[maxvertexcount(4)]
void main(point VGSinput input[1], inout TriangleStream<VPSinput> outStream)
{
	VPSinput output = (VPSinput)0;

	float4 color = voxels.Load(uint4(input[0].tex, 0));
	if (color.w>0)
	{
		//input[0].position = mul(input[0].position, input[0].wvp);

		float4 bbl, bbr, btl, btr, fbl, fbr, ftl, ftr;


		fbl = input[0].position - float4(0.5*g_VoxelSize, 0);
		fbr = fbl;
		fbr.x += g_VoxelSize.x;
		ftl = fbl;
		ftl.y += g_VoxelSize.y;
		ftr = ftl;
		ftr.x += g_VoxelSize.x;

		fbl= mul(fbl, input[0].wvp);
		fbr = mul(fbr, input[0].wvp);
		ftl = mul(ftl, input[0].wvp);
		ftr = mul(ftr, input[0].wvp);

		output.position = fbl;
		output.color = color;
		outStream.Append(output);

		output.position = ftl;
		outStream.Append(output);

		output.position = fbr;
		outStream.Append(output);

		output.position = ftr;
		outStream.Append(output);

		outStream.RestartStrip();

	}

}
