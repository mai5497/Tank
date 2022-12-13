struct PS_IN {
	float4 pos : SV_POSITION;
	float4 wPos : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float2 uv : TEXCOORD2;
	float4	Diffuse		: COLOR0;
};

// �O���[�o��
cbuffer global : register(b1) {
	float4	vEye;		// ���_���W
	float4	vLightDir;	// ��������
	float4	vLa;		// �����F(�A���r�G���g)
	float4	vLd;		// �����F(�f�B�t���[�Y)
	float4	vLs;		// �����F(�X�y�L����)
	// �}�e���A��
	float4	vAmbient;	// �A���r�G���g�F(+�e�N�X�`���L��)
	float4	vDiffuse;	// �f�B�t���[�Y�F
	float4	vSpecular;	// �X�y�L�����F(+�X�y�L�������x)
	float4	vEmissive;	// �G�~�b�V�u�F
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D normalMap : register(t4);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 color1 = modelTex.Sample(samp, pin.uv);

	// ���f���̖@�������̂܂܎g���ꍇ
	//float3 N = normalize(pin.normal);
	// �@���}�b�v�������̖@������肾��
	float3 N = normalMap.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;	// 0~1�̐F�f�[�^���A-1~1�̃x�N�g���f�[�^�ɕϊ�
	N = normalize(N);

	float3 L = normalize(-vLightDir.xyz);

	// �g�U��(Diffuse)���̂̕\�ʂɒ��ڂ�������A���̂̐F��\��
	float diffuse = dot(N, L);
	// ����(Ambient)����̕��̂��甽�˂��ĊԐړI�ɓ�������A���̂̉e�̕����̐F��\��
	float ambient = 0.5f;
	/* ���ʔ���(�X�y�L�����\:specular)
	 *	���˃x�N�g���Ǝ����x�N�g���̈�v��Ŕ��˂̋�����\��
	*/
	float3 R = reflect(-L, N);
	float3 V = normalize(vEye.xyz - pin.wPos.xyz);
	// ���ʔ���
	float specular = max(0, dot(R, V));
	// ���̂܂܂��ƑS�̂����邢�̂ŁA�ݏ悷��Ƃ��������ɂȂ�
	//specular = pow(specular, 5.0f);

	color.rgb = diffuse * 0.5f + ambient + specular;
	color = color * color1;

	return color;
}