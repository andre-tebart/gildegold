/* 0x457c30, free a node, its item list and its subtree */
void d3q_FreeNode(D3QNODE *n)
{
    D3QITEM *it, *next;
    unsigned int i;

    if (n) {
        for (it = n->first; it; it = next) {
            next = it->next;
            pool_Free(&d3_QItemPool, it);
        }
        for (i = 0; i < 4; i++) {
            if (n->child[i])
                d3q_FreeNode(n->child[i]);
        }
        pool_Free(&d3_QNodePool, n);
    }
}
