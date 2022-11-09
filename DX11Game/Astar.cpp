#include "Astar.h"
#include "WallObject.h"
#include "enemy.h"
#include "player.h"

#define ARRAY_NUM(a) (sizeof(a)/sizeof(a[0]))
#define NODE_MAX 1000



struct NODE {
    int i;
    int j;
    int cost;
    struct NODE* parent;
};
typedef struct NODE NODE;

struct LIST {
    NODE* node[NODE_MAX];
    int index;
};
typedef struct LIST LIST;


int *g_Map;
XMINT2 g_root[(MAPHEIGHT-2)*(MAPWIDTH-2)];
XMINT2 g_playerIndex;
XMINT2 g_enemyIndex;

NODE* create_node(int i, int j, int cost);
int g(NODE* s, NODE* n);
int h(NODE* e, NODE* n);
void search_node(LIST* open, LIST* close, NODE* s, NODE* e, NODE* n, NODE* m);






NODE* create_node(int i, int j, int cost) {
    // �Ƃ肠����malloc�Ƃ��g�킸�ɌŒ�
    static NODE n[NODE_MAX];
    static int index = 0;
    n[index].i = i;
    n[index].j = j;
    n[index].cost = cost;
    return &n[index++];
}

int g(NODE* s, NODE* n) {
    return n->cost;
}

int h(NODE* e, NODE* n) {
    return 0;
}

void search_node(LIST* open, LIST* close, NODE* s, NODE* e, NODE* n, NODE* m) {
    int in_open = -1;
    int in_close = -1;
    int i;
    int fdmcost = g(s, n) + h(e, m) + 1;
    int fsmcost = g(s, m) + h(e, m);

    // m��open���X�g�Ɋ܂܂�Ă��邩
    for (i = 0; i < open->index; i++) {
        if (open->node[i] != NULL && m->i == open->node[i]->i && m->j == open->node[i]->j) {
            in_open = i;
            break;
        }
    }

    // m��close���X�g�Ɋ܂܂�Ă��邩
    for (i = 0; i < close->index; i++) {
        if (close->node[i] != NULL && m->i == close->node[i]->i && m->j == close->node[i]->j) {
            in_close = i;
            break;
        }
    }

    // m �� Open���X�g�ɂ� Close���X�g�ɂ��܂܂�Ă��Ȃ��ꍇ
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


XMINT2* search_Root(int enemyNo) {
    FILE* fp;
    //char map[MAPHEIGHT][MAPWIDTH]; // �}�b�v�̑傫���͍l�����Ȃ�
    //char buf[28];

    /*i = y j = x*/
    int i = 0;
    int j = 0;
    int rootIndex = 0;
    int loop = 0;
    NODE s = { 0,0,0 };
    NODE e = { 0,0,0 };
    LIST open;
    LIST close;
    open.index = 0;
    close.index = 0;

    

    /*�}�b�v�f�[�^�Ǎ��͂Ƃ肠�����Ȃ�*/
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

    /*�X�^�[�g�ƃS�[���̓v���C���[�ƃG�l�~�[�������Ă�����̂𒼐ڎ擾����*/
    //for (i = 0; i < MAPHEIGHT; i++) {
    //    for (j = 0; j < MAPWIDTH; j++) {
    //        if (map[i][j] == 'S') {
    //            s.i = i;
    //            s.j = j;
    //            open.node[open.index++] = &s;
    //        }
    //        if (map[i][j] == 'G') {
    //            e.i = i;
    //            e.j = j;
    //        }
    //    }
    //}

    XMINT2 enemyIndex = SetStartIndex(enemyNo);
    s.i = enemyIndex.y;
    s.j = enemyIndex.x;
    open.node[open.index++] = &s;

    XMINT2 playerIndex;/* = SetPlayerIndex();*/
    e.i = playerIndex.y;
    e.j = playerIndex.x;


    while (1) {
        NODE* n = NULL;
        for (i = 0; i < open.index; i++) {
            if (open.node[i] != NULL) {
                int cost = g(&s, open.node[i]);
                if (n == NULL || n->cost > cost) {
                    // �m�[�h�̒��ň�ԍŏ��̃R�X�g�𓾂�
                    n = open.node[i];
                    open.node[i] = NULL;
                }
            }
        }

        // open���烊�X�g���Ȃ��Ȃ����̂ŏI������
        if (n == NULL) {
            //���[�g��������܂���ł���
            break;
        }

        // �S�[������������
        if (n->i == playerIndex.y && n->j == playerIndex.x) {
            n = n->parent;
            while (n->parent != NULL) {
                // ���[�g�ۑ�
                g_root[rootIndex] = XMINT2(n->i,n->j);
                rootIndex++;
                n = n->parent;
            }
            g_root[rootIndex] = XMINT2(-1,-1);    // ���[�g�̗v�f�ԍ����}�C�i�X�ɂȂ邱�Ƃ͂��肦�Ȃ��̂ŁA�Ō���Ƃ��Ĕ��ʂ�����

            XMINT2* pRoot = (XMINT2*)g_root;
            return pRoot;
            break;
        }

        close.node[close.index++] = n;

        // ��̃m�[�h������
        if (n->i >= 1 && g_Map[n->i - 1*MAPWIDTH + n->j] == 0 || (n->i - 1 == playerIndex.y && n->j == playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i - 1, n->j, n->cost + 1));
        }

        // ���̃m�[�h������
        if (n->i <= MAPHEIGHT-1 && g_Map[n->i + 1*MAPWIDTH+n->j] == 0 || (n->i + 1 == playerIndex.y && n->j == playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i + 1, n->j, n->cost + 1));
        }

        // �E�̃m�[�h������
        if (n->j <= MAPWIDTH-1 && g_Map[n->i*MAPWIDTH+n->j + 1] == 0 || (n->i == playerIndex.y && n->j + 1 == playerIndex.x )) {
            search_node(&open, &close, &s, &e, n, create_node(n->i, n->j + 1, n->cost + 1));
        }

        // ���̃m�[�h������
        if (n->j >= 1 && g_Map[n->i*MAPWIDTH+n->j - 1] == 0 || (n->i == playerIndex.y &&  n->j - 1 == playerIndex.x)) {
            search_node(&open, &close, &s, &e, n, create_node(n->i, n->j - 1, n->cost + 1));
        }

        if (loop++ > 1000) {  break; }  // �T���G���[
    }

    //for (i = 0; i < ARRAY_NUM(map); i++) {
    //    for (j = 0; j < 26; j++) {
    //        printf("%c", map[i][j]);
    //    }
    //    printf("\n");
    //}
    return nullptr;
}

void SetMap(int *Map) {
    g_Map = Map;
}