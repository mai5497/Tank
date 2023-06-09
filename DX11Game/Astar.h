//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __ASTAR_H__
#define __ASTAR_H__

//-------------------- インクルード部 --------------------
#include "WallObject.h"
#include "main.h"

//-------------------- プロトタイプ宣言 --------------------
std::vector<XMINT2> search_Root(XMINT2 _index);
void SetMap(int* Map);
void SetPlayerIndex(XMINT2 _index);

#endif