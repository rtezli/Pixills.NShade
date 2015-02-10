// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ConstantBufferData : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 Position : POSITION;
};

// Per-pixel color data passed through the pixel shader.
struct VertexShaderOutput
{
	float3 color : COLOR0;
};

// Simple shader to do vertex processing on the GPU.
VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.color = float3(1.0f, 0.8f, 1.0f);
	return output;
}
