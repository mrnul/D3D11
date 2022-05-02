struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

	output.position = position;
    output.position.x = position.x * abs(sin(color.a / 1000));
	output.position.y = position.y * abs(sin(color.a / 2000));
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;// * abs(sin(color.a / 1000));
}
