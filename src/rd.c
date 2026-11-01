/* rd.c -- tag "rd_" (rd_AllBauplatzToSupermap, rd_GetBKSize in its error texts): helpers that place animals (a building's "dummy_TUER", free cells near a point, a random dummy
   by name), then the vegetation map and the roads. Start: the first user of "dummy_TUER" (.data 0x602854, right
   after anm.c's literals); the road code 0x4bcc20 uses the same literal (no cross-obj pooling in this exe, so one
   obj). The .bss gap 0x6f0744 after anm's globals hints at a new obj here. Names are ours.
   Open: the start is not proven beyond that .bss gap (rd.c could still be one unit with anm.c).
   unit of rea 8cef7d5b (src/game/rd.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "anm.h"
#include "f3gm.h"
#include "d3.h"
#include "d3_light.h"
#include "data_views.h"
#include "d3_sm.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "d3_vec.h"
#include "f3init.h"
#include "path.h"
#include "tr.h"
#include "unk_458810.h"
#include "unk_58bbd0.h"
#include "ch.h"
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (as ai.h: unsigned short (int)): this unit's callers were compiled against the 16-bit parameter (as ahm.c's) */

/* 0x4bc2e0, pos = the "dummy_TUER" (or the object) of a random building of the gm_BuildQuery(1, 6) list */
int rd_DoorPos(float *pos)
{
    G15BLD *bs[0x100];
    int n;
    G15BLD *b;
    char *o;

    n = 0;
    b = (G15BLD *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (gm_IsHomeType((unsigned char *)b) == 0 && gm_IsType10((unsigned char *)b) == 0 && b->object != 0)
            bs[(unsigned short)n++] = b;
        b = (G15BLD *)gm_BuildNext();
    }
    b = bs[util_RandomS(n)];
    o = d3_FindObject(b->object, 0x300, 0, "dummy_TUER");
    if (o != 0)
        d3_PointToWorld(o, o + 0x4c, pos);
    else
        d3_PointToWorld(b->object, b->object + 0x4c, pos);
    return 1;
}
/* 0x4bc390, n points pts near pos (a free cell within -4..5 cells, else pos's cell); returns n @deep behaviour: remaining code hunks are allocation only - pos in ebx vs the original's edi, frame 0x14 vs 0x10 (x in a frame slot where the original reuses the pos argument slot), and the free-cell fallback stores px/pz into x/z where the original forwards them into the arg pushes; clamps now match (inline h->size-2 < x makes VC6 emit add -2 + cmp/jge; a local m gives sub + jle); Q4 fixed with G15GRID/G15SCENE, readable */

int rd_PointsNear(float *pos, int n, float (*pts)[4])
{
    G15GRID *h;
    int px, pz;
    float py;
    int x, z;
    int i;

    h = ((G15SCENE *)d3_DefaultScene)->hmap;
    d3_GetHeightAt(h, pos, &px, &pz, &py);
    for (i = n; i > 0; i--) {
        x = util_RandomS(10) - 4 + px;
        z = util_RandomS(10) - 4 + pz;
        if (h->size - 2 < x)
            x = h->size - 2;
        if (x < 2)
            x = 2;
        if (h->size - 2 < z)
            z = h->size - 2;
        if (z < 2)
            z = 2;
        if (!ch_FindFreeCell((char *)h, pos, pos, x, z, &x, &z, 600, 0)) {
            x = px;
            z = pz;
        }
        d3sm_GetVertex((char *)h, x, z, *pts);
        pts++;
    }
    return n;
}
/* 0x4bc4b0, d3_ForEachObject callback: collects the objects named f->name (at most 32) */

int rd_CollectNamed(char *obj, G15FIND *f)
{
    if (strcmp(obj, f->name) == 0)
        f->objs[f->n++] = obj;
    return f->n < 0x20;
}
/* 0x4bc520, a random object of the default scene named name, 0 if none */

char *rd_RandomNamedObject(char *name)
{
    G15FIND f;

    f.n = 0;
    strcpy(f.name, name);
    d3_ForEachObject(d3_DefaultScene, 0, 0x380, rd_CollectNamed, &f);
    if (f.n == 0)
        return 0;
    return f.objs[util_RandomS(f.n)];
}
/* 0x4bc5a0, the vegetation type with id type, 0 if none */

G15TREE *veg_FindType(int type)
{
    int i;

    for (i = 0; veg_Types[i].id != 0; i++) {
        if (veg_Types[i].id == type)
            return &veg_Types[i];
    }
    return 0;
}
/* 0x4bc5f0, the plant at cell x / z, 0 if none */

G15PLANT *veg_FindPlantAt(G15PLANT *p, int x, int z)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        if (p[i].x == x && p[i].z == z && p[i].state != -1)
            return &p[i];
    }
    return 0;
}
/* 0x4bc640, the plant with 3D object obj, 0 if none */

G15PLANT *veg_FindPlantObject(G15PLANT *p, char *obj)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1 && p[i].obj == obj)
            return &p[i];
    }
    return 0;
}
/* 0x4bc670, the plant with id id, 0 if none */

G15PLANT *veg_FindPlantId(G15PLANT *p, int id)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1 && p[i].id == id)
            return &p[i];
    }
    return 0;
}
/* 0x4bc6a0, the plant whose area (size cells around it) covers cell x / z, 0 if none */

G15PLANT *veg_FindPlantArea(G15PLANT *p, int x, int z)
{
    int i;
    int r;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1) {
            r = p[i].size / 2;
            if (x >= p[i].x - r && x <= p[i].x + r && z >= p[i].z - r && z <= p[i].z + r)
                return &p[i];
        }
    }
    return 0;
}
/* 0x4bc710, 1 if a plant of size cells fits at x / z (a free record, no plant there or in the area) */

int veg_Fits(G15PLANT *p, int x, int z, int size)
{
    int free;
    int i, j, k;

    free = 0;
    for (i = 0; i < 0x40; i++) {
        if (p[i].state == -1)
            free = 1;
        else if (p[i].x == x && p[i].z == z)
            return 0;
    }
    if (!free)
        return 0;
    for (j = 0; j < size; j++) {
        for (k = 0; k < size; k++) {
            if (veg_FindPlantArea(p, x - size / 2 + k, z - size / 2 + j) != 0)
                return 0;
        }
    }
    return 1;
}
/* 0x4bc7c0, sets the plant at x / z (a new record if none) to type (0 removes it); fall: autumn model @deep behaviour: the remaining 35 code hunks are allocation only - t in [esp+0x20] vs [esp+0x24], esi/eax as zero registers, the second loop rotated with a duplicated state test where the original tests once at the top, q->obj = 0 as a register vs the immediate, the q->state = 1 block inline vs out of line on the return path; meaning read against the disassembly and equal (both searches, veg_NextId post-increment, fall==0 with type==0 delete, veg_LoadModel(q, fall), x/z/size). Q2: the field +0x10 is only written -1 and never read in the exe, no evidence for a role, kept unk10 */

G15PLANT *veg_SetPlant(G15PLANT *p, int x, int z, int type, int fall)
{
    G15TREE *t;
    G15PLANT *q;
    int i;

    t = veg_FindType(type);
    q = 0;
    if (p != 0) {
        for (i = 0; i < 0x40; i++) {
            if (p[i].x == x && p[i].z == z && p[i].state != -1)
                goto found;
        }
        i = 0;
        while (p[i].state != -1) {
            if (++i >= 0x40)
                return 0;
        }
        p[i].id = veg_NextId++;
found:
        q = &p[i];
        q->type = type;
        if (fall == 0) {
            q->unk10 = -1;
            if (type == 0) {
                q->state = -1;
                if (q->obj != 0) {
                    d3if_Delete((struct D3OBJECT *)q->obj);
                    q->obj = 0;
                }
            } else
                q->state = 1;
        } else {
            if (q->obj != 0) {
                d3if_Delete((struct D3OBJECT *)q->obj);
                q->obj = 0;
            }
            veg_LoadModel(q, fall);
        }
        q->x = x;
        q->z = z;
        if (t != 0)
            q->size = t->size;
        else
            q->size = 0;
    }
    return q;
}
/* 0x4bc8d0, the plants grow one state (their objects are rebuilt by 0x4bcb90) */

void veg_GrowPlants(G15PLANT *p)
{
    int i;
    G15TREE *t;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1) {
            if (p[i].obj != 0) {
                d3if_Delete((struct D3OBJECT *)p[i].obj);
                p[i].obj = 0;
            }
            t = veg_FindType(p[i].type);
            if (t == 0)
                t = veg_FindType(p[i].type);
            if (p[i].state < t->lastState)
                p[i].state++;
        }
    }
    veg_ShowPlants(p);
}
/* 0x4bc940, loads plant q's model ("%svegetation/*pfl_<type>_0<n>[_FALL].ogr"), names it "_ID<id>" and places it
   on the height map (origin + x * axisx + z * axisz + height * axisy). @deep match-base 2026-10-31: read against the original; 182 vs 180 instructions (was 158), differing 40 (was 162). The placement is pos = origin, then += x * axisx, += z * axisz, += height * axisy component by component through the typed D3TERRAIN (d3_sm.h; origin copied by integer moves, each height re-read per component), and each branch has its own sprintf with the literal format (tail-merged, as the original). Left: register choice in the height part - ours needs a byte register for heights[z * size + x] and spills o (ebx) to [esp+0x20] around it where the original uses ecx with the row base in edx (hunks 0x4bca81, 0x4bcb02, 0x4bcb21, 0x4bcb46); the index spellings heights[size * z + x] / *(heights + z * size + x) / x first compile the same. */

void veg_LoadModel(G15PLANT *q, int fall)
{
    G15TREE *t;
    int n;
    char *o;
    int x, z;
    D3TERRAIN *tr;
    float pos[3];
    char path[0x100];
    char buf[0x100];

    t = veg_FindType(q->type);
    if (fall == 0) {
        n = q->state / t->states;
        if (n < 1)
            n = 1;
        sprintf(path, "%svegetation/*pfl_%s_0%i.ogr", ai_DataPath, t->name, n);
    } else {
        n = q->state / t->states;
        if (n < 1)
            n = 1;
        sprintf(path, "%svegetation/*pfl_%s_0%i_FALL.ogr", ai_DataPath, t->name, n);
    }
    o = d3_LoadGroup(path, 0, 0);
    if (o == 0) {
        sprintf(buf, "init_SetPflanzenMap(): Could not load 3D-Objekt-roup '%s'...", path);
        err_Message(buf);
        return;
    }
    q->obj = o;
    sprintf(buf, "_ID%i", q->id);
    strcat(o, buf);
    ((G15OBJ *)o)->mask = 1;
    tr = d3_Terrain;
    if (tr != 0) {
        pos[0] = tr->origin.x;
        pos[1] = tr->origin.y;
        pos[2] = tr->origin.z;
        x = q->x;
        z = q->z;
        pos[0] += x * tr->axisx.x;
        pos[1] += x * tr->axisx.y;
        pos[2] += x * tr->axisx.z;
        pos[0] += z * tr->axisz.x;
        pos[1] += z * tr->axisz.y;
        pos[2] += z * tr->axisz.z;
        pos[0] += tr->heights[z * tr->size + x] * tr->axisy.x;
        pos[1] += tr->heights[z * tr->size + x] * tr->axisy.y;
        pos[2] += tr->heights[z * tr->size + x] * tr->axisy.z;
        d3_SetObjectPos(o, pos);
    }
    d3_RelightObjects(o);
}
/* 0x4bcb90, shows the plants (loading missing models) */

void veg_ShowPlants(G15PLANT *p)
{
    int i;

    if (p == 0)
        return;
    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1) {
            if (p[i].obj == 0) {
                veg_LoadModel(&p[i], 0);
                if (p[i].obj == 0)
                    continue;
            }
            d3if_ShowTree(p[i].obj, 1);
        }
    }
}
/* 0x4bcbe0, hides the plants */

void veg_HidePlants(G15PLANT *p)
{
    int i;

    if (p == 0)
        return;
    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1 && p[i].obj != 0)
            d3if_ShowTree(p[i].obj, 0);
    }
}
/* 0x4bcc20, lays a road from the building site of object obj (name: 0x58f640) to the next road (marks its cells
   as ground type "EINFACHER_WEG" on the town map, around each cell before 0x780) @deep read whole against the original: fixed the DDA (|dx| > |dz| branch first, the compare on the double fabs results, dz / fabs(dx)), the map fill (>= 0x780 single cells first, then the 3x3 loops with k before the cell x), the fail block placed before the success part; renamed ai_Unkfd7f78 -> d3_Scene0Flags. Left: our VC6 cross-jumps the three inline 'd3sm_Destroy(rd_Road.grid); return 0;' blocks (ai_BuildingName fail, bk == 0, the first rd_PathToRoad) into the shared fail tail (ours 'push edx; jmp X' at +0x10f/+0x195/+0x24d, the original's own 'call d3sm_Destroy; add esp,4; xor eax,eax; pop x4; add esp,0xc8; ret' each), tried all-inline returns without gotos (worse: 197 ins); the rest is frame 0xc4 vs 0xc8 (one slot), v[1] = bk->y0 copied with mov (ours) vs fld/fstp, the fstp scheduled after vs before 'push eax' at the WorldToCell calls, px/pz loaded in the other order (edx/eax) before the third rd_PathToRoad (size: 495 vs 522 instructions, explained: the 27 missing instructions are exactly the three duplicated 9-instruction fail epilogues the original keeps and our compile merges) */

int rd_BuildRoad(char *obj, int name)
{
    G15BKFIND f;
    G15BK *bk;
    float v[3];
    float ax, az, bx, bz;
    float dx, dz;
    float x, z, step;
    int i, j, k;
    char *o;
    int px, pz;
    char buf[0x40];

    rd_Road.map = (G15MAP *)ai_MapRefs;
    if (ai_MapRefs == 0)
        return 0;
    rd_Road.grid = d3sm_Create((struct D3TERRAIN *)ai_MapRefs, ((G15MAP *)ai_MapRefs)->w, 2);
    if (rd_Road.grid == 0)
        return 0;
    rd_Road.prunk = -1;
    rd_Road.simple = -1;
    for (i = 0; i < 8; i++) {
        strcpy(buf, rd_Road.map->ground[i]);
        _strupr(buf);
        if (strstr(buf, "EINFACHER_WEG"))
            rd_Road.simple = i;
        if (strstr(buf, "PRUNK_WEG"))
            rd_Road.prunk = i;
    }
    if (rd_Road.prunk == -1)
        rd_Road.prunk = 1;
    if (rd_Road.simple == -1)
        rd_Road.simple = 2;
    rd_AddBauplaetze(&rd_Road);
    if (!ai_BuildingName(name, f.name)) {
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
    d3_LocalToWorld(obj, v, f.pos);
    f.found = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x180, rd_FindSiteCB, &f);
    if (f.found == 0)
        goto fail;
    bk = rd_GetBKSize(f.found);
    if (bk == 0) {
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    v[0] = rd_Road.map->cw + bk->x0;
    v[1] = bk->y0;
    v[2] = rd_Road.map->ch + bk->z0;
    rd_WorldToCell(&ax, &az, v, f.found, &rd_Road);
    v[0] = bk->x1 - rd_Road.map->cw;
    v[1] = bk->y0;
    v[2] = rd_Road.map->ch + bk->z0;
    rd_WorldToCell(&bx, &bz, v, f.found, &rd_Road);
    if (!rd_PathToRoad(&rd_Road, (int)ax, (int)az, -1, -1)) {
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    memcpy(rd_Road.path2, rd_Road.path, sizeof(rd_Road.path));
    rd_Road.n2 = rd_Road.n;
    if (!rd_PathToRoad(&rd_Road, (int)bx, (int)bz, -1, -1))
        goto fail;
    if (rd_Road.n2 > rd_Road.n && rd_Road.n2 >= 5) {
        memcpy(rd_Road.path2, rd_Road.path, sizeof(rd_Road.path));
        rd_Road.n2 = rd_Road.n;
    } else if (rd_Road.n2 < 5) {
        memcpy(rd_Road.path2[rd_Road.n2], rd_Road.path, sizeof(rd_Road.path));
        rd_Road.n2 += rd_Road.n;
    }
    dx = bx - ax;
    dz = bz - az;
    if (fabs(dx) > fabs(dz)) {
        dz = dz / fabs(dx);
        step = dx > 0.0f ? 1.0f : -1.0f;
        x = ax;
        z = az;
        while ((int)x != (int)bx) {
            rd_Road.path2[rd_Road.n2][0] = (unsigned char)(int)x;
            rd_Road.path2[rd_Road.n2][1] = (unsigned char)(int)z;
            rd_Road.n2++;
            rd_Road.grid->cells[((int)z * rd_Road.map->w + (int)x) * 7] = 6;
            x += step;
            z += dz;
        }
    } else {
        dx = dx / fabs(dz);
        step = dz > 0.0f ? 1.0f : -1.0f;
        x = ax;
        z = az;
        while ((int)z != (int)bz) {
            rd_Road.path2[rd_Road.n2][0] = (unsigned char)(int)x;
            rd_Road.path2[rd_Road.n2][1] = (unsigned char)(int)z;
            rd_Road.n2++;
            rd_Road.grid->cells[(rd_Road.map->w * (int)z + (int)x) * 7] = 6;
            x += dx;
            z += step;
        }
    }
    o = d3_FindObject(obj, 0x100, 0, "dummy_TUER");
    if (o == 0)
        o = obj;
    d3_PointToWorld(o, o + 0x4c, v);
    if (!d3_GetHeightAt((char *)rd_Road.grid, v, &px, &pz, 0)
        || !rd_PathToRoad(&rd_Road, px, pz, -1, -1)) {
fail:
        d3sm_Destroy(rd_Road.grid);
        return 0;
    }
    d3sm_Destroy(rd_Road.grid);
    if (ai_MapSize >= 0x780) {
        for (i = 0; i < rd_Road.n2; i++)
            rd_Road.map->cells[rd_Road.path2[i][1] * rd_Road.map->w + rd_Road.path2[i][0]]
                = (unsigned char)rd_Road.simple;
        for (i = 0; i < rd_Road.n; i++)
            rd_Road.map->cells[rd_Road.path[i][1] * rd_Road.map->w + rd_Road.path[i][0]]
                = (unsigned char)rd_Road.simple;
    } else {
        for (i = 0; i < rd_Road.n2; i++) {
            for (j = -1; j < 2; j++) {
                for (k = -1; k < 2; k++)
                    rd_Road.map->cells[(rd_Road.path2[i][1] + j) * rd_Road.map->w
                        + k + rd_Road.path2[i][0]] = (unsigned char)rd_Road.simple;
            }
        }
        for (i = 0; i < rd_Road.n; i++) {
            for (j = -1; j < 2; j++) {
                for (k = -1; k < 2; k++)
                    rd_Road.map->cells[(rd_Road.path[i][1] + j) * rd_Road.map->w
                        + k + rd_Road.path[i][0]] = (unsigned char)rd_Road.simple;
            }
        }
    }
    if ((d3_Scene0Flags & 2) == 0)
        d3fl_MakeDivide(rd_Road.map);
    return 1;
}
/* 0x4bd340, path_Find from cell x / z to tx / tz (both < 0: the nearest road cell, type 6 / 0xb); 1 if found @deep behaviour: far draft, 234 code hunks - the same operations in another allocation (ebx vs edi for the zero, the parameters read at different frame slots, 0x18 vs 0xc locals) and reordered compares; the nearest-road cell scan and the path_Find call were read and match in meaning */

int rd_PathToRoad(G15ROAD *r, int x, int z, int tx, int tz)
{
    int n;
    int i, j;
    int *c;
    float d, best;
    int row;

    if (tx < 0 || tz < 0) {
        n = r->grid->size;
        best = (float)(n * n * 2);
        tx = -1;
        tz = -1;
        if (n < 1)
            return 0;
        row = 0;
        for (i = 0; i < n; i++) {
            c = (int *)((char *)r->grid->cells + row);
            for (j = 0; j < n; j++) {
                if ((*c == 6 || *c == 0xb)
                    && (d = (float)((i - z) * (i - z) + (j - x) * (j - x))) < best) {
                    tx = j;
                    tz = i;
                    best = d;
                }
                c += 7;
            }
            row += n * 0x1c;
        }
    }
    if (tx > -1 && tz > -1) {
        r->max = 0x400;
        r->pts = r->path;
        r->n = 0;
        return path_Find(&r->n, r->grid, x, z, tx, tz, 3) != 0xffff;
    }
    return 0;
}
/* 0x4bd4a0, puts every building site (0x58f7a0's list) on the grid */

void rd_AddBauplaetze(G15ROAD *r)
{
    int list[0x100];
    int n, i;

    memset(list, 0, sizeof(list));
    n = ai_FindBauplaetze(0, list);
    if (n != 0) {
        for (i = 0; i < n; i++) {
            if (list[i] != 0)
                rd_AllBauplatzToSupermap(r, (char *)list[i]);
        }
    }
}
/* 0x4bd500, marks the area of building site name (its four corners, 0x4bd760) @deep behaviour: far draft, 160 code hunks - frame 0x6c vs 0x70 and the corner positions copied through the FPU in the original (fld/fstp) where our float array copies are integer moves, plus scheduling of the four rd_WorldToCell calls and the rd_FillQuad call; the four corners and the call sequence were read and match in meaning */

void rd_AllBauplatzToSupermap(G15ROAD *r, char *name)
{
    G15BK *bk;
    float v[3];
    float c[8];
    char buf[0x40];

    bk = rd_GetBKSize(name);
    if (bk == 0) {
        sprintf(buf, "rd_AllBauplatzToSupermap(): Unbekannter Bauplatz '%s'", name);
        return;
    }
    v[0] = bk->x0;
    v[1] = bk->y0;
    v[2] = bk->z0;
    rd_WorldToCell(&c[0], &c[1], v, name, r);
    v[0] = bk->x1;
    v[1] = bk->y0;
    v[2] = bk->z0;
    rd_WorldToCell(&c[2], &c[3], v, name, r);
    v[0] = bk->x1;
    v[1] = bk->y0;
    v[2] = bk->z1;
    rd_WorldToCell(&c[4], &c[5], v, name, r);
    v[0] = bk->x0;
    v[1] = bk->y0;
    v[2] = bk->z1;
    rd_WorldToCell(&c[6], &c[7], v, name, r);
    rd_FillQuad(r, c, -1, 0);
}
/* 0x4bd610, d3_ForEachObject callback: the object named f->name (or with one leading character more) within 100
   of f->pos; stops there */

char rd_FindSiteCB(char *obj, G15BKFIND *f)
{
    float v[4];

    if (_strnicmp(obj, f->name, strlen(f->name)) == 0 || _strnicmp(obj + 1, f->name, strlen(f->name)) == 0) {
        d3_PointToWorld(obj, obj + 0x4c, v);
        if (d3_VecEqual(f->pos, v, 100.0f)) {
            f->found = obj;
            return 0;
        }
    }
    return 1;
}
/* 0x4bd6a0, the building site named name, 0 if unknown */

G15BK *rd_GetBKSize(char *name)
{
    int i;
    char buf[0x40];

    for (i = 0; i < ai_NBauplaetze; i++) {
        if (_strcmpi(name, ai_Bauplaetze[i].name) == 0)
            return &ai_Bauplaetze[i];
    }
    sprintf(buf, "rd_GetBKSize(): Unbekannter Bauplatz '%s'", name);
    return 0;
}
/* 0x4bd710, the town map cell x / z of v (local to obj) */

void rd_WorldToCell(float *x, float *z, float *v, char *obj, G15ROAD *r)
{
    float w[4];

    d3_LocalToWorld(obj, v, w);
    *x = (w[0] - r->map->x0) / r->map->cw;
    *z = (w[2] - r->map->z0) / r->map->ch;
}
/* 0x4bd760, fills the quad c (four cells x / z) on the town map with ground type v (-1: not) and on the grid with
   type t (0xff: not), two steps per cell
   @deep behaviour: far draft, 220 code hunks - the FPU block order differs (the original keeps c[0] / c[1] on the stack and loads c[2] / c[3] before the subtracts, ours computes dx/dz/a/b in another order) and 6 slot hunks; the two-step quad fill with the two sqrt(...)+0.5)*2 steps was read and matches in meaning */

void rd_FillQuad(G15ROAD *r, float *c, char v, int t)
{
    float dx, dz, ex, ez;
    float a, b, s;
    float px, pz, qx, qz;
    float fx, fz, x, z;
    int n, m;

    dx = c[2] - c[0];
    dz = c[3] - c[1];
    a = dz * dz + dx * dx;
    ex = c[4] - c[6];
    ez = c[5] - c[7];
    b = ez * ez + ex * ex;
    n = (int)(sqrt(a > b ? a : b) + 0.5) * 2;
    if (n < 1)
        n = 1;
    s = 1.0f / n;
    dx *= s;
    dz *= s;
    ex *= s;
    ez *= s;
    px = c[0];
    pz = c[1];
    qx = c[6];
    qz = c[7];
    for (; n > 0; n--) {
        fx = qx - px;
        fz = qz - pz;
        m = (int)(sqrt(fz * fz + fx * fx) + 0.5) * 2;
        if (m < 1)
            m = 1;
        s = 1.0f / m;
        fx *= s;
        fz *= s;
        x = px;
        z = pz;
        for (; m > 0; m--) {
            if (v != -1)
                r->map->cells[(int)z * r->map->w + (int)x] = v;
            if (t != 0xff)
                r->grid->cells[(r->map->w * (int)z + (int)x) * 7] = t;
            x += fx;
            z += fz;
        }
        px += dx;
        pz += dz;
        qx += ex;
        qz += ez;
    }
}

extern G15ROAD rd_Road;
/* d3_Scene0Flags: view of d3_Scenes (tools/data_views.py) */
G15TREE veg_Types[] = { { 439, "TANNE", 3, 1, 4, 0 }, { 440, "BUCHE", 5, 1, 4, 0 }, { 441, "EICHE", 5, 2, 8, 0 }, { 0, "", 0, 1, 0, 0 } };  /* 0x602860 */
int veg_NextId = 0;  /* 0x6f0748, the next plant id */
G15ROAD rd_Road = {0};  /* 0xf81300 */
