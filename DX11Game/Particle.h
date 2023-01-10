#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "main.h"
#include "Texture.h"
#include "mesh.h"


#define PARTICLE_MAX	(20)	// �p�[�e�B�N���̐�

class Particle {
public:
	Particle();
	~Particle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

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
	HRESULT MakeVertex(ID3D11Device* pDevice);
};


#endif