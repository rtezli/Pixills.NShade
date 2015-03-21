cbuffer Camera : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4   camera;
};

cbuffer Material : register(b1)
{
    float4 materialColor;
}

cbuffer Ambient : register(b2)
{
    float4 ambientColorIntensity;
}

cbuffer PointLight : register(b3)
{
    float pointLightPosition;
}

cbuffer Global : register(b4)
{
    float globalTime;
}

struct VertexShaderInput
{
    float3 position     : POSITION0;
    float3 normal       : NORMAL0;
    float2 uv           : TEXCOORD0;
    float3 bary         : POSITION1;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 color    : COLOR0;
    float4 normal   : NORMAL;

    float4 ambient  : COLOR1;
    float4 light    : POSITION1;
    float4 camera   : POSITION2;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput vertexShaderOutput;
    float4 position = float4(input.position, 1.0f);

    position = mul(position, world);

    position = mul(position, view);
    position = mul(position, projection);

    vertexShaderOutput.position = position;
    vertexShaderOutput.color = materialColor;

    float4 normal = float4(input.normal, 0.0f);

    vertexShaderOutput.normal  = mul(normal, world);
    vertexShaderOutput.ambient = ambientColorIntensity;
    vertexShaderOutput.camera  = camera;

    vertexShaderOutput.light = mul(ambientColorIntensity, world);


    return vertexShaderOutput;
}
