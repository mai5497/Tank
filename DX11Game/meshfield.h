//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __MESHFIELD_H__
#define __MESHFIELD_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"
#include "mesh.h"

//-------------------- �O����` --------------------
class Texture;

//-------------------- �v���g�^�C�v�錾 --------------------
class MeshField {
public:
	//---�֐�
	MeshField();
	~MeshField();

	HRESULT Init(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
	void Uninit();
	void Update();
	void Draw();

private:
	//---�֐�
	HRESULT MakeVertexField(ID3D11Device* pDevice,
		int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ);

	//---�ϐ�
	static MESH mesh;					// ���b�V���\����
	MATERIAL material;					// �}�e���A��
};

#endif