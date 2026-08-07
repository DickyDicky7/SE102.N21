// SpritePS.hlsl - Instanced sprite pixel shader
// Samples the sprite texture with point/clamp filtering and modulates by tint.

Texture2D    g_spriteTexture : register(t0);
SamplerState g_spriteSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD0;
    float4 tint     : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 colour = g_spriteTexture.Sample(g_spriteSampler, input.uv) * input.tint;

    // Discard transparent pixels.
    clip(colour.a - (0.5f / 255.0f));

    return colour;
}
