cbuffer Camera : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4   camera;
};

struct VertexShaderInput
{
    float3 position : POSITION0;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float3 bary     : POSITION1;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput vertexShaderOutput;
    float4 position = float4(input.position, 1.0f);

    position = mul(position, world);
    position = mul(position, view);
    position = mul(position, projection);

    vertexShaderOutput.position = position;
    vertexShaderOutput.uv = input.uv;

    return vertexShaderOutput;
}