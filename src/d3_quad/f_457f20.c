/* 0x457f20, fills the node's bounds from its objects (depth 1: exact object bounds via d3q_GetBounds, deeper: the
   cached bounds in the material) and, below maxdepth and above minitems objects, splits it into four quadrants
   (x/z plane) and builds them recursively. A child that holds the same object count as its siblings (the objects
   straddle every split) is useless and freed again. depth / maxdepth are volatile: the original re-reads them
   from the stack (matching trick). The statement orders of the four child boxes are needed for the bytes. */

void d3q_Build(D3QNODE *n, unsigned int minitems, volatile unsigned int depth, volatile unsigned int maxdepth)
{
    char same = 1;
    D3QITEM *it;
    D3VECTOR4 bmin, bmax;
    D3VECTOR4 lo, hi, half;
    int cnt;
    unsigned int i;

    if (n->count > 0) {
        n->min.x = 1e10f;
        n->min.y = 1e10f;
        n->min.z = 1e10f;
        n->max.x = -1e10f;
        n->max.y = -1e10f;
        n->max.z = -1e10f;
        if (depth <= 1) {
            for (it = n->first; it; it = it->next) {
                if (d3q_GetBounds(it->obj, &bmin.x, &bmax.x)) {
                    n->min.x = min(n->min.x, bmin.x);
                    n->min.y = min(n->min.y, bmin.y);
                    n->min.z = min(n->min.z, bmin.z);
                    n->max.x = max(n->max.x, bmax.x);
                    n->max.y = max(n->max.y, bmax.y);
                    n->max.z = max(n->max.z, bmax.z);
                }
            }
        } else {
            for (it = n->first; it; it = it->next) {
                n->min.x = min(n->min.x, D3Q_MAT(it->obj)->min.x);
                n->min.y = min(n->min.y, D3Q_MAT(it->obj)->min.y);
                n->min.z = min(n->min.z, D3Q_MAT(it->obj)->min.z);
                n->max.x = max(n->max.x, D3Q_MAT(it->obj)->max.x);
                n->max.y = max(n->max.y, D3Q_MAT(it->obj)->max.y);
                n->max.z = max(n->max.z, D3Q_MAT(it->obj)->max.z);
            }
        }
    }
    if (depth < maxdepth && (unsigned int)n->count > minitems) {
        lo.x = n->min.x;
        cnt = -1;
        half.x = n->max.x - n->min.x;
        half.z = n->max.z - n->min.z;
        half.x *= 0.5f;
        lo.z = n->min.z;
        half.z *= 0.5f;
        hi.x = half.x + n->min.x;
        hi.z = half.z + n->min.z;
        n->child[0] = d3q_MakeChild(n, &lo.x, &hi.x);
        lo.x = half.x + n->min.x;
        lo.z = n->min.z;
        hi.x = n->max.x;
        hi.z = half.z + n->min.z;
        n->child[1] = d3q_MakeChild(n, &lo.x, &hi.x);
        hi.x = n->max.x;
        hi.z = n->max.z;
        lo.x = half.x + n->min.x;
        lo.z = half.z + n->min.z;
        n->child[2] = d3q_MakeChild(n, &lo.x, &hi.x);
        lo.z = half.x + n->min.z; /* sic: the original uses half.x (not half.z) here */
        lo.x = n->min.x;
        hi.z = n->max.z;
        hi.x = half.x + n->min.x;
        n->child[3] = d3q_MakeChild(n, &lo.x, &hi.x);
        for (i = 0; i < 4; i++) {
            if (n->child[i]) {
                if (cnt < 0)
                    cnt = n->child[i]->count;
                else if (cnt != n->child[i]->count)
                    same = 0;
            }
        }
        for (i = 0; i < 4; i++) {
            if (n->child[i]) {
                if (same) {
                    d3q_FreeNode(n->child[i]);
                    n->child[i] = 0;
                } else
                    {
                        unsigned int d = depth + 1;
                        d3q_Build(n->child[i], minitems, d, maxdepth);
                    }
            }
        }
    }
}
