cbuffer ConstantBufferData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};


struct VertexShaderInput
{
	float3 vertexPosition		: POSITION0;
	float4 vertexColor			: COLOR0;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD;
	float4 ambientColor			: COLOR1;
	float4 lightPosition		: POSITION1;
};

struct VertexShaderOutput
{
	float4 vertexPosition	: SV_POSITION;
	float4 vertexColor		: COLOR0;
	float3 normal			: NORMAL;
	float4 ambientColor		: COLOR1;
	float4 lightPosition	: POSITION1;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput vertexShaderOutput;
	float4 position = float4(input.vertexPosition, 1.0f);

	position = mul(position, world);
	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.vertexPosition = position;
	vertexShaderOutput.vertexColor = input.vertexColor;
	vertexShaderOutput.normal = mul(input.normal, world);
	vertexShaderOutput.ambientColor = input.ambientColor;
	vertexShaderOutput.lightPosition = input.lightPosition;

	return vertexShaderOutput;
}
