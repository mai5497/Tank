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

	// ���f���̖@�������̂܂܎g���ꍇ
	//float3 N = normalize(pin.normal);
	// �@���}�b�v�������̖@������肾��
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;	// 0~1�̐F�f�[�^���A-1~1�̃x�N�g���f�[�^�ɕϊ�
	N = normalize(N);

	float3 L = normalize(-pin.tangentL);

	// �g�U��(Diffuse)���̂̕\�ʂɒ��ڂ�������A���̂̐F��\��
	float diffuse = dot(N, L);
	// ����(Ambient)����̕��̂��甽�˂��ĊԐړI�ɓ�������A���̂̉e�̕����̐F��\��
	float ambient = 0.5f;
	/* ���ʔ���(�X�y�L�����\:specular)
	 *	���˃x�N�g���Ǝ����x�N�g���̈�v��Ŕ��˂̋�����\��
	*/
	float3 R = reflect(-L, N);
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);
	// ���ʔ���
	float specular = max(0, dot(R, V));
	// ���̂܂܂��ƑS�̂����邢�̂ŁA�ݏ悷��Ƃ��������ɂȂ�
	specular = pow(specular, 10.0f);

	color.rgb = diffuse * 0.5f + ambient + specular;

	return color;
}