//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __BULLET_H__
#define __BULLET_H__


#include "main.h"
#include "GameObject.h"
#include "mesh.h"

enum EBulletType {
	BULLETTYPE_PLAYER = 0,		// プレイヤーの弾
	BULLETTYPE_ENEMY,			// 敵の弾
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

	static MESH mesh_p;					// メッシュ情報
	static MESH mesh_e;					// メッシュ情報
	static MATERIAL material;				// マテリアル

	EBulletType	type;		// 種類
};

#endif