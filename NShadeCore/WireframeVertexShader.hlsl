cbuffer ConstantBufferData : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
	float4x4 reflection;
	float4   camera;
	float    time;
};

struct VertexShaderInput
{
	float3 position		: POSITION0;
	float4 color		: COLOR0;
	float3 normal		: NORMAL;
};

struct VertexShaderOutput
{
	float4 position			: SV_POSITION;
	float4 color			: COLOR0;
	float4 normal			: NORMAL;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput vertexShaderOutput;

	float4 position = float4(input.position, 1.0f);

	position = mul(position, world);
	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.position = position;
	vertexShaderOutput.normal = mul(input.normal, world);
	vertexShaderOutput.color = input.color;
	
	return vertexShaderOutput;
}
