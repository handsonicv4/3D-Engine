#include "ConstantBuffers.hlsl"
#include "Input.hlsl"

[maxvertexcount(3)]
void main(triangle PSinput input[3], inout TriangleStream<PSinput> outStream)
{

	PSinput output = (PSinput)0;

	//Compute triangle normal
	float3 v1 = (input[1].positionWorld - input[0].positionWorld).xyz;
	float3 v2 = (input[2].positionWorld - input[0].positionWorld).xyz;
	float3 t_n = abs(cross(v1, v2));

	//Find dominant axis 
	uint axis_flag = 2;
	if (t_n.x > t_n.y&&t_n.x > t_n.z)
	{
		axis_flag = 0;
	}
	else if (t_n.y > t_n.x&&t_n.y > t_n.z)
	{
		axis_flag = 1;
	}
	//else axis_flag = 2;

	//Do vertex projection and full PSinput;
	for (uint i = 0; i < 3; i++)
	{
		output.positionWorld = input[i].positionWorld;
		output.normal = input[i].normal;
		output.tangent = input[i].tangent;
		output.bitangent = input[i].bitangent;
		output.tex = input[i].tex;
		output.instanceMaterialID = input[i].instanceMaterialID;
		if (axis_flag == 0)//X
		{
			output.position = float4(input[i].position.y, input[i].position.z, 0, 1);
		}
		else if (axis_flag == 1)//Y
		{
			output.position = float4(input[i].position.x, input[i].position.z, 0, 1);
		}
		else//Z
		{
			output.position = float4(input[i].position.x, input[i].position.y, 0, 1);
		}
		outStream.Append(output);
	}
	outStream.RestartStrip();
}
