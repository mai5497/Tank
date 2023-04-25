//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
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

	XMFLOAT3 pos;		// ���W
	XMFLOAT3 dir;		// �����������
	XMFLOAT3 angle;		// �e�N�X�`�����̂̉�]
};
