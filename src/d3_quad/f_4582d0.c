/* 0x4582d0, makes a tree over the objects of the default scene that match mask */

D3QNODE *d3q_Create(D3OBJECT *start, unsigned int mask, unsigned int minitems, unsigned int maxdepth)
{
    D3QNODE *n;
    D3QITEM *it;

    n = pool_Alloc(&d3_QNodePool, 0x48, 0x40);
    n->mask = mask;
    n->minitems = minitems;
    n->maxdepth = maxdepth;
    d3_ForEachObject(d3_DefaultScene, start, mask, (D3OBJECTCB)d3q_AddObject, n);
    d3q_Build(n, minitems, 1, maxdepth);
    if (n->count == 0) {
        pool_Free(&d3_QNodePool, n);
        return 0;
    }
    for (it = n->first; it; it = it->next)
        D3Q_MAT(it->obj)->zone = -1;
    return n;
}
