cbuffer TessellationBuffer
{
    float tessellationAmount;
    float3 padding;
};

struct HullInput
{
    float4 position : SV_POSITION;
    float4 color    : COLOR0;
    float4 normal   : NORMAL;

    float4 ambient  : COLOR1;
    float4 light    : POSITION1;
    float4 camera   : POSITION2;
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

ConstantOutput ColorPatchConstantFunction(InputPatch<HullInput, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutput output;

    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;

    output.inside = tessellationAmount;

    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]
HullOutput main(InputPatch<HullInput, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutput output;

    output.position = patch[pointId].position;
    output.color = patch[pointId].color;

    return output;
}