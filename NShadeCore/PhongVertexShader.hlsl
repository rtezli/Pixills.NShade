cbuffer CameraConstBuffer	: register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

float3 CameraPosition		: register(b1);
float4 PointLight			: register(b2);
float4 AmbientLight			: register(b3);

struct VertexShaderInput
{
	float3 Position			: POSITION0;
	float4 Color			: COLOR0;
	float4 Normal			: NORMAL;
	float2 Uv				: TEXCOORD;
};

struct VertexShaderOutput
{
	float4 Position			: SV_POSITION;
	float4 Color			: COLOR0;
	float4 Normal			: NORMAL;

	float3 CameraPosition	: POSITION1;

	float3 AmbientColor		: COLOR1;
	float  AmbientIntensity : PSIZE0;
	float4 PointPosition	: POSITION2;
	float  PointIntensity	: PSIZE1;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput vertexShaderOutput;
	float4 position = float4(input.Position, 0.0f);

	position = mul(position, world);
	position = mul(position, view);
	position = mul(position, projection);

	vertexShaderOutput.Position = position;
	vertexShaderOutput.Color = input.Color;

	vertexShaderOutput.Normal = mul(input.Normal, world);
	vertexShaderOutput.CameraPosition = CameraPosition;

	vertexShaderOutput.AmbientColor = float3(AmbientLight.x, AmbientLight.y, AmbientLight.z);
	vertexShaderOutput.AmbientIntensity = AmbientLight.w;
	vertexShaderOutput.PointPosition = mul(float4(PointLight.x, PointLight.y, PointLight.z, 0.0f), world);
	vertexShaderOutput.PointIntensity = PointLight.w;

	return vertexShaderOutput;
}
