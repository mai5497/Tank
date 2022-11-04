//************************************************************************************
// 
// �����蔻��[collision.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "collision.h"
#include "GameObject.h"

//====================================================================================
//
//				���̓����蔻��
//
//====================================================================================
bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

bool CollisionSphere(GameObject A, GameObject B) {
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
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}


//====================================================================================
//
//				�I�u�W�F�N�g�̉��o����
//
//====================================================================================
GameObject* Push(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 move, XMFLOAT3 Bpos, XMFLOAT3 Bsize){
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
