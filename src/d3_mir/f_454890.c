/* 0x454890, d3_mir:CreateOutline: turns the boundary edges of the mirror polygon (d3_MirBoundaryEdges: pairs of
   vertices in edges[], nedge of them) into the outline segments: connected edges that are collinear are
   merged, *out gets the (start, end) vertex pairs, *nout their count (2 per segment). Returns 0 only for n < 3.
   used[0..nedge) = edge taken by the current chain, used[nedge..2*nedge) (mark) = edge taken by any chain */

char d3_MirCreateOutline(D3PVERT **pts, unsigned int n, unsigned int *nout, D3PVERT ***out)
{
    D3PVERT **edges;
    unsigned char *used;
    unsigned char *mark;
    D3PVERT **res;
    D3PVERT *start, *end, *p, *q;
    D3VECTOR4 dir, d, nd;
    unsigned int i, j, nres, tail, head;
#define nedge (*(unsigned int *)&pts)

    /* dead stores: they rank mark / tail for the operand order of VC6's addresses (2 of 4 orders) */
    mark = 0;
    tail = 0;
    if (n < 3)
        return 0;
    edges = mem_Alloc(n << 5, "d3_mir:CreateOutlineMaxOut");
    used = mem_Alloc((n + 8) * n, "d3_mir:track_connections");
    d3_MirBoundaryEdges(n, used, pts, (unsigned int *)&pts, edges);
    i = nedge;
    res = mem_Alloc(i * 4, "d3_mir:CreateOutlineOutput");
    nres = 0;
    if (i > 0) {
        mark = used + i;
        D3_ZERO_AT(used, i * 2);
        mark[0] = 1;
        used[0] = 1;
        end = edges[1];
        head = tail = 0;
        start = edges[0];
        dir.x = end->x - start->x;
        dir.y = end->y - start->y;
        dir.z = end->z - start->z;
        d3_Normalize(&dir);
        do {
            for (i = 0; i < nedge; i += 2) {
                if (!used[i] && (p = edges[i]) == end) {
                    q = edges[i + 1];
                    d.x = p->x - q->x;
                    d.y = p->y - q->y;
                    d.z = p->z - q->z;
                    d3_Normalize(&d);
                    nd.x = -d.x;
                    nd.y = -d.y;
                    nd.z = -d.z;
                    if (d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&d, 0.001f)
                        || d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&nd, 0.001f)) {
                        used[i] = 1;
                        mark[tail] = 1;
                        tail = i;
                        end = edges[i + 1];
                        break;
                    }
                }
                if (!used[i] && (p = edges[i + 1]) == start) {
                    q = edges[i];
                    d.x = p->x - q->x;
                    d.y = p->y - q->y;
                    d.z = p->z - q->z;
                    d3_Normalize(&d);
                    nd.x = -d.x;
                    nd.y = -d.y;
                    nd.z = -d.z;
                    if (d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&d, 0.001f)
                        || d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&nd, 0.001f)) {
                        used[i] = 1;
                        /* used[nedge + head], not mark[head]: the form that gives VC6's operand order */
                        used[nedge + head] = 1;
                        head = i;
                        start = edges[i];
                        break;
                    }
                }
            }
            if (i >= nedge) {
                for (j = 0; j < nres; j += 2) {
                    q = res[j + 1];
                    if ((res[j] == start && res[j + 1] == end) || (res[j + 1] == start && res[j] == end))
                        break;
                }
                if (j >= nres) {
                    res[nres++] = start;
                    res[nres++] = end;
                }
                /* next unmarked edge starts a new outline; none left ends the do loop (start stays NULL;
                   this form, not a test after the scan, gives VC6's threaded exit) */
                start = NULL;
                for (i = 0; i < nedge; i += 2) {
                    if (!mark[i]) {
                        D3M_ZERO_VAR(used, pts);
                        tail = head = i;
                        mark[i] = 1;
                        used[i] = 1;
                        end = edges[i + 1];
                        start = edges[i];
                        dir.x = end->x - start->x;
                        dir.y = end->y - start->y;
                        dir.z = end->z - start->z;
                        d3_Normalize(&dir);
                        break;
                    }
                }
            }
        } while (start);
    }
done:
    mem_Free(used);
    mem_Free(edges);
    *nout = nres;
    *out = res;
    return 1;
#undef nedge
}
