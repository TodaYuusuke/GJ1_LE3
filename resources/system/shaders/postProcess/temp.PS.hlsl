
struct PSInput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
};

struct Parameter
{
    int time;
    int rWidth;
    int rHeight;
};

ConstantBuffer<Parameter> gPara : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gDepth : register(t1);
SamplerState gSampler : register(s0);
SamplerState gPointSampler : register(s1);

struct VignettingData {
	float32_t intensity;
};
ConstantBuffer<VignettingData> vData : register(b1);

float32_t Vignetting(float32_t2 uv) {
	float32_t2 correct = uv * (1.0f - uv.xy);
	float vignette = correct.x * correct.y * 16.0f;
	vignette = saturate(pow(vignette, vData.intensity));
	return vignette;
}
float32_t4 main(PSInput input) : SV_TARGET {
	float32_t4 output;
	float2 uv = input.texcoord;

    output = gTexture.Sample(gSampler, uv);

	output.rgb *= Vignetting(uv);

	return output;
}
