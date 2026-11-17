/* d3_io_a.c -- AGI / AGF text model reader ("d3_io_a:" allocation tags): line handlers that fill a reader
   context (materials, points, polygons). Extent in GildeGold.exe: 0x44e570 (right after the object /
   animation loaders of d3_io_t.c) .. 0x44eea0, where the binary reader d3_io_b.c follows. Two objects: the
   .data strings of agi_Kind ("ANIM", "LOCAL" 0x5fc6e4..) close this unit's string run, and d3_io_b.c's
   handler tables (0x5fc6f8) start a new plain .data block after them. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_a.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d3_io_a.h"
#include "d3_io_b.h"
#include "bio.h"
#include "bmp.h"
#include "mem.h"
#include "ts_texture.h"

#define AGI_TOK(sep) strtok(0, sep)
#define AGI_M(c) ((c)->mats[(c)->cur])

/* the strtok separators of the line handlers: named read-only arrays, not literals (the original has them in
   .rdata, its string literals in .data), 4 bytes each (a char[2] would put the second at +2) */
const char agi_SepSpace[4] = " ";   /* 0x5ebb58 */
const char agi_SepQuote[4] = "\"";  /* 0x5ebb5c */

/* The keyword tables of the AGI text reader: {keyword, line handler or sub table, entries of the sub table}, each
   opened by "*INIT*" and closed by "*DONE*" (the handlers run when the reader enters / leaves the block). The
   reader that walked them is not in the image (/OPT:REF removed it; .data keeps the tables, and they keep the
   handlers): no code refers to them. The materials' "*DONE*" handler is 0x44f000 (bgi_NextMat, the one body the
   linker kept for the identical "next material" handlers of both readers). The names are ours. */
typedef struct AGIENT {             /* 12 bytes */
    char *key;                      /* the keyword */
    void *fn;                       /* handler (unused, ctx), or the sub table when n != 0 */
    int n;                          /* entries of the sub table */
} AGIENT;

AGIENT agi_TabMat[16] = {  /* 0x5fc2c8, the keywords of one material */
    {"*INIT*", 0, 0}, {"*DONE*", bgi_NextMat, 0}, {"BASE_MAP", agi_Bitmap0, 0}, {"BUMP_MAP", agi_Bitmap1, 0},
    {"ENVIRONMENT_MAP", agi_Bitmap2, 0}, {"ALPHA_TYPE", agi_Blend, 0}, {"ALPHA_VALUE", agi_MatB1, 0},
    {"COLORKEYING", agi_MatB2, 0}, {"NO_MIPMAP", agi_MatB4, 0}, {"SCROLL_X", agi_MatB8, 0}, {"SCROLL_Y", agi_MatB8, 0},
    {"U_TILE", agi_MatF0, 0}, {"V_TILE", agi_MatF1, 0}, {"U_OFFSET", agi_MatF2, 0}, {"V_OFFSET", agi_MatF3, 0},
    {"UVW_ROTATE", agi_MatF4, 0}
};
AGIENT agi_TabMats[4] = {  /* 0x5fc388, the materials block */
    {"*INIT*", 0, 0}, {"*DONE*", 0, 0}, {"NUM_MATERIALS", agi_Materials, 0}, {"MATERIAL", agi_TabMat, 16}
};
AGIENT agi_TabPoly[6] = {  /* 0x5fc3b8, the keywords of one face */
    {"*INIT*", d3io_NoopCallback, 0}, {"*DONE*", 0, 0}, {"INDEX", agi_PolyVerts, 0}, {"UUUVVVWWW", agi_PolyUV, 0},
    {"NORMAL", d3io_NoopCallback, 0}, {"MATERIAL_REF", agi_PolyMat, 0}
};
AGIENT agi_TabGeom[7] = {  /* 0x5fc400, the mesh block */
    {"*INIT*", 0, 0}, {"*DONE*", agi_EndObject, 0}, {"TIME_INDEX", agi_GeomInt, 0}, {"NUM_VERTICES", agi_NumPoints, 0},
    {"NUM_FACES", agi_NumPolys, 0}, {"VERTICES", agi_Points, 0}, {"FACES", agi_TabPoly, 6}
};
AGIENT agi_TabObj[5] = {  /* 0x5fc454, the object block */
    {"*INIT*", 0, 0}, {"*DONE*", 0, 0}, {"NAME", d3io_NoopCallback, 0}, {"HAS_SHADOW", d3io_NoopCallback, 0},
    {"MESH", agi_TabGeom, 7}
};
AGIENT agi_TabTop[5] = {  /* 0x5fc490, the file's top level */
    {"*INIT*", 0, 0}, {"*DONE*", 0, 0}, {"SPACE", agi_Kind, 0}, {"MATERIALS", agi_TabMats, 4}, {"OBJECT", agi_TabObj, 5}
};

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
    char *s = AGI_TOK(agi_SepQuote);
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
    char *s = AGI_TOK(agi_SepQuote);
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
    char *s = AGI_TOK(agi_SepQuote);
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
    char *s = AGI_TOK(agi_SepQuote);

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
    AGI_M(c).b1 = atoi(AGI_TOK(agi_SepSpace));
}
/* 0x44e910, text-format line handler: reads the current material's b2 byte from the next token */

void agi_MatB2(void *unused, AGICTX *c)
{
    AGI_M(c).b2 = atoi(AGI_TOK(agi_SepSpace));
}
/* 0x44e950, text-format line handler: reads the current material's b4 byte from the next token */

void agi_MatB4(void *unused, AGICTX *c)
{
    AGI_M(c).b4 = atoi(AGI_TOK(agi_SepSpace));
}
/* 0x44e990, text-format line handler: reads the current material's b8 byte from the next token */

void agi_MatB8(void *unused, AGICTX *c)
{
    AGI_M(c).b8 = atoi(AGI_TOK(agi_SepSpace));
}
/* 0x44e9d0, text-format line handler: reads the current material's f[0] float from the next token */

void agi_MatF0(void *unused, AGICTX *c)
{
    AGI_M(c).f[0] = atof(AGI_TOK(agi_SepSpace));
}
/* 0x44ea10, text-format line handler: reads the current material's f[1] float from the next token */

void agi_MatF1(void *unused, AGICTX *c)
{
    AGI_M(c).f[1] = atof(AGI_TOK(agi_SepSpace));
}
/* 0x44ea50, text-format line handler: reads the current material's f[2] float from the next token */

void agi_MatF2(void *unused, AGICTX *c)
{
    AGI_M(c).f[2] = atof(AGI_TOK(agi_SepSpace));
}
/* 0x44ea90, text-format line handler: reads the current material's f[3] float from the next token */

void agi_MatF3(void *unused, AGICTX *c)
{
    AGI_M(c).f[3] = atof(AGI_TOK(agi_SepSpace));
}
/* 0x44ead0, text-format line handler: reads the current material's f[4] float from the next token */

void agi_MatF4(void *unused, AGICTX *c)
{
    AGI_M(c).f[4] = atof(AGI_TOK(agi_SepSpace));
}
/* 0x44eb10, number of materials */

void agi_Materials(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(agi_SepSpace));

    c->nmats = n;
    c->cur = 0;
    if (n > 0)
        c->mats = (AGIMAT *)mem_Alloc(n * sizeof(AGIMAT), "d3_io_a:AGIMaterialBuffer");
}
/* 0x44eb60, the three point indices of a polygon */

void agi_PolyVerts(void *unused, AGICTX *c)
{
    char *t;

    t = AGI_TOK(agi_SepSpace);
    c->polys[c->polyidx].v[0] = atoi(t) + c->ptbase;
    t = AGI_TOK(agi_SepSpace);
    c->polys[c->polyidx].v[1] = atoi(t) + c->ptbase;
    t = AGI_TOK(agi_SepSpace);
    c->polys[c->polyidx].v[2] = atoi(t) + c->ptbase;
}
/* 0x44ebf0, the texture coordinates of a polygon: three u, then three v */

void agi_PolyUV(void *unused, AGICTX *c)
{
    int i;

    for (i = 0; i < 3; i++)
        c->polys[c->polyidx].uv[i][0] = atof(AGI_TOK(agi_SepSpace));
    for (i = 0; i < 3; i++)
        c->polys[c->polyidx].uv[i][1] = atof(AGI_TOK(agi_SepSpace));
}
/* 0x44ec60, the material of a polygon; ends the polygon */

void agi_PolyMat(void *unused, AGICTX *c)
{
    char *t = AGI_TOK(agi_SepSpace);

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
/* 0x44ece0, text-format line handler: reads the object geometry int from the next token into the reader
   context (the field the binary reader's chunk 0x18 fills) */

void agi_GeomInt(void *unused, AGICTX *c)
{
    c->geomInt = atoi(AGI_TOK(agi_SepSpace));
}
/* 0x44ed00, number of points of an object; allocates the point buffer once */

void agi_NumPoints(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(agi_SepSpace));

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
    int n = atoi(AGI_TOK(agi_SepSpace));

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
        c->pts[c->ptidx].x = atof(strtok(line, agi_SepSpace));
        c->pts[c->ptidx].y = atof(strtok(0, agi_SepSpace));
        c->pts[c->ptidx].z = atof(strtok(0, agi_SepSpace));
        c->ptidx++;
    }
    util_ReadLine(f);
}
/* 0x44ee40, empty body: the no-op default used wherever a triangle / picture callback is required
   (d3_TriFuncs entries, skipped picture types), called with different signatures */

void d3io_NoopCallback(void *t)
{
}
/* 0x44ee50, "LOCAL" / "ANIM" keyword of the text format */
void agi_Kind(void *unused, char *out)
{
    char *tok = strtok(0, agi_SepSpace);

    *out = 0;
    if (!strcmp(tok, "LOCAL")) {
        *out = 1;
        return;
    }
    if (!strcmp(tok, "ANIM"))
        *out = 2;
}
