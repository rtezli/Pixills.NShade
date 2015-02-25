cbuffer ConstantBufferData : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct VertexShaderInput
{
	float3 Position		: POSITION0;
	float4 Color		: COLOR0;
};

struct VertexShaderOutput
{
	float4 Position	: SV_POSITION;
	float4 Color	: COLOR0;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput vertexShaderOutput;
	float4 position = float4(input.Position, 1.0f);

	position = mul(position, world);
	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.Position = position;
	vertexShaderOutput.Color = input.Color;

	return vertexShaderOutput;
}
