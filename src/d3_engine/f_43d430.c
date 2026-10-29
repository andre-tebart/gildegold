/* 0x43d430, sets the alpha of a surface: textures are swapped for alpha aliases (or back). @deep B3 draft: after the reading fixes (the three raw *(int*)((char*)s->info.mesh+0x1e0) are now D3_OPS(&s->info)->count, the function is readable) only the address encoding of the two tris[i].tex accesses per loop remains, 4 hunks / 4 of 512 B: ours [edx+ecx+0x14] and [eax+ecx+0x14], original [ecx+edx+0x14] and [ecx+eax+0x14] - the same instructions with base and index swapped. Tried in this state: the store through the local tris[i].tex (469/528 B, reverted); earlier attempts per notes/units/d3_engine.md: tris[i] directly, (tris + i)->, i[tris], 0x28 byte offsets, integer arithmetic i*0x28+(int)tris, unsigned i, a pointer IV. */

void d3_SetObjectAlpha(D3OBJECT *obj, D3OBJINFO *info, D3TEXFLAGS f)
{
    D3SURF *s = (D3SURF *)info;
    D3TEXMAP *map;
    D3PTRI *tris;
    D3TEXTURE *t;
    int i, j, n;

    if (obj && s && D3_OPS(&s->info)->count >= 0) {
        if (f.b[0] >= 0xff && !(f.b[2] & 1)) {
            map = mem_Alloc(D3_OPS(&s->info)->count * 8, "!d3:ChangeObjectTransparency()");
            obj->objectFlags &= 0x7f;
            s->flags.i = 0;
            s->flags.f.b[2] &= 0xfe;
            s->flags.f.b[0] = 0xff;
            n = 0;
            for (i = 0; i < s->info.ntris; i++) {
                tris = (D3PTRI *)s->info.tris;
                if (t = tris[i].tex) {
                    for (j = 0; j < n; j++)
                        if (map[j].from == t)
                            break;
                    if (j == n) {
                        map[j].from = t;
                        map[j].to = tex_ReleaseAlias(t, 1);
                        n++;
                    }
                    ((D3PTRI *)s->info.tris)[i].tex = map[j].to;
                }
            }
            mem_Free(map);
            d3_SetVertexColors(obj, s, 0, f);
            return;
        }
        if (s->flags.f.b[0] == 0xff && !(s->flags.f.b[2] & 1)) {
            obj->objectFlags |= 0x80;
            map = mem_Alloc(D3_OPS(&s->info)->count << 3, "!d3:ChangeObjectTransparency()");
            n = 0;
            for (i = 0; i < s->info.ntris; i++) {
                tris = (D3PTRI *)s->info.tris;
                if (t = tris[i].tex) {
                    for (j = 0; j < n; j++)
                        if (map[j].from == t)
                            break;
                    if (j == n) {
                        map[j].from = t;
                        map[j].to = tex_AliasForFlags(t, f, 1);
                        n++;
                    }
                    ((D3PTRI *)s->info.tris)[i].tex = map[j].to;
                }
            }
            mem_Free(map);
        }
        s->flags.f = f;
        d3_SetVertexColors(obj, s, 1, f);
    }
}
