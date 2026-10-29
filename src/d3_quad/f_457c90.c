/* 0x457c90, add an object that has bounds to the node's list */

char d3q_AddObject(D3OBJECT *obj, D3QNODE *n)
{
    D3QITEM *it;

    if (!obj->material || !D3Q_MAT(obj)->ops || !(obj->flags & 4))
        return 1;
    n->count++;
    it = pool_Alloc(&d3_QItemPool, 8, 0x80);
    it->obj = obj;
    if (n->last) {
        n->last->next = it;
        n->last = it;
    } else {
        n->first = it;
        n->last = it;
    }
    return 1;
}
