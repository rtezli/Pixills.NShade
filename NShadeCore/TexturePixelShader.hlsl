Texture2D    shaderTexture;
SamplerState samplerState;

struct PixelShaderInput
{
    float4 position     : SV_POSITION;
    float2 uv           : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return shaderTexture.Sample(samplerState, input.uv);
}