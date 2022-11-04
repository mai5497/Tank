//-------------------- インクルード部 --------------------
#include "DebugCollision.h"
#include "AssimpModel.h"
#include "GameObject.h"
#include "mesh.h"

//-------------------- マクロ定義 --------------------
#define M_DIFFUSE		XMFLOAT4(0.0f,1.0f,0.0f,0.3f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,1.0f,0.0f,0.3f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

//#define	VALUE_MOVE_PLAYER	(0.155f)	// 移動速度
//#define	RATE_MOVE_PLAYER	(0.025f)	// 移動慣性係数
//#define	VALUE_ROTATE_PLAYER	(4.5f)		// 回転速度
//#define	RATE_ROTATE_PLAYER	(0.1f)		// 回転慣性係数
//
//#define	PLAYER_RADIUS		(10.0f)		// 境界球半径

//-------------------- グローバル変数定義 --------------------
static MESH			g_mesh;					// メッシュ情報
static MATERIAL		g_material;				// マテリアル

//====================================================================================
//	
//			境界球表示
//
//====================================================================================
void DrawCollisionSphere(GameObject gameobject) {
	ID3D11Device* pDevice = GetDevice();
	
	GameObject _GameObject = gameobject;		// 境界球用ゲームオブジェクト

	// 位置・回転・スケールの初期設定はそれぞれのオブジェクトと同じ位置に描画するので
	// 別途記述はなし

	int nVertex = (32 + 1) * (16 + 1);
	int nFace = 32 * 16 * 2;
	VERTEX_3D* pVertexWk = new VERTEX_3D[nVertex];
	int* pIndexWk = new int[nFace * 3];
	// 頂点、法線ベクトル、テクスチャ座標の位置
	VERTEX_3D* pVtx = pVertexWk;
	for (int j = 0; j <= 16; ++j) {
		float v = (float)j / (float)16;
		float ph = XM_PI * v;
		float y = cosf(ph);
		float r = sinf(ph);
		for (int i = 0; i <= 32; ++i) {
			float u = (float)i / (float)32;
			float th = XM_2PI * u;
			float x = r * cosf(th);
			float z = r * sinf(th);
			//pVtx->vtx.x = x * 80 / _GameObject.m_size.x/2;
			//pVtx->vtx.y = y * 80 / _GameObject.m_size.y/2;
			//pVtx->vtx.z = z * 80 / _GameObject.m_size.z/2;
			pVtx->vtx.x = x * _GameObject.collRadius/20;
			pVtx->vtx.y = y * _GameObject.collRadius/20;
			pVtx->vtx.z = z * _GameObject.collRadius/20;
			pVtx->nor.x = x;
			pVtx->nor.y = y;
			pVtx->nor.z = z;
			pVtx->tex.x = u;
			pVtx->tex.y = v;
			pVtx->diffuse = XMFLOAT4(1, 1, 1, 1);
			++pVtx;
		}
	}
	// 面のインデックス
	int* pIdx = pIndexWk;
	for (int j = 0; j < 16; ++j) {
		for (int i = 0; i < 32; ++i) {
			int nIndex = (32 + 1) * j + i;
			// 左下半分
			*pIdx++ = nIndex + (32 + 1);
			*pIdx++ = nIndex;
			*pIdx++ = nIndex + (32 + 1) + 1;
			// 右上半分
			*pIdx++ = nIndex + 1;
			*pIdx++ = nIndex + (32 + 1) + 1;
			*pIdx++ = nIndex;
		}
	}


	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VERTEX_3D) * nVertex;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pVertexWk;
	pDevice->CreateBuffer(&vbd, &initData, &g_mesh.pVertexBuffer);

	CD3D11_BUFFER_DESC ibd(nFace * 3 * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pIndexWk;
	pDevice->CreateBuffer(&ibd, &initData, &g_mesh.pIndexBuffer);

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 0.0f;
	g_material.Emissive = M_EMISSIVE;

	//InitMesh(pVertexWk, nVertex, pIndexWk, nFace * 3);

	delete[] pIndexWk;
	delete[] pVertexWk;

	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;

	g_mesh.pMaterial = &g_material;
	g_mesh.nNumIndex = nFace * 3;

	XMMATRIX mtxWorld, mtxTranslate, mtxScale;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScale = XMMatrixScaling(_GameObject.size.x, _GameObject.size.y, _GameObject.size.z);
	mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	// 回転を反映
	//mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
	//	XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(_GameObject.pos.x, _GameObject.pos.y, _GameObject.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&_GameObject.mtxWorld, mtxWorld);	// 拡縮回転移動を反映させたワールド座標
	g_mesh.mtxWorld = _GameObject.mtxWorld;	// 反映させたワールド座標をメッシュ情報に保存


	//----- 描画 -----
	ID3D11DeviceContext* pDC = GetDeviceContext();

	g_mesh.primitiveType = PT_TRIANGLE;
	//PT_UNDEFINED = 0,
	//	PT_POINT,
	//	PT_LINE,
	//	PT_LINESTRIP,
	//	PT_TRIANGLE,
	//	PT_TRIANGLESTRIP,

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	DrawMesh(pDC, &g_mesh);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効

	//----- 解放 -----
	ReleaseMesh(&g_mesh);
}


