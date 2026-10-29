/* 0x457d00, a child node with the objects of the parent that overlap [min, max] in x and z (0 if none) */

D3QNODE *d3q_MakeChild(D3QNODE *parent, float *min, float *max)
{
    D3QNODE *n;
    D3QITEM *it;
    D3QMAT *m;

    n = pool_Alloc(&d3_QNodePool, 0x48, 0x40);
    for (it = parent->first; it; it = it->next) {
        m = D3Q_MAT(it->obj);
        if (m->min.x <= max[0] && m->max.x >= min[0] && m->min.z <= max[2] && m->max.z >= min[2])
            d3q_AddObject(it->obj, n);
    }
    if (n->count == 0) {
        pool_Free(&d3_QNodePool, n);
        return 0;
    }
    return n;
}
