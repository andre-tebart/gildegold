/* 0x43b080, software rendering: draws the sorted triangles back to front, clipping the ones that cross
   a frustum plane into fans; a triangle whose clip info is the mirror outline's (D3CLIPINFO.mirror) always
   goes through d3_ClipPoly and its winding test is flipped. */

void d3_DrawSortedSW(void)
{
    int i;
    int mode;
    D3PTRI *t;
    D3CLIPINFO *clip;
    float *uv;
    D3TEXTURE *tex;
    unsigned char wrap;
    D3CVERT **v;
    unsigned int k;
    float rhw;
    D3PTRI tri;
    float tuv[14];
    D3CVERT *p;

    d3_TextureSwitches = 0;
    dd_LockNoWait();
    for (i = d3_SortCount - 1; i >= 0; i--) {
        mode = 4;
        t = d3_SortBuf[i].item;
        wrap = 0;
        tex = t->tex;
        clip = t->clip;
        uv = t->src;
        if (tex) {
            if (D3_TEXB(tex, 0x6c) - (D3_TEXB(tex, 0x6e) & 1) < 0xff)
                mode = 3;
            wrap = D3_TEXB(tex, 0x68) & 1;
        }
        p = D3_CV(t->v[0]);
        if (!((p->flags | D3_CV(t->v[1])->flags | D3_CV(t->v[2])->flags) & 0x3f) && !clip->mirror) {
            if (wrap) {
                ((D3UV *)uv)[0] = D3_CV(t->v[0])->uv;
                ((D3UV *)uv)[1] = D3_CV(t->v[1])->uv;
                p = D3_CV(t->v[2]);
                uv[4] = p->tu;
                uv[5] = p->tv;
            }
            d3_TriFuncs[mode](t);
        } else {
            d3_ClipList[0][0] = D3_CV(t->v[0]);
            d3_ClipList[0][1] = D3_CV(t->v[1]);
            d3_ClipList[0][2] = D3_CV(t->v[2]);
            if (wrap) {
                p = D3_CV(t->v[0]);
                p->u = p->tu;
                p->v = p->tv;
                p = D3_CV(t->v[1]);
                p->u = p->tu;
                p->v = p->tv;
                p = D3_CV(t->v[2]);
                p->u = p->tu;
                p->v = p->tv;
            } else {
                p = D3_CV(t->v[0]);
                p->u = uv[0];
                p->v = uv[1];
                p = D3_CV(t->v[1]);
                p->u = uv[2];
                p->v = uv[3];
                p = D3_CV(t->v[2]);
                p->u = uv[4];
                p->v = uv[5];
            }
            d3_ClipIn = 3;
            v = d3_ClipPoly(clip);
            if (d3_ClipOut >= 3) {
                d3_ClipExtraTris += d3_ClipOut - 3;
                for (k = 0; k < d3_ClipOut; k++) {
                    p = v[k];
                    if (!(p->flags & 0x80)) {
                        rhw = 1.0f / p->z;
                        p->sx = d3_ScaleX * rhw * p->x + d3_CenterX;
                        p->sy = d3_ScaleY * p->y * rhw + d3_CenterY;
                    }
                }
                D3_COPY(&tri, t, sizeof(tri));
                p = v[0];
                tri.v[0] = (D3PVERT *)p;
                tri.src = tuv;
                tuv[0] = p->u;
                tuv[1] = p->v;
                for (k = 0; k < d3_ClipOut - 2; k++) {
                    p = v[k + 1];
                    tri.v[1] = (D3PVERT *)p;
                    tuv[2] = p->u;
                    tuv[3] = p->v;
                    p = v[k + 2];
                    tri.v[2] = (D3PVERT *)p;
                    tuv[4] = p->u;
                    tuv[5] = p->v;
                    d3_TriFuncs[mode](&tri);
                }
            }
        }
    }
    dd_Unlock();
}
