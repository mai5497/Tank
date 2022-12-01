//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __BULLET_H__
#define __BULLET_H__


#include "main.h"
#include "GameObject.h"
#include "mesh.h"

enum EBulletType {
	BULLETTYPE_PLAYER = 0,		// �v���C���[�̒e
	BULLETTYPE_ENEMY,			// �G�̒e
	BULLETTYPE_MAX
};

class Bullet : public GameObject {
public:
	Bullet();
	~Bullet();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void FireBullet(XMFLOAT3 _pos, XMFLOAT3 _dir, EBulletType _type);

private:
	void MakeVertexBullet(ID3D11Device* pDevice);
	void Destroy();

	static MESH mesh_p;					// ���b�V�����
	static MESH mesh_e;					// ���b�V�����
	static MATERIAL material;				// �}�e���A��

	EBulletType	type;		// ���
};

#endif