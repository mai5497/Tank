//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __MESHFIELD_H__
#define __MESHFIELD_H__

//-------------------- インクルード部 --------------------
#include "main.h"
#include "mesh.h"

//-------------------- 前方定義 --------------------
class Texture;

//-------------------- プロトタイプ宣言 --------------------
class MeshField {
public:
	//---関数
	MeshField();
	~MeshField();

	HRESULT Init(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
	void Uninit();
	void Update();
	void Draw();

private:
	//---関数
	HRESULT MakeVertexField(ID3D11Device* pDevice,
		int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ);

	//---変数
	static MESH mesh;					// メッシュ構造体
	MATERIAL material;					// マテリアル
};

#endif