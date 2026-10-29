/* 0x448c20, callback of d3_Unk448e60: of all polygons textured with ctx->tex it remembers the vertex nearest
   to each of the four screen corners (by the sum / difference of x and y), with its uv
   @deep B: read all 3 hunks (asmdiff 448cba code 26, 448d27 slots 10, 448d7b code 75): hunk 1 is only the commutative operand order of the pt[0] sum (edx/ecx loaded x/y in the other order; tried swapping the source terms of c->pt[0].x + c->pt[0].y - identical code); slots hunk is x/y swapped in the frame (tried swapping the declarations i,n,np,y,x - identical code); hunk 3 is the same pt[3] comparison with the two sides evaluated R-first and dd_Width reloaded per side in the original vs interleaved with one CSE'd load in ours (tried R > L - evaluation order matches now but the CSE remains, unitdiff unchanged 167/560); no missing/wrong constant, field or condition; readable already */

char d3_CornerCB(D3OBJECT *obj, D3CORNERCTX *c)
{
    D3SURF *s = (D3SURF *)obj->info;
    D3CORNERTRI *t;
    void **tex;
    void *want;
    char fl;
    int i, n, np, x, y;

    if (s) {
        n = D3_OPS(&s->info)->count;
        tex = (void **)s->texHandles;
        want = c->tex;
        for (i = 0; i < n; i++)
            if (tex[i] == want)
                break;
        if (i < n) {
            np = s->info.ntris;
            t = (D3CORNERTRI *)s->info.tris;
            if (np > 0) {
                for (; np > 0; np--, t++) {
                    if ((t->flag24 & 0x80) && !(t->flag26 & 2) && t->tex == want) {
                        c->found = 1;
                        for (i = 0; i < 3; i++) {
                            x = (int)t->v[i]->sx;
                            y = (int)t->v[i]->sy;
                            if (y + x < c->pt[0].y + c->pt[0].x) {
                                c->pt[0].x = x;
                                c->pt[0].y = y;
                                c->uv[0][0] = t->uv[i * 2];
                                c->uv[0][1] = t->uv[i * 2 + 1];
                                c->v[0].x = t->v[i]->x;
                                c->v[0].y = t->v[i]->y;
                                c->v[0].z = t->v[i]->z;
                            }
                            if (dd_Width - x + y - 1 < c->pt[1].y - c->pt[1].x + dd_Width - 1) {
                                c->pt[1].y = y;
                                c->pt[1].x = x;
                                c->uv[1][0] = t->uv[i * 2];
                                c->uv[1][1] = t->uv[i * 2 + 1];
                                c->v[1].x = t->v[i]->x;
                                c->v[1].y = t->v[i]->y;
                                c->v[1].z = t->v[i]->z;
                            }
                            if (dd_Height - y + x - 1 < c->pt[2].x - c->pt[2].y + dd_Height - 1) {
                                c->pt[2].x = x;
                                c->pt[2].y = y;
                                c->uv[2][0] = t->uv[i * 2];
                                c->uv[2][1] = t->uv[i * 2 + 1];
                                c->v[2].x = t->v[i]->x;
                                c->v[2].y = t->v[i]->y;
                                c->v[2].z = t->v[i]->z;
                            }
                            if (dd_Height - y - x + dd_Width - 2 < dd_Height - c->pt[3].y - c->pt[3].x + dd_Width - 2) {
                                c->pt[3].x = x;
                                c->pt[3].y = y;
                                c->uv[3][0] = t->uv[i * 2];
                                c->uv[3][1] = t->uv[i * 2 + 1];
                                c->v[3].x = t->v[i]->x;
                                c->v[3].y = t->v[i]->y;
                                c->v[3].z = t->v[i]->z;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}
