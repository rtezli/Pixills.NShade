cbuffer CameraConstBuffer	: register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
};

cbuffer Camera				: register(b1)
{
	float4 CameraPosition;
}

cbuffer PointLight			: register(b2)
{
	float4 PointLightPosition;
}

cbuffer AmbientLight		: register(b3)
{
	float4 AmbientLightColor;
}

struct VertexShaderInput
{
	float3 Position			: POSITION0;
	float4 Color			: COLOR0;
	float4 Normal			: NORMAL;
	float2 Uv				: TEXCOORD;
	float4 PolyPosition		: POSITION1;
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

	position = mul(position, World);
	position = mul(position, View);
	position = mul(position, Projection);

	vertexShaderOutput.Position = position;
	vertexShaderOutput.Color = input.Color;

	vertexShaderOutput.Normal = mul(input.Normal, World);
	vertexShaderOutput.CameraPosition = CameraPosition;

	vertexShaderOutput.AmbientColor = float3(AmbientLightColor.x, AmbientLightColor.y, AmbientLightColor.z);
	vertexShaderOutput.AmbientIntensity = AmbientLightColor.w;
	vertexShaderOutput.PointPosition = mul(float4(PointLightPosition.x, PointLightPosition.y, PointLightPosition.z, 0.0f), World);
	vertexShaderOutput.PointIntensity = PointLightPosition.w;

	return vertexShaderOutput;
}
