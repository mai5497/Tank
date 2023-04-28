//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __BULLET_H__
#define __BULLET_H__

//-------------------- インクルード --------------------
#include "main.h"
#include "GameObject.h"
#include "mesh.h"

//-------------------- 定数定義 --------------------
enum EBulletType {
	BULLETTYPE_NORMAL = 0,		// 通常の弾
	BULLETTYPE_MISSILE,			// ミサイル
	BULLETTYPE_MAX
};

//-------------------- クラス定義 --------------------
class Bullet : public GameObject {
public:
	//---関数
	Bullet();
	~Bullet();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void FireBullet(XMFLOAT3 _pos, XMFLOAT3 _dir, ObjTag _tag,int objNum,EBulletType _type = BULLETTYPE_NORMAL);

private:
	//---関数
	void MakeVertexBullet(ID3D11Device* pDevice,ObjTag _objType);
	void Destroy();

	//---変数
	static MESH mesh_p;				// メッシュ情報
	static MESH mesh_e;				// メッシュ情報
	static MATERIAL material_p;		// マテリアル
	static MATERIAL material_e;		// マテリアル

	EBulletType	type;				// 種類

	int fireBulletObjNum;			// この弾を打ったオブジェクトの番号
};

#endif