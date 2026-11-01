/* d3_io_a.c -- AGI / AGF text model reader ("d3_io_a:" allocation tags): line handlers that fill a reader
   context (materials, points, polygons). Extent in GildeGold.exe: 0x44e570 (right after the object /
   animation loaders of d3_io_t.c) .. 0x44ee50, where the binary reader d3_io_b.c follows. Names are ours.
   Open: whether d3_io_a.c and d3_io_b.c are one TU (they compile together unchanged; not proven).
   unit of rea 8cef7d5b (src/game/d3_io_a.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d3_io_a.h"
#include "bio.h"
#include "bmp.h"
#include "mem.h"

#define AGI_TOK(sep) strtok(0, sep)
#define AGI_M(c) ((c)->mats[(c)->cur])

/* 0x44e570, exactly one bit set */
char agi_IsPow2(int n)
{
    int i;
    int bits = 0;

    for (i = 0; i < 32; i++) {
        bits += n & 1;
        n >>= 1;
    }
    return bits == 1;
}
/* 0x44e5a0, checks that a bitmap exists, is square and has power of 2 dimensions; the messages are built
   in a local buffer that nothing reads */

int agi_CheckBitmap(char *name, char report)
{
    char buf[256];
    char *path;
    int w, h, ok = 0;

    path = tex_FindFile(name);
    if (path)
        ok = bmp_GetInfo(path, &w, &h, 0);
    if (!ok) {
        if (report) {
            h = -1;
            w = -1;
            strcpy(buf, path);
            strcat(buf, " is an unknown Bitmap-Type!");
            return 0;
        }
    }
    if (w != h && report) {
        strcpy(buf, path);
        strcat(buf, " is not square!");
        return -2;
    }
    if (!(agi_IsPow2(w) && agi_IsPow2(h)) && report) {
        strcpy(buf, path);
        strcat(buf, " does not have power of 2 dimensions!");
        return 0;
    }
    return 1;
}
/* 0x44e6f0, text-format line handler: stores the quoted first bitmap name of the current material
   (file extension stripped) and checks that bitmap */

void agi_Bitmap0(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");
    char *dot = strrchr(s, '.');

    if (dot)
        *dot = 0;
    strcpy(AGI_M(c).name[0], s);
    agi_CheckBitmap(AGI_M(c).name[0], 1);
}
/* 0x44e760, text-format line handler: stores the quoted second bitmap name of the current material
   (file extension stripped) and checks that bitmap */

void agi_Bitmap1(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");
    char *dot = strrchr(s, '.');

    if (dot)
        *dot = 0;
    strcpy(AGI_M(c).name[1], s);
    agi_CheckBitmap(AGI_M(c).name[1], 1);
}
/* 0x44e7d0, text-format line handler: stores the quoted third bitmap name of the current material
   (file extension stripped) and checks that bitmap */

void agi_Bitmap2(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");
    char *dot = strrchr(s, '.');

    if (dot)
        *dot = 0;
    strcpy(AGI_M(c).name[2], s);
    agi_CheckBitmap(AGI_M(c).name[2], 1);
}
/* 0x44e840, text-format line handler: sets the current material's blend mode from the next token,
   "MIX" = 1, "ADD" = 2, anything else = 0 */

void agi_Blend(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");

    AGI_M(c).mode = 0;
    if (!strcmp(s, "MIX")) {
        AGI_M(c).mode = 1;
        return;
    }
    if (!strcmp(s, "ADD"))
        AGI_M(c).mode = 2;
}
/* 0x44e8d0, text-format line handler: reads the current material's b1 byte from the next token */

void agi_MatB1(void *unused, AGICTX *c)
{
    AGI_M(c).b1 = atoi(AGI_TOK(" "));
}
/* 0x44e910, text-format line handler: reads the current material's b2 byte from the next token */

void agi_MatB2(void *unused, AGICTX *c)
{
    AGI_M(c).b2 = atoi(AGI_TOK(" "));
}
/* 0x44e950, text-format line handler: reads the current material's b4 byte from the next token */

void agi_MatB4(void *unused, AGICTX *c)
{
    AGI_M(c).b4 = atoi(AGI_TOK(" "));
}
/* 0x44e990, text-format line handler: reads the current material's b8 byte from the next token */

void agi_MatB8(void *unused, AGICTX *c)
{
    AGI_M(c).b8 = atoi(AGI_TOK(" "));
}
/* 0x44e9d0, text-format line handler: reads the current material's f[0] float from the next token */

void agi_MatF0(void *unused, AGICTX *c)
{
    AGI_M(c).f[0] = atof(AGI_TOK(" "));
}
/* 0x44ea10, text-format line handler: reads the current material's f[1] float from the next token */

void agi_MatF1(void *unused, AGICTX *c)
{
    AGI_M(c).f[1] = atof(AGI_TOK(" "));
}
/* 0x44ea50, text-format line handler: reads the current material's f[2] float from the next token */

void agi_MatF2(void *unused, AGICTX *c)
{
    AGI_M(c).f[2] = atof(AGI_TOK(" "));
}
/* 0x44ea90, text-format line handler: reads the current material's f[3] float from the next token */

void agi_MatF3(void *unused, AGICTX *c)
{
    AGI_M(c).f[3] = atof(AGI_TOK(" "));
}
/* 0x44ead0, text-format line handler: reads the current material's f[4] float from the next token */

void agi_MatF4(void *unused, AGICTX *c)
{
    AGI_M(c).f[4] = atof(AGI_TOK(" "));
}
/* 0x44eb10, number of materials */

void agi_Materials(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(" "));

    c->nmats = n;
    c->cur = 0;
    if (n > 0)
        c->mats = (AGIMAT *)mem_Alloc(n * sizeof(AGIMAT), "d3_io_a:AGIMaterialBuffer");
}
/* 0x44eb60, the three point indices of a polygon */

void agi_PolyVerts(void *unused, AGICTX *c)
{
    char *t;

    t = AGI_TOK(" ");
    c->polys[c->polyidx].v[0] = atoi(t) + c->ptbase;
    t = AGI_TOK(" ");
    c->polys[c->polyidx].v[1] = atoi(t) + c->ptbase;
    t = AGI_TOK(" ");
    c->polys[c->polyidx].v[2] = atoi(t) + c->ptbase;
}
/* 0x44ebf0, the texture coordinates of a polygon: three u, then three v */

void agi_PolyUV(void *unused, AGICTX *c)
{
    int i;

    for (i = 0; i < 3; i++)
        c->polys[c->polyidx].uv[i][0] = atof(AGI_TOK(" "));
    for (i = 0; i < 3; i++)
        c->polys[c->polyidx].uv[i][1] = atof(AGI_TOK(" "));
}
/* 0x44ec60, the material of a polygon; ends the polygon */

void agi_PolyMat(void *unused, AGICTX *c)
{
    char *t = AGI_TOK(" ");

    c->polys[c->polyidx].tex = -1;
    c->polys[c->polyidx].mat = atoi(t);
    c->polyidx++;
}
/* 0x44ecb0, ends an object: its polygons and points count for the following objects */

void agi_EndObject(void *unused, AGICTX *c)
{
    c->polybase += c->npoly;
    c->npoly = 0;
    c->ptbase += c->npts;
    c->npts = 0;
}
/* 0x44ece0, text-format line handler: reads one int token into the context's unk04 field @deep Q1/Q2: no evidence for the handler name or for AGICTX.unk04 - the binary twin bgi_Unk04 (0x44f650) reads the same field, D3BGFDESC covers offset 0x04 with pad02, and the text directive string sits in the parser table beyond 0x44ee50, outside the repo. Q6 added. */

void agi_Unk44ece0(void *unused, AGICTX *c)
{
    c->unk04 = atoi(AGI_TOK(" "));
}
/* 0x44ed00, number of points of an object; allocates the point buffer once */

void agi_NumPoints(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(" "));

    c->npts = n;
    if (n > 0 && !c->pts) {
        c->ptidx = 0;
        c->ptbase = 0;
        c->pts = (AGIPOINT *)mem_Alloc(0x60000, "d3_io_a:AGIPointBuffer");
    }
}
/* 0x44ed50, number of polygons of an object; allocates the polygon buffer once */

void agi_NumPolys(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(" "));

    c->npoly = n;
    if (n > 0 && !c->polys) {
        c->polyidx = 0;
        c->polybase = 0;
        c->polys = (AGIPOLY *)mem_Alloc(0xe0000, "d3_io_a:AGIPolyBuffer");
    }
}
/* 0x44eda0, reads the points, one line each */

void agi_Points(void *f, AGICTX *c)
{
    char *line;
    int i;

    for (i = 0; i < c->npts; i++) {
        line = util_ReadLine(f);
        c->pts[c->ptidx].x = atof(strtok(line, " "));
        c->pts[c->ptidx].y = atof(strtok(0, " "));
        c->pts[c->ptidx].z = atof(strtok(0, " "));
        c->ptidx++;
    }
    util_ReadLine(f);
}
/* 0x44ee40, empty body: the no-op default used wherever a triangle / picture callback is required
   (d3_TriFuncs entries, skipped picture types), called with different signatures */

void d3io_NoopCallback(void *t)
{
}
