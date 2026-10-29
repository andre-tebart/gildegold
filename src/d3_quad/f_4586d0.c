/* 0x4586d0, takes an object out of the node's list and of its children */

char d3q_Remove(D3OBJECT *obj, D3QNODE *n)
{
    D3QITEM *prev = 0, *it;
    D3QNODE *c;
    unsigned int i;

    if (n->first) {
        for (it = n->first; it; prev = it, it = it->next) {
            if (it->obj == obj)
                break;
        }
        if (!it)
            return 1;
        if (prev)
            prev->next = it->next;
        else
            n->first = it->next;
        if (n->last == it)
            n->last = prev;
        n->count--;
        pool_Free(&d3_QItemPool, it);
        i = 0;
        do {
            c = n->child[i];
            if (c) {
                d3q_Remove(obj, c);
                if (c->count == 0) {
                    pool_Free(&d3_QNodePool, c);
                    n->child[i] = 0;
                }
            }
            i++;
        } while (i < 4);
        obj->flags &= ~4;
    }
    return 1;
}
