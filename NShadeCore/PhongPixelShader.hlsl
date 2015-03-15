struct PixelShaderInput
{
    float4 position         : SV_POSITION;
    float4 color            : COLOR0;
    float4 normal           : NORMAL;

    float4 ambient          : COLOR1;
    float4 light            : POSITION1;
    float4 camera           : POSITION2;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float  lightIntensity = 0.5f;

    float  specularPower = 40.00f;

    float4 normal = normalize(input.normal);
    float4 light = normalize(input.light);
    float4 camera = normalize(input.camera);

    float4 ambient = { 0.1f, 0.1f, 0.1f, 0.0f };
    float4 diffuse = saturate(dot(normal, light));
    float4 reflect = normalize(2 * diffuse * normal - light);

    float4 specular = saturate(lightIntensity * pow(saturate(dot(reflect, camera)), specularPower));

    return (ambient + input.color + diffuse + specular) * lightIntensity;
}