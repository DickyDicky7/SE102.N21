struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

PS_INPUT main(uint val : SV_VertexID)
{
    PS_INPUT output;
    // Maps 0, 1, 2 to a full-screen triangle covering the viewport
    output.Tex = float2((val << 1) & 2, val & 2);
    output.Pos = float4(output.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    return output;
}
