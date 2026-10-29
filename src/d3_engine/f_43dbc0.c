/* 0x43dbc0, d3_ForEachObject2 callback: builds the drawable points and triangles of all surfaces
   from the loaded mesh data.
   @deep B: 140 code hunks, the first 118 instructions equal; from the triangle loop (0x43dd26) the same operations are allocated differently - the zero register is ebx throughout ours and ecx/edx/edi in the original (xor/cmp vs test), add vs lea, mov [..],0 vs xor+bl stores, plus one dead 'jmp .+2; xor ecx,ecx' at 0x43dd49 that ours folds; read the whole body against the disassembly: no missing statement, wrong constant, callee or field - whole-function register allocation, match-deep's toolbox. Also Q2 D3PTRI.unk18/unk1c/unk20 and D3PVERT.unk4d plus the Q4 raw *(unsigned char *)&t->v[m]->flags have no evidenced name in callers or data. */

void d3_InflateObject(D3OBJECT *obj)
{
    D3SURF *s;
    D3MESHSRC *src;
    D3PVERT *v;
    D3PTRI *t;
    D3SRCPOLY *p;
    D3TEXTURE *tex;
    int *mat;
    int i, k, m, j;
    char made;
    union {
        unsigned long l;
        D3COLOR c;
    } col;

    if (obj && obj->material) {
        obj->state |= 4;
        d3s_ResetShadows(obj);
        for (i = 0; i <= 3; i++) {
            s = &D3_MESH(obj)->surf[i];
            if (src = (D3MESHSRC *)s->info.mesh) {
                made = 0;
                if (!s->info.tris && src->npolys > 0) {
                    s->info.tris = mem_Alloc((s->info.ntris = src->npolys) * 0x28, "d3:InflateObject(allpolys)");
                    made = 1;
                }
                if (!s->info.keys && src->npoints > 0) {
                    s->info.keys = mem_Alloc(((s->info.nkeys = src->npoints) + 8) * 0x50, "d3:InflateObject(allpoints)");
                } else if (!made)
                    continue;
                col.l = 0xffffffff;
                if (src->npoints > 0)
                    for (k = 0; k < s->info.nkeys + 8; k++) {
                        ((D3PVERT *)s->info.keys)[k].src = src->points + k * 0x18;
                        ((D3PVERT *)s->info.keys)[k].color = 0xffffffff;
                        ((D3PVERT *)s->info.keys)[k].specular = 0xffffffff;
                    }
                mat = mem_Alloc(d3_TexCount * 4, "d3:!InflateObject(mat)");
                D3E_FILL(mat, -1, d3_TexCount * 4);
                for (k = 0, j = 0, t = (D3PTRI *)s->info.tris; k < s->info.ntris; k++, t++) {
                    t->unk20 = 0;
                    ((D3TRIBITS *)&t->drawFlags)->b0 = 0;
                    t->unk1c = 0;
                    t->unk18 = 0;
                    t->src = p = &src->polys[k];
                    for (m = 0; m < 3; m++) {
                        t->v[m] = &((D3PVERT *)s->info.keys)[((D3SRCPOLY *)t->src)->idx[m]];
                        *(unsigned char *)&t->v[m]->flags |= 0x80;
                    }
                    t->flags = 0;
                    if (((D3SRCPOLY *)t->src)->mat >= 0) {
                        m = mat[((D3SRCPOLY *)t->src)->mat];
                        if (m < 0)
                            mat[((D3SRCPOLY *)t->src)->mat] = m = ((D3TEXTURE **)s->texHandles)[j++] - d3_Textures;
                        t->tex = tex = &d3_Textures[m];
                        if (((unsigned char *)tex)[0x6c] < 0xff || (((unsigned char *)tex)[0x6e] & 3)) {
                            col.c.a = ((unsigned char *)tex)[0x6c];
                            if (((unsigned char *)tex)[0x6e] & 1) {
                                col.c.b = ((unsigned char *)tex)[0x6c];
                                col.c.g = ((unsigned char *)tex)[0x6c];
                                col.c.r = ((unsigned char *)tex)[0x6c];
                            } else {
                                col.c.b = 0xff;
                                col.c.g = 0xff;
                                col.c.r = 0xff;
                            }
                            for (m = 0; m < 3; m++) {
                                t->v[m]->color = col.l;
                                t->v[m]->specular = col.l;
                            }
                        }
                        ((D3TRIBITS *)&t->drawFlags)->b3 = tex->flags;
                        for (m = 0; m < 3; m++)
                            t->v[m]->unk4d = tex->flags & 1;
                    } else
                        t->tex = 0;
                }
                mem_Free(mat);
            }
        }
    }
}
