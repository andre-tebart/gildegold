/* d3_io_fl.c -- d3_io_fl: the fast-chunk BGF loader ("d3_io_fl:LoadFastChunk"), mesh helpers and the
   fast-load chunk writer 0x451250 ("d3_io_fl:AddFLChunk(obj)"): 0x450860..0x451e7c (0x4506f0 / 0x4507a0 may
   open the TU; see d3_io_b.c). Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_fl.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_io_t.h"
#include "d3_io_fl.h"
#include "bio.h"
#include "d3_io_b.h"
#include "d3_vec.h"
#include "mem.h"
#include "ts_texture.h"
#include "vfs.h"

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
/* 0x450dc0, face normals (stored behind each polygon) and the smoothed vertex normals */

void d3_CalcNormals(D3BGF *m)
{
    int i, j;
    float *v;
    D3BGFPOLY *p;
    D3VECTOR4 s;
    D3VECTOR *nrm;
    D3BGFVERT *vt;

    p = m->polys;
    for (i = 0; i < m->npolys; i++) {
        v = m->verts;
        d3_TriNormal((D3VECTOR4 *)(v + p[i].v[0] * 6), (D3VECTOR4 *)(v + p[i].v[1] * 6),
                     (D3VECTOR4 *)(v + p[i].v[2] * 6), (D3VECTOR4 *)p[i].pad2c);
    }
    vt = (D3BGFVERT *)m->verts;
    for (i = 0; i < m->nverts; i++, vt++) {
        s.x = 0.0f;
        s.y = 0.0f;
        s.z = 0.0f;
        p = m->polys;
        for (j = 0; j < m->npolys; j++) {
            if (p[j].v[0] == i || p[j].v[1] == i || p[j].v[2] == i) {
                s.x += ((D3VECTOR *)p[j].pad2c)->x;
                s.y += ((D3VECTOR *)p[j].pad2c)->y;
                s.z += ((D3VECTOR *)p[j].pad2c)->z;
            }
        }
        d3_Normalize(&s);
        vt->nx = s.x;
        vt->ny = s.y;
        vt->nz = s.z;
    }
}
/* 0x450ec0, radius and the 8 corners of the bounding box (appended to the vertices) of a mesh @deep Q4: the 24 corner stores are *(float *)((char *)m->verts + N + m->nverts * 0x18); a typed D3BGFVERT view (vt[c].x/.y/.z) changes the codegen and loses the byte match (tried, reverted to the byte-identical form); behaviour byte */

void d3_CalcBounds(D3BGF *m)
{
    float r, d;
    float *v;
    int i, n;
    unsigned int k;
    float b[6];
    float z1;

    r = 0.0f;
    n = m->nverts;
    if (n > 0) {
        v = m->verts;
        for (i = n; i; i--) {
            d = (float)sqrt(v[1] * v[1] + v[0] * v[0] + v[2] * v[2]);
            if (d > r)
                r = d;
            v += 6;
        }
    }
    m->radius = r;
    if (n > 0) {
        for (k = 0; (int)k < 6; k++) {
            if (k & 1)
                b[k] = -1e10f;
            else
                b[k] = 1e10f;
        }
        z1 = b[5];
        if (m->nverts > 0) {
        v = m->verts;
        for (i = n; i; i--) {
            if (v[0] < b[0])
                b[0] = v[0];
            if (v[0] > b[1])
                b[1] = v[0];
            if (v[1] < b[2])
                b[2] = v[1];
            if (v[1] > b[3])
                b[3] = v[1];
            if (v[2] < b[4])
                b[4] = v[2];
            if (z1 < v[2])
                z1 = v[2];
            v += 6;
        }
        }
        m->verts[m->nverts * 6] = b[0];
        *(float *)((char *)m->verts + 4 + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 8 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 1) * 6] = b[1];
        *(float *)((char *)m->verts + 0x1c + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 0x20 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 2) * 6] = b[0];
        *(float *)((char *)m->verts + 0x34 + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0x38 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 3) * 6] = b[1];
        *(float *)((char *)m->verts + 0x4c + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0x50 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 4) * 6] = b[0];
        *(float *)((char *)m->verts + 0x64 + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 0x68 + m->nverts * 0x18) = z1;
        m->verts[(m->nverts + 5) * 6] = b[1];
        *(float *)((char *)m->verts + 0x7c + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 0x80 + m->nverts * 0x18) = z1;
        m->verts[(m->nverts + 6) * 6] = b[0];
        *(float *)((char *)m->verts + 0x94 + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0x98 + m->nverts * 0x18) = z1;
        m->verts[(m->nverts + 7) * 6] = b[1];
        *(float *)((char *)m->verts + 0xac + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0xb0 + m->nverts * 0x18) = z1;
    }
}
/* 0x451180, size of a mesh in the fast chunk file (the description supplies the material and attachment names) */

int d3_FLChunkSize(D3BGF *m, D3BGFDESC *d)
{
    int sz, n, i;
    char *p;

    sz = 0x18;
    if (m->nverts > 0)
        sz = (m->nverts * 3 + 0x1b) * 8;
    n = m->ntex;
    if (n > 0xfe)
        sz = sz + m->npolys * 0x34;
    else
        sz = sz + m->npolys * 0x31;
    sz += n * 6;
    for (i = 0; i < n; i++)
        sz += strlen(d->mats[i].name[2]) + 1 + strlen(d->mats[i].name[1]) + 1 + strlen(d->mats[i].name[0]) + 1;
    n = d->nattach;
    sz += n * 0x18;
    for (i = 0; i < n; i++)
        sz += strlen(d->attach[i].name) + 1;
    return sz;
}

/* 0x451250: the fast-load chunk writer (was unk_451250.c, worker mesh) */
#define D3_COPY_AT(dvar, svar, size) {                              \
    int _cn = (size);                                               \
    __asm mov edi, dvar                                             \
    __asm mov esi, svar                                             \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

/* 0x451250, loads the BGF file path, cleans it up as d3iot_LoadBGF does and appends the converted mesh to
   the file as a fast-load chunk (0x2d ... 0x2f)
   @deep behaviour: the 377 code hunks are register allocation and stack slots only (seen in the hunks: ebx where the original uses edi as the zero register, [ebp-0x198] slot reuse), no meaning difference; Q2 the unk1e4 parameter (the field is D3BGF.nsets now, the parameter name stays); Q4 7 raw access casts in the writer loop */
void d3_AddFLChunk(char *path, char weld)
{
    char buf[256];
    D3VECTOR4 bv;
    D3BGFDESC desc;
    void *f, *out;
    D3BGF *m;
    D3VECTOR4 v;
    int i, j, k, n, mi, size;
    float s1, c1, s2, c2, t1, t2, u, a, b;
    float *py, *pz;
    char *used, *tmp, *data;

    strcpy(buf, path);
    D3_ZERO(&desc, sizeof desc);
    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (f) {
        d3_ReadBGF(f, &desc);
        if (desc.npoly <= 0 || desc.npts <= 0) {
            vfs_fclose(f);
            return;
        }
        m = mem_Alloc(0x20c, "d3_io_fl:AddFLChunk(obj)");
        if (desc.flag01 == 0 && weld) {
            for (i = 0; i < desc.npts; i++) {
                for (j = 0; j < desc.npts; ) {
                    if (i == j || !d3_VecEqual((D3VECTOR *)(desc.pts + i * 6), (D3VECTOR *)(desc.pts + j * 6), 0.001f)) {
                        j++;
                    } else {
                        for (k = 0; k < desc.npoly; k++) {
                            for (n = 0; n < 3; n++) {
                                if (desc.polys[k].v[n] == j)
                                    desc.polys[k].v[n] = i;
                                else if (desc.polys[k].v[n] > j)
                                    desc.polys[k].v[n]--;
                            }
                        }
                        desc.npts--;
                        memmove(desc.pts + j * 6, desc.pts + (j + 1) * 6, (desc.npts - j) * 0x18);
                    }
                }
            }
        }
        m->nverts = desc.npts;
        m->verts = mem_Alloc((desc.npts * 3 + 0x18) * 8, "d3_io_fs:AddFLChunk(poi)");
        s1 = (float)sin(-1.5707963705062866);
        c1 = (float)cos(-1.5707963705062866);
        s2 = (float)sin(3.1415927410125732);
        c2 = (float)cos(3.1415927410125732);
        for (k = 0; k < m->nverts; k++) {
            v.x = desc.pts[k * 6];
            py = &desc.pts[k * 6 + 1];
            pz = &desc.pts[k * 6 + 2];
            t1 = c1 * *py - s1 * *pz;
            t2 = s1 * *py + c1 * *pz;
            u = v.x * c2 + s2 * t2;
            bv.z = t2 * c2 - v.x * s2;
            u = u * -1.0;
            m->verts[k * 6] = bv.z * s2 + c2 * u;
            m->verts[k * 6 + 1] = t1;
            m->verts[k * 6 + 2] = bv.z * c2 - u * s2;
        }
        if (desc.nmat > 0) {
            used = mem_Alloc(desc.nmat, "d3_io_fs:RemoveDoubleMaterials");
            for (i = 1; i - 1 < desc.nmat; i++) {
                if (!used[i - 1]) {
                    used[i - 1] = 1;
                    for (j = i; j < desc.nmat; j++) {
                        if (!memcmp(&desc.mats[i - 1], &desc.mats[j], 0xe0)) {
                            for (k = 0; k < desc.npoly; k++)
                                if (desc.polys[k].mat == j)
                                    desc.polys[k].mat = i - 1;
                            used[j] = 1;
                        }
                    }
                }
            }
            D3FL_ZERO_AT(used, desc.nmat);
            n = desc.nmat;
            for (k = 0; k < desc.npoly; k++)
                if (desc.polys[k].mat >= 0)
                    used[desc.polys[k].mat] = 1;
            for (i = 0; i < n; ) {
                if (!used[i]) {
                    for (k = 0; k < desc.npoly; k++)
                        if (desc.polys[k].mat > i)
                            desc.polys[k].mat--;
                    n--;
                    memmove(&desc.mats[i], &desc.mats[i + 1], (n - i) * 0xe0);
                    memmove(used + i, used + i + 1, n - i);
                } else {
                    i++;
                }
            }
            desc.nmat = n;
            mem_Free(used);
        }
        m->npolys = desc.npoly;
        m->polys = mem_Alloc(desc.npoly * 0x38, "d3_io_fs:AddFLChunk(pol)");
        D3FL_COPY(m->polys, desc.polys, m->npolys * 0x38);
        used = mem_Alloc(desc.nmat, "d3_io_fs:Materials(bool)");
        tmp = mem_Alloc(desc.nmat * 0xe0, "d3_io_fs:TempMat[ReOrder]");
        D3_COPY_AT(tmp, desc.mats, desc.nmat * 0xe0);
        k = 0;
        for (i = 0; i < m->npolys; i++) {
            mi = desc.polys[i].mat;
            if (mi > -1 && !used[mi]) {
                used[mi] = 1;
                for (j = 0; j < m->npolys; j++)
                    if (desc.polys[j].mat == mi)
                        m->polys[j].mat = k;
                D3FL_COPY3((char *)desc.mats + k * 0xe0, tmp + mi * 0xe0, 0xe0);
                k++;
            }
        }
        mem_Free(tmp);
        mem_Free(used);
        for (i = 0; i < m->npolys; i++) {
            mi = m->polys[i].mat;
            if (mi >= 0) {
                for (n = 0; n < 3; n++) {
                    a = (m->polys[i].uv[n][0] * desc.mats[mi].su + 0.5f) - desc.mats[mi].su * 0.5f;
                    b = (m->polys[i].uv[n][1] * desc.mats[mi].sv + 0.5f) - desc.mats[mi].sv * 0.5f;
                    m->polys[i].uv[n][0] = (float)cos(desc.mats[mi].rot) * a - (float)sin(desc.mats[mi].rot) * b + desc.mats[mi].ou;
                    m->polys[i].uv[n][1] = (float)sin(desc.mats[mi].rot + 3.1415927f) * a + (float)cos(desc.mats[mi].rot + 3.1415927f) * b + desc.mats[mi].ov;
                }
            }
        }
        m->ntex = desc.nmat;
        m->nsets = 0;
        d3_CalcBounds(m);
        d3_CalcNormals(m);
        size = vfs_ftell(f);
        vfs_fseek(f, 0, 0);
        data = mem_Alloc(size, "d3_io_fl:NotMyChunkBufferStart");
        vfs_fread(data, size, 1, f);
        vfs_fclose(f);
        out = vfs_Tfopen(buf, "wb", vfs_Root);
        vfs_fwrite(data, size, 1, out);
        mem_Free(data);
        bio_wr_c(out, 0x2d);
        bio_wr_i(out, d3_FLChunkSize(m, &desc));
        bio_wr_i(out, 0xfab50005);
        bio_wr_i(out, m->ntex);
        bio_wr_i(out, m->nverts);
        bio_wr_i(out, m->npolys);
        if (m->nverts > 0)
            for (i = 0; i < m->nverts + 8; i++) {
                bio_wr_v3(out, &m->verts[i * 6]);
                bio_wr_v3(out, &m->verts[i * 6 + 3]);
            }
        bio_wr_i(out, *(int *)&m->radius);
        for (i = 0; i < m->npolys; i++) {
            bio_wr_i(out, m->polys[i].v[0]);
            bio_wr_i(out, m->polys[i].v[1]);
            bio_wr_i(out, m->polys[i].v[2]);
            ((int *)&v)[0] = *(int *)&m->polys[i].uv[0][0];
            ((int *)&v)[1] = *(int *)&m->polys[i].uv[1][0];
            ((int *)&v)[2] = *(int *)&m->polys[i].uv[2][0];
            bio_wr_v3(out, &v);
            ((int *)&v)[0] = *(int *)&m->polys[i].uv[0][1];
            ((int *)&v)[1] = *(int *)&m->polys[i].uv[1][1];
            ((int *)&v)[2] = *(int *)&m->polys[i].uv[2][1];
            bio_wr_v3(out, &v);
            bio_wr_v3(out, m->polys[i].pad2c);
            if (m->ntex > 0xfe) {
                bio_wr_i(out, m->polys[i].mat);
            } else {
                if (m->polys[i].mat == -1)
                    bio_wr_c(out, 0xff);
                else
                    bio_wr_c(out, (char)m->polys[i].mat);
            }
        }
        for (i = 0; i < m->ntex; i++) {
            bio_wr_s(out, desc.mats[i].name[0]);
            bio_wr_s(out, desc.mats[i].name[2]);
            bio_wr_s(out, desc.mats[i].name[1]);
            bio_wr_c(out, desc.mats[i].kind);
            bio_wr_c(out, desc.mats[i].c1);
            bio_wr_c(out, ((desc.mats[i].c3 << 4 | desc.mats[i].c6) * 2 | desc.mats[i].c5) * 2 | desc.mats[i].c2);
            bio_wr_c(out, desc.mats[i].c7 & 0xfe | desc.mats[i].pad4);
            bio_wr_c(out, desc.mats[i].c8);
            bio_wr_c(out, desc.mats[i].c9);
        }
        bio_wr_i(out, desc.nattach);
        for (i = 0; i < desc.nattach; i++) {
            bio_wr_s(out, desc.attach[i].name);
            bio_wr_v3(out, &desc.attach[i].f[0]);
            bio_wr_v3(out, &desc.attach[i].f[3]);
        }
        bio_wr_c(out, 0x2f);
        vfs_ftell(out);
        vfs_fclose(out);
        if (desc.nmat > 0 && desc.mats) {
            mem_Free(desc.mats);
            desc.mats = 0;
        }
        if (m->npolys > 0 && m->polys) {
            mem_Free(m->polys);
            m->polys = 0;
        }
        if (desc.npoly > 0 && desc.polys) {
            mem_Free(desc.polys);
            desc.polys = 0;
        }
        if (m->nverts > 0 && m->verts) {
            mem_Free(m->verts);
            m->verts = 0;
        }
        if (desc.npts > 0 && desc.pts) {
            mem_Free(desc.pts);
            desc.pts = 0;
        }
        mem_Free(m);
    }
}
