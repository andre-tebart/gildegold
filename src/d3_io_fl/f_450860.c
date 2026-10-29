/* 0x450860, loads a mesh from its fast chunk file (the cache the BGF loader looks in first); returns 0 when
   there is none
   @deep behaviour: the 220 code hunks are register allocation only (the original keeps the file handle in ebp and compares against the zero register edi - cmp ebp,edi / cmp eax,edi - where ours uses ebx as zero and test against 0; same instructions otherwise), readable not: Q2 d3_Unk4507a0 (callee in d3_io_b.c, opens the chunk file, no naming evidence beyond that) and the unk1e4 parameter (the field is D3BGF.nsets now, the parameter name stays), Q4 the 9 poly accesses (a typed D3BGFPOLY *p loop was tried and reverted: 102 -> 296 differing asm lines, the original computes m->polys + byte offset per write; the +0x2c block has no named field and Q2 forbids an unk name) */
D3BGF *d3_FindBGF(char *path, char *name, char *texnames, int unk1e4, int ntex)
{
    int a = 0;
    int b;
    D3BGF *m = 0;
    volatile char kept = 0;
    void *f;
    int *pn;
    int cnt, off2, idx, j, c;
    int cb;
    D3VECTOR4 t;
    D3BGFMAT mt;
    D3BGFFLAGS fl;
    D3TEXHANDLE *tex;

    f = d3_Unk4507a0(path);
    if (f) {
        m = (D3BGF *)mem_Alloc(0x20c, "d3_io_fl:LoadFastChunk(obj)");
        strncpy(m->name, name, 0x3f);
        bio_rd_i(f, &m->ntex);
        pn = &m->nverts;
        bio_rd_i(f, pn);
        bio_rd_i(f, &m->npolys);
        if (*pn > 0) {
            m->verts = (float *)mem_Alloc((*pn * 3 + 0x18) * 8, "d3_io_fl:LoadFastChunk(poi)");
            for (cnt = 0; cnt < *pn + 8; cnt++) {
                bio_rd_v3(f, (int *)((char *)m->verts + a));
                bio_rd_v3(f, (int *)((char *)m->verts + a + 0xc));
                a += 0x18;
            }
        }
        bio_rd_i(f, (int *)&m->radius);
        m->polys = (D3BGFPOLY *)mem_Alloc(m->npolys * 0x38, "d3_io_fl:LoadFastChunk(pol)");
        a = 0;
        if (m->npolys > 0) {
            b = 0;
            do {
                bio_rd_i(f, (int *)((char *)m->polys + b + 0x18));
                bio_rd_i(f, (int *)((char *)m->polys + b + 0x1c));
                bio_rd_i(f, (int *)((char *)m->polys + b + 0x20));
                bio_rd_v3(f, (int *)&t);
                *(float *)((char *)m->polys + b) = t.x;
                *(float *)((char *)m->polys + b + 8) = t.y;
                *(float *)((char *)m->polys + b + 0x10) = t.z;
                bio_rd_v3(f, (int *)&t);
                *(float *)((char *)m->polys + b + 4) = t.x;
                *(float *)((char *)m->polys + b + 0xc) = t.y;
                *(float *)((char *)m->polys + b + 0x14) = t.z;
                bio_rd_v3(f, (int *)((char *)m->polys + b + 0x2c));
                *(int *)((char *)m->polys + b + 0x24) = -1;
                if (m->ntex > 0xfe) {
                    bio_rd_i(f, (int *)((char *)m->polys + b + 0x28));
                } else {
                    bio_rd_c(f, (char *)&cb);
                    if ((char)cb == -1)
                        *(int *)((char *)m->polys + b + 0x28) = -1;
                    else
                        *(int *)((char *)m->polys + b + 0x28) = cb & 0xff;
                }
                a++;
                b += 0x38;
            } while (a < m->npolys);
        }
        if (!texnames || unk1e4 <= 0 || ntex != m->ntex) {
            m->nsets = 1;
            m->texnames = (char *)mem_Alloc(m->ntex << 6, "d3_io_fl:LoadFastChunk(texset)");
        } else {
            m->nsets = unk1e4;
            m->texnames = texnames;
            kept = 1;
        }
        cnt = 0;
        if (m->ntex > 0) {
            off2 = 0;
            do {
                bio_rd_s(f, mt.name[0]);
                bio_rd_s(f, mt.name[2]);
                bio_rd_s(f, mt.name[1]);
                bio_rd_c(f, (char *)&mt.kind);
                bio_rd_c(f, &mt.c1);
                bio_rd_c(f, &mt.c2);
                bio_rd_c(f, &mt.pad4);
                bio_rd_c(f, &mt.c8);
                bio_rd_c(f, &mt.c9);
                fl.all = 0;
                fl.s.b0 = 0xff;
                if (mt.kind > 0) {
                    fl.s.b0 = mt.c1;
                    if (mt.kind == 2)
                        fl.s.c0 |= 1;
                }
                fl.s.c1 = mt.c2 & 1;
                mt.c5 = ((unsigned char)mt.c2 >> 1) & 1;
                mt.c6 = ((unsigned char)mt.c2 >> 2) & 0xf;
                mt.c3 = ((unsigned char)mt.c2 >> 6) & 1;
                if (mt.name[2][0]) {
                    if (!kept)
                        strcpy(m->texnames + off2, mt.name[2]);
                    tex = D3BGF_TEXLOAD(m->texnames + off2, fl, (((mt.c3 << 4 | mt.c6) * 2 | mt.c5) * 2) | 1, mt.c8 << 4 | mt.c9);
                } else if (mt.name[0][0]) {
                    if (!kept)
                        strcpy(m->texnames + off2, mt.name[0]);
                    tex = D3BGF_TEXLOAD(m->texnames + off2, fl, ((mt.c3 << 4 | mt.c6) * 2 | mt.c5) * 2, mt.c8 << 4 | mt.c9);
                } else if (mt.name[1][0]) {
                    if (!kept)
                        strcpy(m->texnames + off2, mt.name[1]);
                }
                idx = tex ? (int)(((char *)tex - (char *)d3_Textures) >> 7) : -1;
                for (j = 0; j < m->npolys; j++)
                    if (m->polys[j].mat == cnt)
                        m->polys[j].tex = idx;
                cnt++;
                off2 += 0x40;
            } while (cnt < m->ntex);
        }
        bio_rd_i(f, &idx);
        m->pad208[0] = (char)idx;
        a = 0;
        if (idx > 0) {
            for (j = 0; a < idx; ) {
                bio_rd_s(f, m->attach[j].name);
                bio_rd_v3(f, (int *)&m->attach[j].f[0]);
                bio_rd_v3(f, (int *)&m->attach[j].f[3]);
                a++;
                j++;
            }
        }
        vfs_fclose(f);
    }
    return m;
}
