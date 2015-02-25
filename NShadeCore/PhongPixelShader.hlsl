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
	float4 diffuse		= dot(input.light, input.normal);

	float  specularity	= 32.0f;
	float  specConst	= (specularity + 2.0f) / (2.0f * 3.14f);
	float4 specular		= specConst * dot(input.eye, diffuse);

	return input.color  * (diffuse + specular);
}
