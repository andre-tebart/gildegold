/* 0x45be10, projects the mesh along the light onto the ground and renders the shadow texture @deep B code hunks (591 asm lines, was 625; 1789 bytes): after the rect ends were fixed (+ 1.0 double inside the cast, the original's fadd QWORD 1.0) the rest is allocation - every local of the frame in another slot, the block-max scan keeps the pointer in another register and steps 0x320 per row (0x64 per block in the original), far-branch register roles; the 17 calls and every non-stack constant agree. Q2: dd_LockSurfaceFlags/dd_FillSurface/dd_UnlockSurface and d3d_BuildMipmaps are other units' unnamed functions. Q4 1 raw access */

void d3s_Project(D3OBJECT *obj, D3OBJINFO *info, D3OBJINFO *mesh, D3TERRAIN *t, D3VECTOR *ldir,
                 D3VECTOR *lpos, D3OBJECT *light, D3SSLOT *slot)
{
    D3TEXHANDLE *tex = slot->tex;
    D3VECTOR pos, d;
    D3BOXVERT *v;
    D3SPOLY *p;
    float y, f, inv, x0, x1, z0, z1;
    unsigned int size, mh, m;
    int i, k, bx0, bx1, bz0, bz1;
    unsigned char *b;

    d3s_Count++;
    slot->dir = *ldir;
    y = d3s_GetGround(obj, info, light);
    z0 = 1e10f;
    x0 = 1e10f;
    z1 = -1e10f;
    x1 = -1e10f;
    if (!slot->simple) {
        ((void (*)(D3OBJECT *, D3OBJINFO *, void *))(*(void **)((char *)mesh->mesh + 0x1f0)))(obj, mesh, (char *)obj->material + 8);
        obj->state |= 0x10;
        v = (D3BOXVERT *)mesh->keys;
        if (!(light->objectFlags & 0x10)) {
            for (i = 0; i < mesh->nkeys; i++, v++) {
                d.x = lpos->x - v->x;
                d.y = lpos->y - v->y;
                d.z = lpos->z - v->z;
                f = -((lpos->y - y) / d.y);
                v->x = d.x * f + lpos->x;
                v->y = d.y * f + lpos->y;
                v->z = f * d.z + lpos->z;
                z0 = min(z0, v->z);
                z1 = max(z1, v->z);
                x0 = min(x0, v->x);
                x1 = max(x1, v->x);
            }
        } else {
            inv = ldir->y;
            for (i = 0; i < mesh->nkeys; i++, v++) {
                f = (v->y - y) * (1.0f / -inv);
                v->x = f * ldir->x + v->x;
                v->y = f * ldir->y + v->y;
                v->z = f * ldir->z + v->z;
                z0 = min(z0, v->z);
                z1 = max(z1, v->z);
                x0 = min(x0, v->x);
                x1 = max(x1, v->x);
            }
        }
    } else {
        d3_PointToWorld(obj, &obj->pos, &pos);
        y += 3.0f;
        x0 = pos.x - 16.0f;
        x1 = pos.x + 16.0f;
        z0 = pos.z - 16.0f;
        z1 = pos.z + 16.0f;
    }
    if (fabs(x1) > 16384.0f || fabs(x0) > 16384.0f || fabs(z1) > 16384.0f || fabs(z0) > 16384.0f
        || fabs(x1 - x0) > 16384.0f || fabs(z1 - z0) > 16384.0f)
        return;
    slot->bz0 = -1;
    slot->map = 0;
    slot->size = 0;
    if (d3s_Enabled) {
        if (!t || slot->simple) {
            if (d3sm_Shadow && !slot->simple) {
                slot->ix0 = (int)((x0 - ((float *)d3sm_Shadow)[0]) / ((float *)d3sm_Shadow)[4]);
                slot->ix1 = (int)((x1 - ((float *)d3sm_Shadow)[0]) / ((float *)d3sm_Shadow)[4] + 1.0);
                slot->iz0 = (int)((z0 - ((float *)d3sm_Shadow)[2]) / ((float *)d3sm_Shadow)[6]);
                slot->iz1 = (int)((z1 - ((float *)d3sm_Shadow)[2]) / ((float *)d3sm_Shadow)[6] + 1.0);
                if (slot->ix0 < ((int *)d3sm_Shadow)[8] && slot->ix1 > 0 && slot->iz0 < ((int *)d3sm_Shadow)[8] && slot->iz1 > 0) {
                    slot->map = (unsigned char *)((int *)d3sm_Shadow)[10];
                    slot->size = ((int *)d3sm_Shadow)[8];
                    slot->cell.x = ((float *)d3sm_Shadow)[4];
                    slot->cell.y = ((float *)d3sm_Shadow)[5];
                    slot->cell.z = ((float *)d3sm_Shadow)[6];
                    slot->origin.x = ((float *)d3sm_Shadow)[0];
                    slot->origin.y = ((float *)d3sm_Shadow)[1];
                    slot->origin.z = ((float *)d3sm_Shadow)[2];
                    x0 = slot->ix0 * ((float *)d3sm_Shadow)[4] + ((float *)d3sm_Shadow)[0];
                    x1 = slot->ix1 * ((float *)d3sm_Shadow)[4] + ((float *)d3sm_Shadow)[0];
                    z0 = slot->iz0 * ((float *)d3sm_Shadow)[6] + ((float *)d3sm_Shadow)[2];
                    z1 = slot->iz1 * ((float *)d3sm_Shadow)[6] + ((float *)d3sm_Shadow)[2];
                }
            }
        } else {
            slot->ix0 = (int)((x0 - t->origin.x) / t->axisx.x);
            slot->ix1 = (int)((x1 - t->origin.x) / t->axisx.x + 1.0);
            slot->iz0 = (int)((z0 - t->origin.z) / t->axisz.z);
            slot->iz1 = (int)((z1 - t->origin.z) / t->axisz.z + 1.0);
            size = t->size;
            if (slot->ix0 < (int)size && slot->ix1 > 0 && slot->iz0 < (int)size && slot->iz1 > 0) {
                mh = 0;
                bx0 = max(0, slot->ix0) / ((int *)t)[1];
                bx1 = min((int)size - 1, slot->ix1) / ((int *)t)[1];
                bz0 = max(0, slot->iz0) / ((int *)t)[1];
                bz1 = min((int)size - 1, slot->iz1) / ((int *)t)[1];
                slot->bx0 = bx0;
                slot->bx1 = bx1;
                slot->bz0 = bz0;
                slot->bz1 = bz1;
                for (k = bz0; k <= bz1; k++) {
                    if (bx0 <= bx1) {
                        b = &t->blocks[k][bx0].lod;
                        for (i = bx1 - bx0 + 1; i; i--, b += sizeof(D3SMBLOCK))
                            mh = max(mh, *b);
                    }
                }
                slot->maxh = mh;
                if (mh > 1) {
                    m = ~(mh - 1);
                    slot->ix0 &= m;
                    if (slot->ix1 & mh - 1)
                        slot->ix1 = slot->ix1 + mh & m;
                    slot->iz0 &= m;
                    if (slot->iz1 & mh - 1)
                        slot->iz1 = slot->iz1 + mh & m;
                }
                slot->map = t->heights;
                slot->size = t->size;
                slot->cell.x = t->axisx.x;
                slot->cell.y = t->axisy.y;
                slot->cell.z = t->axisz.z;
                slot->origin.x = t->origin.x;
                slot->origin.y = t->origin.y;
                slot->origin.z = t->origin.z;
                x0 = slot->ix0 * t->axisx.x + t->origin.x;
                x1 = slot->ix1 * t->axisx.x + t->origin.x;
                z0 = slot->iz0 * t->axisz.z + t->origin.z;
                z1 = slot->iz1 * t->axisz.z + t->origin.z;
            }
        }
    }
    slot->z0 = z0;
    slot->x0 = x0;
    slot->x1 = x1;
    slot->z1 = z1;
    slot->y = y;
    if (!slot->simple) {
        size = ((TSTEXSIZE *)tex)->size;
        v = (D3BOXVERT *)mesh->keys;
        for (i = 0; i < mesh->nkeys; i++, v++) {
            ((float *)v)[4] = (v->x - x0) * ((float)size / (x1 - x0));
            ((float *)v)[5] = (v->z - z0) * ((float)size / (z1 - z0));
        }
        if (!tex->surf && d3_Hardware)
            ts_CacheFetch(tex, 2, (int)d3s_Palette);
        if (tex->surf) {
            dd_FillSurface(tex->surf, 0, 0);
            d3s_ShadowBits = dd_LockSurfaceFlags(tex->surf, 0x20);
            for (i = 0, p = (D3SPOLY *)mesh->tris; i < mesh->ntris; i++, p++)
                d3s_ShadowPoly(p, ((TSTEXSIZE *)tex)->bpp, ((TSTEXSIZE *)tex)->size);
            dd_UnlockSurface(tex->surf);
            d3s_ShadowBits = 0;
            if (slot->cache->flag)
                d3d_BuildMipmaps(tex->surf, 0);
        }
    }
}
