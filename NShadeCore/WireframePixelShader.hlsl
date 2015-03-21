struct PixelShaderInput
{
    float4 vertexPosition   : SV_POSITION;
    float4 vertexColor      : COLOR0;
    float4 normal           : NORMAL;
    float  polyposition     : POSITION1;
    float4 ambientColor     : COLOR1;
    float4 lightPosition    : POSITION2;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };
    return input.vertexColor * saturate(dot(input.lightPosition, input.normal));
}
