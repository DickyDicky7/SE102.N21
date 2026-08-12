struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

Texture2D tex0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer PostProcessConstants : register(b0)
{
    float time;
    float frameCount;
    float2 canvasSize;
    float2 texelSize;
    float2 padding;
    float4 mousePosition;
};

#define ANALOG
#define DIGITAL
#define CRT
#define BLOOM
#define DURATION 5.0f
#define AMT      0.5f 
#define SS(a, b, x) (smoothstep(a, b, x) * smoothstep(b, a, x))

float mod(float x, float y)
{
    return x - y * floor(x / y);
}

float3 hash33(float3 p) {
    p  =   frac( p              * 0.1031f );
    p +=   dot  ( p     , p.yzx  + 19.1900f );
    return frac((p.xxy + p.yzz) *  p.zyx  );
}

float gnoise(float3 x) {
    float3 p = floor(x);
    float3 w = frac(x);

    float3 u = w * w * w * (w * (w * 6.0f - 15.0f) + 10.0f);

    float3 ga = hash33(p + float3(0.0f, 0.0f, 0.0f));
    float3 gb = hash33(p + float3(1.0f, 0.0f, 0.0f));
    float3 gc = hash33(p + float3(0.0f, 1.0f, 0.0f));
    float3 gd = hash33(p + float3(1.0f, 1.0f, 0.0f));
    float3 ge = hash33(p + float3(0.0f, 0.0f, 1.0f));
    float3 gf = hash33(p + float3(1.0f, 0.0f, 1.0f));
    float3 gg = hash33(p + float3(0.0f, 1.0f, 1.0f));
    float3 gh = hash33(p + float3(1.0f, 1.0f, 1.0f));

    float va = dot(ga, w - float3(0.0f, 0.0f, 0.0f));
    float vb = dot(gb, w - float3(1.0f, 0.0f, 0.0f));
    float vc = dot(gc, w - float3(0.0f, 1.0f, 0.0f));
    float vd = dot(gd, w - float3(1.0f, 1.0f, 0.0f));
    float ve = dot(ge, w - float3(0.0f, 0.0f, 1.0f));
    float vf = dot(gf, w - float3(1.0f, 0.0f, 1.0f));
    float vg = dot(gg, w - float3(0.0f, 1.0f, 1.0f));
    float vh = dot(gh, w - float3(1.0f, 1.0f, 1.0f));

    float gNoise = va + u.x * (vb - va) +
    u.y * (vc - va) +
    u.z * (ve - va) +
    u.x * u.y * (va - vb - vc + vd) +
    u.y * u.z * (va - vc - ve + vg) +
    u.z * u.x * (va - vb - ve + vf) +
    u.x * u.y * u.z * (-va + vb + vc - vd + ve - vf - vg + vh);

    return 2.0f * gNoise;
}

float gnoise01(float3 x) {
    return 0.5f + 0.5f * gnoise(x);
}

float2 crt(float2 uv) {
    float tht =   atan2(uv.y, uv.x);
    float   r = length(uv        );
    r    /=    (1.0f - 0.1f * r * r);
    uv.x  = r  * cos(tht);
    uv.y  = r  * sin(tht);
    return 0.5f *      (uv  +  1.0f);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;
    float t = time;

    float glitchAmount = SS(DURATION * 0.001f, DURATION * AMT, mod(t, DURATION));
    float displayNoise = 0.0f;
    float3 col = float3(0.0f, 0.0f, 0.0f);
    float2 eps = float2(5.0f / canvasSize.x, 0.0f);
    float2 st = float2(0.0f, 0.0f);

#ifdef CRT
    uv            = crt(uv * 2.0f - 1.0f);
    displayNoise += 1.0f;
#endif

    float y = uv.y * canvasSize.y;
    float distortion = gnoise(float3(0.0f, y * 0.01f, t * 500.0f)) * (glitchAmount * 4.0f + 0.100f);
    distortion *= gnoise(float3(0.0f, y * 0.02f, t * 250.0f)) * (glitchAmount * 2.0f + 0.025f);

#ifdef ANALOG
    displayNoise += 1.0f;
    distortion += smoothstep(0.999f, 1.0f, sin((uv.y + t * 1.6f) * 2.0f)) * 0.02f;
    distortion -= smoothstep(0.999f, 1.0f, sin((uv.y + t) * 2.0f)) * 0.02f;
    st = uv + float2(distortion, 0.0f);
    col.r += tex0.Sample(sampler0, st + eps + distortion).r;
    col.g += tex0.Sample(sampler0, st).g;
    col.b += tex0.Sample(sampler0, st - eps - distortion).b;
#else
    col += tex0.Sample(sampler0, uv).xyz;
#endif

#ifdef DIGITAL
    float bt = floor(t * 30.0f) * 300.0f;
    float blockGlitch = 0.2f + 0.9f * glitchAmount;
    float blockNoiseX  = step(gnoise01(float3(0.0f, uv.x * 3.0f, bt      )), blockGlitch);
    float blockNoiseX2 = step(gnoise01(float3(0.0f, uv.x * 1.5f, bt * 1.2f)), blockGlitch);
    float blockNoiseY  = step(gnoise01(float3(0.0f, uv.y * 4.0f, bt      )), blockGlitch);
    float blockNoiseY2 = step(gnoise01(float3(0.0f, uv.y * 6.0f, bt * 1.2f)), blockGlitch);
    float block = blockNoiseX2 * blockNoiseY2 + blockNoiseX * blockNoiseY;
    st = float2(uv.x + sin(bt) * hash33(float3(uv, 0.5f)).x, uv.y);
    col *= 1.0f - block;
    block *= 1.15f;
    col.r += tex0.Sample(sampler0, st + eps).r * block;
    col.g += tex0.Sample(sampler0, st).g * block;
    col.b += tex0.Sample(sampler0, st - eps).b * block;
#endif

#ifdef BLOOM
    // --- HDR Bloom / Glow ---
    float3 bloom = float3(0.0f, 0.0f, 0.0f);
    float2 directions[8] = {
        float2(1.0f, 0.0f), float2(-1.0f, 0.0f), float2(0.0f, 1.0f), float2(0.0f, -1.0f),
        float2(0.707f, 0.707f), float2(-0.707f, 0.707f), float2(0.707f, -0.707f), float2(-0.707f, -0.707f)
    };
    
    float bloomThreshold = 0.8f;
    float totalWeight = 0.0f;
    
    // Ring 1 (near glow)
    for (int i = 0; i < 8; ++i)
    {
        float2 sampleUV = uv + directions[i] * texelSize * 2.0f;
        float3 sampleCol = tex0.Sample(sampler0, sampleUV).rgb;
        float brightness = dot(sampleCol, float3(0.2126f, 0.7152f, 0.0722f));
        float factor = max(0.0f, brightness - bloomThreshold) / (brightness + 0.0001f);
        bloom += sampleCol * factor * 0.15f;
        totalWeight += 0.15f;
    }
    
    // Ring 2 (far glow)
    for (int j = 0; j < 8; ++j)
    {
        float2 sampleUV = uv + directions[j] * texelSize * 4.5f;
        float3 sampleCol = tex0.Sample(sampler0, sampleUV).rgb;
        float brightness = dot(sampleCol, float3(0.2126f, 0.7152f, 0.0722f));
        float factor = max(0.0f, brightness - bloomThreshold) / (brightness + 0.0001f);
        bloom += sampleCol * factor * 0.1f;
        totalWeight += 0.1f;
    }
    
    if (totalWeight > 0.0f)
    {
        bloom /= totalWeight;
    }
    
    col += bloom * 1.6f;
#endif

    displayNoise = clamp(displayNoise, 0.0f, 1.0f);
    col += (0.15f + 0.65f * glitchAmount) * hash33(float3(input.Tex * canvasSize, mod(frameCount, 1000.0f))).r * displayNoise;
    col -= (0.25f + 0.75f * glitchAmount) * sin(4.0f * t + uv.y * canvasSize.y * 1.750f) * displayNoise;

#ifdef CRT
    float vig = 8.0f * uv.x * uv.y * (1.0f - uv.x) * (1.0f - uv.y);
    col *= pow(max(vig, 0.0f), 0.25f) * 1.5f;
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
        col *= 0.0f;
#endif

    return float4(col, 1.0f);
}
