/* d3_io_t.c -- BGF mesh loader and object / animation file loading of the d3 scene I/O ("d3_io_t:" / "d3_io:"
   allocation tags), 0x44cd40..0x44e570; tag "d3_int_io:" (d3_int_io.c) is a separate TU. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_t.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_io_t.h"
#include "d3_io_fl.h"
#include "data_views.h"
#include "d3_int_io.h"
#include "d3_anim.h"
#include "bio.h"
#include "d3_io_b.h"
#include "d3_vec.h"
#include "err.h"
#include "mem.h"
#include "ts_texture.h"
#include "d3_vtx.h"
#include "vfs.h"
#define d3anim_NextFrameIo ((int (*)(D3IOANIMFLAGS, int, int, int, int))d3anim_NextFrame)  /* 0x4324e0 (d3_anim.h: D3ANIMFLAGS f): @original this unit passes its field-wise D3IOANIMFLAGS (12 bytes), as the original TU did */

/* 0x44c510, sets the directory the stock objects are searched in */

void d3io_SetStockDir(char *path)
{
    d3io_StockDir = vfs_FindDir(path, vfs_Root);
}
/* 0x44c530, finds the file of a stock object ("*" name ext) in the stock directory */

char *d3io_StockFile(char *name, char *ext)
{
    char buf[0x100];

    strcpy(buf, "*");
    strcat(buf, name);
    strcat(buf, ext);
    return vfs_GetPath(buf, d3io_StockDir, vfs_Root, d3io_StockPath);
}
/* 0x44c5d0, the loaded stock object of that name */

int d3io_FindStock(char *name)
{
    D3BGF *s;

    for (s = d3_StockListStart.next; s != &d3_StockListEnd; s = s->next)
        if (!_strnicmp(name, s->name, 0x3f))
            return (int)s;
    return 0;
}
/* 0x44c610, builds surf from the stock object `name`: vertices and triangles, vertex colours from the colour
   key, the texture set; counts the surface (unless it is the shadow surface 3) */

void d3io_AttachStock(D3OBJECT *obj, D3SURF *surf, char *name)
{
    D3IOCOL col;
    D3IOSTOCK *stock;
    D3IOSURF *s;
    D3IOTRI *tri;
    D3IOTEX *tex;
    D3MESH *mesh;
    char buf[0x100];
    int i, k;

    stock = (D3IOSTOCK *)d3io_FindStock(name);
    if (stock) {
        s = (D3IOSURF *)surf;
        s->nkeys = stock->npoints;
        s->ntris = stock->npolys;
        s->stock = stock;
        s->f194 = 0;
        d3_AllocObjectPolys((D3OBJINFO *)s);
        col.all = -1;
        if (s->nkeys > 0)
            for (i = 0; i < s->nkeys + 8; i++) {
                s->keys[i].src = stock->points + i * 0x18;
                s->keys[i].f4c = 0;
                s->keys[i].col40.all = -1;
                s->keys[i].col44.all = -1;
            }
        tri = s->tris;
        for (i = 0; i < s->ntris; i++, tri++) {
            tri->f20 = 0;
            tri->fl.b0 = 0;
            tri->f1c = 0;
            tri->f18 = 0;
            tri->src = (D3IOPOLY *)stock->polys + i;
            for (k = 0; k < 3; k++) {
                D3IOVERT *v = &s->keys[tri->src->idx[k]];
                tri->v[k] = v;
                v->f4c |= 0x80;
            }
            tri->f24 = 0;
            if (tri->src->tex >= 0) {
                tex = (D3IOTEX *)d3_Textures + tri->src->tex;
                tri->tex = (D3IOTEX *)d3_Textures + tri->src->tex;
                if (tex->colorkey < 0xff || (tex->flags & 3)) {
                    col.a = tex->colorkey;
                    if (tex->flags & 1) {
                        col.b = tex->colorkey;
                        col.g = tex->colorkey;
                        col.r = tex->colorkey;
                    } else {
                        col.b = 0xff;
                        col.g = 0xff;
                        col.r = 0xff;
                    }
                    for (k = 0; k < 3; k++) {
                        tri->v[k]->col40 = col;
                        tri->v[k]->col44 = col;
                    }
                }
                tri->fl.b3 = tex->f68;
                for (k = 0; k < 3; k++)
                    tri->v[k]->f4d = tex->f68 & 1;
            } else
                tri->tex = 0;
        }
        s->texSet = 0;
        s->texset = mem_Alloc(stock->ntex * 4, "d3_io:CurTexSet");
        for (i = 0; i < stock->ntex; i++)
            for (k = 0; k < s->ntris; k++)
                if (s->tris[k].src && s->tris[k].src->slot == i) {
                    if (s->tris[k].tex) {
                        s->texset[i] = s->tris[k].tex;
                        tex_AddRef((D3FLTEX *)s->texset[i], 1);
                    } else
                        sprintf(buf, "stock object %s, texture %s not found", stock, stock->texnames[i]);
                    break;
                }
        for (i = 0; i < stock->ntex; i++)
            if (!s->texset[i]) {
                strcpy(buf, "Error attaching Stock-Object \"");
                strcat(buf, stock->name);
                strcat(buf, "\": Not all Textures used/found!");
                break;
            }
        mesh = D3_MESH(obj);
        if (surf != &mesh->surf[3]) {
            mesh->nsurf++;
            D3IO_214(obj) = 4;
            if (!obj->info) {
                obj->info = d3_SelectSurf(obj);
                if (!obj->info) {
                    k = 0;
                    do {
                        if (k >= 3)
                            break;
                        if (D3_MESH(obj)->surf[k].info.ntris > 0 && D3_MESH(obj)->surf[k].info.nkeys > 0 &&
                            D3_MESH(obj)->surf[k].info.mesh)
                            obj->info = &D3_MESH(obj)->surf[k].info;
                        k++;
                    } while (!obj->info);
                }
            }
        }
        stock->refs++;
    }
}
/* 0x44c9d0, the file name(s) of a stock object's LOD: lod < 0 the "_s" variant, lod 0 the best one found
   (mode 2) or the base name, else "name_<lod - 1>" */

char d3io_LodName(char *name, char *name2, int lod, char *out, char *out2)
{
    if (lod < 0) {
        if (d3io_Lod.shadow) {
            strcpy(out, name);
            strcat(out, "_s");
            if (name2 && out2) {
                strcpy(out2, name2);
                strcat(out2, "_s");
            }
            return 1;
        }
    } else if (lod == 0) {
        if (d3io_Lod.mode == 2) {
            for (lod = 2; lod >= 0; lod--) {
                if (lod > 0) {
                    sprintf(out, "%s_%i", name, lod - 1);
                    if (name2 && out2)
                        sprintf(out2, "%s_%i", name2, lod - 1);
                } else {
                    strcpy(out, name);
                    if (name2 && out2)
                        strcpy(out2, name2);
                }
                if (d3io_StockFile(out, ".bgf"))
                    return 1;
            }
        } else {
            strcpy(out, name);
            if (name2 && out2)
                strcpy(out2, name2);
            return 1;
        }
    } else {
        int k = d3io_Lod.mode == 2 ? 2 - lod : lod;
        sprintf(out, "%s_%i", name, k - 1);
        if (name2 && out2)
            sprintf(out2, "%s_%i", name2, k - 1);
        return 1;
    }
    return 0;
}
/* 0x44cbb0, attaches the stock object `name` to obj: into surf, or (surf 0) its LODs into the mesh's
   surfaces 0..2 and the "_s" variant into surface 3 */

void d3_AttachMesh(D3OBJECT *obj, int unk, char *name)
{
    D3SURF *surf = (D3SURF *)unk;
    char buf[0x100];
    int i, n;

    if (surf) {
        if (d3io_FindStock(name)) {
            if (!obj->material)
                d3_AddObjectDrawData(obj);
            d3io_AttachStock(obj, surf, name);
        }
    } else {
        d3io_LodName(name, 0, 0, buf, 0);
        if (d3io_FindStock(buf)) {
            if (!obj->material)
                d3_AddObjectDrawData(obj);
            d3io_AttachStock(obj, &D3_MESH(obj)->surf[0], buf);
            if (d3io_LodName(name, 0, -1, buf, 0) && d3io_FindStock(buf))
                d3io_AttachStock(obj, &D3_MESH(obj)->surf[3], buf);
            if (d3io_Lod.mode == 1) {
                for (i = 1, n = 1; i < 3; i++) {
                    if (n >= 3)
                        break;
                    if (d3io_LodName(name, 0, i, buf, 0) && d3io_FindStock(buf)) {
                        d3io_AttachStock(obj, &D3_MESH(obj)->surf[n], buf);
                        n++;
                    }
                }
            }
        }
    }
}
/* 0x44cd40, drops a reference to a mesh and frees it with its textures when the last one is gone */
void d3iot_DeleteStockObject(D3BGF *m)
{
    int i, t, max;
    char *used;
    int z = 0;

    if (--m->refs <= z) {
        m->next->prev = m->prev;
        m->prev->next = m->next;
        max = -1;
        for (i = z; i < m->npolys; i++)
            if (m->polys[i].tex > max)
                max = m->polys[i].tex;
        used = mem_Alloc(max + 1, "d3_io_t:DeleteStockObject");
        for (i = z; i < m->npolys; i++) {
            t = m->polys[i].tex;
            if (t != -1 && !used[t]) {
                tex_Release((D3TEXHANDLE *)&((D3TEXTURE *)d3_Textures)[t]);
                used[m->polys[i].tex] = 1;
            }
        }
        mem_Free(used);
        mem_Free(m->verts);
        m->verts = 0;
        mem_Free(m->polys);
        m->polys = 0;
        mem_Free(m->texnames);
        m->texnames = 0;
        mem_Free(m);
    }
}
/* 0x44ce40, radius and bounding box (8 corners appended to the vertices) of a mesh */

void d3iot_CalcBounds(D3BGF *m)
{
    float r, d, dz, z1;
    float *v, *v0;
    int i, n;
    D3VECTOR4 mn, mx;

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
    m->radius = m->diagonal = r;
    m->cx = 0.0f;
    m->cy = 0.0f;
    m->cz = 0.0f;
    if (n > 0) {
        mx.x = -1e10f;
        z1 = -1e10f;
        mx.y = -1e10f;
        mn.x = 1e10f;
        mn.y = 1e10f;
        mn.z = 1e10f;
        v0 = m->verts;
        v = m->verts;
        for (i = n; i; i--) {
            if (!(mn.x < v[0])) mn.x = v[0];
            mn.y = min(mn.y, v[1]);
            mn.z = min(mn.z, v[2]);
            if (!(mx.x > v[0])) mx.x = v[0];
            mx.y = max(mx.y, v[1]);
            z1 = max(z1, v[2]);
            v += 6;
        }
        v0[n * 6] = mn.x;
        D3BGF_CORNER(m, 0)->y = mn.y;
        D3BGF_CORNER(m, 0)->z = mn.z;
        m->verts[(m->nverts + 1) * 6] = mx.x;
        D3BGF_CORNER(m, 1)->y = mn.y;
        D3BGF_CORNER(m, 1)->z = mn.z;
        m->verts[(m->nverts + 2) * 6] = mn.x;
        D3BGF_CORNER(m, 2)->y = mx.y;
        D3BGF_CORNER(m, 2)->z = mn.z;
        m->verts[(m->nverts + 3) * 6] = mx.x;
        D3BGF_CORNER(m, 3)->y = mx.y;
        D3BGF_CORNER(m, 3)->z = mn.z;
        m->verts[(m->nverts + 4) * 6] = mn.x;
        D3BGF_CORNER(m, 4)->y = mn.y;
        D3BGF_CORNER(m, 4)->z = z1;
        m->verts[(m->nverts + 5) * 6] = mx.x;
        D3BGF_CORNER(m, 5)->y = mn.y;
        D3BGF_CORNER(m, 5)->z = z1;
        m->verts[(m->nverts + 6) * 6] = mn.x;
        D3BGF_CORNER(m, 6)->y = mx.y;
        D3BGF_CORNER(m, 6)->z = z1;
        m->verts[(m->nverts + 7) * 6] = mx.x;
        D3BGF_CORNER(m, 7)->y = mx.y;
        D3BGF_CORNER(m, 7)->z = z1;
        v = m->verts + m->nverts * 6;
        mx.x -= mn.x;
        mx.y -= mn.y;
        dz = z1 - mn.z;
        d = (float)sqrt(dz * dz + mx.y * mx.y + mx.x * mx.x);
        m->diagonal = d;
        mn.x = 0.0f;
        mn.y = 0.0f;
        mn.z = 0.0f;
        for (i = 8; i; i--) {
            mn.x += v[0];
            mn.y += v[1];
            mn.z += v[2];
            v += 6;
        }
        m->cx = mn.x * 0.125f;
        m->cy = mn.y * 0.125f;
        m->cz = mn.z * 0.125f;
    }
}
/* 0x44d1c0, finishes a loaded mesh: always computes its bounds (radius, diagonal, centre, box corners),
   and its normals when bounds is set; returns nothing */

void d3iot_Finish(D3BGF *m, char bounds)
{
    d3iot_CalcBounds(m);
    if (bounds)
        d3_CalcNormals(m);
}
/* 0x44d1f0, reads the texture set (.TXS) of a mesh: w x h names of 0x40 bytes */

void d3iot_LoadTexSet(char *name, char **out, int *w, int *h)
{
    char *path;
    void *f;
    int i, j;

    if (name) {
        path = d3io_StockFile(name, ".TXS");
        if (path) {
            f = vfs_Tfopen(path, "rb", vfs_Root);
            if (f) {
                bio_rd_i(f, &i);
                if (i == 0x23f209ae) {
                    bio_rd_i(f, &i);
                    bio_rd_i(f, &j);
                    if (i > 0 && j > 0) {
                        if (*out) {
                            mem_Free(*out);
                            *out = 0;
                        }
                        *out = mem_Alloc(j * i * 0x40, "d3_io:LoadtextureSet");
                        *w = i;
                        *h = j;
                        for (i = 0; i < *w; i++)
                            for (j = 0; j < *h; j++)
                                bio_rd_s(f, *out + (*h * i + j) * 0x40);
                    }
                }
                vfs_fclose(f);
            }
        }
    }
}

/* 0x44d330, loads a BGF mesh (or finds it among the loaded ones)
   @deep tiebreak: insensitive (headers sample, 32 samples one codegen). Kept: the RemoveDoubleMaterials
   loop 0-based with j = i + 1 (398 -> 311 ins) and ntexsets <= 0 (-> 309, the original's cmp edi,ebx/jle
   with 0 in ebx). Tried, no lever (= control): if/else swap and while form of the weld j-loop, &desc.pts[]
   spelling, !flag01, *(used + i - 1), cm_Pi + rot order, bv.y/av.y for t1/t2. Worse: the poly-index if
   order (402), memmove before npts-- (552), mi = i - 1 temp (417), 1-based i <= nmat (369 alone), bv.z
   after u negation (385 but frame 0x2c0). Lead: the uv loop (0x44dab7..0x44db8a) matches the original
   almost exactly when written with desc.mats[mi].f at every use instead of the mt pointer (code 177 -> 118
   hunks) but the 9 extra desc references move desc above av/bv in the ebp frame (desc.mats [ebp-0x1a0] vs
   the original's [ebp-0x1c0]) and turn rot + cm_Pi into fld cm_Pi; fadd rot: the original has more av/bv
   references or fewer desc ones elsewhere (field refs whose stores VC6 drops do not count: bv.y for t2 =
   control). Left: weld loop allocation (original i in ebx with [ebp+8], m in edi reloaded from [ebp-0xc];
   ours m in ebx, i in the [ebp+0xc] slot), vertex rotate loop (original materializes py/pz in edx/ecx,
   computes u before bv.z), the texture loop's [ebp+0x18] -> [ebp+0xc] parameter-slot reuse. (tiebreak:
   insensitive, sampled) prior: read the whole function against the original: the
   same call set (no dcp diffclass call difference) and no global store difference (no differs line), 940
   vs 936 instructions, 3047 vs 3054 bytes, frames equal. Q2 settled: the parameter is ntexsets (d3_FindBGF
   assigns it to m->nsets); the M10 is cleared by loading cm_Pi (the shared pi, cm.c 0x5eb7dc) in the two
   sin/cos arguments instead of the literal. What is left is allocation: the vertex-weld loop keeps i in
   ebx in the original and reloads j from a slot, while ours spills i to the [ebp+0xc] argument slot and
   reloads it for the i == j compare and the poly index rewrite (the 272 code / 136 slots / 96 regs / 48
   fpu hunks are this frame-slot and register permutation, the frame itself equal). Tried: the ntexsets
   rename, cm_Pi, the loop-variable spelling; none moves the register assignment. */

D3BGF *d3iot_LoadBGF(char *path, char *name, char *texnames, int ntexsets, int ntex)
{
    D3BGF *m = 0;
    char buf[256];
    D3BGFDESC desc;
    void *f;
    int i, j, k, n, mi;
    float s1, c1, s2, c2, t2, u, t1;
    float *py, *pz;
    D3VECTOR4 av, bv;
    char *used, *used2;
    char *tmp;

    strcpy(buf, path);
    _strupr(buf);
    if (strstr(buf, ".BGF")) {
        m = d3_FindBGF(buf, name, texnames, ntexsets, ntex);
        if (m) {
            d3iot_Finish(m, 0);
            return m;
        }
        d3_AddFLChunk(buf, d3_BGFCanWeld(buf));
        m = d3_FindBGF(buf, name, texnames, ntexsets, ntex);
        if (m) {
            d3iot_Finish(m, 0);
            return m;
        }
        D3_ZERO(&desc, sizeof desc);
        f = vfs_Tfopen(buf, "rb", vfs_Root);
        if (f) {
            d3_ReadBGF(f, &desc);
            vfs_fclose(f);
            if (desc.npoly > 0 && desc.npts > 0) {
                m = mem_Alloc(0x20c, "d3_io:LoadAGI(obj)");
                strncpy(m->name, name, 0x3f);
                m->name[0x3f] = 0;
                if (desc.flag01 == 0) {
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
                m->verts = mem_Alloc((desc.npts * 3 + 0x18) * 8, "d3_io:LoadAGF(poi)");
                s1 = (float)sin(-1.5707963705062866);
                c1 = (float)cos(-1.5707963705062866);
                s2 = (float)sin(3.1415927410125732);
                c2 = (float)cos(3.1415927410125732);
                for (k = 0; k < m->nverts; k++) {
                    av.x = desc.pts[k * 6];
                    py = &desc.pts[k * 6 + 1];
                    pz = &desc.pts[k * 6 + 2];
                    t1 = c1 * *py - s1 * *pz;
                    t2 = c1 * *pz + s1 * *py;
                    u = av.x * c2 + s2 * t2;
                    bv.z = t2 * c2 - av.x * s2;
                    u = u * -1.0f;
                    m->verts[k * 6] = bv.z * s2 + c2 * u;
                    m->verts[k * 6 + 1] = t1;
                    m->verts[k * 6 + 2] = bv.z * c2 - u * s2;
                }
                if (desc.nmat > 0) {
                    used = mem_Alloc(desc.nmat, "d3_io:RemoveDoubleMaterials");
                    n = desc.nmat;
                    for (i = 0; i < desc.nmat; i++) {
                        if (!used[i]) {
                            used[i] = 1;
                            for (j = i + 1; j < desc.nmat; j++) {
                                if (!memcmp(&desc.mats[i], &desc.mats[j], 0xe0)) {
                                    for (k = 0; k < desc.npoly; k++)
                                        if (desc.polys[k].mat == j)
                                            desc.polys[k].mat = i;
                                    used[j] = 1;
                                }
                            }
                        }
                    }
                    mem_Free(used);
                    used = mem_Alloc(desc.nmat, "d3_io:RemoveUnusedMaterials");
                    n = desc.nmat;
                    for (k = 0; k < desc.npoly; k++)
                        if (desc.polys[k].mat >= 0)
                            used[desc.polys[k].mat] = 1;
                    for (i = 0; i < n; ) {
                        if (!used[i]) {
                            for (k = 0; k < desc.npoly; k++)
                                if (i < desc.polys[k].mat)
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
                m->polys = mem_Alloc(desc.npoly * 0x38, "d3_io:LoadAGF(pol)");
                D3_COPY(m->polys, desc.polys, m->npolys * 0x38);
                used = mem_Alloc(desc.nmat, "d3_io:Materials(bool)");
                tmp = mem_Alloc(desc.nmat * 0xe0, "d3_io:TempMat[ReOrder]");
                D3_COPY(tmp, desc.mats, desc.nmat * 0xe0);
                i = 0;
                for (k = 0; k < m->npolys; k++) {
                    mi = desc.polys[k].mat;
                    if (mi >= 0 && !used[mi]) {
                        used[mi] = 1;
                        for (j = 0; j < m->npolys; j++)
                            if (desc.polys[j].mat == mi)
                                m->polys[j].mat = i;
                        {
                            char *src = tmp + mi * 0xe0;
                            D3_COPY((char *)desc.mats + i * 0xe0, src, 0xe0);
                        }
                        i++;
                    }
                }
                mem_Free(tmp);
                mem_Free(used);
                for (k = 0; k < m->npolys; k++) {
                    mi = m->polys[k].mat;
                    if (mi >= 0) {
                        D3BGFMAT *mt = &desc.mats[mi];
                        for (n = 0; n < 3; n++) {
                            float a, b;
                            a = (m->polys[k].uv[n][0] * mt->su + 0.5f) - mt->su * 0.5f;
                            b = (m->polys[k].uv[n][1] * mt->sv + 0.5f) - mt->sv * 0.5f;
                            m->polys[k].uv[n][0] = (float)cos(mt->rot) * a - (float)sin(mt->rot) * b + mt->ou;
                            m->polys[k].uv[n][1] = (float)sin(mt->rot + 3.14159274f) * a + (float)cos(mt->rot + 3.14159274f) * b + mt->ov;
                        }
                    }
                }
                if (!texnames || ntexsets <= 0 || ntex != desc.nmat) {
                    m->ntex = desc.nmat;
                    m->nsets = 1;
                    m->texnames = mem_Alloc(desc.nmat << 6, "d3_io:LoadAGF(texset)");
                    for (k = 0; k < desc.nmat; k++) {
                        D3BGFMAT *mt = &desc.mats[k];
                        if (mt->name[2][0])
                            strcpy(m->texnames + k * 0x40, mt->name[2]);
                        else if (mt->name[0][0])
                            strcpy(m->texnames + k * 0x40, mt->name[0]);
                        else if (mt->name[1][0])
                            strcpy(m->texnames + k * 0x40, mt->name[1]);
                    }
                } else {
                    m->ntex = ntex;
                    m->nsets = ntexsets;
                    m->texnames = texnames;
                }
                used2 = mem_Alloc(m->npolys, "d3_io:LoadAGF(texref)");
                for (k = 0; k < m->ntex; k++) {
                    D3BGFFLAGS fl;
                    D3BGFMAT *mt = &desc.mats[k];
                    D3TEXHANDLE *tex;
                    fl.all = 0;
                    fl.s.b0 = 0xff;
                    if (mt->kind) {
                        fl.s.b0 = mt->c1;
                        if (mt->kind == 2)
                            fl.s.c0 |= 1;
                    }
                    fl.s.b1 = mt->c7;
                    fl.s.c1 = mt->c2;
                    if (mt->name[2][0] == 0)
                        tex = D3BGF_TEXLOAD(m->texnames + k * 0x40, fl, ((mt->c3 << 4 | mt->c6) * 2 | mt->c5) * 2, mt->c8 << 4 | mt->c9);
                    else
                        tex = D3BGF_TEXLOAD(m->texnames + k * 0x40, fl, (((mt->c3 << 4 | mt->c6) * 2 | mt->c5) * 2) | 1, mt->c8 << 4 | mt->c9);
                    mi = tex ? tex - (D3TEXHANDLE *)d3_Textures : -1;
                    for (j = 0; j < m->npolys; j++) {
                        if (!used2[j] && m->polys[j].mat == k) {
                            m->polys[j].tex = mi;
                            used2[j] = 1;
                        }
                    }
                }
                mem_Free(used2);
                for (k = 0; k < desc.nattach; k++) {
                    strncpy(m->attach[k].name, desc.attach[k].name, 0x3f);
                    m->attach[k].name[0x3f] = 0;
                    m->attach[k].f[0] = desc.attach[k].f[0];
                    m->attach[k].f[1] = desc.attach[k].f[1];
                    m->attach[k].f[2] = desc.attach[k].f[2];
                    m->attach[k].f[3] = desc.attach[k].f[3];
                    m->attach[k].f[4] = desc.attach[k].f[4];
                    m->attach[k].f[5] = desc.attach[k].f[5];
                }
                d3iot_Finish(m, 1);
            }
            if (desc.nmat > 0 && desc.mats) {
                mem_Free(desc.mats);
                desc.mats = 0;
            }
            if (desc.npoly > 0 && desc.polys) {
                mem_Free(desc.polys);
                desc.polys = 0;
            }
            if (desc.npts > 0 && desc.pts)
                mem_Free(desc.pts);
        }
    }
    return m;
}
/* 0x44df20, radius of the mesh behind an object info (the first argument is not used) */

float d3iot_GetRadius(int unused, D3OBJINFO *info)
{
    if (info && info->mesh)
        return ((D3BGF *)info->mesh)->radius;
    return 0.0f;
}
/* 0x44df40, loads an object file (name + ".bgf") and links it into the stock list */

D3BGF *d3io_LoadObjectFile(char *name, char *file)
{
    D3BGF *obj = 0;
    char *p;
    void *data;
    int y, z;
    void *keep;
    char path[256];

    p = d3io_StockFile(name, ".bgf");
    if (p) {
        y = 0;
        z = 0;
        data = 0;
        strcpy(path, p);
        d3iot_LoadTexSet(file, (char **)&data, &y, &z);
        if (data == 0)
            d3iot_LoadTexSet(name, (char **)&data, &y, &z);
        keep = data;
        obj = d3iot_LoadBGF(path, file, (char *)data, y, z);
        if (obj != 0) {
            if (keep != 0 && obj->texnames != keep)
                mem_Free(keep);
            obj->Release = d3iot_DeleteStockObject;
            obj->TransformNormals = d3_TransformNormals;
            obj->Transform = d3_TransformVerts;
            obj->GetBox = d3_GetBoxVerts;
            obj->GetRadius = d3iot_GetRadius;
        } else {
            if (keep != 0)
                mem_Free(keep);
        }
        if (obj != 0) {
            obj->prev = d3_StockListEnd.prev;
            d3_StockListEnd.prev = obj;
            obj->next = &d3_StockListEnd;
            obj->prev->next = obj;
        }
    }
    return obj;
}
/* 0x44e080, loads an object file and its variants (the name pair for lod -1, 1, 2) */

D3BGF *d3io_LoadObject(char *a, char *b)
{
    char x[256];
    char y[256];
    D3BGF *ret;
    int t;
    int i;

    d3io_LodName(a, b, 0, y, x);
    t = d3io_FindStock(x);
    if (t == 0 && (t = d3io_FindStock(y), t == 0)) {
        ret = d3io_LoadObjectFile(y, x);
        if (d3io_LodName(a, b, -1, y, x))
            d3io_LoadObjectFile(y, x);
        if (d3io_Lod.mode == 1) {
            for (i = 1; i < 3; i++) {
                if (d3io_LodName(a, b, i, y, x))
                    d3io_LoadObjectFile(y, x);
            }
            return ret;
        }
        return ret;
    }
    return (D3BGF *)t;
}
/* 0x44e190, the same, counting a reference on every object loaded */

D3BGF *d3io_LoadObjectRef(char *a, char *b)
{
    char x[256];
    char y[256];
    D3BGF *ret;
    D3BGF *o;
    int t;
    int i;

    d3io_LodName(a, b, 0, y, x);
    t = d3io_FindStock(x);
    if (t == 0 && (t = d3io_FindStock(y), t == 0)) {
        ret = d3io_LoadObjectFile(y, x);
        if (ret != 0)
            ret->refs++;
        if (d3io_LodName(a, b, -1, y, x) && (o = d3io_LoadObjectFile(y, x), o != 0))
            o->refs++;
        if (d3io_Lod.mode == 1) {
            for (i = 1; i < 3; i++) {
                if (d3io_LodName(a, b, i, y, x) && (o = d3io_LoadObjectFile(y, x), o != 0))
                    o->refs++;
            }
            return ret;
        }
        return ret;
    }
    return (D3BGF *)t;
}
/* 0x44e2c0, frees a stock object that still has references */

void d3io_ReleaseObject(D3BGF *obj)
{
    if (obj != 0 && obj->refs > 0)
        d3iot_DeleteStockObject(obj);
}
/* 0x44e2e0, loads the animation file (name + ".oam") of an object: keys of a path */

D3IOPATH *d3_LoadObjectAnim(char *file, D3IOOBJ *obj, char reload)
{
    D3IOPATH *p = 0;
    char *path;
    void *f;
    int magic;
    D3IOKEY **keys;
    int i;
    int n;

    if (obj->path != 0)
        d3anim_FreePath((D3OBJECT *)obj);
    path = d3io_StockFile(file, ".oam");
    if ((path != 0 && obj->type == 4) || obj->type == 3) {
        f = vfs_Tfopen(path, "rb", (int)vfs_Root);
        bio_rd_i(f, &magic);
        if (magic == 0x3a610001) {
            p = (D3IOPATH *)mem_Alloc(0x44, "d3_io:ObjAnim");
            bio_rd_i(f, &p->nkeys);
            bio_rd_i(f, (int *)&p->flags);
            p->flags.c5 = 0;
            keys = &p->keys;
            p->flags.d1 = reload;
            n = bio_rd_array_quick(f, 0x58, (void **)keys);
            if (n != p->nkeys)
                err_Message("d3_LoadObjectAnim: Wrong number of KeyFrames in saved array!");
            (*keys)[p->nkeys - 1].time = (*keys)[p->nkeys - 2].time;
            p->pos[0] = obj->pos[0];
            p->pos[1] = obj->pos[1];
            p->pos[2] = obj->pos[2];
            p->rot[0] = obj->rot[0];
            p->rot[1] = obj->rot[1];
            p->rot[2] = obj->rot[2];
            for (i = 0; i < p->nkeys; i++)
                (*keys)[i].time *= 3;
            if (p->flags.rev) {
                n = p->nkeys - 1;
                p->key = n;
                p->time = p->keys[n].time - 1;
                p->next = d3anim_NextFrameIo(p->flags, n, 0, n, p->nkeys);
            } else {
                p->next = 1;
                p->key = 0;
                p->time = 0;
            }
        }
        vfs_fclose(f);
    }
    obj->path = p;
    obj->time = d3_Time;
    return p;
}
/* 0x44e4a0, loads an animation stream "animations/" + name and links it into the animation list, before its
   end sentinel d3anim_Tail */

D3ANIMDATA *d3_LoadAnimStream(char *name, char *file, char unk)
{
    char buf[256];
    D3ANIMDATA *a;

    strcpy(buf, d3_AnimDir);
    strcat(buf, name);
    a = (D3ANIMDATA *)d3_FindAnim(file);
    if (a != 0) {
        sprintf(buf, "d3_LoadAnimStream(): animation already in stock: %s", name);
        return a;
    }
    a = (D3ANIMDATA *)d3_LoadBinaryAnimStream(buf, file, unk);
    if (a != 0) {
        a->prev = d3anim_Tail.prev;
        d3anim_Tail.prev = a;
        a->next = &d3anim_Tail;
        a->prev->next = a;
    }
    return a;
}

D3IOLOD d3io_Lod = { 2 };  /* 0x5fbf78, which stock object LODs are loaded */
char d3_AnimDir[256] = { 97, 110, 105, 109, 97, 116, 105, 111, 110, 115, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x5fbf80, "animations/" */
char d3io_StockPath[256] = {0};  /* 0x633a00 */
