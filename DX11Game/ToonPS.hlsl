struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 wPos : TEXCOORD2;
};

// グローバル
cbuffer global : register(b0) {
	matrix	g_mtxWVP;			// ワールド×ビュー×射影行列
	matrix	g_mtxWorld;			// ワールド行列
	matrix	g_mtxTexture;		// テクスチャ行列
	float4	g_vCameraPos;		// 視点座標(ワールド空間)
	float4	g_vLightDir;		// 光源方向(ワールド空間)
	float4	g_vLightAmbient;	// 環境光
	float4	g_vLightDiffuse;	// 拡散光
	float4	g_vLightSpecular;	// 鏡面反射光
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D rampTex : register(t4);	//ランプテクスチャ


float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = modelTex.Sample(samp,pin.uv);

	/*内積
	*	同じ方向のベクトル同士なら正の値
	*	垂直方向のベクトル同士なら０
	*	反対方向のベクトル同士なら負の値
	*/

	/* V1とV2の内積
	*	V1.x * V2.x + V1.y * V2.y + V1.z * V2.z
	*/

	/*
	* オブジェクトの法線とライトのベクトルから内積を計算
	* そのまま計算すると明るくなる部分が負の値になるので、
	* 「あえて」ライトのベクトルを反転させる
	*/
	float3 N = normalize(pin.normal);
	float3 L = normalize(-g_vLightDir.xyz);

	// 拡散光(Diffuse)物体の表面に直接あたる光、物体の色を表す
	float diffuse = dot(N, L);


	// 内積の値をもとにランプテクスチャの色を引っ張り出す
	// 内積の結果は-1~1なので0~1に変換する
	diffuse = diffuse * 0.5f + 0.5f;
	float shadow = rampTex.Sample(samp, float2(diffuse,0.0f));

	color.rgb *= shadow;

	return color;
}