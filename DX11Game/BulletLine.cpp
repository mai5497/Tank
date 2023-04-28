#include "BulletLine.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "player.h"
#include "input.h"

//-------------------- 定数定義 --------------------
#define	TEX_FILENAME	(L"data/texture/FX_BulletLine.png")	// テクスチャ ファイル名
#define LINE_WIDTH				(50)
#define LINE_HEIGHT				(10)

#define M_DIFFUSE				XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER					(1.0f)
#define M_AMBIENT				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)


BulletLine::BulletLine() {

}

BulletLine::~BulletLine() {

}

void BulletLine::Init(GameObject *_pObj) {
	pObj = _pObj;

	ID3D11Device* pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	// マテリアルの設定
	mat.Diffuse = M_DIFFUSE;
	mat.Ambient = M_AMBIENT;
	mat.Specular = M_SPECULAR;
	mat.Power = M_POWER;
	mat.Emissive = M_EMISSIVE;
	line.pMaterial = &mat;
	// テクスチャの読み込み
	std::unique_ptr<Texture> pTexture_p = std::make_unique<Texture>();
	HRESULT isLoad_p = pTexture_p->SetTexture(pDevice, TEX_FILENAME);
	if (FAILED(isLoad_p)) {
		MessageBox(NULL, _T("弾予測線テクスチャ読み込み失敗"), _T("error"), MB_OK);
	}
	line.pTexture = pTexture_p->GetTexture();

	XMStoreFloat4x4(&line.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&line.mtxWorld, XMMatrixIdentity());

	dir = XMFLOAT3(-pObj->mtxWorld._31, -pObj->mtxWorld._32, -pObj->mtxWorld._33);
	pos = XMFLOAT3(pObj->pos.x + dir.x*LINE_WIDTH, 5.0f, pObj->pos.z + dir.z*LINE_WIDTH);
	angle = XMFLOAT3(90.0f, pObj->rotModel.y + 90.0f, pObj->rotModel.z);

	XMStoreFloat4x4(&line.mtxWorld, XMMatrixTranslation(pos.x,pos.y,pos.z));
}

void BulletLine::Uninit() {
	ReleaseMesh(&line);
}

void BulletLine::Update() {
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));	// 正規化
	pos = XMFLOAT3(pObj->pos.x  + dir.x * LINE_WIDTH , 5.0f, pObj->pos.z + dir.z * LINE_WIDTH);
	//pos = XMFLOAT3(0.0, 5.0f, 0.0);
	
	float radian = atan2f(dir.z, dir.x);
	float degree = radian * 180 / 3.14f;
	angle = XMFLOAT3(90.0f, -degree, pObj->rotModel.z);

	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate;

	// ワールドマトリックスの初期化
	_mtxWorld = XMMatrixIdentity();

	// 回転を反映
	_mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(angle.x),
		XMConvertToRadians(angle.y), XMConvertToRadians(angle.z));
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

	// 移動を反映
	_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&line.mtxWorld, _mtxWorld);

}

void BulletLine::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	// 光源無効
	SetBlendState(BS_ALPHABLEND);	// αブレンディング有効
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	SetZBuffer(true);// Zバッファ有効

	// 描画
	DrawMesh(pDC, &line);

	// 元に戻す
	SetZBuffer(false);// Zバッファ無効
	SetBlendState(BS_NONE);		// αブレンディング無効
	CLight::Get()->SetEnable();	// 光源有効
}

//====================================================================================
//
//				角度を保存
//
//====================================================================================
void BulletLine::SetDir(XMFLOAT3 _dir) {
	dir = _dir;
}

//====================================================================================
//
//				頂点情報の作成
//
//====================================================================================
void BulletLine::MakeVertexBullet(ID3D11Device* pDevice) {
	// 一時的な頂点配列を生成
	line.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[line.nNumVertex];

	line.primitiveType = PT_TRIANGLESTRIP;

	// 頂点配列の中身を埋める
	pVertexWk[0].vtx = XMFLOAT3(-LINE_WIDTH, LINE_HEIGHT, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(LINE_WIDTH, LINE_HEIGHT, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-LINE_WIDTH, -LINE_HEIGHT, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(LINE_WIDTH, -LINE_HEIGHT, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// 一時的なインデックス配列を生成
	line.nNumIndex = 4;
	int* pIndexWk = new int[4];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ/インデックス バッファ生成
	MakeMeshVertex(pDevice, &line, pVertexWk, pIndexWk);

	// 一時的な頂点配列/インデックス配列を解放
	delete[] pIndexWk;
	delete[] pVertexWk;
}
