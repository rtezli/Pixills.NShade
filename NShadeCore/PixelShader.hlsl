struct PixelShaderInput
{
	float4 vertexPosition	: SV_POSITION;
	float4 vertexColor		: COLOR0;
	float4 normal			: NORMAL;
	float4 ambientColor		: COLOR1;
	float4 lightPosition	: POSITION;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };
	float4 ambient = { 0.1, 0.0, 0.0, 1.0 };
	return ambient + diffuse * saturate(dot(input.lightPosition, input.normal));
	//return input.vertexColor;
}
