struct PixelShaderInput
{
	float4 vertexPosition	: SV_POSITION;
	float4 vertexColor		: COLOR0;
	float4 normal			: NORMAL;

	float4 ambient			: COLOR1;		// Color and Intensity
	float  reflective		: COLOR2;		// Color and Intensity
	float4 light			: POSITION1;	// Position and Intensity
	float4 eye 				: POSITION2;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };

	float4 lPosition = float4(input.light.x, input.light.y, input.light.z, 0.0f);
	float  lIntensity = saturate(input.light.w);

	float4	aColor = float4(input.ambient.x, input.ambient.y, input.ambient.z, 0.0f);
	float	aIntensity = saturate(input.light.w);

	float	lDiffuse = lIntensity * dot(lPosition, input.normal);
	float	lSpecular = input.reflective * dot(input.eye, input.normal);

	return (input.vertexColor + aColor * aIntensity) * saturate(lDiffuse + lSpecular);
}
