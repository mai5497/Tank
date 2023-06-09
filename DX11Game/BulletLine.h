//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __BULLETLINE_H__
#define __BULLETLINE_H__


//-------------------- インクルード部 --------------------
#include "main.h"

#include "mesh.h"

//-------------------- 前方定義 --------------------
class GameObject;

//-------------------- クラス定義 --------------------
class BulletLine {
public:
	//---関数
	BulletLine();
	~BulletLine();

	void Init(GameObject* _pObj);
	void Uninit();
	void Update();
	void Draw();

	void SetDir(XMFLOAT3 _dir);
private:
	//---関数
	void MakeVertexBullet(ID3D11Device* pDevice);
	
	//---変数
	MESH line;			// ガイドの線のメッシュ
	MATERIAL mat;		// マテリアル

	GameObject* pObj;	// 出すオブジェクトを保存する
		
	XMFLOAT3 pos;		// 座標
	XMFLOAT3 dir;		// 向かせる方向
	XMFLOAT3 angle;		// テクスチャ自体の回転
};

#endif // !__BULLETLINE_H__