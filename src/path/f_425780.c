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
