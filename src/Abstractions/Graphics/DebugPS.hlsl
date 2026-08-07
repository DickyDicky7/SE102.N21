// DebugPS.hlsl - Debug line pixel shader

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 colour   : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.colour;
}
