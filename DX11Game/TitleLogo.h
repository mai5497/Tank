#ifndef __TITLELOGO_H__
#define __TITLELOGO_H__

#include "main.h"
#include "polygon.h"
#include "Texture.h"

#define SPRING_TOTAL	(4)	// �S���Ђ�����
#define STRING_TOTAL	(6)	// ������


class TitleLogo {
public:
	TitleLogo();
	~TitleLogo();

	void Init();
	void Reset();	// �^�C�g�����s�[�g�p�֐�
	void Uninit();
	void Update();
	void Draw();

private:
	static LPCWSTR pszTexFName; 

	static ID3D11ShaderResourceView* pTexture;

	struct SPRING {
		int		status;				// �X�e�[�^�X
		bool	onLock;				// �Œ�t���O
		XMFLOAT2 position;			// �ʒu���W (Position)
		XMFLOAT2 velocity;			// ���x (Velocity)
		XMFLOAT2 acceleration;		// �����x (Acceleration)
		XMFLOAT2 resultant;			// ���� (Resultant)
		XMFLOAT2 gravity;			// �d�� (Gravity)
		float	mass;				// �΂˂̏d��
		XMFLOAT2 tension_up;		// ���� (Tension) �����
		XMFLOAT2 tension_down;		// ���� (Tension) ������
	};

	SPRING springs[STRING_TOTAL][SPRING_TOTAL];

};



#endif