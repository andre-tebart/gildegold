/* anm.c -- animals: module tag "anm:animals" (mem_Alloc tag), the animal names "DOG", "CAT", "BIRD", "COW", "SHEEP",
   "CHICK", "PIG", "HORSE" (.data 0x602688, right after unk_4b6480.c's table), "katze_KATZE", "hund_HUND", ..
   Start: 0x4bb810 (first user of the tag); 0x4bb720 / 0x4bb7d0 before it post random messages (unk_4b6480.c).
   End: the place helpers after anm_UpdateSheep (0x4bc2e0..) share "dummy_TUER" with the road code, so they are rd.c's.
   Names are ours.
   Open: anm.c cannot include d3.h / ch.h (ch.h and rd.h redeclare d3 functions with other types), so the animal
   object is read through raw offsets; CHARACTER.unk1a0 (a scale) and D3OBJECT.unk224 still need names.
   unit of rea 8cef7d5b (src/game/anm.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <string.h>
#include "anm.h"
#include "f3gm.h"
#include "mem.h"
#include "tr.h"
#include "ch.h"
#include "d3_engine.h"
#include "d3_interface.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "unk_458810.h"
#include "unk_5d33b0.h"
extern unsigned char ai_GfxDetail;  /* 0x6f35c4 (unk_5d33b0.c: OPTIONBYTES): @original anm_Update reads it as a byte object (0x4bb884 mov al, byte ptr [0x6f35c4]); through the 4-byte union, .byte[0] or a char view of it, VC6 loads the dword (mov eax, dword ptr) */
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (as ai.h: unsigned short (int)): this unit's callers were compiled against the 16-bit parameter (as ahm.c's) */

/* 0x4bb810, allocates the animal list ("anm:animals") and resets the animal count and the first record */
void anm_Alloc(void)
{
    anm_Animals = mem_Alloc(0x2480, "anm:animals");
    anm_NAnimals = 0;
    anm_Animals[0].obj = 0;
}
/* 0x4bb840, frees the animal list */

void anm_Free(void)
{
    if (anm_Animals != 0) {
        mem_Free(anm_Animals);
        anm_Animals = 0;
    }
}

/* d3_Time from which the record may be flagged for removal (remove): 2100 ms after its birth time */
#define ANM_AGE_TIME(rec) ((rec).birth + 0x834)

/* 0x4bb860, the animal step: every 350 ms maybe a new animal (not in season 3), then one of the 32 records is
   updated by kind and removed when flagged
   @deep Q2 + Q4: ai_GfxDetail (0x6f35c4, no writer in src; the step reads it as the animal limit 0/16/32 - name unproven) and 3 raw accesses obj+0x34->+0x1ec->+0x96d flag 0x40 (D3MATERIAL has no field at 0x96d) */
void anm_Update(void)
{
    int scene, s, m;

    scene = d3_CurrentScene;
    s = gm_ClockSeason(ai_Time);
    d3if_SelectScene(0, 1);
    if (ai_GfxDetail == 0)
        m = 0;
    else
        m = ai_GfxDetail == 1 ? 0x10 : 0x20;
    if (d3_Time > anm_LastSpawn + 0x15e) {
        ANMREC *a;

        if ((int)(util_RandomS(m * 2) - m) > anm_NAnimals && s != 3 && (a = anm_NewAnimal()) != 0) {
            switch (util_RandomS(100) > 0x1e ? util_RandomS(5) + 3 : util_RandomS(2)) {
            case 1:
                anm_CreateCat(a);
                break;
            case 0:
                anm_CreateDog(a);
                break;
            case 4:
                anm_CreateSheep(a);
                break;
            case 3:
                anm_CreateCow(a);
                break;
            case 6:
                anm_CreateAnimal(a, 6);
                break;
            case 7:
                anm_CreateAnimal(a, 7);
                break;
            }
        }
        anm_LastSpawn = d3_Time;
    }
    if (anm_Animals[anm_NextAnimal].obj != 0) {
        switch (anm_Animals[anm_NextAnimal].kind) {
        case 0:
        case 1:
            anm_UpdateCat(&anm_Animals[anm_NextAnimal]);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            anm_UpdateSheep(&anm_Animals[anm_NextAnimal]);
            break;
        case 2:
            anm_UpdateBird(&anm_Animals[anm_NextAnimal]);
            break;
        }
        if (s == 3)
            anm_Animals[anm_NextAnimal].remove = 1;
        /* the expression written out each time, no local for it or for the record: VC6 then reloads unk20 and
           recomputes the record address inside the block, as the original does */
        if (d3_Time > ANM_AGE_TIME(anm_Animals[anm_NextAnimal])) {
            if (util_RandomF() > ANM_AGE_TIME(anm_Animals[anm_NextAnimal])
                                     / (d3_Time - ANM_AGE_TIME(anm_Animals[anm_NextAnimal])))
                anm_Animals[anm_NextAnimal].remove = 1;
        }
        if (anm_Animals[anm_NextAnimal].remove
            && ((*(unsigned char *)(*(char **)(*(char **)((char *)anm_Animals[anm_NextAnimal].obj + 0x34) + 0x1ec)
                                    + 0x96d) & 0x40)
                || s == 3 || anm_NAnimals > m))
            anm_RemoveAnimal(&anm_Animals[anm_NextAnimal]);
    }
    d3if_SelectScene(scene, 1);
    anm_NextAnimal = (anm_NextAnimal + 1) % 32;
}
/* 0x4bbb00, a free animal record (counted, time stamped), 0 if all 32 are taken */

ANMREC *anm_NewAnimal(void)
{
    int i;

    for (i = 0; i < 32; i++) {
        if (anm_Animals[i].obj == 0)
            break;
    }
    if (i == 32)
        return 0;
    anm_NAnimals++;
    anm_Animals[i].birth = d3_Time;
    return &anm_Animals[i];
}
/* 0x4bbb50, removes animal a (ch_Delete of its object) */

void anm_RemoveAnimal(ANMREC *a)
{
    ch_Delete(a->obj);
    memset(a, 0, sizeof(ANMREC));
    anm_NAnimals--;
}
/* 0x4bbb80, kind 2 (once, unk08 -1): picks one of the buildings of the 0x501190(1, 6) list with an object (0x61)
   and collects the positions of the others within 6000 of it (into a local list, which is dropped); unk08 = their
   count * 3
   @deep Q2 + Q4: the gm_BuildQuery/501030/502520/5024f0 callees are f3gm.c's gm_BuildQuery/BuildNext/IsHomeType/IsType10 (AIBLD* return; a void* declaration in anm.h is a new lint cross-TU conflict) and 3 raw *(T*) building accesses at +0x61 */

void anm_UpdateBird(ANMREC *a)
{
    char *b, *f;
    unsigned short n, k;
    int i;
    float p[4];
    char *e;
    float q[4];
    float pts[16][4];
    char *bs[256];

    if (a->nPoints != -1)
        return;
    n = 0;
    for (b = gm_BuildQuery(1, 6); b != 0; b = gm_BuildNext()) {
        if (!gm_IsHomeType(b) && !gm_IsType10(b) && *(int *)(b + 0x61) != 0)
            bs[n++] = b;
    }
    f = bs[util_RandomS(n)];
    e = *(char **)(f + 0x61);
    d3_PointToWorld(e, e + 0x4c, q);
    k = 0;
    for (i = 0; i < n && k < 0x10; i++) {
        b = bs[i];
        e = *(char **)(b + 0x61);
        d3_PointToWorld(e, e + 0x4c, p);
        if (b != f && d3_VecEqual(q, p, 6000.0f)) {
            pts[k++][0] = p[0];
            pts[k++][1] = p[1];
            pts[k++][2] = p[2];
        }
    }
    a->nPoints = k;
}
/* 0x4bbcd0, creates a cat (kind 1) at a free place (0x4bc2e0) @deep Q4: 3 raw *(T*) accesses on the animal object (scale 0x1a0, kind 0x04, 0x34->0x224); typed fields need ch.h in anm.c, blocked by rd.c's char* d3_DefaultScene view and the d3.h/ch.h/rd.h conflicts in the lead comment */

void anm_CreateCat(ANMREC *a)
{
    float pos[4];

    if (!rd_DoorPos(pos))
        return;
    a->obj = ch_Create("katze_KATZE", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.0f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 1;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
/* 0x4bbd60, creates a dog (kind 0) at a free place (0x4bc2e0) @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see the lead comment) */

void anm_CreateDog(ANMREC *a)
{
    float pos[4];

    if (!rd_DoorPos(pos))
        return;
    a->obj = ch_Create("hund_HUND", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.0f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 0;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
/* 0x4bbde0, creates a sheep (kind 4) at the dummy "dummy_SHEEP" @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see the lead comment) */

void anm_CreateSheep(ANMREC *a)
{
    char *o;
    float pos[4];

    o = rd_RandomNamedObject("dummy_SHEEP");
    if (o == 0)
        return;
    d3_PointToWorld(o, o + 0x4c, pos);
    a->obj = ch_Create("schaf_SCHAF", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.5f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 4;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
/* 0x4bbe70, creates a cow (kind 3) at the dummy "dummy_COW" @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see the lead comment) */

void anm_CreateCow(ANMREC *a)
{
    char *o;
    float pos[4];

    o = rd_RandomNamedObject("dummy_COW");
    if (o == 0)
        return;
    d3_PointToWorld(o, o + 0x4c, pos);
    a->obj = ch_Create("kuh_KUH", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.5f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 3;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
/* 0x4bbf00, creates an animal of kind k at the dummy "dummy_<name of k>" (cows, pigs and horses with their own
   models, the others with the dummy's name)
   @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see the lead comment) */

void anm_CreateAnimal(ANMREC *a, int k)
{
    float pos[4];
    char buf[0x80];
    char *o;

    sprintf(buf, "dummy_%s", anm_KindNames[k]);
    o = rd_RandomNamedObject(buf);
    if (o == 0)
        return;
    d3_PointToWorld(o, o + 0x4c, pos);
    if (k == 3)
        strcpy(buf, "kuh_KUH");
    else if (k == 7)
        strcpy(buf, "pferd_PFERD");
    else if (k == 6)
        strcpy(buf, "schwein_SCHWEIN");
    a->obj = ch_Create(buf, pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.0f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = k;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
/* 0x4bc030, dogs and cats (kinds 0 / 1; name from the string it puts into its actions): mostly stand (dogs also
   bark), else walk to 1..3 points near where they were created
   @deep Q4: 2 raw accesses (CHARACTER anim test at 0x128, d3_DefaultScene+0xb0 hmap); typed fields need ch.h/d3.h in anm.c, blocked by rd.c's char* d3_DefaultScene arithmetic (see the lead comment) */

void anm_UpdateCat(ANMREC *a)
{
    int px;
    char *h;
    int pz;
    float py;
    int n;
    float pts[8][4];
    float (*p)[4];
    char *act;

    if (*(int *)((char *)a->obj + 0x128) != 0)
        return;
    if (util_RandomS(100) > 0x1e) {
        if (a->kind == 0) {
            if (util_RandomS(100) > 0x32)
                ch_QueueAnim(a->obj, "bewegung/stehen", util_RandomS(3) + 1);
            else
                ch_QueueAnim(a->obj, "bewegung/bellen", util_RandomS(3) + 1);
        } else
            ch_QueueAnim(a->obj, "bewegung/stehen", util_RandomS(3) + 1);
        return;
    }
    h = *(char **)((char *)d3_DefaultScene + 0xb0);
    n = util_RandomS(3) + 1;
    n = rd_PointsNear(a->pos, n, pts);
    if (n > 0) {
        for (p = pts; n != 0; n--, p++) {
            if (d3_GetHeightAt(h, *p, &px, &pz, &py)) {
                act = ch_AddAction(a->obj, 0x2d, px, pz, 0);
                strcpy(act + 0x70, "anm_UpdateCat");
            }
        }
    }
}
/* 0x4bc1a0, the farm animals (kinds 3..7; name from the string it puts into its actions): mostly stand or graze,
   else walk to 1..3 points near where they were created
   @deep Q4: 2 raw accesses (CHARACTER anim test at 0x128, d3_DefaultScene+0xb0 hmap); typed fields need ch.h/d3.h in anm.c, blocked by rd.c's char* d3_DefaultScene arithmetic (see the lead comment) */

void anm_UpdateSheep(ANMREC *a)
{
    int px;
    char *h;
    int pz;
    float py;
    int n;
    float pts[8][4];
    float (*p)[4];
    char *act;

    if (*(int *)((char *)a->obj + 0x128) != 0)
        return;
    if (util_RandomS(100) > 0x14) {
        if (util_RandomS(100) > 0x32)
            ch_QueueAnim(a->obj, "bewegung/stehen", util_RandomS(3) + 1);
        else
            ch_QueueAnim(a->obj, "bewegung/fressen", util_RandomS(3) + 1);
        return;
    }
    h = *(char **)((char *)d3_DefaultScene + 0xb0);
    n = util_RandomS(3) + 1;
    n = rd_PointsNear(a->pos, n, pts);
    if (n > 0) {
        for (p = pts; n != 0; n--, p++) {
            if (d3_GetHeightAt(h, *p, &px, &pz, &py)) {
                act = ch_AddAction(a->obj, 0x2d, px, pz, 0);
                strcpy(act + 0x70, "anm_UpdateSheep");
            }
        }
    }
}

extern char ai_Time[14];
char anm_KindNames[8][0x20] = { "DOG", "CAT", "BIRD", "COW", "SHEEP", "CHICK", "PIG", "HORSE" };  /* 0x602688, "DOG", "CAT", "BIRD", "COW", "SHEEP", "CHICK", "PIG", "HORSE" */
int anm_NAnimals = 0;  /* 0x6f0734, the number of animals */
ANMREC * anm_Animals = 0;  /* 0x6f0738, the animal list (32 records, "anm:animals") */
int anm_NextAnimal = 0;  /* 0x6f073c, the animal updated next */
unsigned int anm_LastSpawn = 0;  /* 0x6f0740, d3_Time of the last spawn try */
char ai_Time[14] = {0}; /* 0x770ae0, AITIME (ai.h): 14 bytes (int, short, int, int); a 3-byte placeholder
                           here put anm_NAnimals / anm_Animals right behind it in .bss, so cm.c's message 3
                           (ai_Time[6] / [10]) overwrote the animal list pointer */
