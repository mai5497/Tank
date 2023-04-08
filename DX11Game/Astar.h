//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __ASTAR_H__
#define __ASTAR_H__

#include "WallObject.h"
#include "main.h"

std::vector<XMINT2> search_Root(XMINT2 _index);
void SetMap(int* Map);
void SetPlayerIndex(XMINT2 _index);

#endif