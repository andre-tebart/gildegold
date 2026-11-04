/* d3_io_b.c -- binary BGF reader ("d3_io_b:" allocation tags): continues the AGI text reader of
   d3_io_a.c ("d3_io_a:"), probably the same TU (0x44e570..). A chunk parser (bgi_Parse) walks tables of
   {id, handler, count}; the handlers fill a BGIRD (the same context layout as the text reader's AGICTX).
   Names are ours.
   Open: whether the last two functions (0x4506f0, 0x4507a0; the latter's only caller is d3_io_fl.c's
   0x450860) open d3_io_fl.c instead; not tested.
   unit of rea 8cef7d5b (src/game/d3_io_b.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_io_t.h"
#include "d3_io_a.h"
#include "d3_io_b.h"
#include "bio.h"
#include "err.h"
#include "mem.h"
#include "vfs.h"

#define BGI_M(c) ((c)->mats[(c)->cur])
#define BGI_P(c) ((c)->polys[(c)->polyidx])

/* 0x44ee50, "LOCAL" / "ANIM" keyword of the text format */
void agi_Kind(void *unused, char *out)
{
    char *tok = strtok(0, " ");

    *out = 0;
    if (!strcmp(tok, "LOCAL")) {
        *out = 1;
        return;
    }
    if (!strcmp(tok, "ANIM"))
        *out = 2;
}
/* 0x44eea0, reads a chunk id: 0x2b at the end of the file, 0x27 for an invalid byte */

int bgi_ReadId(void *f)
{
    int id;
    char c;

    if (!vfs_fread(&c, 1, 1, f))
        return 0x2b;
    id = c;
    if (id < 0 || id >= 0x3b)
        id = 0x27;
    return id;
}
/* 0x44eee0, index of an id in a table (0x27 when missing); 0x28 ends the table */

int bgi_Find(int id, BGIENT *t, int n)
{
    int i;

    for (i = 0; i < 0x40; i++, t++) {
        if (id == t->id)
            return i;
        if (t->id == 0x28)
            break;
    }
    return 0x27;
}
/* 0x44ef10, parses the chunks of one table level; 0x2f / 0x2b end it, 0x28 runs its end handler */

void bgi_Parse(void *f, BGIENT *t, int n, BGIRD *c)
{
    int id, i;
    BGIENT *e;
    void (*h)(void *, BGIRD *);

    id = bgi_ReadId(f);
    if (id != 0x27) {
        while (1) {
            if (id == 0x2f || id == 0x2b)
                break;
            if (id == 0x28) {
                i = bgi_Find(0x28, t, n);
                if (i != 0x27 && (h = (void (*)(void *, BGIRD *))t[i].fn))
                    h(f, c);
                break;
            }
            i = bgi_Find(id, t, n);
            if (i != 0x27) {
                h = (void (*)(void *, BGIRD *))t[i].fn;
                e = &t[i];
                if (!h || e->n) {
                    if (!e->n) {
                        bgi_Depth++;
                        bgi_Parse(f, t, n, c);
                    } else {
                        bgi_Depth++;
                        bgi_Parse(f, (BGIENT *)e->fn, e->n, c);
                    }
                } else {
                    h(f, c);
                }
            }
            id = bgi_ReadId(f);
            if (id == 0x27) {
                bgi_Depth--;
                return;
            }
        }
    }
    bgi_Depth--;
}
/* 0x44f000, next material */

void bgi_NextMat(void *f, BGIRD *c)
{
    c->cur++;
}
/* 0x44f010, an int that is not used */

void bgi_SkipInt(void *f, BGIRD *c)
{
    int t = 0;

    bio_rd_i(f, &t);
}
/* 0x44f030, binary-format record 7: stores the first bitmap name of the current material (file extension
   stripped) */

void bgi_Bitmap0(void *f, BGIRD *c)
{
    char buf[256];
    char *dot;

    bio_rd_s(f, buf);
    dot = strrchr(buf, '.');
    if (dot)
        *dot = 0;
    strcpy(BGI_M(c).name[0], buf);
}
/* 0x44f090, binary-format record 9: stores the second bitmap name of the current material (file extension
   stripped) */

void bgi_Bitmap1(void *f, BGIRD *c)
{
    char buf[256];
    char *dot;

    bio_rd_s(f, buf);
    dot = strrchr(buf, '.');
    if (dot)
        *dot = 0;
    strcpy(BGI_M(c).name[1], buf);
}
/* 0x44f0f0, binary-format record 8: stores the third bitmap name of the current material (file extension
   stripped) */

void bgi_Bitmap2(void *f, BGIRD *c)
{
    char buf[256];
    char *dot;

    bio_rd_s(f, buf);
    dot = strrchr(buf, '.');
    if (dot)
        *dot = 0;
    strcpy(BGI_M(c).name[2], buf);
}
/* 0x44f160, binary-format record 0xa: reads the current material's kind byte (0xc0) */

void bgi_MatKind(void *f, BGIRD *c)
{
    bio_rd_c(f, (char *)&BGI_M(c).kind);
}
/* 0x44f190, binary-format record 0x26: reads the current material's c1 byte (0xc1) */

void bgi_MatC1(void *f, BGIRD *c)
{
    bio_rd_c(f, (char *)&BGI_M(c).c1);
}
/* 0x44f1c0, flags packed in one byte */

void bgi_MatFlags1(void *f, BGIRD *c)
{
    unsigned char b;

    bio_rd_c(f, (char *)&b);
    BGI_M(c).c2 = b & 1;
    BGI_M(c).c5 = (b >> 1) & 1;
    BGI_M(c).c6 = (b >> 2) & 0xf;
    BGI_M(c).c3 = (b >> 6) & 1;
}
/* 0x44f270, binary-format record 0xc: reads the current material's second flag byte (bit 0 to pad4, the
   other seven bits to c7) */

void bgi_MatFlags2(void *f, BGIRD *c)
{
    char b;

    bio_rd_c(f, &b);
    BGI_M(c).pad4 = b & 1;
    BGI_M(c).c7 = b & 0xfe;
}
/* 0x44f2d0, binary-format record 0xd: reads the current material's c8 byte (0xc8) */

void bgi_MatC8(void *f, BGIRD *c)
{
    bio_rd_c(f, &BGI_M(c).c8);
}
/* 0x44f300, binary-format record 0xe: reads the current material's c9 byte (0xc9) */

void bgi_MatC9(void *f, BGIRD *c)
{
    bio_rd_c(f, &BGI_M(c).c9);
}
/* 0x44f330, binary-format record 0xf: reads the current material's u scale (su) */

void bgi_MatSu(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).su);
}
/* 0x44f360, binary-format record 0x10: reads the current material's v scale (sv) */

void bgi_MatSv(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).sv);
}
/* 0x44f390, binary-format record 0x11: reads the current material's u offset (ou) */

void bgi_MatOu(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).ou);
}
/* 0x44f3c0, binary-format record 0x12: reads the current material's v offset (ov) */

void bgi_MatOv(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).ov);
}
/* 0x44f3f0 */

void bgi_MatRot(void *f, BGIRD *c)
{
    bio_rd_i(f, (int *)&BGI_M(c).rot);
}
/* 0x44f420, number of materials */

void bgi_NumMats(void *f, BGIRD *c)
{
    int n;

    bio_rd_i(f, &c->nmats);
    n = c->nmats;
    c->cur = 0;
    if (n > 0)
        c->mats = (D3BGFMAT *)mem_Alloc(n * sizeof(D3BGFMAT), "d3_io_b:BGIMaterialBuffer");
}
/* 0x44f470, the three point indices of a polygon */

void bgi_PolyVerts(void *f, BGIRD *c)
{
    int a;

    bio_rd_i(f, &a);
    BGI_P(c).v[0] = c->ptbase + a;
    bio_rd_i(f, &a);
    BGI_P(c).v[1] = c->ptbase + a;
    bio_rd_i(f, &a);
    BGI_P(c).v[2] = c->ptbase + a;
}
/* 0x44f500, texture coordinates of a polygon: three u, three v, then three values that are skipped */

void bgi_PolyUV(void *f, BGIRD *c)
{
    int i, t;

    for (i = 0; i < 3; i++)
        bio_rd_i(f, (int *)&BGI_P(c).uv[i][0]);
    for (i = 0; i < 3; i++)
        bio_rd_i(f, (int *)&BGI_P(c).uv[i][1]);
    for (i = 0; i < 3; i++)
        bio_rd_i(f, &t);
}
/* 0x44f580, three ints that are skipped */

void bgi_SkipInt3(void *f, BGIRD *c)
{
    int i, t;

    for (i = 0; i < 3; i++)
        bio_rd_i(f, &t);
}
/* 0x44f5a0, the material of a polygon (a byte, or an int with more than 254 materials) */

void bgi_PolyMat(void *f, BGIRD *c)
{
    unsigned char b;

    BGI_P(c).tex = -1;
    if (c->nmats > 0xfe) {
        bio_rd_i(f, &BGI_P(c).mat);
        c->polyidx++;
        return;
    }
    bio_rd_c(f, (char *)&b);
    if (b == 0xff) {
        BGI_P(c).mat = -1;
        c->polyidx++;
        return;
    }
    BGI_P(c).mat = b;
    c->polyidx++;
}
/* 0x44f650 */

void bgi_Unk04(void *f, BGIRD *c)
{
    bio_rd_i(f, &c->unk04);
}
/* 0x44f660, number of points; allocates the point buffer once */

void bgi_NumPoints(void *f, BGIRD *c)
{
    bio_rd_i(f, &c->npts);
    if (c->npts > 0 && !c->pts) {
        c->ptidx = 0;
        c->ptbase = 0;
        c->pts = (float (*)[6])mem_Alloc(0x60000, "d3_io_b:BGIPointBuffer");
    }
}
/* 0x44f6b0, number of polygons; allocates the polygon buffer once */

void bgi_NumPolys(void *f, BGIRD *c)
{
    bio_rd_i(f, &c->npoly);
    if (c->npoly > 0 && !c->polys) {
        c->polyidx = 0;
        c->polybase = 0;
        c->polys = (D3BGFPOLY *)mem_Alloc(0xe0000, "d3_io_b:BGIPolyBuffer");
    }
}
/* 0x44f700, reads the points (x, y, z) */

void bgi_Points(void *f, BGIRD *c)
{
    int i;

    for (i = 0; i < c->npts; i++) {
        bio_rd_i(f, (int *)&c->pts[c->ptidx][0]);
        bio_rd_i(f, (int *)&c->pts[c->ptidx][1]);
        bio_rd_i(f, (int *)&c->pts[c->ptidx][2]);
        c->ptidx++;
    }
}
/* 0x44f770, a string that is not used */

void bgi_SkipStr(void *f, BGIRD *c)
{
    char buf[256];

    bio_rd_s(f, buf);
}
/* 0x44f790 */

void bgi_SkipInt2(void *f, BGIRD *c)
{
    int t;

    bio_rd_i(f, &t);
}
/* 0x44f7a0, two flag bits */

void bgi_Flags(void *f, BGIRD *c)
{
    unsigned char b;

    bio_rd_c(f, (char *)&b);
    c->flag0 = b & 1;
    c->flag1 = (b >> 1) & 1;
}
/* 0x44f7d0 */

void bgi_SkipDword(void *f, BGIRD *c)
{
    int t;

    vfs_fread(&t, 4, 1, f);
}
/* 0x44f7f0, the attachment points: byte, name, byte, vector, byte, vector each */

void bgi_Attach(void *f, BGIRD *c)
{
    int n = 0, i;
    char b;

    bio_rd_i(f, &n);
    c->nattach = n;
    for (i = 0; i < n; i++) {
        bio_rd_c(f, &b);
        bio_rd_s(f, c->attach[i].name);
        bio_rd_c(f, &b);
        bio_rd_v3(f, (int *)&c->attach[i].f[0]);
        bio_rd_c(f, &b);
        bio_rd_v3(f, (int *)&c->attach[i].f[3]);
    }
}

/* 0x5fc6f8 */
static BGIENT bgi_TabMat[16] = {
    {6, bgi_SkipInt, 0}, {7, bgi_Bitmap0, 0}, {9, bgi_Bitmap1, 0}, {8, bgi_Bitmap2, 0},
    {10, bgi_MatKind, 0}, {0x26, bgi_MatC1, 0}, {0xb, bgi_MatFlags1, 0}, {0xc, bgi_MatFlags2, 0},
    {0xd, bgi_MatC8, 0}, {0xe, bgi_MatC9, 0}, {0xf, bgi_MatSu, 0}, {0x10, bgi_MatSv, 0},
    {0x11, bgi_MatOu, 0}, {0x12, bgi_MatOv, 0}, {0x13, bgi_MatRot, 0}, {0x28, bgi_NextMat, 0}
};

/* 0x5fc7b8 */
static BGIENT bgi_TabMats[3] = {
    {4, bgi_NumMats, 0}, {5, bgi_TabMat, 0x10}, {0x28, 0, 0}
};

/* 0x5fc7dc */
static BGIENT bgi_TabPoly[5] = {
    {0x1d, bgi_PolyVerts, 0}, {0x1e, bgi_PolyUV, 0}, {0x1f, bgi_SkipInt3, 0}, {0x20, bgi_PolyMat, 0},
    {0x28, 0, 0}
};

/* 0x5fc818 */
static BGIENT bgi_TabGeom[6] = {
    {0x18, bgi_Unk04, 0}, {0x19, bgi_NumPoints, 0}, {0x1a, bgi_NumPolys, 0}, {0x1b, bgi_Points, 0},
    {0x1c, bgi_TabPoly, 5}, {0x28, agi_EndObject, 0}
};

/* 0x5fc860 */
static BGIENT bgi_TabObj[4] = {
    {0x15, bgi_SkipStr, 0}, {0x16, bgi_SkipInt2, 0}, {0x17, bgi_TabGeom, 6}, {0x28, 0, 0}
};

/* 0x5fc890 */
static BGIENT bgi_TabTop[7] = {
    {2, bgi_Flags, 0}, {1, bgi_SkipInt, 0}, {0, bgi_SkipDword, 0}, {3, bgi_TabMats, 3},
    {0x14, bgi_TabObj, 4}, {0x37, bgi_Attach, 0}, {0x28, 0, 0}
};
int bgi_Depth = 0;  /* 0x633b00 */

/* 0x44f870, reads a BGF file into a description */
void d3_ReadBGF(void *f, D3BGFDESC *desc)
{
    int magic, n;

    vfs_fread(&magic, 4, 1, f);
    if (bgi_ReadId(f) == 0x2e) {
        bio_rd_i(f, &n);
        bgi_Parse(f, bgi_TabTop, 7, (BGIRD *)desc);
    }
}

/* 0x44f8c0..0x450858 (was unk_44f8c0.c, worker anim) */

/* 0x44f8c0, reads an animation stream (chunk 0x30): per frame the vertices, uv faces, position / angles and link
   points; then makes everything relative to frame 0 and packs the points into bytes */
D3BASTREAM *d3_LoadBinaryAnimStream(char *path, char *name, char relative)
{
    int first = -1;
    int last = -1;
    int nvfix = -1;
    int nuvfix = -1;
    D3BASTREAM *stream = 0;          /* result, created at the frame count chunk (0x23) */
    int nframes = 0;
    int nsub = 1;
    int nlinks = 0;
    char readuv = 0;
    char readverts = 1;
    VFSFILE *f;
    int hdr, tag, i, j, frame, pt;   /* frame: index of the frame being read, pt: point index */
    int time, nv, nuv, ofs;
    D3BAVEC v, w, mn, mx;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fread(&hdr, 4, 1, f);
        tag = bgi_ReadId(f);
        if (tag == 0x30) {
            bio_rd_i(f, &frame);
            tag = bgi_ReadId(f);
            if (tag == 1) {
                bio_rd_i(f, &frame);
                if ((unsigned)frame > 0xabcd0001) {
                    vfs_fclose(f);
                    return 0;
                }
            }
            tag = bgi_ReadId(f);
            if (tag == 0x23) {
                bio_rd_i(f, &nframes);
                if (nframes > 0) {
                    stream = mem_Alloc(sizeof(D3BASTREAM), "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream)");
                    strncpy(stream->name, name, 0x3f);
                    stream->nframes = nframes;
                    stream->first = 0;
                    stream->unk168 = 0;
                    stream->last = stream->nframes - 1;
                    stream->relative = relative;
                    stream->frames = mem_Alloc(stream->nframes * sizeof(D3BAFRAME),
                                          "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata)");
                }
                tag = bgi_ReadId(f);
            }
            if (tag == 0x33) {
                bio_rd_c(f, &readverts);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x24) {
                bio_rd_c(f, &readuv);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x37) {
                bio_rd_i(f, &nlinks);
                if (stream)
                    stream->nlinks = nlinks;
                tag = bgi_ReadId(f);
            }
            if (tag == 0x36) {
                bio_rd_i(f, &nsub);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x34) {
                bio_rd_i(f, &nvfix);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x35) {
                bio_rd_i(f, &nuvfix);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x29) {
                bio_rd_i(f, &first);
                if (stream)
                    stream->first = first;
                tag = bgi_ReadId(f);
            }
            if (tag == 0x2a) {
                bio_rd_i(f, &last);
                if (stream)
                    stream->last = last;
                tag = bgi_ReadId(f);
            }
            if (stream && stream->first > stream->last) {
                frame = stream->last;
                stream->last = stream->first;
                stream->first = frame;
            }
            for (frame = 0; frame < nframes; frame++) {
                time = -1;
                ofs = 0;
                for (j = 0; j < nsub; j++) {
                    if (tag == 0x18) {
                        bio_rd_i(f, &time);
                        if (stream)
                            stream->frames[frame].time = time;
                        tag = bgi_ReadId(f);
                    }
                    nv = 0;
                    if (readverts && tag == 0x19) {
                        bio_rd_i(f, &nv);
                        if (stream) {
                            if (stream->nverts == 0) {
                                if (nvfix >= 0)
                                    stream->nverts = nvfix;
                                else
                                    stream->nverts = nv;
                            } else if (nvfix == -1 && stream->nverts != nv)
                                err_Message("Inconsistent number of vertices in d3_LoadBinaryAnimStream()");
                        }
                        tag = bgi_ReadId(f);
                    }
                    if (tag == 0x21) {
                        if (stream && ofs == 0) {
                            if (nvfix > 0)
                                stream->frames[frame].points = mem_Alloc(nvfix * sizeof(D3BAPOINT),
                                    "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata->points)");
                            else
                                stream->frames[frame].points = mem_Alloc(nv * sizeof(D3BAPOINT),
                                    "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata->points)");
                        }
                        for (i = 0; i < nv; i++) {
                            bio_rd_v3(f, (int *)&v);
                            if (stream) {
                                if (frame == 0) {
                                    stream->frames[frame].points[ofs + i].x = v.x;
                                    stream->frames[frame].points[ofs + i].y = v.y;
                                    stream->frames[frame].points[ofs + i].z = v.z;
                                } else {
                                    stream->frames[frame].points[ofs + i].x = v.x - stream->frames[0].points[ofs + i].x;
                                    stream->frames[frame].points[ofs + i].y = v.y - stream->frames[0].points[ofs + i].y;
                                    stream->frames[frame].points[ofs + i].z = v.z - stream->frames[0].points[ofs + i].z;
                                }
                            }
                        }
                        ofs += nv;
                        tag = bgi_ReadId(f);
                        if (tag != 0x28)
                            err_Message("Expected EXP_DONE in d3_LoadBinaryAnimStream(): ReadVertices");
                        else
                            tag = bgi_ReadId(f);
                    }
                    if (readuv && tag == 0x1a) {
                        bio_rd_i(f, &nuv);
                        tag = bgi_ReadId(f);
                        if (tag == 0x22) {
                            for (i = 0; i < nuv; i++) {
                                bio_rd_v3(f, (int *)&v);
                                bio_rd_v3(f, (int *)&w);
                            }
                            tag = bgi_ReadId(f);
                        }
                        if (tag != 0x28)
                            err_Message("Expected EXP_DONE in d3_LoadBinaryAnimStream(): ReadUVFaces");
                        else
                            tag = bgi_ReadId(f);
                    }
                }
                if (tag == 0x31) {
                    bio_rd_v3(f, (int *)&v);
                    bio_rd_v3(f, (int *)&w);
                    if (stream) {
                        stream->frames[frame].pos[0] = v.x;
                        stream->frames[frame].pos[1] = v.y;
                        stream->frames[frame].pos[2] = v.z;
                        stream->frames[frame].rot[0] = w.x;
                        stream->frames[frame].rot[1] = w.y;
                        stream->frames[frame].rot[2] = w.z;
                    }
                    tag = bgi_ReadId(f);
                }
                for (i = 0; i < nlinks; i++) {
                    if (tag == 0x38) {
                        bio_rd_s(f, stream->links[i]);
                        tag = bgi_ReadId(f);
                    }
                    if (tag == 0x39) {
                        bio_rd_v3(f, (int *)stream->frames[frame].links[i].a);
                        tag = bgi_ReadId(f);
                    }
                    if (tag == 0x3a) {
                        bio_rd_v3(f, (int *)stream->frames[frame].links[i].b);
                        tag = bgi_ReadId(f);
                    }
                }
            }
            if (tag == 0x32) {
                bio_rd_v3(f, (int *)&v);
                bio_rd_v3(f, (int *)&w);
                tag = bgi_ReadId(f);
            }
            if (tag != 0x2f)
                err_Message("Expected EXP_EOCHUNK in d3_LoadBinaryAnimStream(): EndOfFrameLoop");
            if (stream) {
                if (nvfix >= 0)
                    nv = nvfix;
                if (nuvfix >= 0)
                    nuv = nuvfix;
                if (stream->first < 0)
                    stream->first = 0;
                if (stream->first >= stream->nframes)
                    stream->first = stream->nframes - 1;
                if (stream->last < 0)
                    stream->last = 0;
                if (stream->last >= stream->nframes)
                    stream->last = stream->nframes - 1;
                for (frame = 0; frame < nframes - 1; frame++)
                    stream->frames[frame].dt = stream->frames[frame + 1].time - stream->frames[frame].time;
                stream->frames[nframes - 1].dt = stream->frames[nframes - 2].dt;
                for (frame = 1; frame < nframes; frame++) {
                    stream->frames[frame].pos[0] -= stream->frames[0].pos[0];
                    stream->frames[frame].pos[1] -= stream->frames[0].pos[1];
                    stream->frames[frame].pos[2] -= stream->frames[0].pos[2];
                    stream->frames[frame].rot[0] -= stream->frames[0].rot[0];
                    stream->frames[frame].rot[1] -= stream->frames[0].rot[1];
                    stream->frames[frame].rot[2] -= stream->frames[0].rot[2];
                }
                stream->frames[0].pos[0] = 0.0f;
                stream->frames[0].pos[1] = 0.0f;
                stream->frames[0].pos[2] = 0.0f;
                stream->frames[0].rot[0] = 0.0f;
                stream->frames[0].rot[1] = 0.0f;
                stream->frames[0].rot[2] = 0.0f;
                for (frame = 0; frame < stream->nverts; frame++) {
                    stream->frames[0].points[frame].x = 0.0f;
                    stream->frames[0].points[frame].y = 0.0f;
                    stream->frames[0].points[frame].z = 0.0f;
                }
                for (frame = 0; frame < nframes; frame++) {
                    if (stream->relative) {
                        for (i = 0; i < stream->nverts; i++) {
                            stream->frames[frame].points[i].x -= stream->frames[frame].pos[0];
                            stream->frames[frame].points[i].y -= stream->frames[frame].pos[1];
                            stream->frames[frame].points[i].z -= stream->frames[frame].pos[2];
                        }
                        for (i = 0; i < nlinks; i++) {
                            stream->frames[frame].links[i].a[0] -= stream->frames[frame].pos[0];
                            stream->frames[frame].links[i].a[1] -= stream->frames[frame].pos[1];
                            stream->frames[frame].links[i].a[2] -= stream->frames[frame].pos[2];
                        }
                    }
                    mn.x = 1e10f;
                    mn.y = 1e10f;
                    mn.z = 1e10f;
                    mx.x = -1e10f;
                    mx.y = -1e10f;
                    mx.z = -1e10f;
                    if (nv > 0) {
                        D3BAPOINT *p = stream->frames[frame].points;
                        for (i = nv; i; i--, p++) {
                            mn.x = (((p->x) > (mn.x)) ? (mn.x) : (p->x * 1.0f));
                            mn.y = (((p->y) > (mn.y)) ? (mn.y) : (p->y));
                            mn.z = (((p->z) > (mn.z)) ? (mn.z) : (p->z));
                            mx.x = (((p->x) < (mx.x)) ? (mx.x) : (p->x * 1.0f));
                            mx.y = (((p->y) < (mx.y)) ? (mx.y) : (p->y));
                            mx.z = (!((mx.z) > (p->z)) ? (p->z) : (mx.z));
                        }
                    }
                    stream->frames[frame].min[0] = mn.x;
                    stream->frames[frame].min[1] = mn.y;
                    stream->frames[frame].min[2] = mn.z;
                    v.x = mx.x - mn.x;
                    v.y = mx.y - mn.y;
                    v.z = mx.z - mn.z;
                    stream->frames[frame].scale[0] = v.x / 255.0f;
                    stream->frames[frame].scale[1] = v.y / 255.0f;
                    stream->frames[frame].scale[2] = v.z / 255.0f;
                    stream->frames[frame].dt *= 3;
                    stream->frames[frame].packed = mem_Alloc(nv * 3,
                        "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata->wpoints)");
                    for (pt = 0; pt < nv; pt++) {
                        w.x = stream->frames[frame].points[pt].x - mn.x;
                        w.y = stream->frames[frame].points[pt].y - mn.y;
                        w.z = stream->frames[frame].points[pt].z - mn.z;
                        stream->frames[frame].packed[pt * 3] = (unsigned char)((w.x * 255.0f) / v.x);
                        stream->frames[frame].packed[pt * 3 + 1] = (unsigned char)((w.y * 255.0f) / v.y);
                        stream->frames[frame].packed[pt * 3 + 2] = (unsigned char)((w.z * 255.0f) / v.z);
                    }
                    mem_Free(stream->frames[frame].points);
                    stream->frames[frame].points = 0;
                }
            }
        }
        vfs_fclose(f);
    }
    return stream;
}
/* 0x4506f0, 0 when the file's header flags have bit 1 set (format 2 only), else 1 */

char d3_Unk4506f0(char *path)
{
    VFSFILE *f;
    int hdr, n;
    char c;
    char ret = 1;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fread(&hdr, 4, 1, f);
        bio_rd_c(f, &c);
        bio_rd_i(f, &n);
        bio_rd_c(f, &c);
        bio_rd_i(f, &n);
        bio_rd_c(f, &c);
        ret = 1;
        if (c == 2) {
            bio_rd_c(f, &c);
            if ((c & 2) == 2)
                ret = 0;
        }
        vfs_fclose(f);
    }
    return ret;
}
/* 0x4507a0, opens a chunk file and skips to the chunk with id 0xfab50005; 0 if there is none */

VFSFILE *d3_Unk4507a0(char *path)
{
    VFSFILE *f;
    int hdr, len, id, tag;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fread(&hdr, 4, 1, f);
        while (1) {
            tag = bgi_ReadId(f);
            if (tag == 0x2d) {
                bio_rd_i(f, &len);
                bio_rd_i(f, &id);
                if (id == 0xfab50005)
                    return f;
                vfs_fseek(f, len - 4, 1);
            } else if (tag == 0x2b)
                break;
            else {
                bio_rd_i(f, &len);
                vfs_fseek(f, len, 1);
            }
        }
        vfs_fclose(f);
    }
    return 0;
}
