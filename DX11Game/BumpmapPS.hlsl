struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 wPos : TEXCOORD2;
	//float3 tangentL : TANGENT0;
};

cbuffer Light : register(b0) {
	float4 lightDir;
};
cbuffer Camera : register(b1) {
	float4 camPos;
}

SamplerState samp : register(s0);
Texture2D normalMap : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// モデルの法線をそのまま使う場合
	//float3 N = normalize(pin.normal);
	// 法線マップから代わりの法線を取りだす
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;	// 0~1の色データを、-1~1のベクトルデータに変換
	N = normalize(N);

	float3 L = normalize(-pin.tangentL);

	// 拡散光(Diffuse)物体の表面に直接あたる光、物体の色を表す
	float diffuse = dot(N, L);
	// 環境光(Ambient)周りの物体から反射して間接的に当たる光、物体の影の部分の色を表す
	float ambient = 0.5f;
	/* 鏡面反射(スペキュラ―:specular)
	 *	反射ベクトルと視線ベクトルの一致具合で反射の強さを表す
	*/
	float3 R = reflect(-L, N);
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);
	// 鏡面反射
	float specular = max(0, dot(R, V));
	// そのままだと全体が明るいので、累乗するといい感じになる
	specular = pow(specular, 10.0f);

	color.rgb = diffuse * 0.5f + ambient + specular;

	return color;
}