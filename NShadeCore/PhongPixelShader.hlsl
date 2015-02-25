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
	float  specularPower  = 0.1f;
	float  lightIntensity = 0.1f;//input.light.w;
	float4 diffuse		  = lightIntensity * dot(input.light, input.normal);
	float4 specular		  = lightIntensity * dot(input.light, diffuse) * specularPower;
	return input.color    * (diffuse + specular);
}
