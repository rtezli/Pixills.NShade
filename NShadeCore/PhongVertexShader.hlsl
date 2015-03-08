cbuffer CameraConstBuffer : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
	float4	 cameraPosition;
};

float4 positionIntensity : register(c0);
float4 colorIntensity : register(c1);

struct VertexShaderInput
{
	float3 position		: POSITION0;
	float4 color		: COLOR0;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct VertexShaderOutput
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR0;
	float4 normal	: NORMAL;

	float4 ambient	: COLOR1;
	float4 light	: POSITION1;
	float4 camera	: POSITION2;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput vertexShaderOutput;
	float4 position = float4(input.position, 1.0f);

	position = mul(position, world);

	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.position = position;
	vertexShaderOutput.color	= input.color;

	float4 normal = float4(input.normal, 0.0f);

	vertexShaderOutput.ambient	= colorIntensity;
	vertexShaderOutput.normal	= mul(normal, world);
	vertexShaderOutput.camera	= cameraPosition;
	vertexShaderOutput.light	= mul(positionIntensity, world);


	return vertexShaderOutput;
}
