/* 0x4587b0, builds the tree again from the objects of the scene */

void d3q_Rebuild(D3OBJECT *start, D3QNODE *n)
{
    int i;

    if (n) {
        i = 0;
        do {
            d3q_FreeNode(n->child[i]);
            i++;
        } while (i < 4);
        d3_ForEachObject(d3_DefaultScene, start, n->mask | 0x200, (D3OBJECTCB)d3q_AddObject, n);
        d3q_Build(n, n->minitems, 1, n->maxdepth);
    }
}
