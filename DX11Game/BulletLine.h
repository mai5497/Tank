//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#include "main.h"

#include "mesh.h"

class GameObject;
class BulletLine {
public:
	BulletLine();
	~BulletLine();

	void Init(GameObject* _pObj);
	void Uninit();
	void Update();
	void Draw();

	void SetDir(XMFLOAT3 _dir);
private:
	void MakeVertexBullet(ID3D11Device* pDevice);
	
	MESH line;
	MATERIAL mat;

	GameObject* pObj;

	XMFLOAT3 pos;		// 座標
	XMFLOAT3 dir;		// 向かせる方向
	XMFLOAT3 angle;		// テクスチャ自体の回転
};
