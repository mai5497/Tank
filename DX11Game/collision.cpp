//************************************************************************************
// 
// �����蔻��[collision.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Collision.h"
#include "GameObject.h"

std::vector<std::shared_ptr<GameObject>> Collision::pObjects;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Collision::Collision() {

}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Collision::~Collision() {
	// �I�u�W�F�N�g�̉��
	int delCnt = pObjects.size();
	for (int i = 0; i < delCnt; i++) {
		pObjects[i].reset();
	}

	pObjects.clear();
}


//====================================================================================
//
//				�R���W�������X�g�ւ̃I�u�W�F�N�g�̒ǉ�
//
//====================================================================================
void Collision::AddList(const std::shared_ptr<GameObject>& _gameObject) {
	pObjects.emplace_back(_gameObject);
}


//====================================================================================
//
//				�R���W�������X�g����I�u�W�F�N�g�̍폜
//
//====================================================================================
void Collision::DelList(int _index) {
	pObjects[_index]->hitList.clear();
	pObjects.erase(pObjects.begin() + _index);
}

//====================================================================================
//
//				�����蔻��̍X�V
//
//====================================================================================
void Collision::Update() {
	// ���ꂼ��̃I�u�W�F�N�g�̓������Ă�����̃��X�g�X�V�̂��߂�
	// �������Ă�����̃��X�g������������
	for (int i = 0; i < pObjects.size(); i++) {
		pObjects[i]->hitList.clear();
	}

	// �����蔻��
	for (int j = 0; j < pObjects.size(); j++) {
		for (int i = j + 1; i < pObjects.size(); i++) {
			if (pObjects[j]->collType == Collision::STATIC &&
				pObjects[i]->collType == Collision::STATIC) {
				/*
				* �����蔻��̃`�F�b�N������I�u�W�F�N�g�������������Ȃ��I�u�W�F�N�g�Ȃ�
				* �`�F�b�N�̕K�v���Ȃ����߃X�L�b�v����
				*/
				continue;
			} else if (pObjects[j]->isCollision == false ||
					   pObjects[i]->isCollision == false) {
				/*
				* �����蔻�肪�����ɂȂ��Ă���I�u�W�F�N�g�͓����蔻��𖳎�����
				*/
				continue;
			}

			if (CollisionAABB(*pObjects[j], *pObjects[i])) {
				if (pObjects[j]->collType == Collision::DYNAMIC &&
					pObjects[i]->collType == Collision::STATIC) {
					if (pObjects[j]->myTag == GameObject::ObjTag::ENEMY && pObjects[i]->myTag == GameObject::ObjTag::WALL) {
						break;
					} else {
						Push(pObjects[j], pObjects[i]);
					}
				} else if (pObjects[j]->collType == Collision::STATIC &&
						   pObjects[i]->collType == Collision::DYNAMIC) {
					if (pObjects[i]->myTag == GameObject::ObjTag::ENEMY && pObjects[j]->myTag == GameObject::ObjTag::WALL) {
						break;
					} else {
						Push(pObjects[i], pObjects[j]);

					}
				} else if (pObjects[j]->collType == Collision::DYNAMIC &&
						   pObjects[i]->collType == Collision::DYNAMIC) {
					Push(pObjects[j], pObjects[i]);
					Push(pObjects[i], pObjects[j]);
				}

				// �������Ă�����̃��X�g�ւ̒ǉ�
				pObjects[i]->hitList.emplace_back(*pObjects[j]);
				pObjects[j]->hitList.emplace_back(*pObjects[i]);
			} 
		}
	}
}


//====================================================================================
//
//				���̓����蔻��
//
//====================================================================================
bool Collision::CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br) {
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

bool Collision::CollisionSphere(GameObject A, GameObject B) {
	XMFLOAT3 vA, vB, vZero;
	vZero = XMFLOAT3(0, 0, 0);

	XMStoreFloat3(&vA, XMVector3TransformCoord(XMLoadFloat3(&vZero), XMLoadFloat4x4(&A.mtxWorld)));
	XMStoreFloat3(&vB, XMVector3TransformCoord(XMLoadFloat3(&vZero), XMLoadFloat4x4(&B.mtxWorld)));


	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dz = vA.z - vB.z;
	float dr = A.collRadius + B.collRadius;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}



//====================================================================================
//
//				AABB���m�̓����蔻��
//
//====================================================================================
bool Collision::CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize) {
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

bool Collision::CollisionAABB(GameObject A, GameObject B) {
	return (A.pos.x - A.collSize.x / 2 <= B.pos.x + B.collSize.x / 2) &&
		(B.pos.x - B.collSize.x / 2 <= A.pos.x + A.collSize.x / 2) &&
		(A.pos.y - A.collSize.y / 2 <= B.pos.y + B.collSize.y / 2) &&
		(B.pos.y - B.collSize.y / 2 <= A.pos.y + A.collSize.y / 2) &&
		(A.pos.z - A.collSize.z / 2 <= B.pos.z + B.collSize.z / 2) &&
		(B.pos.z - B.collSize.z / 2 <= A.pos.z + A.collSize.z / 2);
}


//====================================================================================
//
//				�I�u�W�F�N�g�̉��o����
//
//====================================================================================
GameObject* Collision::Push(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 move, XMFLOAT3 Bpos, XMFLOAT3 Bsize) {
	//---���O�Ɋe�I�u�W�F�N�g�̏��𔲂����
	//DirectX::XMFLOAT3 dPos = pDynamic->GetPos();
	//DirectX::XMFLOAT3 sPos = pStatic->GetPos();
	//DirectX::XMFLOAT3 dSize = pDynamic->GetSize();
	//DirectX::XMFLOAT3 sSize = pStatic->GetSize();
	//DirectX::XMFLOAT3 dMove = pDynamic->GetMove();

	// �߂�l�p�Q�[���I�u�W�F�N�g��`
	GameObject _GameObject;


	//---�v�Z���𔲂��o��
	//�ړ��ʂ���ړ�������������߂�
	//���ړ���������͂P��-1�ŕ\��
	//�O�����Z�qA ? B : C
	//A�̏������������Ă���B�A�s�����Ȃ�C
	DirectX::XMFLOAT3 dir(
		move.x > 0 ? 1 : -1,
		move.y > 0 ? 1 : -1,
		move.z > 0 ? 1 : -1);
	//����ƈꏏ
	/*
	if (dMove.x > 0)
	{
		dMove.x = 1;
	}
	else
	{
		dMove.y = -1;
	}
	*/

	//���ۂɓ����蔻��̂߂荞�񂾂̂́A
	//�I�u�W�F�N�g�̍��W�ł͂Ȃ��A�p������
	//�߂荞��ł��邱�Ƃ�����̂ŁA
	//�Ώۂ̊p���v�Z�ŋ��߂�
	DirectX::XMFLOAT3 dPoint(
		Apos.x + dir.x * Asize.x * 0.5f,
		Apos.y + dir.y * Asize.y * 0.5f,
		Apos.z + dir.z * Asize.z * 0.5f);
	//�ړ��O�̊p�̍��W
	//���̂܂܂̈ړ��ʂňړ��O�̍��W��\������
	//����ƕ��������_�̌v�Z�덷�ɂ��A�ړ�
	//�x�N�g���̎��_���ʂ̓����ɂ��邱�ƂɂȂ�A
	//�ђʂ��Ă��Ȃ��Ɣ��肳���B
	//�����������邽�߂Ɉړ��̑O�̈ʒu�������Ȃ�
	//�悤�Ɍv�Z�̎������A�ړ��ʂ�傫������
	DirectX::XMFLOAT3 prePoint(
		dPoint.x - move.x * 1.1f,
		dPoint.y - move.y * 1.1f,
		dPoint.z - move.z * 1.1f);
	//�Փ˂��ꂽ���̊p���v�Z
	//�Փ˂��ꂽ���̊p�͈ړ������𔽓]����
	//�����Ɍ������đ傫���̔����𑫂��Ƌ��߂���
	DirectX::XMFLOAT3 sPoint(
		Bpos.x - dir.x * Bsize.x * 0.5f,
		Bpos.y - dir.y * Bsize.y * 0.5f,
		Bpos.z - dir.z * Bsize.z * 0.5f);
	//�ʏ�̈�_����ړ��x�N�g���̎n�_�ƏI�_��
	//�������x�N�g�����v�Z����
	DirectX::XMFLOAT3 vStart(
		prePoint.x - sPoint.x,
		prePoint.y - sPoint.y,
		prePoint.z - sPoint.z);
	DirectX::XMFLOAT3 vEnd(
		dPoint.x - sPoint.x,
		dPoint.y - sPoint.y,
		dPoint.z - sPoint.z);

	//���ꂼ��̖ʂƌv�Z
	//�e�ʂ̖@���x�N�g��
	DirectX::XMFLOAT3 Normal[] =
	{
		DirectX::XMFLOAT3(-dir.x, 0, 0),
		DirectX::XMFLOAT3(0, -dir.y, 0),
		DirectX::XMFLOAT3(0, 0, -dir.z),
	};
	for (int i = 0; i < _countof(Normal); i++) {
		//�ʂ̖@���Ƃ̓��ς��v�Z
		float dotS =
			vStart.x * Normal[i].x +
			vStart.y * Normal[i].y +
			vStart.z * Normal[i].z;
		float dotE =
			vEnd.x * Normal[i].x +
			vEnd.y * Normal[i].y +
			vEnd.z * Normal[i].z;
		//���ꂼ��̓��ς̌��ʂ����ƕ���
		//����Έړ��x�N�g����ʂ��ђʂ��Ă���
		//���ƕ��̑g�ݍ��킹���ǂ�����
		//�|���Z�Ŕ���ł���
		//(��x��=��/��x��=��/��x��=��)
		if (dotS * dotE < 0) {
			//�ړ��x�N�g���̕����ɂ߂荞�񂾗ʂ�
			//�߂��̂ł͂Ȃ��A�ʂ̕����Ɍ�������
			//�߂�(�߂������͓��ς̒l�Ɠ�����)
			dotE = fabsf(dotE);
			Apos.x += Normal[i].x * dotE;
			Apos.y += Normal[i].y * dotE;
			Apos.z += Normal[i].z * dotE;
			_GameObject.pos = Apos;
			//���ׂĂ̈ړ��ʂ�0�ɂ��Ă��܂���
			//�ق��̃I�u�W�F�N�g�Ɖ����o���̌v�Z��
			//����ۂɈړ��ʂ��Ȃ����ƂɂȂ�̂�
			//�ړ��ʂ�0�ɂ��Ȃ���΂Ȃ�Ȃ���������
			//0�ɂ���
			move.x *= 1 - fabsf(Normal[i].x);
			move.y *= 1 - fabsf(Normal[i].y);
			move.z *= 1 - fabsf(Normal[i].z);
			_GameObject.moveVal = move;

			return &_GameObject;
		}
	}

	return nullptr;	// �����o�����s
}
void Collision::Push(std::shared_ptr<GameObject> A, std::shared_ptr<GameObject> B) {
	//---���O�Ɋe�I�u�W�F�N�g�̏��𔲂����
	DirectX::XMFLOAT3 APos = A->pos;
	DirectX::XMFLOAT3 BPos = B->pos;
	DirectX::XMFLOAT3 ASize = A->collSize;
	DirectX::XMFLOAT3 BSize = B->collSize;
	DirectX::XMFLOAT3 AMove = A->moveVal;


	//---�v�Z���𔲂��o��
	//�ړ��ʂ���ړ�������������߂�
	//���ړ���������͂P��-1�ŕ\��
	//�O�����Z�qA ? B : C
	//A�̏������������Ă���B�A�s�����Ȃ�C
	DirectX::XMFLOAT3 dir(
		AMove.x > 0 ? 1 : -1,
		AMove.y > 0 ? 1 : -1,
		AMove.z > 0 ? 1 : -1);
	//����ƈꏏ
	/*
	if (dMove.x > 0)
	{
		dMove.x = 1;
	}
	else
	{
		dMove.y = -1;
	}
	*/

	//���ۂɓ����蔻��̂߂荞�񂾂̂́A
	//�I�u�W�F�N�g�̍��W�ł͂Ȃ��A�p������
	//�߂荞��ł��邱�Ƃ�����̂ŁA
	//�Ώۂ̊p���v�Z�ŋ��߂�
	DirectX::XMFLOAT3 dPoint(
		APos.x + dir.x * ASize.x * 0.5f,
		APos.y + dir.y * ASize.y * 0.5f,
		APos.z + dir.z * ASize.z * 0.5f);
	//�ړ��O�̊p�̍��W
	//���̂܂܂̈ړ��ʂňړ��O�̍��W��\������
	//����ƕ��������_�̌v�Z�덷�ɂ��A�ړ�
	//�x�N�g���̎��_���ʂ̓����ɂ��邱�ƂɂȂ�A
	//�ђʂ��Ă��Ȃ��Ɣ��肳���B
	//�����������邽�߂Ɉړ��̑O�̈ʒu�������Ȃ�
	//�悤�Ɍv�Z�̎������A�ړ��ʂ�傫������
	DirectX::XMFLOAT3 prePoint(
		dPoint.x - AMove.x * 1.1f,
		dPoint.y - AMove.y * 1.1f,
		dPoint.z - AMove.z * 1.1f);
	//�Փ˂��ꂽ���̊p���v�Z
	//�Փ˂��ꂽ���̊p�͈ړ������𔽓]����
	//�����Ɍ������đ傫���̔����𑫂��Ƌ��߂���
	DirectX::XMFLOAT3 sPoint(
		BPos.x - dir.x * BSize.x * 0.5f,
		BPos.y - dir.y * BSize.y * 0.5f,
		BPos.z - dir.z * BSize.z * 0.5f);
	//�ʏ�̈�_����ړ��x�N�g���̎n�_�ƏI�_��
	//�������x�N�g�����v�Z����
	DirectX::XMFLOAT3 vStart(
		prePoint.x - sPoint.x,
		prePoint.y - sPoint.y,
		prePoint.z - sPoint.z);
	DirectX::XMFLOAT3 vEnd(
		dPoint.x - sPoint.x,
		dPoint.y - sPoint.y,
		dPoint.z - sPoint.z);

	//���ꂼ��̖ʂƌv�Z
	//�e�ʂ̖@���x�N�g��
	XMFLOAT3 Normal[] =
	{
		XMFLOAT3(-dir.x, 0, 0),
		XMFLOAT3(0, -dir.y, 0),
		XMFLOAT3(0, 0, -dir.z),
	};
	for (int i = 0; i < _countof(Normal); i++) {
		//�ʂ̖@���Ƃ̓��ς��v�Z
		float dotS =
			vStart.x * Normal[i].x +
			vStart.y * Normal[i].y +
			vStart.z * Normal[i].z;
		float dotE =
			vEnd.x * Normal[i].x +
			vEnd.y * Normal[i].y +
			vEnd.z * Normal[i].z;
		//���ꂼ��̓��ς̌��ʂ����ƕ���
		//����Έړ��x�N�g�����ʂ��ђʂ��Ă���
		//���ƕ��̑g�ݍ��킹���ǂ�����
		//�|���Z�Ŕ���ł���
		//(��x��=��/��x��=��/��x��=��)
		if (dotS * dotE < 0) {
			//�ړ��x�N�g���̕����ɂ߂荞�񂾗ʂ�
			//�߂��̂ł͂Ȃ��A�ʂ̕����Ɍ�������
			//�߂�(�߂������͓��ς̒l�Ɠ�����)
			dotE = fabsf(dotE);
			APos.x += Normal[i].x * dotE;
			APos.y += Normal[i].y * dotE;
			APos.z += Normal[i].z * dotE;
			A->pos = APos;
			//���ׂĂ̈ړ��ʂ�0�ɂ��Ă��܂���
			//�ق��̃I�u�W�F�N�g�Ɖ����o���̌v�Z��
			//����ۂɈړ��ʂ��Ȃ����ƂɂȂ�̂�
			//�ړ��ʂ�0�ɂ��Ȃ���΂Ȃ�Ȃ���������
			//0�ɂ���
			AMove.x *= 1 - fabsf(Normal[i].x);
			AMove.y *= 1 - fabsf(Normal[i].y);
			AMove.z *= 1 - fabsf(Normal[i].z);
			A->moveVal = AMove;
		}
	}
}
