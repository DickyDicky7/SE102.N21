// SpriteVS.hlsl - Instanced sprite vertex shader
// Transforms unit quad vertices from local space to clip space.
// Order of transformations: Local -> World (per-instance) -> View -> Projection

// --- Per-frame camera constants --------------------------------------------
cbuffer CameraConstants : register(b0)
{
    row_major float4x4 g_View;
    row_major float4x4 g_Projection;
};

struct VS_INPUT
{
    // Per-vertex (slot 0): the static unit quad.
    float2 localPosition : POSITION;     // LOCAL space
    float2 cornerWeight  : TEXCOORD0;    // (0,0) TL .. (1,1) BR selector

    // Per-instance (slot 1): one sprite.
    float4 worldRow0     : WORLD0;
    float4 worldRow1     : WORLD1;
    float4 worldRow2     : WORLD2;
    float4 worldRow3     : WORLD3;
    float4 sourceRect    : TEXCOORD1;    // (u0, v0, u1, v1) in the atlas
    float4 tint          : COLOR0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
    float4 tint     : COLOR0;
};

PS_INPUT main(VS_INPUT input)
{
    // Rebuild the per-instance world matrix from its four rows.
    float4x4 world = float4x4(input.worldRow0,
                              input.worldRow1,
                              input.worldRow2,
                              input.worldRow3);

    // Row-vector convention throughout: v' = v * M.
    float4 localPosition = float4(input.localPosition, 0.0f, 1.0f);
    float4 worldPosition = mul(localPosition, world);
    float4 viewPosition  = mul(worldPosition, g_View);
    float4 clipPosition  = mul(viewPosition,  g_Projection);

    PS_INPUT output;
    output.position = clipPosition;

    // Map sourceRect (u0, v0, u1, v1) to quad corner weights.
    output.uv   = lerp(input.sourceRect.xy, input.sourceRect.zw, input.cornerWeight);
    output.tint = input.tint;

    return output;
}
