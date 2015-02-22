cbuffer ConstantBufferData : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
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

struct PixelShaderInput
{
	float4 vertexPosition	: SV_POSITION;
	float4 vertexColor		: COLOR0;
	float4 normal			: NORMAL;
	float4 ambientColor		: COLOR1;
	float4 lightPosition	: POSITION1;
};

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput vertexShaderOutput;
	float4 position = float4(input.vertexPosition, 1.0f);

	position = mul(position, world);
	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.vertexPosition = position;
	vertexShaderOutput.vertexColor = input.vertexColor;

	float4 normal = float4(input.normal, 0.0f);

	vertexShaderOutput.normal = mul(normal, world);
	vertexShaderOutput.ambientColor = input.ambientColor;
	vertexShaderOutput.lightPosition = mul(input.lightPosition, world);

	return vertexShaderOutput;
}
