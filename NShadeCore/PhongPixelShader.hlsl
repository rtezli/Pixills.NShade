struct PixelShaderInput
{
	float4 position			: SV_POSITION;
	float4 color			: COLOR0;
	float4 normal			: NORMAL;

	float4 ambient			: COLOR1;
	float4 light			: POSITION1;
	float4 eye				: POSITION2;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float  lightIntensity = 0.9f;
	float  specularPower = 8.0f;

	float4 normal = normalize(input.normal);
	float4 light = normalize(input.light);
	float4 eye = normalize(input.eye);

	float4 diffuse = lightIntensity * saturate(dot(normal, light));
	float4 reflect = normalize(2 * diffuse * normal - light);

	float4 specular = pow(saturate(dot(reflect, eye)), specularPower);
	
	return input.color + diffuse + specular;
}
