cbuffer ConstantBufferData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};


struct VertexShaderInput
{
	float3 position : VERT_POSITION;
	float3 normal : VERT_NORMAL;
	float4 vertexColor : VERT_COLOR;
	float3 ambientLightColor : AMB_COLOR;
	matrix lightPoistion : LIGHT_POSITION;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput vertexShaderOutput;
	float4 position = float4(input.position, 1.0f);

    position = mul(position, world);
	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.pos = position;
	vertexShaderOutput.color = input.vertexColor;

	return vertexShaderOutput;
}
