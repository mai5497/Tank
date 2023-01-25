//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __MESHFIELD_H__
#define __MESHFIELD_H__

#include "main.h"
#include "mesh.h"

class Texture;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class MeshField {
public:
	MeshField();
	~MeshField();

	HRESULT Init(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
	void Uninit();
	void Update();
	void Draw();

private:
	HRESULT MakeVertexField(ID3D11Device* pDevice,
		int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ);

	static MESH mesh;							// 構造体
	//static std::unique_ptr<Texture> pTexture;
	//std::unique_ptr<Texture> pShaderTex;
	MATERIAL material;					// マテリアル

};

#endif