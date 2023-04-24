//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

//-------------------- �C���N���[�h --------------------
#include "main.h"
#include "Texture.h"
#include "mesh.h"

//-------------------- �萔 --------------------
#define PARTICLE_MAX	(20)	// �p�[�e�B�N���̐�

//-------------------- �N���X��` --------------------
class Particle {
public:
	Particle();
	~Particle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//---�p�[�e�B�N���֘A
	struct PARTICLE {
		int status;					// ���
		int timer;					// �^�C�}�[	
		XMFLOAT2 pos;				// �ʒu
		float angle;				// �\���p�x
		float size;					// �\���T�C�Y
		XMFLOAT2	velocity;		// ���x
		XMFLOAT2	acceleration;	// �����x
		XMFLOAT4 color;
	};
	static	MESH		mesh;
	static 	MATERIAL	material;

	PARTICLE particles[PARTICLE_MAX];

private:
	static std::unique_ptr<Texture> pTexture;

	bool isButtonOnce;	// ��x�������͂���������
};


#endif