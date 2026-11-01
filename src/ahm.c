/* ahm.c -- the alchemist building and the recipes (name from the tags "ahm_AllocRecipePrototypes" / "ahm_AllocPlayerPrototypes": the data block 0x60f1b0 with the
table of 8 ints, then the strings "RCPID" .. "alchemy_data.ini"). Starts at 0x4eaa50 (the first function of the building kind 0x1e); the end 0x4ed170
   rests on the .data order only (the next block, the avatar table 0x60f310, is gv.c's). Names are ours.
   Open: ahm.h is not canonical - its mem_Alloc `const char *tag` conflicts with gv.h's `char *tag`, so he.c / ls.c / cm.h
   and others still declare ahm.c's functions locally (some K&R).
   unit of rea 8cef7d5b (src/game/ahm.c); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ahm.h"
#include "f3gm.h"
#include "net.h"
#include "vfs.h"
#define ai_TimeK (*(AHMCLOCK *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as ahm.c copies it: @original 3 ints and a short (0x4eb43a lea edx,[eax+0x52]; stores 4/4/4/2) */
#define cm_SendExSetHEI ((void (*)(int, char *))cm_SendExSetHE)  /* 0x4c4c60 (cm.h: unsigned char k): @original ahm.c pushes the key as an int (0x4eb43e push 0xfffffffe) */

#define vfs_freadU ((unsigned (*)(void *, int, int, VFSFILE *))vfs_fread)  /* 0x430bd0, vfs.h int: @original this unit compares the count as unsigned (ahm_LoadDfn 0x4ebabd test eax,eax; jbe for `<= 0`) */

unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: the function masks its argument to 16 bits (0x42ec00: test si,si; and esi,0xffff) and the shuffler passes the count through mov cx,WORD PTR ds:0x6f0e28 (0x4eadd8), so this TU's callers were compiled against the 16-bit parameter and result (ai.h's view is int) */

/* 0x4eaa50, the character the alchemist object p (building kind 0x1e) belongs to: the character with the id of the record at +0x65; 0 when p is another kind or has no record */
char *ahm_OwnerChar(unsigned char *p)
{
    AHMOBJ *o = (AHMOBJ *)p;

    if (ai_BTypes[o->kind * 0x24d] == 0x1e && o->rec != 0)
        return (char *)gm_FindChar(o->rec->id);
    return 0;
}
/* 0x4eaa90, the number of the places in use (the six AHMSLOT at +0xc of the record) of the object of the character c (0x4eaad0), 0 when it has none */

int ahm_UsedSlots(char *c)
{
    int n, i;
    AHMSLOT *p;

    if (ahm_HasRecord(c) == 0)
        return 0;
    n = 0;
    p = ((AHMCHAR *)c)->obj->rec->slots;
    for (i = 6; i != 0; i--) {
        if (p->id != 0)
            n++;
        p++;
    }
    return n;
}
/* 0x4eaad0, 1 when the character c (flag 4 of +0x1cc) has an object (+0x16c) of the building kind 0x1e with a record at +0x65 */

int ahm_HasRecord(char *c)
{
    AHMCHAR *ch = (AHMCHAR *)c;

    if ((c[0x1cc] & 4) != 0 && ch->obj != 0) {
        if (ai_BTypes[ch->obj->kind * 0x24d] == 0x1e)
            return ch->obj->rec != 0;
        return 0;
    }
    return 0;
}
/* 0x4eab20, the share (byte / 252) number i (0 .. 3) of the record of the character c's object (+8 .. +0xb); -1.0 when it has none */

float ahm_Share(char *c, int i)
{
    if (ahm_HasRecord(c) != 0) {
        if (i == 0)
            return (float)((AHMCHAR *)c)->obj->rec->share[0] * 0.003968254f;
        if (i == 1)
            return (float)((AHMCHAR *)c)->obj->rec->share[1] * 0.003968254f;
        if (i == 2)
            return (float)((AHMCHAR *)c)->obj->rec->share[2] * 0.003968254f;
        if (i == 3)
            return (float)((AHMCHAR *)c)->obj->rec->share[3] * 0.003968254f;
    }
    return -1.0f;
}
/* 0x4eabd0, the place of c's record with the id (a forwarding function to 0x4eabe0) */

AHMSLOT *ahm_SlotOf(char *c, int id)
{
    return ahm_FindSlot(c, id);
}
/* 0x4eabe0, the place of the record of the character c with the good id; 0 when there is none */

AHMSLOT *ahm_FindSlot(char *c, int id)
{
    int i;

    if (ahm_HasRecord(c) != 0) {
        for (i = 0; i < 6; i++) {
            if (((AHMCHAR *)c)->obj->rec->slots[i].id == id)
                return ((AHMCHAR *)c)->obj->rec->slots + i;
        }
    }
    return 0;
}
/* 0x4eac20, 1 when the number of the places in use (ahm_UsedSlots) is below the limit (the share 2 times the six places) */

int ahm_HasFreeSlot(char *c)
{
    int n;

    if (ahm_HasRecord(c) == 0)
        return 0;
    n = ahm_UsedSlots(c);
    return (int)(ahm_Share(c, 2) * 6.0f) > n;
}
/* 0x4eac60, fills the three steps ai_WalkSteps from the shuffled table ai_PlayerPrototypes (the entries in turn) with the running positions (x, y) on a 25 x 25 grid;
   when the table has been walked through once it is shuffled again (ahm_ShufflePlayers)
   @deep B3 draft: 54 code hunks, all register scheduling - the table pointer ai_PlayerPrototypes is hoisted into ebp before the do-loop with the entry fields read via [ebp+eax*4+N]; the original reloads [0x6f0e2c] each iteration and reads the fields through the entry pointer in eax; our s++ is scheduled at the loop top ([ecx-0x1c] stores) while the original does lea edx,[ecx+4] then add ecx,0x18. Every statement, constant, field offset, the 3-step bound and the trailing ahm_ShufflePlayers(1) match. Last-bytes leads tried: loop end as ai_WalkSteps+3 (54 -> 72 code hunks, WORSE, reverted), as (int)&ai_PlayerPrototypeCount (same 54, kept for Q3), the trailing compare from the index side ai_WalkIndex >= count (62 -> 54, kept), the menu reload lead (drop the g local, re-read ai_PlayerPrototypes[i] at the use, same 54), the dup-tail lead (branch polarity, kept). Q1..Q7 pass. Left: allocation/scheduling of the table and step pointers. */

void ahm_FillSteps(void)
{
    AHMSTEP *s;
    int i;

    if (ai_PlayerPrototypeCount != 0) {
        s = ai_WalkSteps;
        do {
            i = ai_WalkIndex % ai_PlayerPrototypeCount;
            ai_WalkIndex++;
            s->grid.w[0] = ai_PlayerPrototypes[i].w[0];
            s->grid.w[1] = ai_PlayerPrototypes[i].w[1];
            s->grid.w[2] = ai_PlayerPrototypes[i].w[2];
            s->y = ai_WalkY;
            s->x = ai_WalkX;
            s->on = 1;
            ai_WalkY = (ai_WalkDY + ai_WalkY) % 0x19;
            ai_WalkX = (ai_WalkDX + ai_WalkX) % 0x19;
            s++;
        } while ((int)s < (int)((unsigned)ai_WalkSteps + 0x48));
        if (ai_WalkIndex >= ai_PlayerPrototypeCount)
            ahm_ShufflePlayers(1);
    }
}
/* 0x4ead20, seeds the generator (init: from the time) and shuffles the table; with init the walk is restarted: start positions random on the grid, the steps taken
   from the table 0x60f1b0 */

void ahm_ShufflePlayers(int init)
{
    unsigned int t;

    if (init != 0)
        ai_WalkSeed.seed16 = (unsigned short)(timeGetTime() % 0x7fff) + 1;
    srand(ai_WalkSeed.seed & 0xffff);
    ahm_ShuffleTable();
    if (init != 0) {
        ai_WalkX = util_Random(0x19) & 0xffff;
        ai_WalkDX = ahm_Tab[util_Random(8) & 0xffff];
        ai_WalkY = util_Random(0x19) & 0xffff;
        t = util_Random(8);
        ai_WalkIndex = 0;
        ai_WalkDY = ahm_Tab[t];
    }
}
/* 0x4eadc0, shuffles the player prototype table (2 * count random swaps of two entries) */

void ahm_ShuffleTable(void)
{
    AHMGRID t;
    int n, i, j;

    for (n = ai_PlayerPrototypeCount * 2; n > 0; n--) {
        i = util_Random(ai_PlayerPrototypeCount);
        j = util_Random(ai_PlayerPrototypeCount);
        if (i != j) {
            t = ai_PlayerPrototypes[i];
            ai_PlayerPrototypes[i] = ai_PlayerPrototypes[j];
            ai_PlayerPrototypes[j] = t;
        }
    }
}
/* 0x4eae60, the entry of the recipe prototype table (ai_RecipePrototypes, count ai_RecipePrototypeCount) with the id; 0 when there is none */

AHMGRID *ahm_FindRecipe(int id)
{
    int i;
    AHMGRID *p;

    i = 0;
    p = ai_RecipePrototypes;
    if (ai_RecipePrototypeCount > 0) {
        do {
            if (p->rcp.id == id)
                return ai_RecipePrototypes + i;
            i++;
            p++;
        } while (i < ai_RecipePrototypeCount);
    }
    return 0;
}
/* 0x4eaea0, a random entry of the recipe prototype table the character c has no place for yet (a random start and a step that is not a multiple of the count); 0 when
   c may not take more (ahm_HasFreeSlot) */

AHMGRID *ahm_RandomRecipe(char *c)
{
    unsigned int step, pos, k, r;

    if (ahm_HasFreeSlot(c) != 0) {
        step = (util_Random(ai_RecipePrototypeCount & 0xffff) + 1);
        r = (int)step % ai_RecipePrototypeCount;
        while (r == 0) {
            step++;
            r = (int)step % ai_RecipePrototypeCount;
        }
        pos = util_Random(ai_RecipePrototypeCount) & 0xffff;
        k = ai_RecipePrototypeCount;
        while (k != 0) {
            if (ahm_SlotOf(c, ai_RecipePrototypes[pos].rcp.id) == 0)
                return &ai_RecipePrototypes[pos];
            k--;
            pos = (int)(pos + step) % ai_RecipePrototypeCount;
        }
    }
    return 0;
}
/* 0x4eaf50, the research time of the recipe g for the character c, in tenths of minutes (0x3840 = the 1440 minutes of a day times 10): one to two days by the
   recipe's ATTR0 (the float at +4, 0 .. 1: 14400 * (2 - ATTR0)), reduced by up to half by the share 0 (the alchemist's skill) */

int ahm_ResearchTime(char *c, AHMGRID *g)
{
    int a, b;

    a = (int)((1.0f - g->rcp.a) * -14400.0f);
    b = 0x3840 - a;
    return b - (int)(ahm_Share(c, 0) * b * 0.5f);
}
/* 0x4eafa0, the research time in minutes (ahm_ResearchTime / 10): the amount the research event 0x95 (0x56d2d0) counts down by the elapsed minutes (0x4fe280) */

int ahm_ResearchMinutes(char *c, AHMGRID *g)
{
    return (int)(ahm_ResearchTime(c, g) * 0.1f);
}
/* 0x4eafd0, the progress the character c makes researching the recipe g in *cap minutes (*cap gets the minutes used): ahm_ResearchTime * (1 - share 2 / 2) * (1 - the
   value of the character's place of g) is the time t the rest of the place needs; min(*cap, t) are used and the progress they are worth ((used / t) * (1 - value)) is
   returned; 0.0 when the place is full / not there / nothing is left */

float ahm_ResearchProgress(char *c, AHMGRID *g, int *cap)
{
    float s2;
    AHMSLOT *slot;
    int n, b, t, m;

    s2 = ahm_Share(c, 2);
    slot = ahm_SlotOf(c, g->rcp.id);
    if (slot == 0)
        return 0.0f;
    if (slot->v >= 1.0f)
        return 0.0f;
    n = *cap;
    if (n <= 0)
        return 0.0f;
    b = ahm_ResearchTime(c, g);
    t = (int)(b * (1.0f - s2 * 0.5f) * (1.0f - slot->v));
    if (t <= 0)
        return 0.0f;
    m = n < t ? n : t;
    *cap = m;
    return (float)m / t * (1.0f - slot->v);
}
/* 0x4eb0b0, the progress the character c makes training the share i in *cap minutes (*cap gets the minutes used): 14400 * (1 - share) is the time the rest of the
   share needs; returns (used / time) * (1 - share), 0.0 when the share is not in [0, 1) */

float ahm_ShareProgress(char *c, int i, int *cap)
{
    float f;
    int t, m;

    f = ahm_Share(c, i);
    if (f < 0.0f || f >= 1.0f)
        return 0.0f;
    t = (int)(14400.0f * (1.0f - f));
    m = *cap < t ? *cap : t;
    *cap = m;
    return (float)m / t * (1.0f - f);
}
/* 0x4eb130, the factor of the place with the good id of the character c: 1.0 + share 1 * the value of the place; 1.0 when it has no place */

float ahm_PlaceFactor(char *c, int id)
{
    AHMSLOT *slot;

    slot = ahm_FindSlot(c, id);
    if (ahm_HasRecord(c) == 0)
        return 1.0f;
    if (slot == 0)
        return 1.0f;
    return ahm_Share(c, 1) * slot->v + 1.0f;
}
/* 0x4eb180, the research state of the object o: the first research event (kinds 4, 0x95 .. 0x97) of the event list 0x53e400 (1, 3, *(o + 1)) decides: 4 (only with
   flag: *a = 0, *f = 0.0), 1 (the share of the research done in *f, 0 .. 1), 2 / 3 (the same, with the recipe / share id (+0xb0) in *a); 0 when there is none.
   The clamp is max(min(share, 1), 0) written out (each operand evaluated twice, as a MIN / MAX macro pair expands); the share lives in a stack slot,
   case 0x95 in its own variable (the original keeps it in the dead `a` argument slot, 0x96 / 0x97 in the `o` slot). */

int ahm_ResearchState(char *o, int *a, float *f, int flag)
{
    AHMRESEARCH *ev;
    float s, v;

    ev = (AHMRESEARCH *)he_FindEvent(1, 3, ((AHMOBJ *)o)->id);
    while (ev != 0) {
        switch (ev->kind) {
        default:
            ev = (AHMRESEARCH *)he_NextEvent();
            break;
        case 0x95:
            s = ((float)ev->total - (float)ev->amount) / (float)ev->total;
            *f = s;
            *f = (1.0f < s ? 1.0f : s) > 0.0f ? (1.0f < s ? 1.0f : s) : 0.0f;
            return 1;
        case 0x96:
            v = ((float)ev->start - (float)ev->total) / (float)ev->start;
            *f = v;
            *f = (1.0f < v ? 1.0f : v) > 0.0f ? (1.0f < v ? 1.0f : v) : 0.0f;
            *a = ev->id;
            return 2;
        case 0x97:
            v = ((float)ev->start - (float)ev->total) / (float)ev->start;
            *f = v;
            *f = (1.0f < v ? 1.0f : v) > 0.0f ? (1.0f < v ? 1.0f : v) : 0.0f;
            *a = ev->id;
            return 3;
        case 4:
            if (flag == 0)
                return 0;
            *f = 0.0f;
            *a = 0;
            return 4;
        }
    }
    return 0;
}
/* 0x4eb410, restarts the research events (kinds 0x95 .. 0x97) of the object o: the due clock (+0x52) is set to the game clock ai_Time and the event is told
   (0x4c4c60, -2) */

void ahm_RescheduleResearch(char *o)
{
    AHMRESEARCH *ev;

    ev = (AHMRESEARCH *)he_FindEvent(1, 3, ((AHMOBJ *)o)->id);
    while (ev != 0) {
        if (0x95 <= ev->kind && ev->kind <= 0x97) {
            ev->due = ai_TimeK;
            cm_SendExSetHEI(-2, (char *)ev);
        }
        ev = (AHMRESEARCH *)he_NextEvent();
    }
}
/* 0x4eb480, loads the prototype tables at start: with mode 0 from "/gamedata/ai/alchemy.dfn" (ahm_LoadDfn), else from "alchemy_data.ini" and then written back
   to the .dfn (ahm_SaveDfn); every load restarts the walk (ahm_ShufflePlayers); 0 when the ini load failed, else 1 */

int ahm_LoadPrototypes(int mode)
{
    if (mode != 0) {
        if (ahm_LoadIni() != 0) {
            ahm_ShufflePlayers(1);
            return ahm_SaveDfn();
        }
        return 0;
    }
    if (ahm_LoadDfn() != 0)
        ahm_ShufflePlayers(1);
    return 1;
}

/* unlinked; the mask zeroes 0 and below, the cap is 0xfc (the original's cmp eax,0xfc / jl) */
static __inline int ahm_ClampAttr(char *s)
{
    int v;

    v = atoi(s);
    v = v & ((v <= 0) - 1);
    if (v >= 0xfc)
        v = 0xfc;
    return v;
}

/* unlinked; the original evaluates atof(s) again at every use (the min/max are macros) and does it in double (the constants are QWORDs,
   only the store to the float table field narrows) */
#define ahm_min(a, b) (((a) < (b)) ? (a) : (b))
#define ahm_max(a, b) (((a) > (b)) ? (a) : (b))
static __inline double ahm_ClampShare(char *s)
{
    return ahm_min(ahm_max(atof(s) * 0.01, 0.0), 1.0);
}

/* 0x4eb4c0, reads the player and recipe prototypes from "alchemy_data.ini" (next to the exe): [MAIN_PLAYER_SPECS] NUM_OF_PLAYER_PROTOTYPES, then [PLR_%li] ATTR0 .. ATTR3
   (0 .. 252, bytes 0 .. 3), ATTR4 (18 .. 30, byte 4), ATTR5 (an object name, 0x4ebf50, int +8) into the table 0x6f0e2c (12 bytes each); [MAIN_RECIPES_SPECS]
   NUM_OF_RECIPES_PROTOTYPES, then [RCP_%li] ATTR0 / ATTR1 (percent, 0 .. 1 floats +4 / +8) and RCPID (an object name, int +0) into 0x6f0e34; 1 when all was read,
   else the tables are freed and 0. The clamps: ahm_ClampAttr keeps 1 .. 0xfb and caps at 0xfc, ATTR4 is 0x12 .. 0x1e; ahm_ClampShare is min(max(atof / 100, 0), 1)
   in double, re-evaluating atof (0x4eb8d7 .. is four atof calls per check).
   @deep B3 draft: two hunks left, both the cleanup (fail) block's place: ours sits at the end of the function, the original's right after the
   recipe-alloc check / before the recipe loop (0x4eb83b, with its ATTR4 tail at 0x4eb883), so the ATTR4 0x12 tail and the two dup-tails differ
   in position only; the block itself is the same 18 instructions. Leads tried: the ATTR4 clamp as <= 0x12 / >= 0x1e and as the swapped
   if/else (0x4eb75e, gained, kept), the goto fail vs fallthrough shapes (source order puts the label last), the union views (AHMGRID
   plr/rcp/w) that removed the frame temp and made the stores direct. Q1..Q7 pass. */
int ahm_LoadIni(void)
{
    char ini[260], buf[256], sec[256], *p;
    int i;

    GetModuleFileNameA((HMODULE)main_Instance, ini, 0x104);
    p = strrchr(ini, '\\');
    p[1] = 0;
    strcat(ini, "alchemy_data.ini");
    GetPrivateProfileStringA("MAIN_PLAYER_SPECS", "NUM_OF_PLAYER_PROTOTYPES", "", buf, 0x100, ini);
    ai_PlayerPrototypeCount = atoi(buf);
    if (ai_PlayerPrototypeCount > 0) {
        if (ai_PlayerPrototypes != 0) {
            mem_Free(ai_PlayerPrototypes);
            ai_PlayerPrototypes = 0;
        }
        ai_PlayerPrototypes = mem_Alloc(ai_PlayerPrototypeCount * 12, "ahm_AllocPlayerPrototypes");
        if (ai_PlayerPrototypes == 0)
            goto fail;
        for (i = 0; i < ai_PlayerPrototypeCount; i++) {
            sprintf(sec, "PLR_%li", i);
            GetPrivateProfileStringA(sec, "ATTR0", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[0] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR1", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[1] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR2", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[2] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR3", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[3] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR4", "20", buf, 0x100, ini);
            p = (char *)(long)atoi(buf);
            if ((int)p > 0x12) {
                if ((int)p >= 0x1e)
                    p = (char *)0x1e;
            } else
                p = (char *)0x12;
            ai_PlayerPrototypes[i].plr.attr[4] = (unsigned char)(int)p;
            GetPrivateProfileStringA(sec, "ATTR5", "ob_NULL", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.id = ahm_ObjectId(buf);
        }
        GetPrivateProfileStringA("MAIN_RECIPES_SPECS", "NUM_OF_RECIPES_PROTOTYPES", "", buf, 0x100, ini);
        ai_RecipePrototypeCount = atoi(buf);
        if (ai_RecipePrototypeCount > 0) {
            if (ai_RecipePrototypes != 0) {
                mem_Free(ai_RecipePrototypes);
                ai_RecipePrototypes = 0;
            }
            ai_RecipePrototypes = mem_Alloc(ai_RecipePrototypeCount * 12, "ahm_AllocRecipePrototypes");
            if (ai_RecipePrototypes != 0) {
                for (i = 0; i < ai_RecipePrototypeCount; i++) {
                    sprintf(sec, "RCP_%li", i);
                    GetPrivateProfileStringA(sec, "ATTR0", "25", buf, 0x100, ini);
                    ai_RecipePrototypes[i].rcp.a = ahm_ClampShare(buf);
                    GetPrivateProfileStringA(sec, "ATTR1", "50", buf, 0x100, ini);
                    ai_RecipePrototypes[i].rcp.b = ahm_ClampShare(buf);
                    GetPrivateProfileStringA(sec, "RCPID", "ob_NULL", buf, 0x100, ini);
                    ai_RecipePrototypes[i].rcp.id = ahm_ObjectId(buf);
                }
                return 1;
            }
        }
    }
    if (ai_PlayerPrototypes != 0) {
        mem_Free(ai_PlayerPrototypes);
        ai_PlayerPrototypes = 0;
    }
fail:
    if (ai_RecipePrototypes != 0) {
        mem_Free(ai_RecipePrototypes);
        ai_RecipePrototypes = 0;
    }
    ai_RecipePrototypeCount = 0;
    ai_PlayerPrototypeCount = 0;
    return 0;
}
/* 0x4eba80, reads the prototypes from "/gamedata/ai/alchemy.dfn" (the magic 0x61486d52, the counts 1 .. 100, the player entries (5 bytes + the id) and the recipe
   entries (3 ints)); 1 when all was read, else the tables are freed and 0 */

int ahm_LoadDfn(void)
{
    void *f;
    int magic, i;

    f = vfs_Tfopen("/gamedata/ai/alchemy.dfn", "rb", (int)vfs_Root);
    if (f != 0) {
        if (0 < vfs_freadU(&magic, 4, 1, f) && magic == 0x61486d52 && 0 < vfs_freadU(&ai_PlayerPrototypeCount, 4, 1, f) &&
            ai_PlayerPrototypeCount >= 1 && ai_PlayerPrototypeCount <= 0x64) {
            if (ai_PlayerPrototypes != 0) {
                mem_Free(ai_PlayerPrototypes);
                ai_PlayerPrototypes = 0;
            }
            ai_PlayerPrototypes = mem_Alloc(ai_PlayerPrototypeCount * 12, "ahm_AllocPlayerPrototypes");
            if (ai_PlayerPrototypes != 0) {
                for (i = 0; i < ai_PlayerPrototypeCount; i++) {
                    if (vfs_freadU(&ai_PlayerPrototypes[i].plr.attr[0], 1, 1, f) <= 0 ||
                        vfs_freadU(&ai_PlayerPrototypes[i].plr.attr[1], 1, 1, f) <= 0 ||
                        vfs_freadU(&ai_PlayerPrototypes[i].plr.attr[2], 1, 1, f) <= 0 ||
                        vfs_freadU(&ai_PlayerPrototypes[i].plr.attr[3], 1, 1, f) <= 0 ||
                        vfs_freadU(&ai_PlayerPrototypes[i].plr.attr[4], 1, 1, f) <= 0 ||
                        vfs_freadU(&ai_PlayerPrototypes[i].plr.id, 4, 1, f) <= 0)
                        goto bad;
                }
                if (0 < vfs_freadU(&ai_RecipePrototypeCount, 4, 1, f) && ai_RecipePrototypeCount >= 1 && ai_RecipePrototypeCount <= 0x64) {
                    if (ai_RecipePrototypes != 0) {
                        mem_Free(ai_RecipePrototypes);
                        ai_RecipePrototypes = 0;
                    }
                    ai_RecipePrototypes = mem_Alloc(ai_RecipePrototypeCount * 12, "ahm_AllocRecipePrototypes");
                    if (ai_RecipePrototypes != 0) {
                        for (i = 0; i < ai_RecipePrototypeCount; i++) {
                            if (vfs_freadU(&ai_RecipePrototypes[i].rcp.id, 4, 1, f) <= 0 ||
                                vfs_freadU(&ai_RecipePrototypes[i].rcp.a, 4, 1, f) <= 0 ||
                                vfs_freadU(&ai_RecipePrototypes[i].rcp.b, 4, 1, f) <= 0)
                                goto bad;
                        }
                        vfs_fclose(f);
                        return 1;
                    }
                }
            }
        }
bad:
        vfs_fclose(f);
    }
    if (ai_PlayerPrototypes != 0) {
        mem_Free(ai_PlayerPrototypes);
        ai_PlayerPrototypes = 0;
    }
    if (ai_RecipePrototypes != 0) {
        mem_Free(ai_RecipePrototypes);
        ai_RecipePrototypes = 0;
    }
    ai_RecipePrototypeCount = 0;
    ai_PlayerPrototypeCount = 0;
    return 0;
}
/* 0x4ebd60, writes the prototypes to "/gamedata/ai/alchemy.dfn" in the format 0x4eba80 reads; 1 when all was written */

int ahm_SaveDfn(void)
{
    void *f;
    int magic, i;

    magic = 0x61486d52;
    f = vfs_Tfopen("/gamedata/ai/alchemy.dfn", "wb", (int)vfs_Root);
    if (f != 0) {
        if (vfs_fwrite(&magic, 4, 1, f) > 0 && vfs_fwrite(&ai_PlayerPrototypeCount, 4, 1, f) > 0) {
            for (i = 0; i < ai_PlayerPrototypeCount; i++) {
                if (vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[0], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[1], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[2], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[3], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[4], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.id, 4, 1, f) <= 0)
                    goto bad;
            }
            if (vfs_fwrite(&ai_RecipePrototypeCount, 4, 1, f) > 0) {
                for (i = 0; i < ai_RecipePrototypeCount; i++) {
                    if (vfs_fwrite(&ai_RecipePrototypes[i].rcp.id, 4, 1, f) <= 0 ||
                        vfs_fwrite(&ai_RecipePrototypes[i].rcp.a, 4, 1, f) <= 0 ||
                        vfs_fwrite(&ai_RecipePrototypes[i].rcp.b, 4, 1, f) <= 0)
                        goto bad;
                }
                vfs_fclose(f);
                return 1;
            }
        }
bad:
        vfs_fclose(f);
    }
    return 0;
}
/* 0x4ebf50, the object id of the name without its "ob_" prefix (0x4fe900; the tail call) */

int ahm_ObjectId(char *name)
{
    return gm_FindObjectProto(name + 3);
}
/* 0x4ebf60, frees the prototype tables and clears the counts */

void ahm_FreePrototypes(void)
{
    if (ai_PlayerPrototypes != 0) {
        mem_Free(ai_PlayerPrototypes);
        ai_PlayerPrototypes = 0;
    }
    if (ai_RecipePrototypes != 0) {
        mem_Free(ai_RecipePrototypes);
        ai_RecipePrototypes = 0;
    }
    ai_RecipePrototypeCount = 0;
    ai_PlayerPrototypeCount = 0;
}
/* 0x4ebfb0, writes the walk state to the save file f (the save game 0x5190b0 in ls.c calls it): the five ints 0x6f0dc8 .. 0x6f0dd8, the seed word 0x6f0ddc
   (2 bytes), then the three steps (flag, x, y, the player-prototype view of the grid entry: five attribute bytes + its id); 1 when all was written, 0 when a
   write failed.
   Matching notes: the unsigned `<= 0` checks (test eax,eax; jbe + an inline `pop edi; pop esi; ret`) are what the original's early-return epilogues need;
   the loop is the direct index `ai_WalkSteps[i]`, which is what puts VC6's induction variable at the step + 0x10 (`mov edi,0x6f0df0`, `cmp edi,0x6f0e38`) -
   every pointer spelling (char* byte offsets, AHMSTEP *p with a pointer or an index loop) re-centred it at +0x14, so the index is the original's source
   shape (notes/vc6-matching.md, the struct-array loop lesson). Q1..Q7 pass. */

int ahm_SaveWalk(void *f)
{
    int i;

    if (vfs_fwrite(&ai_WalkX, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkDX, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkY, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkDY, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkIndex, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkSeed, 2, 1, f) <= 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (vfs_fwrite(&ai_WalkSteps[i].on, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].x, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].y, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[0], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[1], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[2], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[3], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[4], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.id, 4, 1, f) <= 0)
            return 0;
    }
    return 1;
}
/* 0x4ec140, reads the walk state written by ahm_SaveWalk (0x4ebfb0, the load game 0x51c950 in ls.c calls it from version 0x10049) and reshuffles the table
   without restarting the walk (ahm_ShufflePlayers(0)); 1 when all was read, 0 when a read failed */


int ahm_LoadWalk(void *f)
{
    int i;

    if (ls_Read(&ai_WalkX, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkDX, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkY, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkDY, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkIndex, 4, 1, f) == 0)
        return 0;
    if (ls_Read(&ai_WalkSeed, 2, 1, f) == 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (ls_Read(&ai_WalkSteps[i].on, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].x, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].y, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[0], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[1], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[2], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[3], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.attr[4], 1, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_WalkSteps[i].grid.plr.id, 4, 1, f) == 0)
            return 0;
    }
    ahm_ShufflePlayers(0);
    return 1;
}
/* 0x4ec2d0, the step number i (0 .. 2) of 0x6f0de0; 0 for others */


AHMSTEP *ahm_WalkStep(int i)
{
    if (i >= 0 && i < 3)
        return &ai_WalkSteps[i];
    return 0;
}
/* 0x4ec2f0, clears the step p (its flag) */

void ahm_ClearStep(AHMSTEP *p)
{
    int i;

    i = 0;
    do {
        if (&ai_WalkSteps[i] == p) {
            ai_WalkSteps[i].on = 0;
            return;
        }
        i++;
    } while (i < 3);
}
/* 0x4ec320, the value of the walk step p (as ahm_LoadIni stored them), used by the tutorial results window (0x57bf00 passes it to the text builder
   0x5176d0): the four attribute bytes (each / 252 = 0x5ecee0) squared and added, plus the two values of the recipe entry of the step's id (ahm_FindRecipe)
   when there is one, both times 500 (0x5ed098), as an int shifted by 5. The two sums are added, not chosen (the 0.0 at 0x5eb788 is the FPU value the
   branch does or does not replace).
   Find (B3 draft): same statements as the original, but VC6 schedules the four squares differently - the original squares each attribute right after loading it
   and folds it into the sum (`fld st(0); fmul st,st(1); faddp st(2),st; fstp st(0)`, sum in an FPU register at st(2)), ours keeps the four scaled values on
   the FPU and squares them at the end (`fld st(3); fmul st,st(4)` ...), 88 of 176 bytes. Spelling leads tried, all giving the deferred form: the sum as one
   expression, as four `sum = sum + x * x` statements interleaved with the loads, and parenthesised left-associative. The rest of the body (the call, the
   test, the v branch, the __ftol) matches. Q1..Q7 pass.
   @deep behaviour draft (B3): 88 of 176 bytes, all from the FPU scheduling of the four squares - the original squares each attribute right after loading it and folds it into the sum in an FPU register (fld st(0); fmul st,st(1); faddp st(2),st; fstp st(0)), ours keeps the four scaled values on the FPU and squares them at the end (fld st(3); fmul st,st(4)), plus the temp slot ([esp+8] vs the original's [esp+4]) and the pop esi position that follow from it. Read: disasm 0x4ec320..0x4ec3cf, the caller 0x57bf00 (its result goes to the text builder 0x5176d0) and the constants 0x5eb788 0.0 / 0x5ecee0 1/252 / 0x5ed098 500.0. Meaning fixed in this session: the recipe values are a second sum (v) added to the squared attributes, not a replacement (the 0.0 branch). Spelling leads tried, one build each, all giving the deferred form: one sum expression, four sum = sum + x*x statements interleaved with the loads, parenthesised left-associative. Q1..Q7 pass (typed AHMSTEP.grid.plr view, named after its role). */

int ahm_StepValue(char *p)
{
    AHMSTEP *s = (AHMSTEP *)p;
    float v, sum, a, b, c, d;
    AHMGRID *g;

    v = 0.0f;
    g = ahm_FindRecipe(s->grid.plr.id);
    a = (float)s->grid.plr.attr[0] * 0.003968254f;
    b = (float)s->grid.plr.attr[1] * 0.003968254f;
    c = (float)s->grid.plr.attr[2] * 0.003968254f;
    d = (float)s->grid.plr.attr[3] * 0.003968254f;
    sum = ((a * a + b * b) + c * c) + d * d;
    if (g != 0)
        v = g->rcp.b + g->rcp.a;
    return (int)(v * 500.0f + sum * 500.0f) << 5;
}
/* 0x4ec3d0, the value of the alchemist object p (the same formula as ahm_StepValue, for a whole building): the four shares of its record squared and added,
   plus the recipe value of each of its six places (ahm_FindRecipe) added to the place's value, both sums times 500 (0x5ed098), as an int shifted by 5. Used
   by the alchemist's windows (0x57cf90, 0x57de60) as the value they build their texts from (0x5176d0). -1 when p has no owner character (ahm_OwnerChar).
   Matching notes: the two products are separate statements with the recipe sum first (`v = sum2 * 500.0f; v = v + sum * 500.0f;`) - one expression made
   VC6 factor them into (sum + sum2) * 500 and left an extra fxch; sum2 = 0.0f is assigned before the ahm_OwnerChar call, as the original clears the slot
   before the call. */

int ahm_ObjectValue(unsigned char *p)
{
    AHMOBJ *o = (AHMOBJ *)p;
    AHMREC *rec;
    char *c;
    float s, sum, sum2, v;
    AHMGRID *g;
    int i;

    sum2 = 0.0f;
    c = ahm_OwnerChar(p);
    if (c == 0)
        return -1;
    rec = o->rec;
    s = ahm_Share(c, 0);
    sum = s * s;
    s = ahm_Share(c, 1);
    sum = s * s + sum;
    s = ahm_Share(c, 2);
    sum = s * s + sum;
    s = ahm_Share(c, 3);
    sum = s * s + sum;
    for (i = 0; i < 6; i++) {
        g = ahm_FindRecipe(rec->slots[i].id);
        if (g != 0)
            sum2 = g->rcp.a + rec->slots[i].v + sum2;
    }
    v = sum2 * 500.0f;
    v = v + sum * 500.0f;
    return (int)v << 5;
}
/* 0x4ec490, posts the type 99 message (0x4c6170) that makes the alchemist building o (kind 0x1e) create its owner character from the walk step s: o's id, the
   step's prototype entry, the step's name indexes x / y and the +8 of the recipe entry whose id is the prototype's place id (ahm_FindRecipe); -1 when there is no
   such recipe entry. The prototype is copied with its union assignment: that is what makes VC6 hold the entry's address in a register (lea [s+4]). */

int ahm_SendCreateAlchemist(unsigned char *o, AHMSTEP *s)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    AHMGRID *g;
    AHMCREATEMSG m;

    g = ahm_FindRecipe(s->grid.plr.id);
    if (g == 0)
        return -1;
    m.building = obj->id;
    m.proto = s->grid;
    m.x = s->x;
    m.y = s->y;
    m.value = g->w[2];
    return ((int (*)(void *))cm_SendCreateAlchemist)(&m);
}

#define AHM_ZERO6(m) memset(m, 0, 24)

/* 0x4ec500, creates the alchemist character for the object o (p[2]) of the building kind 0x1e from the order record p (+0xc .. +0xf four shares, +0x10 a value for +0xa,
   +0x14 / +0x20 the first place with its value, +0x18 / +0x1c the name indexes); the first free slot of ai_Chars (or one of kind 0xf), the relations to all others random
   -0x20 .. 0x1f; returns the character id or 0xffff ("ahm_AllocAhmInfo" allocates the record at +0x65). The character record is written
   through raw offsets: ai.h's AICHAR names id, kind, netId, health, name, jobId, title, bld / bld2, money, flags1cc, mood3Rate, partner, unk215 .. unk217 and
   mask; the unnamed ones (+0x10 .. +0x20 the start values, +0x50, +0x5c, +0x80, +0x9, +0xc, +0x18c, +0x190, +0x1f0, +0x1f4, +0x20c) carry no evidence yet.
   @deep B3 draft, 196 differing instructions: the whole statement/offset set was read against the original (asmdiff) and is complete and equal - no missing/extra statement, constant, type or call; the difference is the compiler's allocation from the first instruction on (a zero-register in ebx pushed for gm_FindById's 0,0 and reused for the byte/dword 0 stores vs our immediates and edi=0; the -1 in ebx vs eax; the character pointer frame base ebp vs eax; the ai_NextObjectId store schedule after the name strcpy and the util_Random block; the +0x210/-1, +0x18c, +0x20c stores). Last-bytes pass: spell.py menu assessed - 4ec510 rep-stos (memset/zero-init spellings; the memset(c,0,0x21c) already has the original's rep stos form), 4ec6af / 4ec761 / 4ec927 dup-tail (no if/else arm to move: the tails are the loop and call continuations), 4ec8ab reload (the [sp+0x14] list head is already re-read from memory at every use, the expression and parameter spellings give the same code), 4ec717 / 4ec916 code (the [esi+0x1c] store and the relation-loop store order are scheduling, the multiset is equal). No meaning-class lead with a concrete source form is left; what remains is allocation / scheduling. Q2: ai_Unk144a7a8 and ai_Unk144a80c are the two character-name pointer tables in the exe's BSS (0x144a7a8 / 0x144a80c, filled at runtime: no writer in any exported function, no string or data evidence for first/last name) - no evidenced name. Q4: 54 raw reads/stores of the 0x21c record; the AICHAR fields named in the comment can be typed, but +0x10 .. +0x20 (1000.0f / 12.0f / the random start values), +0x50, +0x5c, +0x80, +0x9, +0xc, +0x18c, +0x190, +0x1f0, +0x1f4, +0x20c have no evidence, and the typed rewrite re-schedules the store block (the unit's typed-view lesson). Q1/Q5/Q6/Q7 done: named, described, the three callees have prototypes in ahm.h. */
unsigned short ahm_CreateAlchemist(int *p)
{
    unsigned char *o;
    char *c;
    char *sp;
    int n, i, j, k;
    float v;
    int *g;

    gm_FindById(&o, 0, 0, p[2]);
    if (o == 0)
        return 0xffff;
    if (ai_BTypes[o[0] * 0x24d] != 0x1e)
        return 0xffff;
    for (n = 0; n < 0x300; n++)
        if (*(short *)ai_Chars[n] == -1)
            break;
    if (n == 0x300) {
        for (n = 0; n < 0x300; n++)
            if (ai_Chars[n][2] == 0x0f)
                break;
        if (n == 0x300)
            return 0xffff;
    }
    if (ai_Chars[n][2] == 0x0f)
        ai_CharCount--;
    c = ai_Chars[n];
    memset(c, 0, 0x21c);
    *(int *)(c + 4) = -1;
    *(unsigned short *)c = 0xffff;
    memset(c + 0x5c, 0xff, 0x20);
    *(unsigned char **)(c + 0x170) = o;
    *(unsigned char **)(c + 0x16c) = o;
    ai_CharCount++;
    *(unsigned short *)c = (unsigned short)n;
    *(int *)(c + 4) = ai_NextObjectId;
    ai_NextObjectId++;
    c[2] = 0;
    *(int *)(c + 0x1f0) = 0;
    *(int *)(c + 0x210) = -1;
    c[0x215] = 0;
    c[0x216] = 0;
    c[0x217] = 0;
    c[0x218] = 1;
    *(float *)(c + 0x1e4) = 1.0f;
    c[8] = 100;
    *(unsigned short *)(c + 0xa) = (unsigned char)((char *)p)[0x10];
    c[0x164] = 0x52;
    c[0x166] = 0;
    c[0x165] = 0;
    c[0x1f4] = 0;
    *(int *)(c + 0x190) = 4;
    *(int *)(c + 0x194) = 0;
    c[0xd] = 1;
    *(int *)(c + 0x18c) = -1;
    *(int *)(c + 0x20c) = -1;
    *(short *)(c + 0x50) = 0x7fff;
    for (i = 0; i < 5; i++)
        c[0x80 + i] = (char)(util_Random(0x40) + 0x54);
    c[9] = 0;
    strcpy(c + 0x30, ai_Unk144a7a8[p[6]]);
    strcpy(c + 0x40, ai_Unk144a80c[p[7]]);
    *(float *)(c + 0x1c) = 1000.0f;
    v = util_RandomSq();
    *(float *)(c + 0x20) = 12.0f;
    *(float *)(c + 0x14) = v * 7.0f + 30.0f - 2.0f;
    k = (int)(*(float *)(c + 0x1c) * 0.2f);
    *(float *)(c + 0x18) = *(float *)(c + 0x1c);
    *(float *)(c + 0x10) = *(float *)(c + 0x1c) * 0.33f + (float)util_Random(k) - *(float *)(c + 0x1c) * 0.1f;
    j = 0;
    for (i = 0; i < 0x90000; i += 0x300) {
        ai_Relations[0][*(unsigned short *)c + i] = (signed char)util_Random(0x40) - 0x20;
        ai_RelationMemory[0][*(unsigned short *)c + i] = 0;
        ai_Relations[0][*(unsigned short *)c * 0x300 + j] = (signed char)util_Random(0x40) - 0x20;
        ai_RelationMemory[0][*(unsigned short *)c * 0x300 + j] = 0;
        j++;
    }
    g = (int *)gm_Query(*(int *)(o + 0x5d), 1, 0, 0x24b);
    gm_PlaceOnStage(*(int *)(c + 4), *(int *)(o + 1), g != 0 ? *(int *)((char *)g + 2) : -1, 0, 0);
    gm_ChooseHeadIcon(c);
    if (*(int *)(o + 0x65) == 0)
        *(void **)(o + 0x65) = mem_Alloc(0x3c, "ahm_AllocAhmInfo");
    memset(*(void **)(o + 0x65), 0, 0x3c);
    **(int **)(o + 0x65) = *(int *)(c + 4);
    *(float *)(*(char **)(o + 0x65) + 4) = ((float)(unsigned char)((char *)p)[0xf] * 0.003968254f + 1.0f) * 0.5f;
    (*(char **)(o + 0x65))[8] = ((char *)p)[0xc];
    (*(char **)(o + 0x65))[9] = ((char *)p)[0xd];
    (*(char **)(o + 0x65))[10] = ((char *)p)[0xe];
    (*(char **)(o + 0x65))[11] = ((char *)p)[0xf];
    *(int *)(*(char **)(o + 0x65) + 0xc) = p[5];
    sp = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0xff);
    if (sp != 0) {
        while (*(unsigned short **)(sp + 0x14) != 0)
            gm_RemoveObject(sp + 0x14, **(unsigned short **)(sp + 0x14));
    }
    *(int *)(*(char **)(o + 0x65) + 0x10) = p[8];
    if (sp != 0)
        gm_AddObjekt(*(int *)(sp + 2), p[5], 2);
    gm_RefreshGoodsLists(ai_OwnPlayer);
    *(int *)(c + 0x1cc) |= 4;
    return *(unsigned short *)c;
}
/* 0x4ec940, the message 'SsCa' (cm_SendCommand) of the character c with the id and the value v clamped to 0 .. 1; -1 when c has no object (0x4eaad0) @deep B3 draft, close (69/69 instructions, 12 differing): the two out-of-line return blocks are in the other order - the original has m.value.f = 0.0f first (0x4ec9c1), ours the 1.0 block - plus the block's lea register (ecx vs eax); the 42 leading instructions and both branch tests match (fcomp 0x5eb788 0.0 / 0x5eb780 1.0, and 0x4100 / test ah,5 jp = the !(v > 0) / !(v < 1) NaN forms). Read: disasm 0x4ec940..0x4ec9f4, cm_SendCommand, the sibling ahm_SendSetPlaceValue. Leads tried, one build each, no gain: the else-form, the inverted form, if (!(v > 0)), the 1.0 block inside the outer if with the 0.0 block after it (no else keyword - 12 as before), and the two-guard early-return form (worse: 30). spell.py: no menu rule matches the 4ec9c1 hunk, 'no hunk matched the menu; escalate'. Q1..Q7 pass. */

int ahm_SendSetShare(char *c, int id, float v)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.charId = ((AHMCHAR *)c)->id;
    m.tag = AHM_CMD_SET_SHARE;
    m.id = id;
    if (v > 0.0f) {
        if (v < 1.0f) {
            m.value.f = v;
            return ((int (*)(void *))cm_SendCommand)(&m);
        }
    } else {
        m.value.f = 0.0f;
        return ((int (*)(void *))cm_SendCommand)(&m);
    }
    m.value.f = 1.0f;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
/* 0x4eca00, the 'SsCa' handler: sets the share m->id (0 .. 3) of the record rec (+8 .. +0xb) to the value m->value.f times 252 (the inverse of 0x4eab20); 0 for
   another number */

int ahm_SetShare(int *p, char *rec)
{
    AHMORDER *m = (AHMORDER *)p;
    AHMREC *r = (AHMREC *)rec;

    switch (m->id) {
    case 0:
        r->share[0] = (char)(int)(m->value.f * 252.0f);
        return 1;
    case 1:
        r->share[1] = (char)(int)(m->value.f * 252.0f);
        return 1;
    case 2:
        r->share[2] = (char)(int)(m->value.f * 252.0f);
        return 1;
    case 3:
        r->share[3] = (char)(int)(m->value.f * 252.0f);
        return 1;
    default:
        return 0;
    }
}
/* 0x4ecaa0, the message 'Rdaa' (cm_SendCommand) of the character c with the table entry of id: its id and +8; -1 when c has no object or no free place (0x4eac20) or the entry does not exist */

int ahm_SendAddPlace(char *c, int id)
{
    AHMGRID *g;
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    if (ahm_HasFreeSlot(c) == 0)
        return -1;
    g = ahm_FindRecipe(id);
    if (g == 0)
        return -1;
    AHM_ZERO6(&m);
    m.charId = ((AHMCHAR *)c)->id;
    m.tag = AHM_CMD_ADD_PLACE;
    m.id = g->rcp.id;
    m.value.i = g->w[2];
    return ((int (*)(void *))cm_SendCommand)(&m);
}
/* 0x4ecb30, puts the order p (+8 the id, +0x10 the value) into the first free place of the record rec (6 pairs at +0xc) of the object o (+0x5d the list); 0 when all are in use */

int ahm_AddPlace(int *p, char *rec, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    AHMREC *r = (AHMREC *)rec;
    AHMORDER *m = (AHMORDER *)p;
    int i;
    AHMGOOD *sp;
    int *q;

    q = (int *)(rec + 0xc);
    i = 0;
    do {
        if (*q == 0) {
            sp = (AHMGOOD *)gm_Query(obj->objs, 2, 6, 0, 0xff);
            if (net_Conn.s != INVALID_SOCKET)   /* a network game: the server's id counter wins */
                ai_NextObjectId = m->nextObjectId;
            r->slots[i].id = m->id;
            r->slots[i].v = m->value.f;
            if (sp != 0)
                gm_AddObjekt(sp->objId, p[2], 2);
            gm_RefreshGoodsLists(ai_OwnPlayer);
            return 1;
        }
        i++;
        q += 2;
    } while (i < 6);
    return 0;
}
/* 0x4ecbc0, removes the place of the id p[2] from the record rec and from the list of the object o; 0 when there is none */

int ahm_RemovePlace(int *p, char *rec, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    AHMREC *r = (AHMREC *)rec;
    int i;
    char *sp;
    int *q;

    q = (int *)(rec + 0xc);
    i = 0;
    do {
        if (*q == p[2]) {
            sp = gm_Query(obj->objs, 2, 6, 0, 0xff);
            if (sp != 0)
                gm_RemoveObject(sp + 0x14, p[2]);
            r->slots[i].id = 0;
            r->slots[i].v = 0.0f;
            return 1;
        }
        i++;
        q += 2;
    } while (i < 6);
    return 0;
}
/* 0x4ecc30, the message 'cRSa' (0x4c61b0) of the character c for the place id with the value v clamped to 0 .. 1; -1 when c has no object or no such place (0x4eabe0) @deep B3 draft, close (89/89 instructions, 12 differing): the same clamp block order as ahm_SendSetShare - the original lays the m.value.f = 0.0f block out first (0x4ecccb), ours the 1.0 block, and the block's lea register differs (edx vs eax); the 56 leading instructions and both branch tests match. Read: disasm 0x4ecc30..0x4eccf8, the sibling ahm_SendSetShare. Leads tried (same source family as the sibling, one build each): the else-form, the inverted form, if (!(v > 0)), the no-else form (1.0 inside the outer if, 0.0 after - the current source, 12), the two-guard early-return form (worse, 30 on the sibling). spell.py: no menu rule matches the 4ecccb hunk. Q1..Q7 pass. */

int ahm_SendSetPlaceValue(char *c, int id, float v)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    if (ahm_FindSlot(c, id) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.charId = ((AHMCHAR *)c)->id;
    m.tag = AHM_CMD_SET_PLACE;
    m.id = id;
    if (v > 0.0f) {
        if (v < 1.0f) {
            m.value.f = v;
            return ((int (*)(void *))cm_SendCommand)(&m);
        }
        m.value.f = 1.0f;
        return ((int (*)(void *))cm_SendCommand)(&m);
    }
    m.value.f = 0.0f;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
/* 0x4ecd10, the 'cRSa' handler: sets the value of the place m->id in the record rec to m->value.f; 0 when there is none */

int ahm_SetPlaceValue(int *p, char *rec)
{
    AHMORDER *m = (AHMORDER *)p;
    AHMREC *r = (AHMREC *)rec;
    int i;

    for (i = 0; i < 6; i++) {
        if (r->slots[i].id == m->id) {
            r->slots[i].v = m->value.f;
            return 1;
        }
    }
    return 0;
}
/* 0x4ecd50, the message 'SmSa' (0x4c61b0) of the character c with the value v; -1 when c has no object */

int ahm_SendAddValue(char *c, int v)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.tag = AHM_CMD_ADD_VALUE;
    m.charId = ((AHMCHAR *)c)->id;
    m.value.i = v;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
/* 0x4ecdb0, the 'SmSa' handler: adds m->value.f to the value at +4 of the record rec, clamped to 0 .. 1 @deep B3 draft, 13 differing instructions (44 -> 41 differing bytes this session): the clamp semantics are proven against the original (0 <= f < 1 store f, f >= 1 store 1.0f, f < 0 / NaN store 0.0f; the original compares f with 0.0, 1.0, then 0.0 again). The remaining hunks are the test schedule: the original emits the 0.0 fcom/and-eax,0x4100/jne test and the 1.0 fcom/test-ah,0x5/jp test up front, our form evaluates the 1.0 test with jnp at the L1 join (same operations, other block placement). spell.py menu dup-tail at 4ecdbe: all three prescriptions tried, one build each - statements in both arms (nested if/else with the store in each arm: 15 differing instructions), pull them after the branch (12-13, kept: the current form), swap the branch polarity (the f >= 0.0f outer / f < 1.0f inner nested form: 17). Also tried the f <= 0.0f / f >= 1.0f clamp (15) and the two early returns (18). Readable; Q1..Q7 hold. */

int ahm_AddValue(int *p, char *rec)
{
    AHMORDER *m = (AHMORDER *)p;
    AHMREC *r = (AHMREC *)rec;
    float f;

    f = m->value.f + r->value;
    if (!(f > 0.0f))
        f = 0.0f;
    if (!(f < 1.0f))
        f = 1.0f;
    r->value = f;
    return 1;
}
/* 0x4ece10, the message 'ErFi' (cm_SendCommand) of the character c; -1 when c has no object */

int ahm_SendDelete(char *c)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.tag = AHM_CMD_DELETE;
    m.charId = ((AHMCHAR *)c)->id;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
/* 0x4ece60, deletes the alchemist of the record rec (+0 the character id): clears its object pointers, removes the places from the list of the object o, zeroes the record; 0 when the character is gone */

int ahm_DeleteAlchemist(int unused, int *rec, unsigned char *o)
{
    char *c;
    char *sp;
    int *q;
    int i;

    c = gm_FindChar(rec[0]);
    if (c != 0) {
        sp = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0xff);
        *(int *)(c + 0x170) = 0;
        *(int *)(c + 0x16c) = 0;
        q = rec + 3;
        for (i = 6; i != 0; i--) {
            if (*q != 0) {
                if (sp != 0)
                    gm_RemoveObject(sp + 0x14, *q);
                *q = 0;
                q[1] = 0;
            }
            q += 2;
        }
        memset(rec, 0, 0x3c);
        unk_Unk503c60(*(unsigned short *)c, 1);
        return 1;
    }
    return 0;
}
/* 0x4ecf10, writes the record of the object o (+0x65; a blank one with id -1 when it has none) to the file f: id (4), float (4), 4 shares (1 each), 6 places (2 x 4); 1 when all was written */


int ahm_WriteRecord(void *f, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    int *r;
    int rec[15];
    int i;

    r = (int *)obj->rec;
    memset(rec, 0, 0x3c);
    rec[0] = -1;
    if (r == 0)
        r = rec;
    if (vfs_fwrite(r, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(r + 1, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(r + 2, 1, 1, f) <= 0)
        return 0;
    if (vfs_fwrite((char *)r + 9, 1, 1, f) <= 0)
        return 0;
    if (vfs_fwrite((char *)r + 10, 1, 1, f) <= 0)
        return 0;
    if (vfs_fwrite((char *)r + 11, 1, 1, f) <= 0)
        return 0;
    for (i = 0; i < 6; i++) {
        if (vfs_fwrite(r + 3 + i * 2, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(r + 4 + i * 2, 4, 1, f) <= 0)
            return 0;
    }
    return 1;
}
/* 0x4ed040, reads the record of 0x4ecf10 from the file f into the object o (+0x65, allocated when missing); 1 when all was read @deep B3 draft, 136 differing instructions: the body was read against the original and is complete (the id, float, 4 share bytes, 6 place pairs and the record copy into the allocated +0x65 block; ls_Read is ls.c's 0x51c920). The hunks are the prologue (the original keeps the file in edi, ours in esi), the frame slots, and the return-0 block placement: the original inlines pop edi/pop esi/pop ebx/add esp,0x3c/ret after every failed read and skips it with jne, ours routes every return 0 to one shared out-of-line tail with je. Lead tried this session: the !ls_Read(...) form (compiles to the same 136 differing instructions - the source comparison is not the cause); the same family lesson from ahm_WriteRecord (0x4ecf10, escalated): the shared-tail / prologue difference is compiler block placement. */

int ahm_ReadRecord(void *f, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    int rec[15];
    int i;
    int *s, *d;

    if (ls_Read(&rec[0], 4, 1, f) == 0)
        return 0;
    if (ls_Read(&rec[1], 4, 1, f) == 0)
        return 0;
    if (ls_Read((char *)rec + 8, 1, 1, f) == 0)
        return 0;
    if (ls_Read((char *)rec + 9, 1, 1, f) != 0) {
        if (ls_Read((char *)rec + 10, 1, 1, f) == 0)
            return 0;
        if (ls_Read((char *)rec + 11, 1, 1, f) != 0) {
            i = 0;
            d = &rec[3];
            while (ls_Read(d - 1 + 0, 4, 1, f) != 0) {
                if (ls_Read(d, 4, 1, f) == 0)
                    return 0;
                i++;
                d += 2;
                if (i > 5) {
                    if (obj->rec == 0)
                        obj->rec = mem_Alloc(0x3c, "ahm_AllocAhmInfo");
                    s = rec;
                    d = (int *)obj->rec;
                    for (i = 15; i != 0; i--)
                        *d++ = *s++;
                    return 1;
                }
            }
        }
        return 0;
    }
    return 0;
}

extern char ai_Chars[768][0x21c];
extern char * ai_BTypes;
extern signed char ai_Relations[768][768];
extern signed char ai_RelationMemory[768][768];
int ahm_Tab[8] = { 1, 3, 7, 11, 13, 17, 19, 21 };  /* 0x60f1b0 */
/* ai_Unk144a7a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144a80c: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_WalkX = 0;  /* 0x6f0dc8 */
int ai_WalkDX = 0;  /* 0x6f0dcc */
int ai_WalkY = 0;  /* 0x6f0dd0 */
int ai_WalkDY = 0;  /* 0x6f0dd4 */
int ai_WalkIndex = 0;  /* 0x6f0dd8 */
AHMSEED ai_WalkSeed = {0};  /* 0x6f0ddc */
AHMSTEP ai_WalkSteps[3] = {0};  /* 0x6f0de0 */
int ai_PlayerPrototypeCount = 0;  /* 0x6f0e28 */
AHMGRID * ai_PlayerPrototypes = 0;  /* 0x6f0e2c */
int ai_RecipePrototypeCount = 0;  /* 0x6f0e30 */
AHMGRID * ai_RecipePrototypes = 0;  /* 0x6f0e34 */
unsigned short ai_PlayerId = 0;  /* 0x6f25b8: the local player's character id (the owner field of the building records); defined here because ahm.c does not read it */
char ai_Chars[768][0x21c] = {0};  /* 0x770b80, the characters */
char * ai_BTypes = 0;  /* 0x7e3f30 */
signed char ai_Relations[768][768] = {0};  /* 0x7e5360 */
signed char ai_RelationMemory[768][768] = {0};  /* 0x889f80 */
unsigned short d2_HighBitMask = 0;  /* 0xf8a11c, mask of the half dark colours; defined here because ahm.c does not read it */
