/* 0x455a90, per-frame update and draw of a particle system: runs its callback (no callback or a zero return deletes
   the system), builds one camera-facing quad (two tris) per live particle and puts the visible tris on the sort list */

void par_Draw(D3PARTSYS *ps)
{
    D3MATRIX m;
    D3PARTICLE *p;
    D3PVERT *v, *v1, *v2, *v3;
    D3PTRI *t;
    D3TEXTURE *tex;
    PARCOL col;
    D3VECTOR4 c;
    float rz, half, sx, sy, hw, hh;
    unsigned char fog;
    char vis;
    int i, n;
    D3PTRI *t1;

    if (ps->callback && ps->callback(ps, d3_Time)) {
        d3_TriCount += ps->count * 2;
        d3_VertCount += ps->count * 4;
        d3_WorldMatrix(PAR_OBJ(ps), (PAR_OBJ(ps)->state & 0x80) ? 0 : d3_Camera, 0, &m);
        for (i = 0; i < ps->count; i++) {
            p = &ps->particles[i];
            vis = 0;
            t = &PAR_TRIS(ps)[i * 2];
            if ((p->flags & 1) && p->a > 0) {
                rz = 0.0f;
                half = p->size * 0.5f;
                v = &PAR_VERTS(ps)[i * 4];
                c.x = (m._11 * p->pos.x) + m._31 * p->pos.z + m._21 * p->pos.y + m._41;
                c.y = (m._12 * p->pos.x) + m._32 * p->pos.z + m._22 * p->pos.y + m._42;
                c.z = (m._13 * p->pos.x) + m._33 * p->pos.z + m._23 * p->pos.y + m._43;
                rz = 1.0f / c.z;
                d3_Dist2 = c.x * c.x + c.y * c.y + c.z * c.z;
                fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                    ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                if (!(d3_MinZ < c.z))
                    d3_MinZ = c.z;
                if (!(d3_MaxZ > c.z))
                    d3_MaxZ = c.z;
                vis = 1;
                sx = c.x * (d3_ScaleX * rz) + d3_CenterX;
                sy = c.y * (d3_ScaleY * rz) + d3_CenterY;
                hw = (d3_ScaleX * rz) * half;
                hh = -((d3_ScaleY * rz) * half);
                if (sx - hw >= d3_Viewport.right || sx + hw < d3_Viewport.left
                    || sy - hh >= d3_Viewport.bottom || sy + hh < d3_Viewport.top
                    || c.z >= d3_ZFar || c.z < d3_ZNear)
                    vis = 0;
            }
            t1 = t + 1;
            PAR_TF(&t[0])->vis = PAR_TF(t1)->vis = vis;
            if (vis) {
                v1 = v + 1;
                v2 = v + 2;
                v3 = v + 3;
                if (p->frame > 0)
                    tex = tex_GetFrame(ps->texindex, p->frame);
                else
                    tex = (D3TEXTURE *)ps->tex;
                t[0].v[0] = v;
                t[0].v[1] = v1;
                t[0].v[2] = v2;
                t[0].src = d3_ParUV0;
                t[0].tex = tex;
                t1->v[0] = v;   /* through t1: needed for VC6's order (the tex test after this store) */
                t[1].v[1] = v3;
                t[1].v[2] = v1;
                t[1].src = d3_ParUV1;
                t[1].tex = tex;
                if (tex && (PAR_TEXBITS(tex) & 1)) {
                    col.c.r = p->r * p->a >> 8;
                    col.c.g = p->g * p->a >> 8;
                    col.c.b = p->b * p->a >> 8;
                } else
                    col.color = p->color;
                v->color = v1->color = v2->color = v3->color = col.color;
                v->fog = v1->fog = v2->fog = v3->fog = fog;
                PAR_VF(v)->b4 = PAR_VF(v1)->b4 = PAR_VF(v2)->b4 = PAR_VF(v3)->b4 = 0;
                PAR_VF(v)->b5 = PAR_VF(v1)->b5 = PAR_VF(v2)->b5 = PAR_VF(v3)->b5 = 0;
                PAR_VF(v)->vis = PAR_VF(v1)->vis = PAR_VF(v2)->vis = PAR_VF(v3)->vis = 1;
                v->sx = sx - hw;
                v->sy = sy - hh;
                v->rhw = rz;
                v->z = c.z;
                v->x = c.x - half;
                v->y = c.y + half;
                PAR_VF(v)->l = PAR_VF(v2)->l = v->sx < d3_Viewport.left;
                PAR_VF(v)->r = PAR_VF(v2)->r = v->sx >= d3_Viewport.right;
                PAR_VF(v)->t = PAR_VF(v3)->t = v->sy < d3_Viewport.top;
                PAR_VF(v)->b = PAR_VF(v3)->b = v->sy >= d3_Viewport.bottom;
                v1->sx = sx + hw;
                v1->sy = sy + hh;
                v1->rhw = rz;
                v1->z = c.z;
                v1->x = c.x + half;
                v1->y = c.y - half;
                PAR_VF(v1)->l = PAR_VF(v3)->l = v1->sx < d3_Viewport.left;
                PAR_VF(v1)->r = PAR_VF(v3)->r = v1->sx >= d3_Viewport.right;
                PAR_VF(v1)->t = PAR_VF(v2)->t = v1->sy < d3_Viewport.top;
                PAR_VF(v1)->b = PAR_VF(v2)->b = v1->sy >= d3_Viewport.bottom;
                v2->sx = sx - hw;
                v2->sy = sy + hh;
                v2->rhw = rz;
                v2->x = c.x - half;
                v2->y = c.y - half;
                v2->z = c.z;
                v3->sx = sx + hw;
                v3->sy = sy - hh;
                v3->rhw = rz;
                v3->x = c.x + half;
                v3->y = c.y + half;
                v3->z = c.z;
            }
        }
        n = min(ps->count * 2, d3_SortMax - (int)d3_SortCount);
        if (d3_Hardware) {
            for (t = PAR_TRIS(ps); n > 0; n--, t++) {
                if (t->flags & 0x80) {
                    D3TEXTURE *tt = t->tex;
                    d3_SortPtr->item = t;
                    if (tt) {
                        d3_SortPtr->key = (tt - d3_Textures) + 0x8000;
                        D3PAR_TEXSTAMP(tt) = d3_FrameCount;
                    } else
                        d3_SortPtr->key = 0;
                    t->clip = &d3_ClipNone;
                    d3_SortPtr++;
                    d3_SortCount++;
                }
            }
        } else {
            v = PAR_VERTS(ps);
            for (i = 0; i < ps->count * 4; i++, v++) {
                if (v->flags & 0x80)
                    v->c.r = (unsigned char)((v->c.b * 0.11f + v->c.r * 0.3f + v->c.g * 0.59f) * 0.25f);
            }
            for (t = PAR_TRIS(ps); n > 0; n--, t++) {
                if (t->flags & 0x80) {
                    d3_SortPtr->item = t;
                    t->clip = &d3_ClipNone;
                    d3_SortPtr->key = (unsigned long)((d3_InvZFar * 16777216.0f) * t->v[0]->z * 3.0);
                    d3_SortPtr++;
                    d3_SortCount++;
                }
            }
        }
    } else
        par_Delete(ps);
}
