// DebugVS.hlsl - Debug line vertex shader
// Transforms debug vertices from world space to clip space.

cbuffer CameraConstants : register(b0)
{
    row_major float4x4 g_View;
    row_major float4x4 g_Projection;
};

struct VS_INPUT
{
    float3 worldPosition : POSITION;
    float4 colour        : COLOR0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 colour   : COLOR0;
};

PS_INPUT main(VS_INPUT input)
{
    float4 worldPosition = float4(input.worldPosition, 1.0f);
    float4 viewPosition   = mul(worldPosition, g_View);
    float4 clipPosition   = mul(viewPosition,  g_Projection);

    PS_INPUT output;
    output.position = clipPosition;
    output.colour   = input.colour;
    return output;
}
