Texture2D shaderTexture;
SamplerState SampleType;

struct PixelShaderInput
{
    float4 position     : POSITION0;
    float2 uv           : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.uv);
}