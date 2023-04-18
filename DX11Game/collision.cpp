//************************************************************************************
// 
// 当たり判定[collision.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Collision.h"
#include "GameObject.h"

std::vector<std::shared_ptr<GameObject>> Collision::pObjects;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Collision::Collision() {
	
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Collision::~Collision() {
	int delCnt = pObjects.size();
	for (int i = 0; i < delCnt; i++) {
		pObjects[i].reset();
	}

	pObjects.clear();
}


//====================================================================================
//
//				コリジョンリストへのオブジェクトの追加
//
//====================================================================================
void Collision::AddList(const std::shared_ptr<GameObject>& _gameObject) {
	pObjects.emplace_back(_gameObject);
}


//====================================================================================
//
//				コリジョンリストからオブジェクトの削除
//
//====================================================================================
void Collision::DelList(int _index) {
	pObjects[_index]->hitList.clear();
	pObjects.erase(pObjects.begin() + _index);
}

//====================================================================================
//
//				当たり判定の更新
//
//====================================================================================
void Collision::Update() {
	// それぞれのオブジェクトの当たっているものリスト更新のために
	// 当たっているものリストを初期化する
	for (int i = 0; i < pObjects.size(); i++) {
		pObjects[i]->hitList.clear();
	}

	// 当たり判定
	for (int j = 0; j < pObjects.size(); j++) {
		for (int i = j + 1; i < pObjects.size(); i++) {
			if (pObjects[j]->collType == Collision::STATIC &&
				pObjects[i]->collType == Collision::STATIC) {
				/*
				* 当たり判定のチェックをするオブジェクトが両方うごかないオブジェクトなら
				* チェックの必要がないためスキップする
				*/
				continue;
			} else if (pObjects[j]->isCollision == false ||
				pObjects[i]->isCollision == false) {
				/*
				* 当たり判定が無効になっているオブジェクトは当たり判定を無視する
				*/
				continue;
			}

			if (CollisionAABB(*pObjects[j], *pObjects[i])) {
				if (pObjects[j]->collType == Collision::DYNAMIC &&
					pObjects[i]->collType == Collision::STATIC) {
					Push(pObjects[j], pObjects[i]);
				} else if (pObjects[j]->collType == Collision::STATIC &&
					pObjects[i]->collType == Collision::DYNAMIC) {
					Push(pObjects[i], pObjects[j]);
				} else if (pObjects[j]->collType == Collision::DYNAMIC &&
					pObjects[i]->collType == Collision::DYNAMIC) {
					Push(pObjects[j], pObjects[i]);
					//Push(pObjects[i], pObjects[j]);
				}

				// 当たっているものリストへの追加
				pObjects[i]->hitList.emplace_back(pObjects[j]->myTag);
				pObjects[j]->hitList.emplace_back(pObjects[i]->myTag);
			} /*else {
				pObjects[i]->isHit = false;
				pObjects[j]->isHit = false;
			}*/
		}

	}
}


//====================================================================================
//
//				球の当たり判定
//
//====================================================================================
bool Collision::CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
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
//				AABB同士の当たり判定
//
//====================================================================================
bool Collision::CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

bool Collision::CollisionAABB(GameObject A, GameObject B) {
	return (A.pos.x - A.collSize.x/2 <= B.pos.x + B.collSize.x/2) &&
		(B.pos.x - B.collSize.x/2 <= A.pos.x + A.collSize.x/2) &&
		(A.pos.y - A.collSize.y/2 <= B.pos.y + B.collSize.y/2) &&
		(B.pos.y - B.collSize.y/2 <= A.pos.y + A.collSize.y/2) &&
		(A.pos.z - A.collSize.z/2 <= B.pos.z + B.collSize.z/2) &&
		(B.pos.z - B.collSize.z/2 <= A.pos.z + A.collSize.z/2);
}

//bool Collision::CollisionOBB(GameObject A, GameObject B) {
//	// ワールドマトリックス取得
//	XMFLOAT4X4 mWA = A.mtxWorld;
//	XMFLOAT4X4 mWB = B.mtxWorld;
//	
//	// 各方向ベクトルの確保
//	// （N***:標準化方向ベクトル）
//	// モデルの方向ベクトルを取得,モデルの方向ベクトルとボックスコライダーの長さをかける
//	XMVECTOR NAe1 = XMVectorSet(mWA._11,mWA._12,mWA._13,0.0f), Ae1 = NAe1 * A.collSize.x;
//	XMVECTOR NAe2 = XMVectorSet(mWA._21, mWA._22, mWA._23, 0.0f), Ae2 = NAe2 * A.collSize.y;
//	XMVECTOR NAe3 = XMVectorSet(mWA._31, mWA._32, mWA._33, 0.0f), Ae3 = NAe3 * A.collSize.z;
//	XMVECTOR NBe1 = XMVectorSet(mWB._11, mWB._12, mWB._13, 0.0f), Be1 = NBe1 * B.collSize.x;
//	XMVECTOR NBe2 = XMVectorSet(mWB._21, mWB._22, mWB._23, 0.0f), Be2 = NBe2 * B.collSize.y;
//	XMVECTOR NBe3 = XMVectorSet(mWB._31, mWB._32, mWB._33, 0.0f), Be3 = NBe3 * B.collSize.z;
//	XMVECTOR Interval = XMVectorSet(A.pos.x - B.pos.x, A.pos.y - B.pos.y, A.pos.z - B.pos.z,0.0f);
//
//	// 分離軸 : Ae1
//
//	//vS = NAe1 | NBe1
//	//vL = Ae1 | Be1
//	//vD = Interval
//	FLOAT rA = XMVector3Length(&Ae1);
//	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//
//	FLOAT L;
//	XMStoreFloat3(&L, XMVector3Dot(&Interval, &NAe1));
//	L = fabs(L);
//
//	if (L > rA + rB)
//		return false; // 衝突していない
//
//	 // 分離軸 : Ae2
//	rA = D3DXVec3Length(&Ae2);
//	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Ae3
//	rA = D3DXVec3Length(&Ae3);
//	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Be1
//	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be1);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Be2
//	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : Be3
//	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C11
//	D3DXVECTOR3 Cross;
//	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C12
//	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C13
//	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C21
//	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C22
//	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C23
//	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C31
//	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C32
//	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離軸 : C33
//	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//		return false;
//
//	// 分離平面が存在しないので「衝突している」
//	return true;
//}
//
//// 分離軸に投影された軸成分から投影線分長を算出
//float LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = 0) {
//	// 3つの内積の絶対値の和で投影線分長を計算
//	// 分離軸Sepは標準化されていること
//	float r1, r2, r3;
//	XMStoreFloat(&r1, XMVector3Dot(*Sep, *e1));
//	r1 = fabs(r1);
//	XMStoreFloat(&r2, XMVector3Dot(*Sep, *e2));
//	r2 = fabs(r2);
//	XMStoreFloat(&r3, XMVector3Dot(*Sep, *e3));
//	FLOAT r3 = e3 ? (fabs(r3)) : 0;
//	return r1 + r2 + r3;
//}
//
//


//====================================================================================
//
//				オブジェクトの押出処理
//
//====================================================================================
GameObject* Collision::Push(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 move, XMFLOAT3 Bpos, XMFLOAT3 Bsize){
	//---事前に各オブジェクトの情報を抜き取る
	//DirectX::XMFLOAT3 dPos = pDynamic->GetPos();
	//DirectX::XMFLOAT3 sPos = pStatic->GetPos();
	//DirectX::XMFLOAT3 dSize = pDynamic->GetSize();
	//DirectX::XMFLOAT3 sSize = pStatic->GetSize();
	//DirectX::XMFLOAT3 dMove = pDynamic->GetMove();

	// 戻り値用ゲームオブジェクト定義
	GameObject _GameObject;


	//---計算情報を抜き出す
	//移動量から移動する方向を求める
	//※移動する方向は１か-1で表す
	//三項演算子A ? B : C
	//Aの条件が成立してたらB、不成立ならC
	DirectX::XMFLOAT3 dir(
		move.x > 0 ? 1 : -1,
		move.y > 0 ? 1 : -1,
		move.z > 0 ? 1 : -1);
	//これと一緒
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

	//実際に当たり判定のめり込んだのは、
	//オブジェクトの座標ではなく、角だけが
	//めり込んでいることもあるので、
	//対象の角を計算で求める
	DirectX::XMFLOAT3 dPoint(
		Apos.x + dir.x * Asize.x * 0.5f,
		Apos.y + dir.y * Asize.y * 0.5f,
		Apos.z + dir.z * Asize.z * 0.5f);
	//移動前の角の座標
	//そのままの移動量で移動前の座標を表そうと
	//すると浮動小数点の計算誤差により、移動
	//ベクトルの視点が面の内側にあることになり、
	//貫通していないと判定される。
	//これを回避するために移動の前の位置が遠くなる
	//ように計算の時だけ、移動量を大きくする
	DirectX::XMFLOAT3 prePoint(
		dPoint.x - move.x * 1.1f,
		dPoint.y - move.y * 1.1f,
		dPoint.z - move.z * 1.1f);
	//衝突された側の角を計算
	//衝突された側の角は移動方向を反転した
	//方向に向かって大きさの半分を足すと求められる
	DirectX::XMFLOAT3 sPoint(
		Bpos.x - dir.x * Bsize.x * 0.5f,
		Bpos.y - dir.y * Bsize.y * 0.5f,
		Bpos.z - dir.z * Bsize.z * 0.5f);
	//面上の一点から移動ベクトルの始点と終点へ
	//向かうベクトルを計算する
	DirectX::XMFLOAT3 vStart(
		prePoint.x - sPoint.x,
		prePoint.y - sPoint.y,
		prePoint.z - sPoint.z);
	DirectX::XMFLOAT3 vEnd(
		dPoint.x - sPoint.x,
		dPoint.y - sPoint.y,
		dPoint.z - sPoint.z);

	//それぞれの面と計算
	//各面の法線ベクトル
	DirectX::XMFLOAT3 Normal[] =
	{
		DirectX::XMFLOAT3(-dir.x, 0, 0),
		DirectX::XMFLOAT3(0, -dir.y, 0),
		DirectX::XMFLOAT3(0, 0, -dir.z),
	};
	for (int i = 0; i < _countof(Normal); i++) {
		//面の法線との内積を計算
		float dotS =
			vStart.x * Normal[i].x +
			vStart.y * Normal[i].y +
			vStart.z * Normal[i].z;
		float dotE =
			vEnd.x * Normal[i].x +
			vEnd.y * Normal[i].y +
			vEnd.z * Normal[i].z;
		//それぞれの内積の結果が正と負で
		//あれば移動ベクトル画面を貫通している
		//正と負の組み合わせかどうかは
		//掛け算で判定できる
		//(正x負=負/正x正=正/負x負=正)
		if (dotS * dotE < 0) {
			//移動ベクトルの方向にめり込んだ量を
			//戻すのではなく、面の方向に向かって
			//戻す(戻す距離は内積の値と等しい)
			dotE = fabsf(dotE);
			Apos.x += Normal[i].x * dotE;
			Apos.y += Normal[i].y * dotE;
			Apos.z += Normal[i].z * dotE;
			_GameObject.pos = Apos;
			//すべての移動量を0にしてしまうと
			//ほかのオブジェクトと押し出しの計算を
			//する際に移動量がないことになるので
			//移動量を0にしなければならない成分だけ
			//0にする
			move.x *= 1 - fabsf(Normal[i].x);
			move.y *= 1 - fabsf(Normal[i].y);
			move.z *= 1 - fabsf(Normal[i].z);
			_GameObject.moveVal = move;

			return &_GameObject;
		}
	}

	return nullptr;	// 押し出し失敗
}
void Collision::Push(std::shared_ptr<GameObject> A, std::shared_ptr<GameObject> B) {
	//---事前に各オブジェクトの情報を抜き取る
	DirectX::XMFLOAT3 APos = A->pos;
	DirectX::XMFLOAT3 BPos = B->pos;
	DirectX::XMFLOAT3 ASize = A->collSize;
	DirectX::XMFLOAT3 BSize = B->collSize;
	DirectX::XMFLOAT3 AMove = A->moveVal;


	//---計算情報を抜き出す
	//移動量から移動する方向を求める
	//※移動する方向は１か-1で表す
	//三項演算子A ? B : C
	//Aの条件が成立してたらB、不成立ならC
	DirectX::XMFLOAT3 dir(
		AMove.x > 0 ? 1 : -1,
		AMove.y > 0 ? 1 : -1,
		AMove.z > 0 ? 1 : -1);
	//これと一緒
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

	//実際に当たり判定のめり込んだのは、
	//オブジェクトの座標ではなく、角だけが
	//めり込んでいることもあるので、
	//対象の角を計算で求める
	DirectX::XMFLOAT3 dPoint(
		APos.x + dir.x * ASize.x * 0.5f,
		APos.y + dir.y * ASize.y * 0.5f,
		APos.z + dir.z * ASize.z * 0.5f);
	//移動前の角の座標
	//そのままの移動量で移動前の座標を表そうと
	//すると浮動小数点の計算誤差により、移動
	//ベクトルの視点が面の内側にあることになり、
	//貫通していないと判定される。
	//これを回避するために移動の前の位置が遠くなる
	//ように計算の時だけ、移動量を大きくする
	DirectX::XMFLOAT3 prePoint(
		dPoint.x - AMove.x * 1.1f,
		dPoint.y - AMove.y * 1.1f,
		dPoint.z - AMove.z * 1.1f);
	//衝突された側の角を計算
	//衝突された側の角は移動方向を反転した
	//方向に向かって大きさの半分を足すと求められる
	DirectX::XMFLOAT3 sPoint(
		BPos.x - dir.x * BSize.x * 0.5f,
		BPos.y - dir.y * BSize.y * 0.5f,
		BPos.z - dir.z * BSize.z * 0.5f);
	//面上の一点から移動ベクトルの始点と終点へ
	//向かうベクトルを計算する
	DirectX::XMFLOAT3 vStart(
		prePoint.x - sPoint.x,
		prePoint.y - sPoint.y,
		prePoint.z - sPoint.z);
	DirectX::XMFLOAT3 vEnd(
		dPoint.x - sPoint.x,
		dPoint.y - sPoint.y,
		dPoint.z - sPoint.z);

	//それぞれの面と計算
	//各面の法線ベクトル
	XMFLOAT3 Normal[] =
	{
		XMFLOAT3(-dir.x, 0, 0),
		XMFLOAT3(0, -dir.y, 0),
		XMFLOAT3(0, 0, -dir.z),
	};
	for (int i = 0; i < _countof(Normal); i++) {
		//面の法線との内積を計算
		float dotS =
			vStart.x * Normal[i].x +
			vStart.y * Normal[i].y +
			vStart.z * Normal[i].z;
		float dotE =
			vEnd.x * Normal[i].x +
			vEnd.y * Normal[i].y +
			vEnd.z * Normal[i].z;
		//それぞれの内積の結果が正と負で
		//あれば移動ベクトルが面を貫通している
		//正と負の組み合わせかどうかは
		//掛け算で判定できる
		//(正x負=負/正x正=正/負x負=正)
		if (dotS * dotE < 0) {
			//移動ベクトルの方向にめり込んだ量を
			//戻すのではなく、面の方向に向かって
			//戻す(戻す距離は内積の値と等しい)
			dotE = fabsf(dotE);
			APos.x += Normal[i].x * dotE;
			APos.y += Normal[i].y * dotE;
			APos.z += Normal[i].z * dotE;
			A->pos = APos;
			//すべての移動量を0にしてしまうと
			//ほかのオブジェクトと押し出しの計算を
			//する際に移動量がないことになるので
			//移動量を0にしなければならない成分だけ
			//0にする
			AMove.x *= 1 - fabsf(Normal[i].x);
			AMove.y *= 1 - fabsf(Normal[i].y);
			AMove.z *= 1 - fabsf(Normal[i].z);
			A->moveVal = AMove;
		}
	}
}
