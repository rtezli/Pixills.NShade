struct PixelShaderInput
{
	float4 Position	: SV_POSITION;
	float4 Color	: COLOR0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return input.Color;
}
