struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 wPos : TEXCOORD2;
};

// �O���[�o��
cbuffer global : register(b0) {
	matrix	g_mtxWVP;			// ���[���h�~�r���[�~�ˉe�s��
	matrix	g_mtxWorld;			// ���[���h�s��
	matrix	g_mtxTexture;		// �e�N�X�`���s��
	float4	g_vCameraPos;		// ���_���W(���[���h���)
	float4	g_vLightDir;		// ��������(���[���h���)
	float4	g_vLightAmbient;	// ����
	float4	g_vLightDiffuse;	// �g�U��
	float4	g_vLightSpecular;	// ���ʔ��ˌ�
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D rampTex : register(t4);	//�����v�e�N�X�`��


float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = modelTex.Sample(samp,pin.uv);

	/*����
	*	���������̃x�N�g�����m�Ȃ琳�̒l
	*	���������̃x�N�g�����m�Ȃ�O
	*	���Ε����̃x�N�g�����m�Ȃ畉�̒l
	*/

	/* V1��V2�̓���
	*	V1.x * V2.x + V1.y * V2.y + V1.z * V2.z
	*/

	/*
	* �I�u�W�F�N�g�̖@���ƃ��C�g�̃x�N�g��������ς��v�Z
	* ���̂܂܌v�Z����Ɩ��邭�Ȃ镔�������̒l�ɂȂ�̂ŁA
	* �u�����āv���C�g�̃x�N�g���𔽓]������
	*/
	float3 N = normalize(pin.normal);
	float3 L = normalize(-g_vLightDir.xyz);

	// �g�U��(Diffuse)���̂̕\�ʂɒ��ڂ�������A���̂̐F��\��
	float diffuse = dot(N, L);


	// ���ς̒l�����ƂɃ����v�e�N�X�`���̐F����������o��
	// ���ς̌��ʂ�-1~1�Ȃ̂�0~1�ɕϊ�����
	diffuse = diffuse * 0.5f + 0.5f;
	float shadow = rampTex.Sample(samp, float2(diffuse,0.0f));

	color.rgb *= shadow;

	return color;
}