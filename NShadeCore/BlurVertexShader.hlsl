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

    float4 position  : SV_POSITION;
    float2 uv        : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float texelSize;

    // Change the position vector to be 4 units for proper matrix calculations.
    float4 pos = float4(input.position, 1.0f);

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(pos, world);
    output.position = mul(pos, view);
    output.position = mul(pos, projection);

    // Store the texture coordinates for the pixel shader.
    output.uv = input.uv;
  

    // Determine the floating point size of a texel for a screen with this specific width.
    //texelSize = 1.0f / screenWidth;

    texelSize = 1.0f / 1920;

    // Create UV coordinates for the pixel and its four horizontal neighbors on either side.
    output.texCoord1 = input.uv + float2(texelSize * -4.0f, 0.0f);
    output.texCoord2 = input.uv + float2(texelSize * -3.0f, 0.0f);
    output.texCoord3 = input.uv + float2(texelSize * -2.0f, 0.0f);
    output.texCoord4 = input.uv + float2(texelSize * -1.0f, 0.0f);
    output.texCoord5 = input.uv + float2(texelSize *  0.0f, 0.0f);
    output.texCoord6 = input.uv + float2(texelSize *  1.0f, 0.0f);
    output.texCoord7 = input.uv + float2(texelSize *  2.0f, 0.0f);
    output.texCoord8 = input.uv + float2(texelSize *  3.0f, 0.0f);
    output.texCoord9 = input.uv + float2(texelSize *  4.0f, 0.0f);

    return output;
}