/* 0x454780, the boundary edges of the point set pts: for every pair (i, j) not taken yet the triangle
   (origin, i, j) is a hull edge when all other points lie on its positive side; the pairs go to out (2 per
   edge, *count of them), used is the n x n matrix that marks the taken pairs */
char d3_MirBoundaryEdges(unsigned int n, unsigned char *used, D3PVERT **pts, unsigned int *count, D3PVERT **out)
{
    D3VECTOR4 nrm;
    D3PVERT *p;
    unsigned char *row;
    unsigned i, j, k;

    *count = 0;
    for (i = 0; i < n; i++) {
        row = used + i;
            for (j = 0; j < n; j++, row += n) {
                if (i != j && used[i * n + j] == 0) {
                    d3_TriNormalM((D3VECTOR *)&d3_Origin, pts[i], pts[j], &nrm);
                    for (k = 0; k < n; k++) {
                        if (k != i && k != j) {
                            p = pts[k];
                            if (nrm.x * p->x + nrm.y * p->y + nrm.z * p->z < -0.01)
                                break;
                        }
                    }
                    if (k == n) {
                        out[(*count)++] = pts[i];
                        out[(*count)++] = pts[j];
                        *row = 1;
                        used[i * n + j] = 1;
                    }
                }
            }
    }
    return 1;
}
