//************************************************************************************
// 
// A*[Astar.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Astar.h"
#include "WallObject.h"
#include "enemy.h"
#include "player.h"

//-------------------- 定数定義 --------------------
#define ARRAY_NUM(a) (sizeof(a)/sizeof(a[0]))
#define NODE_MAX 1000


//-------------------- 構造体 --------------------
struct NODE {
    int i;
    int j;
    int cost;
    struct NODE* parent;
};

struct LIST {
    NODE* node[NODE_MAX];
    int index;
};

//-------------------- グローバル変数 --------------------
int g_Map[MAPHEIGHT][MAPWIDTH];
std::vector<XMINT2> g_root;
XMINT2 g_playerIndex=XMINT2(0,0);
XMINT2 g_enemyIndex;

//-------------------- プロトタイプ宣言 --------------------
NODE* create_node(int i, int j, int cost);
int g(NODE* s, NODE* n);
int h(NODE* e, NODE* n);
void search_node(LIST* open, LIST* close, NODE* s, NODE* e, NODE* n, NODE* m);


//====================================================================================
//
//				ノード作成
//
//====================================================================================
NODE* create_node(int i, int j, int cost) {
    static NODE n[NODE_MAX];
    static int index = 0;
    n[index].i = i;
    n[index].j = j;
    n[index].cost = cost;
    return &n[index++];
}

//====================================================================================
//
//				実コストを返す
//
//====================================================================================
int g(NODE* s, NODE* n) {
    return n->cost;
}

//====================================================================================
//
//				推定コストを返す
//
//====================================================================================
int h(NODE* e, NODE* n) {
    return 0;
}

//====================================================================================
//
//				経路探索
//
//====================================================================================
void search_node(LIST* open, LIST* close, NODE* s, NODE* e, NODE* n, NODE* m) {
    int in_open = -1;
    int in_close = -1;
    int i;
    int fdmcost = g(s, n) + h(e, m) + 1;
    int fsmcost = g(s, m) + h(e, m);

    // mがopenリストに含まれているか
    for (i = 0; i < open->index; i++) {
        if (open->node[i] != NULL && m->i == open->node[i]->i && m->j == open->node[i]->j) {
            in_open = i;
            break;
        }
    }

    // mがcloseリストに含まれているか
    for (i = 0; i < close->index; i++) {
        if (close->node[i] != NULL && m->i == close->node[i]->i && m->j == close->node[i]->j) {
            in_close = i;
            break;
        }
    }

    // m が Openリストにも Closeリストにも含まれていない場合
    if (in_open == -1 && in_close == -1) {
        m->parent = n;
        open->node[open->index++] = m;
    }

    if (in_open > -1) {
        if (fdmcost < fsmcost) {
            m->parent = n;
        }
    }

    if (in_close > -1) {
        if (fdmcost < fsmcost) {
            m->parent = n;
            open->node[open->index++] = m;
            close->node[in_close] = NULL;
        }
    }
}


//====================================================================================
//
//				経路探索
//
//====================================================================================
std::vector<XMINT2> search_Root(XMINT2 _index) {
    FILE* fp;

    /*i = y j = x*/
    int i = 0;
    int j = 0;

    int loop = 0;
    NODE s = { 0,0,0 };
    NODE e = { 0,0,0 };
    LIST open;
    LIST close;
    open.index = 0;
    close.index = 0;

    g_root.push_back(XMINT2(-1, -1));   // ルートは配列の先頭にゴールに近い側の座標が入るため後ろからvectorの読み込みをする
                                        // ルートに-1が入ることはないため、ルートの読み込みが終わった判定に使う

    

    /*マップデータ読込はとりあえずなし*/
    //if (fopen_s(&fp, "map.txt", "r") != 0) {
    //    return 1;
    //}

    //while (fgets(buf, sizeof(buf), fp) != NULL) {
    //    for (j = 0; j < 26; j++) {
    //        map[i][j] = buf[j];
    //    }
    //    i++;
    //}
    //fclose(fp);

    s.i = _index.y;
    s.j = _index.x;
    open.node[open.index++] = &s;

    e.i = g_playerIndex.y;
    e.j = g_playerIndex.x;


    // 既に同じ位置にいる場合計算をスキップ
    if (s.i == e.i && s.j == e.j) {

        std::vector<XMINT2> pRoot = g_root;
        return pRoot;
    }


    while (1) {
        NODE* n = NULL;
        for (i = 0; i < open.index; i++) {
            if (open.node[i] != NULL) {
                int cost = g(&s, open.node[i]);
                if (n == NULL || n->cost > cost) {
                    // ノードの中で一番最小のコストを得る
                    n = open.node[i];
                    open.node[i] = NULL;
                }
            }
        }

        // openからリストがなくなったので終了する
        if (n == NULL) {
            std::vector<XMINT2> pRoot = g_root;
            return pRoot;
        }

        // ゴールが見つかった
        if (n->i == g_playerIndex.y && n->j == g_playerIndex.x) {
            n = n->parent;
            while (n->parent != NULL) {
                // ルート保存
                //*rootIndex = XMINT2(n->i, n->j);
                g_root.push_back(XMINT2(n->j, n->i));

                n = n->parent;
            }

            std::vector<XMINT2> pRoot = g_root;
            return pRoot;
        }

        close.node[close.index++] = n;

        // 上のノードを検索
        if (n->i >= 1 && g_Map[n->i - 1][n->j] == 0 || (n->i - 1 == g_playerIndex.y && n->j == g_playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i - 1, n->j, n->cost + 1));
        }

        // 下のノードを検索
        if (n->i <= MAPHEIGHT - 1 && g_Map[n->i + 1][n->j] == 0 || (n->i + 1 == g_playerIndex.y && n->j == g_playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i + 1, n->j, n->cost + 1));
        }

        // 右のノードを検索
        if (n->j <= MAPWIDTH-1 && g_Map[n->i][n->j + 1] == 0 || (n->i == g_playerIndex.y && n->j + 1 == g_playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i, n->j + 1, n->cost + 1));
        }

        // 左のノードを検索
        if (n->j >= 1 && g_Map[n->i][n->j-1] == 0 || (n->i == g_playerIndex.y &&  n->j - 1 == g_playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i, n->j - 1, n->cost + 1));
        }

        if (loop++ > 1000) {  break; }  // 探索エラー
    }

    std::vector<XMINT2> pRoot = g_root;
    return pRoot;
}

//====================================================================================
//
//				マップの登録
//
//====================================================================================
void SetMap(int *Map) {
    for (int j = 0; j < MAPHEIGHT; j++) {
        for (int i = 0; i < MAPWIDTH; i++) {
            g_Map[j][i] = Map[i+j*MAPWIDTH];
        }
    }
}

//====================================================================================
//
//				プレイヤーの座標をマップの要素番号にしたものを格納
//
//====================================================================================
void SetPlayerIndex(XMINT2 _index) {
    g_playerIndex = _index;
}
