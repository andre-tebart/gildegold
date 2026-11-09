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

extern float ai_FloatZero;  /* 0x5eb788, the shared 0.0f (cm.c) */

/* 0x450860, loads a mesh from its fast chunk file (the cache the BGF loader looks in first); returns 0 when
   there is none
   @deep whole asmdiff read; typed the polygon / vertex / attachment reads (D3BGFPOLY.v / uv / n / tex /
   mat, D3BGFVERT, D3BGF.nattach), the attachment loop is one for (the extra test eax,eax; jle gone). Left
   (ours=425 original=425, 26 differing): two register / scheduling hunks only. 1) fl.s.c1 = mt.c2 & 1 is
   the same add/xor/and 2/xor bit insert, the original in bl (0x450b97 mov bl,al; add bl,bl; xor bl,cl; and
   bl,2; xor cl,bl) before it loads mt.name[2][0] into bl, ours in dl; 2) both D3BGF_TEXLOAD calls push the
   same four arguments (flags byte, kind byte, fl, texnames + off2) with the loads of fl ([esp+0x14]) and
   m->texnames + off2 in other registers / order (0x450c2a / 0x450c34, 0x450c96 / 0x450ca8). The prover
   stops at a 1-byte shift (+0x351 / +0x32c), the same bitfield shr. */
D3BGF *d3_FindBGF(char *path, char *name, char *texnames, int ntexsets, int ntex)
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

    f = d3_OpenFastChunk(path);
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
                bio_rd_v3(f, (int *)&((D3BGFVERT *)m->verts)[cnt].x);
                bio_rd_v3(f, (int *)&((D3BGFVERT *)m->verts)[cnt].nx);
            }
        }
        bio_rd_i(f, (int *)&m->radius);
        m->polys = (D3BGFPOLY *)mem_Alloc(m->npolys * 0x38, "d3_io_fl:LoadFastChunk(pol)");
        for (a = 0; a < m->npolys; a++) {
            bio_rd_i(f, &m->polys[a].v[0]);
            bio_rd_i(f, &m->polys[a].v[1]);
            bio_rd_i(f, &m->polys[a].v[2]);
            bio_rd_v3(f, (int *)&t);
            m->polys[a].uv[0][0] = t.x;
            m->polys[a].uv[1][0] = t.y;
            m->polys[a].uv[2][0] = t.z;
            bio_rd_v3(f, (int *)&t);
            m->polys[a].uv[0][1] = t.x;
            m->polys[a].uv[1][1] = t.y;
            m->polys[a].uv[2][1] = t.z;
            bio_rd_v3(f, (int *)&m->polys[a].n);
            m->polys[a].tex = -1;
            if (m->ntex > 0xfe) {
                bio_rd_i(f, &m->polys[a].mat);
            } else {
                bio_rd_c(f, (char *)&cb);
                if ((char)cb == -1)
                    m->polys[a].mat = -1;
                else
                    m->polys[a].mat = cb & 0xff;
            }
        }
        if (!texnames || ntexsets <= 0 || ntex != m->ntex) {
            m->nsets = 1;
            m->texnames = (char *)mem_Alloc(m->ntex << 6, "d3_io_fl:LoadFastChunk(texset)");
        } else {
            m->nsets = ntexsets;
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
        m->nattach = (unsigned char)idx;
        for (a = 0; a < idx; a++) {
            bio_rd_s(f, m->attach[a].name);
            bio_rd_v3(f, (int *)&m->attach[a].f[0]);
            bio_rd_v3(f, (int *)&m->attach[a].f[3]);
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
                     (D3VECTOR4 *)(v + p[i].v[2] * 6), (D3VECTOR4 *)&p[i].n);
    }
    vt = (D3BGFVERT *)m->verts;
    for (i = 0; i < m->nverts; i++, vt++) {
        s.x = 0.0f;
        s.y = 0.0f;
        s.z = 0.0f;
        p = m->polys;
        for (j = 0; j < m->npolys; j++) {
            if (p[j].v[0] == i || p[j].v[1] == i || p[j].v[2] == i) {
                s.x += p[j].n.x;
                s.y += p[j].n.y;
                s.z += p[j].n.z;
            }
        }
        d3_Normalize(&s);
        vt->nx = s.x;
        vt->ny = s.y;
        vt->nz = s.z;
    }
}
/* 0x450ec0, radius and the 8 corners of the bounding box (appended to the vertices) of a mesh */

void d3_CalcBounds(D3BGF *m)
{
    float r, d;
    float *v;
    int i, n;
    unsigned int k;
    float b[6];
    float z1;

    r = ai_FloatZero;
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
        /* the 8 box corners are appended as vertex records (six floats each: x, y, z and three unused
           normals); the min/max of each axis is b[0]/b[1] (x), b[2]/b[3] (y), b[4]/z1 (z) */
        m->verts[m->nverts * 6] = b[0];
        m->verts[m->nverts * 6 + 1] = b[2];
        m->verts[m->nverts * 6 + 2] = b[4];
        m->verts[(m->nverts + 1) * 6] = b[1];
        m->verts[(m->nverts + 1) * 6 + 1] = b[2];
        m->verts[(m->nverts + 1) * 6 + 2] = b[4];
        m->verts[(m->nverts + 2) * 6] = b[0];
        m->verts[(m->nverts + 2) * 6 + 1] = b[3];
        m->verts[(m->nverts + 2) * 6 + 2] = b[4];
        m->verts[(m->nverts + 3) * 6] = b[1];
        m->verts[(m->nverts + 3) * 6 + 1] = b[3];
        m->verts[(m->nverts + 3) * 6 + 2] = b[4];
        m->verts[(m->nverts + 4) * 6] = b[0];
        m->verts[(m->nverts + 4) * 6 + 1] = b[2];
        m->verts[(m->nverts + 4) * 6 + 2] = z1;
        m->verts[(m->nverts + 5) * 6] = b[1];
        m->verts[(m->nverts + 5) * 6 + 1] = b[2];
        m->verts[(m->nverts + 5) * 6 + 2] = z1;
        m->verts[(m->nverts + 6) * 6] = b[0];
        m->verts[(m->nverts + 6) * 6 + 1] = b[3];
        m->verts[(m->nverts + 6) * 6 + 2] = z1;
        m->verts[(m->nverts + 7) * 6] = b[1];
        m->verts[(m->nverts + 7) * 6 + 1] = b[3];
        m->verts[(m->nverts + 7) * 6 + 2] = z1;
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
   @deep whole asmdiff read; fixed: the material de-dup loop is 0-based with j = i + 1 (VC6's induction
   variable i+1 in [ebp+0xc], used[i] as [eax+edx-1], mat = i as lea [esi-1] now match), the uv rows are
   written from float fields, bio_wr_c takes unsigned char (push 0xff, as the original), the radius is
   written through D3BGF.radiusBits. Left: (1) allocation: the original keeps m in its slot [ebp-0x10] and
   0 in ebx through the weld loops, ours m in ebx (+ a copy in [ebp-0x20]); (2) the vertex rotation
   (0x451494..0x451517): the same products and sums, the original holds py / pz in edx / ecx (lea
   [eax+ecx+4] / [eax+ecx+8]) and evaluates s1*py before c1*pz and u before bv.z, ours folds them into
   [eax+ecx-0x14] and orders the FPU operands otherwise; (3) the uv row copies (0x451b9d..0x451bd5): the
   original copies the three floats with integer moves off one &m->polys[i] (add eax,ebx), ours fld/fstp
   with m->polys reloaded after each store (tried an array v and int copies: no change / worse); (4) block
   layout: the original places the early return of an empty BGF (vfs_fclose; its own epilogue) inside the
   polygon write loop at 0x451c0b, ours after the main epilogue, which also shifts the add esp merges
   around 0x451a56..0x451b04. (references: M8 at +0xc2b: ours refers to vfs_fclose (0x430aa0), the
   original's field to 0x422620 (mem_Free), explained: the ref line (ours vfs_fclose at +0xc2b, the
   original's mem_Free) is the alignment pairing our early-return block (vfs_fclose) at the end with the
   original's final mem_Free(m): the original has the same vfs_fclose call at 0x451c0c, placed
   mid-function; both sides call vfs_fclose and mem_Free the same number of times) */
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
            for (i = 0; i < desc.nmat; i++) {
                if (!used[i]) {
                    used[i] = 1;
                    for (j = i + 1; j < desc.nmat; j++) {
                        if (!memcmp(&desc.mats[i], &desc.mats[j], sizeof(D3BGFMAT))) {
                            for (k = 0; k < desc.npoly; k++)
                                if (desc.polys[k].mat == j)
                                    desc.polys[k].mat = i;
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
        bio_wr_i(out, m->radiusBits);
        for (i = 0; i < m->npolys; i++) {
            bio_wr_i(out, m->polys[i].v[0]);
            bio_wr_i(out, m->polys[i].v[1]);
            bio_wr_i(out, m->polys[i].v[2]);
            v.x = m->polys[i].uv[0][0];
            v.y = m->polys[i].uv[1][0];
            v.z = m->polys[i].uv[2][0];
            bio_wr_v3(out, (int *)&v);
            v.x = m->polys[i].uv[0][1];
            v.y = m->polys[i].uv[1][1];
            v.z = m->polys[i].uv[2][1];
            bio_wr_v3(out, (int *)&v);
            bio_wr_v3(out, (int *)&m->polys[i].n);
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
