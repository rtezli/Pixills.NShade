#define PI acos(-1)

struct PixelShaderInput
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

float4 main(PixelShaderInput input) : SV_TARGET
{
	float  lightIntensity = 0.45f;
	float  specularPower = 70.00f;

	float4 normal = normalize(input.Normal);
	float4 light = normalize(input.PointPosition);
	float4 camera = normalize(float4(input.CameraPosition, 0.0f));

	float4 ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	float4 diffuse = saturate(dot(normal, light));
	float4 reflect = normalize(2 * diffuse * normal - light);

	float4 specular = saturate(lightIntensity * pow(saturate(dot(reflect, camera)), specularPower));
	
	return (ambient + input.Color + diffuse + specular) * lightIntensity;
}
