/* 0x43a7d0, Direct3D rendering: draws the sorted triangles in texture order through the vertex buffer,
   clipping the ones that cross a frustum plane into fans.
   @deep behaviour draft: 779 code hunks / 36 slots; the frame differs at once (orig sub esp,0x80, ours 0x78 - two dwords of locals missing) and so does the register allocation (orig zeroes edi and keeps the texture in edi/[ebp-0x18], ours uses esi); the source is the round-3 first draft (notes/units/d3_engine.md: 'First draft only: register allocation differs from the start'), so the local set has to be re-derived, which is match-deep work */

void d3_DrawSortedD3D(void)
{
    char wrap;
    int max;
    D3TLV *vb;
    unsigned int n;
    D3TLV *p;
    D3TEXV *cur;
    float *uv;
    int h;
    float zmin;
    D3PTRI *t;
    int i;
    struct IDirect3DTexture2 *last;
    D3CLIPINFO *clip;
    D3CVERT **vl;
    D3CVERT *q;
    unsigned int k;
    float rhw;
    D3TLV *a, *b;
    D3TLV tmp;

    wrap = 0;
    last = 0;
    cur = 0;
    d3_Unkfc2dc0 = 0;
    d3_TextureSwitches = 0;
    if (d3_MinZ > 0.0f)
        zmin = d3_MinZ;
    else
        zmin = 0.0f;
    d3_ZScale = 1.0f / (d3_MaxZ + 50.0f - zmin);
    d3_BeginScene(d3_FogOn, d3_FogColor, 1);
    h = d3d_GetVertexBuffer(&vb, &max);
    max -= 0x80;
    p = vb;
    n = 0;
    for (i = 0; i < (int)d3_SortCount; i++) {
        t = d3_SortBuf[i].item;
        uv = t->src;
        clip = t->clip;
        if ((D3TEXV *)t->tex != cur) {
            d3d_DrawTriangles((void *)h, vb, n);
            h = d3d_GetVertexBuffer(&vb, &max);
            max -= 0x80;
            p = vb;
            n = 0;
            cur = (D3TEXV *)t->tex;
            if (!d3_Unk63345d && cur && cur->d3dtex != last) {
                last = cur->d3dtex;
#line 1219
                D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, last));
                d3_TextureSwitches++;
                if (cur->flags & 8)
                    d3_Unkfc2dc0 += (cur->bytes * cur->w * cur->w) >> 3;
                else
                    d3_Unkfc2dc0 += (int)(cur->bytes * cur->w * cur->w * 1.3333334f);
            }
            if (cur) {
                wrap = cur->flags & 1;
                d3_SetBlendStateI(cur->alpha < 0xff, cur->unk6e & 1, (cur->unk6e >> 1) & 1);
            }
        }
        if (cur) {
            d3_UOfs = d3_UOfsTab[cur->uofs & 0x1f] + *(float *)&t->unk1c;
            d3_VOfs = d3_VOfsTab[cur->vofs & 0x1f] + *(float *)&t->unk20;
        } else {
            d3_VOfs = 0.0f;
            d3_UOfs = 0.0f;
        }
        d3_ZOfs = *(float *)&t->unk18 - zmin;
        if (((D3_CV(t->v[0])->flags | D3_CV(t->v[1])->flags | D3_CV(t->v[2])->flags) & 0x3f) + clip->mirror == 0) {
            if (wrap) {
                D3E_TLV(p++, t->v[0], &D3_CV(t->v[0])->tu);
                D3E_TLV(p++, t->v[1], &D3_CV(t->v[1])->tu);
                D3E_TLV(p++, t->v[2], &D3_CV(t->v[2])->tu);
                n += 3;
            } else {
                D3E_TLV(p++, t->v[0], uv);
                D3E_TLV(p++, t->v[1], uv + 2);
                D3E_TLV(p++, t->v[2], uv + 4);
                n += 3;
            }
        } else {
            d3_ClipList[0][0] = D3_CV(t->v[0]);
            d3_ClipList[0][1] = D3_CV(t->v[1]);
            d3_ClipList[0][2] = D3_CV(t->v[2]);
            if (wrap) {
                D3_CV(t->v[0])->u = d3_UOfs + D3_CV(t->v[0])->tu;
                D3_CV(t->v[0])->v = d3_VOfs + D3_CV(t->v[0])->tv;
                D3_CV(t->v[1])->u = d3_UOfs + D3_CV(t->v[1])->tu;
                D3_CV(t->v[1])->v = d3_VOfs + D3_CV(t->v[1])->tv;
                D3_CV(t->v[2])->u = d3_UOfs + D3_CV(t->v[2])->tu;
                D3_CV(t->v[2])->v = d3_VOfs + D3_CV(t->v[2])->tv;
            } else {
                D3_CV(t->v[0])->u = d3_UOfs + uv[0];
                D3_CV(t->v[0])->v = d3_VOfs + uv[1];
                D3_CV(t->v[1])->u = d3_UOfs + uv[2];
                D3_CV(t->v[1])->v = d3_VOfs + uv[3];
                D3_CV(t->v[2])->u = d3_UOfs + uv[4];
                D3_CV(t->v[2])->v = d3_VOfs + uv[5];
            }
            d3_ClipIn = 3;
            vl = d3_ClipPoly(clip);
            if (d3_ClipOut >= 3) {
                d3_ClipExtraTris += d3_ClipOut - 3;
                for (k = 0; k < d3_ClipOut; k++) {
                    q = vl[k];
                    if (!(q->flags & 0x80)) {
                        rhw = 1.0f / q->z;
                        q->sx = d3_ScaleX * q->x * rhw + d3_CenterX;
                        q->sy = d3_ScaleY * q->y * rhw + d3_CenterY;
                    }
                }
                q = vl[0];
                D3E_TLV2(&tmp, q, &q->u, &q->v);
                for (k = 1; k < d3_ClipOut - 1; k++) {
                    D3E_COPY3(p++, &tmp, sizeof(tmp));
                    q = vl[k];
                    a = p;
                    D3E_TLV2(p++, q, &q->u, &q->v);
                    q = vl[k + 1];
                    b = p;
                    D3E_TLV2(p++, q, &q->u, &q->v);
                    n += 3;
                    if (((t->flags >> 6) & 1) - ((t->drawFlags >> 2) & 1) - clip->mirror == 1
                        && (tmp.sy - b->sy) * (tmp.sx - a->sx) < (tmp.sy - a->sy) * (tmp.sx - b->sx)) {
                        p -= 3;
                        n -= 3;
                    }
                }
            }
        }
        if (n >= (unsigned int)max) {
            d3d_DrawTriangles((void *)h, vb, n);
            h = d3d_GetVertexBuffer(&vb, &max);
            max -= 0x80;
            p = vb;
            n = 0;
        }
    }
    d3d_DrawTriangles((void *)h, vb, n);
    d3_EndScene();
}
