/* 0x45e210, d3_ForEachObject callback: maps the object's vertices into the height map and queues its
   shadow casting triangles
   @deep behaviour: 193 code hunks, the whole 213-line asmdiff read, no statement missing - the same operations in other registers / stack slots / idioms: frame sub esp 0x24 vs 0x18 and 3 vs 4 callee-saved regs (the object and the mesh pointer kept in different regs), the tri->cast bitfield update is the same shl/xor bit-7 idiom with other registers (and bit 6 already cleared by solid = 0), max(a->h,b->h) as byte cmp+jbe+and 0xff vs zero-extended dword cmp+jg, the 0xc0 solid/cast pair as load/or/store vs or byte [mem],0xc0; no meaning error found */

char d3sm_ShadowCB(D3OBJECT *obj, D3SM *h)
{
    D3SMMESH *m = (D3SMMESH *)obj->info;
    D3SMTRI *tri;
    D3SMVERT *v, *a, *b, *c;
    D3VECTOR4 nrm;
    float inv[3];
    float size;
    int i, n, hmax;

    if (!m)
        return 1;
    if ((h->mask & (obj->stateFlags >> 2 & 3)) && !(obj->stateFlags & 0x10))
        return 1;
    for (i = 0; i < 3; i++)
        inv[i] = 1.0f / (&h->cell.x)[i];
    for (i = 0, v = m->keys; i < m->nkeys; i++, v++) {
        v->gx = (v->x - h->origin.x) * inv[0] + 0.875f;
        v->gz = (v->z - h->origin.z) * inv[2] + 0.875f;
        v->h = (unsigned char)min(254.0f, max(1.0f, (v->y - h->origin.y) * inv[1]));
        v->flags = (!(h->mask & (obj->stateFlags >> 2 & 3)) | ((obj->stateFlags & 0x20) == 0x20) << 1) << 1 | (obj->stateFlags & 0x10) == 0x10;
    }
    m = (D3SMMESH *)obj->info;
    n = min(m->ntris, d3_SortMax - d3_SortCountI);
    for (tri = m->tris; n > 0; n--, tri++) {
        a = tri->v[0];
        b = tri->v[1];
        c = tri->v[2];
        d3_TriNormal(a, b, c, &nrm);
        if (fabs(nrm.y) < 0.05f && !(obj->stateFlags & 0x40)) {
            tri->solid = 1;
            tri->cast = 1;
        } else {
            tri->solid = 0;
            tri->cast = (tri->f26 & 4) || b->gz * c->gx - b->gx * c->gz + b->gx * a->gz - b->gz * a->gx + c->gz * a->gx - c->gx * a->gz < 0.0f;
        }
        if (tri->cast && !(tri->f26 & 2)) {
            size = (float)h->size;
            if (a->gx >= 0.0f && a->gx < size && a->gz >= 0.0f && a->gz < size
                && b->gx >= 0.0f && b->gx < size && b->gz >= 0.0f && b->gz < size
                && c->gx >= 0.0f && c->gx < size && c->gz >= 0.0f && c->gz < size) {
                d3_SortPtrV->tri = tri;
                d3_SortCountI++;
                hmax = max(a->h, b->h);
                hmax = max(hmax, c->h);
                d3_SortPtrV->key = hmax * 0x300;
                d3_SortPtrV++;
            }
        }
    }
    return 1;
}
