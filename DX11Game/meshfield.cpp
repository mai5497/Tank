//************************************************************************************
// 
// メッシュ地面[meshfield.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
//-------------------- インクルード部 --------------------
#include "Meshfield.h"
#include "Shader.h"
#include "Texture.h"


//-------------------- 定数定義 --------------------
#define	TEXTURE_FILENAME	(L"data/texture/field004.jpg")		// テクスチャファイル名
#define	BUMP_TEXTURE		(L"data/texture/field004_bump.png")	// テクスチャファイル名

#define M_DIFFUSE			(XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
#define M_SPECULAR			(XMFLOAT4(0.0f,0.0f,0.0f,1.0f))
#define M_POWER				(50.0f)
#define M_AMBIENT			(XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
#define M_EMISSIVE			(XMFLOAT4(0.0f,0.0f,0.0f,1.0f))

#define	VALUE_MOVE			(5.0f)							// 移動量
#define	VALUE_ROTATE		(0.2f)							// 回転量

MESH MeshField::mesh;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
MeshField::MeshField() {

}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
MeshField::~MeshField() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT MeshField::Init(int nNumBlockX, int nNumBlockZ,
	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ) {
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();
	HRESULT hr;

	// 位置・回転の初期設定
	mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マテリアルの初期設定
	material.Diffuse = M_DIFFUSE;
	material.Ambient = M_AMBIENT;
	material.Specular = M_SPECULAR;
	material.Power = M_POWER;
	material.Emissive = M_EMISSIVE;
	mesh.pMaterial = &material;

	// テクスチャの読み込み
	//if(pTexture == nullptr){
	std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();
	//}
	if (mesh.pTexture == nullptr) {
		hr = pTexture->SetTexture(pDevice, TEXTURE_FILENAME);
		mesh.pTexture = pTexture->GetTexture();
	}
	pTexture.reset();

	XMStoreFloat4x4(&mesh.mtxTexture, XMMatrixIdentity());

	// 頂点情報の作成
	hr = MakeVertexField(pDevice, nNumBlockX, nNumBlockZ,
		fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);

	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void MeshField::Uninit(void) {
	// メッシュ解放
	ReleaseMesh(&mesh);
}

//====================================================================================
//
//				更新
//
//====================================================================================
void MeshField::Update(void) {
	// メッシュ更新
	UpdateMesh(&mesh);
}

//====================================================================================
//
//				描画
//
//====================================================================================
void MeshField::Draw(void) {
	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// メッシュの描画
	DrawMesh(pDC, &mesh);
}

//====================================================================================
//
//				頂点の作成
//
//====================================================================================
HRESULT MeshField::MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ) {
	// プリミティブ種別設定
	mesh.primitiveType = PT_TRIANGLESTRIP;

	// 頂点数の設定
	mesh.nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定(縮退ポリゴン用を考慮する)
	mesh.nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// 頂点配列の作成
	VERTEX_3D* pVertexWk = new VERTEX_3D[mesh.nNumVertex];

	// インデックス配列の作成
	int* pIndexWk = new int[mesh.nNumIndex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	for (int z = 0; z < nNumBlockZ + 1; ++z) {
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			// 頂点座標の設定
			pVtx->vtx.x = x * fSizeBlockX - (nNumBlockX * 0.5f) * fSizeBlockX;
			pVtx->vtx.y = 0.0f;
			pVtx->vtx.z = -z * fSizeBlockZ + (nNumBlockZ * 0.5f) * fSizeBlockZ;

			// 法線の設定
			pVtx->nor = XMFLOAT3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//インデックス配列の中身を埋める
	int* pIdx = pIndexWk;
	for (int z = 0; z < nNumBlockZ; ++z) {
		if (z > 0) {
			// 縮退ポリゴンのためのダブりの設定
			*pIdx++ = (z + 1) * (nNumBlockX + 1);
		}
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			*pIdx++ = (z + 1) * (nNumBlockX + 1) + x;
			*pIdx++ = z * (nNumBlockX + 1) + x;
		}
		if (z < nNumBlockZ - 1) {
			// 縮退ポリゴンのためのダブりの設定
			*pIdx++ = z * (nNumBlockX + 1) + nNumBlockX;
		}
	}

	// 頂点バッファ/インデックス バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &mesh, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pVertexWk;
	delete[] pIndexWk;

	return hr;
}
