
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

struct RadialBlurData {
	float32_t2 center;
	float32_t blurWidth;
};
ConstantBuffer<RadialBlurData> radialData : register(b1);

float32_t3 RadialBlur(float32_t3 rgb, float32_t2 uv) {
	const int32_t kNumSamples = 10;
	float32_t3 result = rgb;

	float32_t2 direction = uv - radialData.center;
	for(int32_t sampleIndex = 1; sampleIndex < kNumSamples; sampleIndex++) {
		float32_t2 texcoord = uv + direction * radialData.blurWidth * float32_t(sampleIndex);
		result += gTexture.Sample(gSampler, texcoord).rgb;
	}

	return result * rcp(kNumSamples);
}
struct GrayScaleData {
	float32_t intensity;
};
ConstantBuffer<GrayScaleData> gsData : register(b2);

float32_t3 GrayScale(float32_t3 color) {
    float32_t3 result = dot(color, float32_t3(0.2125f, 0.7154f, 0.0721f));
    return lerp(color, result, gsData.intensity);
}

struct VignettingData {
	float32_t intensity;
};
ConstantBuffer<VignettingData> vData : register(b3);

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

	output.rgb = RadialBlur(output.rgb, uv);

	output.rgb = GrayScale(output.rgb);

	output.rgb *= Vignetting(uv);

	return output;
}
