//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "main.h"
#include "AssimpModel.h"
#include "collision.h"

class GameObject {
public:
	enum ObjTag {
		NONE = 0,
		PLAYER,
		ENEMY,
		WALL,
		BULLET

	};

	//---�֐�
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//---�ϐ�
	XMFLOAT3 pos;
	XMFLOAT3 size;
	XMFLOAT3 moveVal;

	XMINT2 mapIndex;	// 0,1�̃}�b�v��������v�f�ԍ������ɂȂ�̂����i�[

	Collision::eCollisionType collType;	// �I�u�W�F�N�g���ÓI�����I��
	float collRadius;
	XMFLOAT3 collSize;

	XMFLOAT4X4 mtxWorld;
	XMFLOAT3   rotModel;		// ���݂̌���
	
	bool isCollision;	// �����蔻��̃I���I�t������
	bool isHit;			// �����ɓ������Ă���

	ObjTag myTag;		// ���̃I�u�W�F�N�g�̃^�O
	std::vector<ObjTag> hitList;	// ���̃I�u�W�F�N�g�ɓ������Ă���I�u�W�F�N�g�̃^�O���擾

	bool use;

	int gameObjNum;	// GameObjManager�ɂ���gameobj�̂肷�Ƃ̉��ԖڂɊi�[����Ă��邩
	
protected:
	int	shadowNum;		// �ۉe�ԍ�

};


#endif