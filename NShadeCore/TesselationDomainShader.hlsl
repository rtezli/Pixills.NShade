cbuffer Camera
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4   camera;
};

struct ConstantOutput
{
    float edges[3]  : SV_TessFactor;
    float inside    : SV_InsideTessFactor;
};

struct HullOutput
{
    float3 position : POSITION;
    float4 color    : COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color    : COLOR0;
    float4 normal   : NORMAL;

    float4 ambient  : COLOR1;
    float4 light    : POSITION1;
    float4 camera   : POSITION2;
};

[domain("tri")]
PixelInput main(ConstantOutput input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutput, 3> patch)
{
    float3 vertexPosition;
    PixelInput output;

    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;

    output.position = mul(float4(vertexPosition, 1.0f), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.color = patch[0].color;

    return output;
}