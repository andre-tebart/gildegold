/* 0x45c620, updates the shadow of obj cast by light @deep B code hunks 409 (279 asm lines, was 283; 660 bytes): the original builds the tex_Load flags as a 4-byte local (memset + b[0]=0x7f + b[2]|=2, pushed from [esp+0x10]) where our source passes the same value as the literal 0x2007f, and spills the mesh 0x1d8 float to [esp+0x14] (frame 0x3c vs ours 0x34); a D3TEXFLAGS local + float local reproduce that frame but were WORSE in two tries (682/314 and 688/305 asm lines), so the slot layout is deep's; register roles (zero register ebp/ebx, esi/edi light/info) differ. Meaning fixes done: type >= 5, power test with a double 0.0, block scan field is .lod (was .hi). Q2 unk090 / Q4 4 raw accesses remain */

char d3s_Update(D3OBJECT *obj, D3OBJECT *light, D3TERRAIN *t)
{
    D3SSLOT *slot = 0;
    D3OBJINFO *info, *mesh;
    D3VECTOR ldir, lpos, opos;
    unsigned int size, i;
    int k, j;
    float eps;
    char redo;
    unsigned char mh;
    unsigned char *b;

    if (!d3s_TexSize || obj->type >= 5 || !(obj->objectFlags & 4) || !obj->material || !(light->objectFlags & 4)
        || *(unsigned int *)((char *)d3s_Object->material + 0x100) >= d3s_MaxPolys * 2 - 2
        || light->unk090.y == 0.0)
        return 0;
    for (k = 0; k < 4; k++) {
        if (D3S_SLOTS(obj->material)[k].tex && D3S_SLOTS(obj->material)[k].light == light) {
            slot = &D3S_SLOTS(obj->material)[k];
            break;
        }
    }
    if (k == 4) {
        for (k = 0; D3S_SLOTS(obj->material)[k].tex; k++) {
            if (k + 1 > 3)
                return 0;
        }
        slot = &D3S_SLOTS(obj->material)[k];
        slot->light = light;
        slot->map = 0;
        slot->size = 0;
        slot->maxh = 0;
        slot->bz0 = -1;
        slot->dir.x = 0;
        slot->simple = obj->objectFlags >> 3 & 1;
        slot->dir.y = 0;
        slot->dir.z = 0;
        if (!slot->simple) {
            size = d3s_TexSize;
            if (obj->info->mesh && d3s_LodTexSize) {
                for (i = 0; i < 2 && !(*(float *)((char *)obj->info->mesh + 0x1d8) < d3s_Dist[i]); i++)
                    size *= 2;
                size = min(size, d3s_MaxTexSize);
            }
            slot->tex = d3s_CacheTexture(slot, d3s_ShadowMips, size, d3_FrameCountU, &slot->cache);
        } else {
            slot->tex = ((D3TEXHANDLE *(*)(char *, int, int, int))tex_Load)("Schatten", 0x2007f, 0, 0);
            if (slot->tex && !slot->tex->surf)
                tex_Reload(slot->tex, 0);
        }
    }
    if (!slot || !slot->tex)
        return 0;
    if (slot->cache)
        slot->cache->time = d3_FrameCountU;
    info = obj->info;
    mesh = info;
    if (*(int *)((char *)obj->material + 0x5c8) > 0 && *(int *)((char *)obj->material + 0x5c4) > 0)
        mesh = (D3OBJINFO *)((char *)obj->material + 0x5bc);
    d3_PointToWorld(light, &light->pos, &lpos);
    if (!(light->objectFlags & 0x10)) {
        eps = 1.0f;
        d3_PointToWorld(obj, &obj->pos, &opos);
        ldir.x = opos.x - lpos.x;
        ldir.y = opos.y - lpos.y;
        ldir.z = opos.z - lpos.z;
    } else {
        eps = 0.01f;
        d3_DirToWorld(light, &d3_ZAxis, &ldir);
    }
    redo = !d3_VecEqual(&ldir, &slot->dir, eps) || *((char *)info + 0x194) ? 1 : 0;
    if (t) {
        if (!redo) {
            j = slot->bz0;
            if (j != -1 && !slot->simple) {
                mh = 0;
                if (slot->bz1 < j)
                    return 0;
                for (; j <= slot->bz1; j++) {
                    if (slot->bx0 <= slot->bx1) {
                        b = &t->blocks[j][slot->bx0].lod;
                        for (k = slot->bx1 - slot->bx0 + 1; k; k--, b += sizeof(D3SMBLOCK))
                            mh = max(mh, *b);
                    }
                }
                if (!mh)
                    return 0;
                if (mh != slot->maxh)
                    redo = 1;
            }
        }
    }
    if (redo)
        d3s_Project(obj, info, mesh, t, &ldir, &lpos, light, slot);
    if (d3s_AddShadow(slot, d3_Terrain))
        d3s_Object->state |= 4;
    return 0;
}
