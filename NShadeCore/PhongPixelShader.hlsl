struct PixelShaderInput
{
	float4 vertexPosition	: SV_POSITION;
	float4 vertexColor		: COLOR0;
	float4 normal			: NORMAL;
	float4 ambientColor		: COLOR1;
	float4 lightPosition	: POSITION1;
	float4 eyePosition		: POSITION2;
};

float4 main(PixelShaderInput input) : SV_TARGET
{ 
	float4 diffuse  = saturate(dot(input.lightPosition, input.normal));
	float4 specular = saturate(dot(input.eyePosition, input.normal));
	return input.vertexColor * (diffuse + specular) + input.ambientColor;
}
