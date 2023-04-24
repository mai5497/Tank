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
	BULLETTYPE_NORMAL = 0,		// 通常の弾
	BULLETTYPE_MISSILE,			// ミサイル
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

	static void FireBullet(XMFLOAT3 _pos, XMFLOAT3 _dir, ObjTag _tag,int objNum,EBulletType _type = BULLETTYPE_NORMAL);

private:
	void MakeVertexBullet(ID3D11Device* pDevice,ObjTag _objType);
	void Destroy();

	static MESH mesh_p;					// メッシュ情報
	static MESH mesh_e;					// メッシュ情報
	static MATERIAL material_p;				// マテリアル
	static MATERIAL material_e;				// マテリアル

	EBulletType	type;		// 種類

	int fireBulletObjNum;	// この弾を打ったオブジェクトの番号
};

#endif