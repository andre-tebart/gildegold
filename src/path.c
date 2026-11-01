/* path.c -- original file name unknown.
   A* path search on a square grid: 8 neighbours, terrain cost per node type and unit type, Manhattan
   distance heuristic, open list as a sorted linked list in the nodes. path_Search2 grows the search
   from both ends and stops where they meet. Extent in GildeGold.exe (inferred, not proven):
   0x424f30..0x425bc0. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/path.c); boundaries in decl/text.dcl */
#include <stdlib.h>
#include "path.h"

/* 0x424f30, finds a path on map from (sx, sy) to (ex, ey) for the mover type, with the search path_Mode selects;
   writes at most out->max cells (x, y) from the node the search returns back along the parents and returns that node, 0xffff
   when the target is off the map or not reached */
unsigned short path_Find(PATHOUT *out, PMAP *map, int sx, int sy, int ex, int ey, int type)
{
    unsigned short start, n;

    if (map && ex <= map->size && ey <= map->size && ex >= 0 && ey >= 0) {
        out->count = 0;
        if (path_Mode) {
            start = path_Search1(map, sx, sy, ex, ey, type);
            if (start != 0xffff)
                for (n = start; out->count < out->max && n != 0xffff; n = map->nodes[n].parent) {
                    out->xy[out->count * 2] = path_Mask & n;
                    out->xy[out->count * 2 + 1] = n >> path_Shift;
                    out->count++;
                }
        } else {
            start = path_Search2(map, sx, sy, ex, ey, type);
            if (start != 0xffff)
                for (n = start; out->count < out->max && n != 0xffff; n = map->nodes[n].parent) {
                    out->xy[out->count * 2] = path_Mask & n;
                    out->xy[out->count * 2 + 1] = n >> path_Shift;
                    out->count++;
                }
        }
        return start;
    }
    return 0xffff;
}

#define PATH_INITNODE(nd, fl) {                                     \
    (nd)->stamp = path_Stamp;                                       \
    (nd)->g = 0.0f;                                                 \
    (nd)->h = 0.0f;                                                 \
    (nd)->f = (nd)->h + (nd)->g;                                    \
    (nd)->flags = (fl);                                             \
    (nd)->next = 0xffff;                                            \
    (nd)->prev = 0xffff;                                            \
    (nd)->parent = 0xffff;                                          \
}

/* 0x4250a0, bidirectional A*: grows the search alternately from the start and the goal node until
   one search reaches the other (path_Meet) and returns the chain to the caller; the start node
   index, or 0xffff when no path exists. Sets the path_ globals for the running search.
   @deep behaviour only: size 976 = original, instruction multiset differs only by register allocation (zero reg ebx vs edi, ebp/edx/ecx choices) and the double-compare idiom (and eax,0x100 vs test ah,0x5/jnp, loop and eax,0x4100); readable met */
unsigned short path_Search2(PMAP *map, int sx, int sy, short ex, int ey, int type)
{
    int k;
    unsigned int a;
    unsigned short b;

    path_Shift = 0;
    path_Mask = map->size - 1;
    for (k = map->size; k >>= 1; )
        path_Shift++;
    path_DY[0] = 0;
    path_DY[1] = -map->size;
    path_DY[2] = 0;
    path_DY[3] = map->size;
    path_DY[4] = -map->size;
    path_DY[5] = -map->size;
    path_DY[6] = map->size;
    path_DY[7] = map->size;
    path_Count = 0;
    a = (sy << path_Shift) + sx;
    path_Start = a;
    path_Goal = (short)(ey << path_Shift) + ex;
    path_Stamp++;
    PATH_INITNODE(&map->nodes[(unsigned short)a], 5);
    if (path_Start != path_Goal) {
        b = path_Goal;
        PATH_INITNODE(&map->nodes[b], 9);
        path_Tab1Sel = path_Tab1[type];
        path_Tab2Sel = path_Tab2[type];
        path_Costs = path_Terrain[type];
        if (path_Costs[map->nodes[(unsigned short)a].type] >= 0.0 && path_Costs[map->nodes[b].type] >= 0.0) {
            path_Weight = 0.0f;
            for (k = 1; k < 15; k++)
                if (path_Costs[k] >= 0.0)
                    path_Weight += path_Costs[k];
            path_Weight = 7.0f;
            path_Target = path_Goal;
            path_SetFlag = 4;
            path_TestFlag = 8;
            a = path_Expand(map, a);
            while ((short)a != -1) {
                if ((unsigned short)a == path_Goal) {
                    path_Reverse(map, a);
                    return path_Start;
                }
                if ((map->nodes[(unsigned short)a].flags & 0xc) == 0xc) {
                    path_Reverse(map, a);
                    map->nodes[(unsigned short)a].parent = path_Meet;
                    return path_Start;
                }
                path_SetFlag = 8;
                path_TestFlag = 4;
                path_Target = path_Start;
                b = path_Expand(map, b);
                if (b == 0xffff)
                    break;
                if (b == path_Start)
                    return path_Start;
                if ((map->nodes[b].flags & 0xc) == 0xc) {
                    path_Reverse(map, path_Meet);
                    map->nodes[path_Meet].parent = b;
                    return path_Start;
                }
                path_Count++;
                path_Target = path_Goal;
                path_SetFlag = 4;
                path_TestFlag = 8;
                a = path_Expand(map, a);
            }
        }
        a = 0xffff;
    }
    return a;
}
/* 0x425470, one-directional A*: expands from the start node alone until it reaches the goal;
   the start node index, or 0xffff when no path exists. Sets the path_ globals like path_Search2.
   @deep behaviour only: size 784 = original, remaining code hunks are register allocation and the same double-compare idiom (and eax,0x100 vs test ah,0x5/jnp, loop and eax,0x4100) as path_Search2; readable met */

unsigned short path_Search1(PMAP *map, int sx, int sy, short ex, int ey, int type)
{
    int k;
    unsigned int a;
    unsigned short b;

    path_Shift = 0;
    path_Mask = map->size - 1;
    for (k = map->size; k >>= 1; )
        path_Shift++;
    path_DY[0] = 0;
    path_DY[1] = -map->size;
    path_DY[2] = 0;
    path_DY[3] = map->size;
    path_DY[4] = -map->size;
    path_DY[5] = -map->size;
    path_DY[6] = map->size;
    path_DY[7] = map->size;
    path_Count = 0;
    a = (sy << path_Shift) + sx;
    path_Start = a;
    path_Goal = (short)(ey << path_Shift) + ex;
    path_Stamp++;
    PATH_INITNODE(&map->nodes[(unsigned short)a], 5);
    if (path_Start != path_Goal) {
        b = path_Goal;
        PATH_INITNODE(&map->nodes[b], 9);
        path_Tab1Sel = path_Tab1[type];
        path_Tab2Sel = path_Tab2[type];
        path_Costs = path_Terrain[type];
        if (path_Costs[map->nodes[(unsigned short)a].type] >= 0.0 && path_Costs[map->nodes[b].type] >= 0.0) {
            path_Weight = 0.0f;
            for (k = 1; k < 15; k++)
                if (path_Costs[k] >= 0.0)
                    path_Weight += path_Costs[k];
            path_Weight = 7.0f;
            path_Target = path_Goal;
            path_SetFlag = 4;
            path_TestFlag = 8;
            a = path_Expand(map, a);
            while ((short)a != -1) {
                if ((unsigned short)a == path_Goal) {
                    path_Reverse(map, a);
                    return path_Start;
                }
                if ((map->nodes[(unsigned short)a].flags & 0xc) == 0xc) {
                    path_Reverse(map, a);
                    map->nodes[(unsigned short)a].parent = path_Meet;
                    return path_Start;
                }
                path_Count++;
                path_SetFlag = 4;
                path_TestFlag = 8;
                path_Target = path_Goal;
                a = path_Expand(map, a);
            }
        }
        a = 0xffff;
    }
    return a;
}
/* 0x425780, expands node n of map: flags it closed, tests its 8 neighbours against the other
   search (path_TestFlag), relaxes their g/f and keeps the open list sorted by f; returns the open
   list head, or (unsigned short)n when it met the other search (path_Meet is set).
   @deep behaviour only: after the double 0.0 and counting-for fixes the remaining hunks are register allocation and stack slot layout (our frame sub esp,0x14 vs original sub esp,0x10, slots 16); readable met */

unsigned int path_Expand(PMAP *map, unsigned short n)
{
    PNODE *cur, *nb, *p;
    unsigned int head, k, q;
    int dir;
    int dx, dy;
    float g, h, f;

    cur = &map->nodes[(unsigned short)n];
    head = cur->next;
    cur->flags = cur->flags & 0xfe | 2;
    if (head != 0xffff)
        map->nodes[head].prev = cur->prev;
    for (dir = 0; dir < 8; dir++) {
        k = (unsigned short)(path_DY[dir & 7] + path_DX[dir & 7] + n);
        nb = &map->nodes[k];
        if (nb->stamp == path_Stamp && (nb->flags & path_TestFlag)) {
            nb->flags |= path_SetFlag;
            cur->flags |= path_TestFlag;
            path_Meet = k;
            return (unsigned short)n;
        }
        g = path_Step[dir] * path_Costs[nb->type] + cur->g;
        if (path_Costs[nb->type] >= 0.0) {
            if (nb->stamp == path_Stamp) {
                if (nb->g <= g)
                    continue;
                if (nb->stamp != path_Stamp || !(nb->flags & 1))
                    goto calc;
                h = nb->h;
                if (nb->prev == 0xffff)
                    head = nb->next;
                else
                    map->nodes[nb->prev].next = nb->next;
                if (nb->next != 0xffff)
                    map->nodes[nb->next].prev = nb->prev;
            } else {
calc:
                dx = (path_Target >> path_Shift) - (k >> path_Shift);
                dy = (path_Target & path_Mask) - (k & path_Mask);
                h = (float)(abs(dx) + abs(dy)) * path_Weight;
            }
            f = h + g;
            nb->parent = (unsigned short)n;
            nb->g = g;
            nb->f = f;
            nb->h = h;
            if (nb->stamp != path_Stamp || !(nb->flags & 2)) {
                nb->stamp = path_Stamp;
                nb->flags &= ~2;
                nb->flags = nb->flags & ~path_TestFlag | 1;
                nb->flags |= path_SetFlag;
                if ((short)head == -1) {
                    nb->prev = 0xffff;
                    nb->next = 0xffff;
                    head = k;
                } else {
                    q = head;
                    p = &map->nodes[(unsigned short)head];
                    while (p->next != 0xffff) {
                        if (f < p->f) {
                            nb->next = q;
                            nb->prev = p->prev;
                            if (p->prev != 0xffff)
                                map->nodes[p->prev].next = (unsigned short)k;
                            else
                                head = k;
                            p->prev = (unsigned short)k;
                            break;
                        }
                        q = p->next;
                        p = &map->nodes[q];
                    }
                    if (map->nodes[(unsigned short)q].next == 0xffff) {
                        nb->prev = q;
                        nb->next = 0xffff;
                        map->nodes[(unsigned short)q].next = (unsigned short)k;
                    }
                }
            }
            if ((unsigned short)k == path_Target)
                return head;
        }
    }
    return head;
}
/* 0x425af0, turns the parent chain that ends at node `last` into a forward next chain: the chain
   nodes get parent = next from `last` back to the root.
   @deep behaviour only: the parameter mask, the parent-chain walk and the next-chain walk are the same instructions, the remaining hunks are register allocation (esi/edi/ecx) and the 0xffff constant register; readable met */

void path_Reverse(PMAP *map, unsigned int last)
{
    unsigned int cur, p;

    cur = last & 0xffff;
    if (map->nodes[cur].parent != 0xffff) {
        map->nodes[cur].next = 0xffff;
        while (cur != 0xffff) {
            p = map->nodes[cur].parent;
            if (p != 0xffff) {
                map->nodes[p].next = cur;
                p = map->nodes[cur].parent;
                last = p;
            }
            cur = p;
        }
        while (last != 0xffff) {
            map->nodes[last].parent = map->nodes[last].next;
            last = map->nodes[last].next;
        }
    }
}

int path_Mode = 0;  /* 0x633184, 1 = one-directional search */
unsigned char path_SetFlag = 0;  /* 0x1460aa0 */
float * path_Costs = 0;  /* 0x1460aa4, path_Terrain[type] of the running search */
unsigned char path_TestFlag = 0;  /* 0x1460aa8 */
float path_Tab1Sel = 0;  /* 0x1460aac, path_Tab1[type], written per search, never read */
unsigned short path_Count = 0;  /* 0x1460ab0 */
short path_Stamp = 0;  /* 0x1460ab2 */
unsigned short path_Goal = 0;  /* 0x1460ab4 */
unsigned short path_Start = 0;  /* 0x1460ab6 */
float path_Tab2Sel = 0;  /* 0x1460ab8, path_Tab2[type], written per search, never read */
unsigned short path_Target = 0;  /* 0x1460abc */
unsigned short path_Meet = 0;  /* 0x1460abe */
float path_Weight = 0;  /* 0x1460ac0 */
int path_Mask = 63;  /* 0x5f7f60, size - 1 */
int path_Shift = 6;  /* 0x5f7f64, log2(size) */
short path_DX[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };  /* 0x5f7f68 */
short path_DY[8] = { 0, -64, 0, 64, -64, -64, 64, 64 };  /* 0x5f7f78, set per search from the grid size */
float path_Step[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.41420996f, 1.41420996f, 1.41420996f, 1.41420996f };  /* 0x5f7f88, 1 or sqrt(2) */
float path_Terrain[][15] = { { 999.0f, 20.0f, 30.0f, 30.0f, 30.0f, 30.0f, 1.0f, 30.0f, 40.0f, 30.0f, -1.0f, 10.0f, 0.5f, -1.0f, 99.0f }, { 99.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.25f, 0.75f, -1.0f, 99.0f }, { 99.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 1.0f, 50.0f, 60.0f, 50.0f, -1.0f, 10.0f, 0.5f, -1.0f, 99.0f }, { 99.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 1.0f, 50.0f, 50.0f, 50.0f, -1.0f, 1.0f, 1.0f, -1.0f, 99.0f }, { 99.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 99.0f, 1.0f, 1.0f, -1.0f, 1.0f }, { 999.0f, 99.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 99.0f, 10.0f, 0.5f, -1.0f, 99.0f } };  /* 0x5f7fa8, cost per type (6 rows); < 0 = impassable */
float path_Tab1[6] = { 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.0f };  /* 0x5f8110, per type: 0.25, 0.0 for one type */
float path_Tab2[6] = { 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.0f };  /* 0x5f8128, same shape as path_Tab1 */
