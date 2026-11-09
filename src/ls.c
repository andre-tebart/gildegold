/* ls.c -- the save game (0x519010 ..): the tag "ls_" ("ls:tmp" 0x623654, "ls_LoadCharacter(): invalid next or prev-action pointer ...", "ls_SendGame"), the language number of a name (0x519010: the table "GERMAN" .. 0x5ee978), the save game writer 0x5190b0 (header 0x5194c0: "SCENARIO", the player, the party records and the screen shot of 0xe100 bytes, then one writer per subsystem), the pointer <-> index conversions before / after a save (0x519a00 / 0x519aa0 / 0x519b50) and the other writers (0x519bf0 ..), the readers (0x51c950 ..) and the network send / receive of a save game (0x520a30 / 0x520b80). The unit starts at its first global 0x62364c (the language number, -1); its strings run to "ls_SendGame" 0x62376c / "rbn" 0x623778, and it ends at 0x520ec0, sv.c's first function. Names are ours.
   Open: two of ls_LoadGame's readers are unnamed (unk_Unk520200, unk_Unk520310).
   unit of rea 8cef7d5b (src/game/ls.c); boundaries in decl/text.dcl */
struct D3SM;
struct D3TERRAIN;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "f3gm.h"
#include "gv.h"
#include "ai.h"
#include "vfs.h"
#include "he.h"
#include "f3gm_carry.h"
#include "txt.h"
#include "data_views.h"
#include "ls.h"
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the player slots */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeCopyT (*(AITIME *)&ai_TimeCopy)  /* 0x75e130, ai_TimeCopy (cm.c: int[4]) as the AITIME it holds */
#define sv_PanelShownT (*(struct AIOBJ **)&sv_PanelShown)  /* 0x6f2310, sv_PanelShown (sv.c: unsigned short *) as the AIOBJ (room object) it points to */
#include "d3_types.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int gm_TreeNodeCount;
extern unsigned gm_OwnNameHeads[136];
extern GAPDATA full_data_92bcb3[];
#include "cm_types.h"
extern void main_LoadProgressCallback(int kind, int cur, int total);
extern int gm_TreeNodeCount;
extern int gm_QueryDepth;

#include "cm.h"
int ai_NextPartyId = 1;  /* 0x61a658 */
int ls_Language = 65535;  /* 0x62364c, the language number ( */
int ls_Version = 65626;  /* 0x623650 */
const char ls_LanguageNames[][0x40] = { "GERMAN", "ENGLISH", "FRENCH", "ITALIAN", "SPANISH" };  /* 0x5ee978, the language names (5 of */
extern int gm_PlayerRecords[688];          /* 0x7d5f80, the 16 player records (0xac bytes each; gm_PlayerRecord, f3gm.c) */
/* gm_PlayerRecordBytes: view of gm_PlayerRecords (tools/data_views.py) */
int ai_NextCrimeId = 0;  /* 0x6f1b88 .. */
int ls_SpareInt = 0;  /* 0x6f1bcc */
unsigned char ls_SaveSlot = 0;  /* 0x6f1c38, the slot the save header records */
/* ai_Unk76fa24: view of cm_SaveAcks (tools/data_views.py) */
int ai_NextEventId = 0;  /* 0x6f2618 */
int he_EventTop = 0;  /* 0x6f261c, the highest used index of he_Events (own object: ai_NextEventId is a 4-byte int) */
extern int ai_NextCharNumber, ai_NextPlayerNumber;   /* 0x6f1bb8, 0x6f1bbc, 0x6f1bc0 */
/* ls_RefTargets: view of cm_SendStates (tools/data_views.py) */
int gv_WriteCrimes();                     /* 0x4f28d0 */

#define LS_W(p, sz) if (vfs_fwrite((p), (sz), 1, f) <= 0) return 0;
#include "rd_types.h"                 /* G15BK */
int gv_WriteGraveyard();                     /* 0x4f17f0 */

#define LS_R(p, sz) if (ls_Read((int)(p), (sz), 1, f) == 0) return 0;

extern int f3gm_ObjectCount;                /* 0x6f1bdc */
char d3if_ClearScene(int index);                    /* 0x447e60 */

extern int hst_TextCurrent, hst_TextFirst, hst_KeyNext, hst_KeyFirst;   /* 0x601060, 0x601058, 0x60106c, 0x601064 .. 0x60106c */
extern unsigned char hst_TextsOn;               /* 0x6f0724 */
extern char hst_Key[];                 /* 0xf83520, the history key ("G12", 0x20 bytes; the save keeps 0x10) */
#include "hst_types.h"
#include "f3init.h"
#include "sv.h"
#include "ahm.h"
#include "am.h"
#include "amt.h"
#include "anm.h"
#include "hst.h"
#include "main.h"
#include "mem.h"
#include "rt.h"
#include "poker.h"
#include "mission.h"
#include "sv_t.h"
#include "lc.h"
#include "ch.h"
#include "d2.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_sm.h"
#include "menu.h"
#include "dd_vesa.h"
#include "hmt.h"
int gv_ReadEffects();                     /* 0x4ed910 */

/* he_EventTop: view of ai_NextEventId (tools/data_views.py) */
int he_QuickTop = 0;  /* 0x6f2620 / */
unsigned char he_Events[339968] = {0};  /* 0x6fab40, the */
char * he_QuickList[10921] = {0};  /* 0x74db40, pointers to the */
int he_EventCount = 0;  /* 0x7585e4 */
void (*he_RunHandlers[5544])(unsigned char *) = {0};  /* 0x758600, the event run handlers by type (he.c */
/* ai_Unk75e134 / ai_Unk75e138 / ai_Unk75e13c: views of ai_TimeCopy (data_views.h), the clock copy */

void he_FreeEvents();                    /* 0x53d590 */

char *strstr();                   /* 0x5de6b0, strstr-like (the "hozfaeller" misspelling check) */

/* 0x519010, the language number of the name s (case-insensitive, one of the five of the table 0x5ee978), 0xffff when unknown; kept in 0x62364c */

int ls_LanguageOf(char *s)
{
    char buf[0x40];
    int i;

    strcpy(buf, s);
    _strupr(buf);
    ls_Language = 0xffff;
    for (i = 0; i < 5; i++) {
        if (strcmp(ls_LanguageNames[i], buf) == 0) {
            ls_Language = i;
            return i;
        }
    }
    return 0xffff;
}
/* 0x5190a0, the language number found by 0x519010 */


int ls_GetLanguage(void)
{
    return ls_Language;
}
/* 0x5190b0, writes the save game name (vfs_Tfopen "wb"): remembers the path in ls_SavePath, turns the references
   into ids (ls_RefsToIds), writes the header (ls_WriteSaveHeader, flags) and the fixed globals (the id counters, the
   clock, the own player and its character's netId, the character counters, the update block), then the writers of
   the subsystems in this order: objects, buildings, player records, characters, player slots, offices, the scene
   relink and, when (flags & 2) == 0, the graveyard, relations, statistics, avatars, the player table, parties,
   history, message queues, environment, scene records, missions, slot table, settings, walk, hmt and routes. The
   first writer that fails closes the file, turns the ids back into references (ls_IdsToRefs) and returns 0; returns
   1 after the last.
   @deep read against the whole original 0x5190b0..0x5194bb: every failure path closes the file with
   vfs_fclose(f) - the original pushes f before each branch (push esi; jne) and calls vfs_fclose with it,
   the draft called it without an argument through a cast (that kept the blocks apart, 72 differing, but it
   was not the original's call); the step order, the fixed globals and their sizes are equal. Readability:
   the 23 placeholders named (ls_RefsToIds / ls_IdsToRefs, the writers, the id counters), the rough-draft marker dropped,
   the prototypes of ai_SaveMissions / ai_SaveBookmarks / ai_SaveSettings in their owners' headers and
   hmt.h included (typing commit). Left: block layout - the original keeps one failure block per step (call
   vfs_fclose; add esp,4; call ls_IdsToRefs; pop esi; xor eax,eax; pop ebx; ret, at 0x5191e1, 0x519200,
   ...) and returns the f == 0 case inline (jne; pop esi; pop ebx; ret at 0x5190ce), ours cross-jumps all
   of them into one tail (je 0x519425 / je 0x519432); the step blocks themselves are equal (size: 331 vs
   389 instructions, explained: ours=331 original=389: the 58 instructions are the duplicated failure tails
   - the original has 24 copies of the 7-instruction close / restore / return block plus the inline early
   return, ours one shared copy (VC6 tail merging of identical blocks)) */


int ls_SaveGame(int name, char *path, unsigned flags)
{
    int f;

    f = vfs_Tfopen(name, "wb", vfs_Root);
    if (f == 0)
        return 0;
    strcpy(ls_SavePath.path, path);
    ls_RefsToIds();
    if (ls_WriteSaveHeader(f, flags) == 0) {
        vfs_fclose(f);
        ls_IdsToRefs();
        return 0;
    }
    vfs_fwrite(&ai_NextObjectId, 4, 1, f);
    vfs_fwrite(&ai_NextEventId, 4, 1, f);
    vfs_fwrite(&ai_Time, 0xe, 1, f);
    vfs_fwrite(&ai_OwnPlayer, 1, 1, f);
    vfs_fwrite(ai_PlayerCharC + 4, 4, 1, f);
    vfs_fwrite(&ai_NextCharNumber, 4, 1, f);
    vfs_fwrite(&ai_NextPlayerNumber, 4, 1, f);
    vfs_fwrite(&ai_CharCount, 4, 1, f);
    vfs_fwrite(&ls_SpareInt, 4, 1, f);
    vfs_fwrite(&ai_Update, sizeof(AIUPDATE), 1, f);   /* 0xf89580 */
    vfs_fwrite(&ai_NextPartyId, 4, 1, f);
    vfs_fwrite(&ai_NextCrimeId, 4, 1, f);
#define LS_STEP(fn) if (fn(f) == 0) { vfs_fclose(f); ls_IdsToRefs(); return 0; }
    LS_STEP(ls_WriteObjects)
    LS_STEP(ls_WriteBuildings)
    LS_STEP(ls_WritePlayerRecords)
    LS_STEP(ls_WriteCharacters)
    LS_STEP(ls_WritePlayerSlots)
    LS_STEP(amt_SaveAemter)
    LS_STEP(ls_RelinkScene)
    if ((flags & 2) == 0) {
        LS_STEP(gv_WriteCrimes)
        LS_STEP(ls_WriteRelations)
        LS_STEP(ls_WriteStatistics)
        LS_STEP(ls_WriteAvatars)
        LS_STEP(ls_WriteEventTables)
        LS_STEP(ls_WriteParties)
        LS_STEP(ls_WriteHistory)
        LS_STEP(ls_WriteMessageQueues)
        LS_STEP(ls_WriteEnvironment)
        LS_STEP(ls_WriteSceneRecords)
        LS_STEP(ai_SaveMissions)
        LS_STEP(ai_SaveBookmarks)
        LS_STEP(ai_SaveSettings)
        LS_STEP(ahm_SaveWalk)
        LS_STEP(hmt_Save)
        LS_STEP(rt_SaveRoutes)
    }
    vfs_fclose(f);
    ls_IdsToRefs();
    return 1;
}
/* 0x5194c0, writes the save game header (LSHDR) field by field to f: the version 0x1005a (kept in ls_Version), the
   flags (the argument), the own player's slot (its name, the two words +0x40 / +0x44, the byte +0x48), ai_NumPlayers,
   the clock, then the player's character (its texts +0x30 / +0x40, the bytes +0xc, the word +0x54, the bytes +0x164 /
   +0x166 / +0xd, its worth through gm_GetWorth / gm_ConvertMoney and the byte +9) or, without one (ai_PlayerId
   0xffff), "SCENARIO" and zero bytes (the worth word is then written uninitialized, as in the original), then
   ls_SaveSlot, the id of the player's building (or the fallback one, -1 none), the id of the object the info panel
   shows (-1 none), the second word of the 8 cm_SaveAcks slots, the screen shot (0xa0 x 0x78 pixels of
   ai_ScreenImage split into 3 bytes each by dd_SplitRGB, 0xe100 bytes), ai_Difficulty and the first 0x60 bytes of
   ls_SavePath. Returns 1, 0 when one of the checked writes fails (the shot buffer is then not freed). */


int ls_WriteSaveHeader(int f, unsigned flags)
{
    unsigned char noJob, noByteD, noOffice, noByte9;
    int ref;
    int worth;
    int y;
    unsigned char *shot;
    int version;
    char noText[0x10] = "SCENARIO";
    LSHDR hdr;
    int i, x, k;
    GMPLAYERSLOT *slot;

    noByte9 = noByteD = noOffice = noJob = 0;
    ref = -1;
    k = 0;
    shot = (unsigned char *)mem_Alloc(0xe100, "ls:tmp");
    version = 0x1005a;
    ls_Version = 0x1005a;
    if (vfs_fwrite(&version, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&flags, 1, 1, f) <= 0)
        return 0;
    slot = &ai_PlayerSlotsT[ai_OwnPlayer];
    strcpy(hdr.player, slot->name);
    hdr.playerWords[0] = slot->mapX;
    hdr.playerWords[1] = slot->mapY;
    hdr.playerByte = slot->maxPlayers;
    hdr.players = ai_NumPlayers;
    if (vfs_fwrite(hdr.player, 0x20, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(hdr.playerWords, 8, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&hdr.playerByte, 1, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&hdr.players, 1, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_Time, 0xe, 1, f) <= 0)
        return 0;
    if (ai_PlayerId != 0xffff) {
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0x30, 0x10, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0x40, 0x10, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0xc, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0x54, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0x164, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0x166, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 0xd, 1, 1, f) <= 0)
            return 0;
        worth = gm_ConvertMoney(gm_GetWorth(ai_PlayerId), ai_OwnPlayer);
        if (vfs_fwrite(&worth, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(ai_Chars[ai_PlayerId] + 9, 1, 1, f) <= 0)
            return 0;
    } else {
        if (vfs_fwrite(noText, 0x10, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&noJob, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&noOffice, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&noByteD, 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&worth, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&noByte9, 1, 1, f) <= 0)
            return 0;
    }
    if (vfs_fwrite(&ls_SaveSlot, 1, 1, f) <= 0)
        return 0;
    if (ai_PlayerBuildingT != 0)
        ref = ai_PlayerBuildingT->id;
    else if (ai_FallbackBuildingT != 0)
        ref = ai_FallbackBuildingT->id;
    else
        ref = -1;
    vfs_fwrite(&ref, 4, 1, f);
    if (sv_PanelShownT != 0)
        ref = sv_PanelShownT->objId;
    else
        ref = -1;
    vfs_fwrite(&ref, 4, 1, f);
    for (i = 0; i < 8; i++)
        vfs_fwrite(&cm_SaveAcks[i][1], 4, 1, f);
    for (y = 0; y < 0x78; y++) {
        for (x = 0; x < 0xa0; x++) {
            dd_SplitRGB(ai_ScreenImage[k], shot + k * 3, shot + k * 3 + 1, shot + k * 3 + 2);
            k++;
        }
    }
    if (vfs_fwrite(shot, 0xe100, 1, f) <= 0)
        return 0;
    if (shot != 0)
        mem_Free(shot);
    vfs_fwrite(&ai_Difficulty, 4, 1, f);
    vfs_fwrite(ls_SavePath.path, 0x60, 1, f);
    return 1;
}
/* 0x519a00, before a save: the objects of the buildings get their building back (the owner word of the
   building's scene object, AIBLD.object), and the references of cm_SendStates (its value word: 1 a character,
   2 a building, 3 an object, 9 a scene party) are turned into plain ids (the referenced record's id) */


void ls_RefsToIds(void)
{
    int saved = d3_CurrentScene;
    int k;
    AIBLD *b;
    G17STAT *p;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = (AIBLD *)(ai_Blds + k);
        if (b->type != 0 && b->object != 0)
            ((D3OBJECT *)b->object)->owner = (void *)b->id;
    }
    d3if_SelectScene(saved, 1);
    for (p = cm_SendStates; (int)&p->value < (int)ls_RefTargets + 0x50000; p++) {
        if (p->value != 0) {
            if (p->kind == 1)
                p->value = ((AICHAR *)p->value)->netId;
            if (p->kind == 3)
                p->value = ((AIOBJ *)p->value)->objId;
            if (p->kind == 2)
                p->value = ((AIBLD *)p->value)->id;
            if (p->kind == 9)
                p->value = ((G28REC *)p->value)->id;
        }
    }
}
/* 0x519aa0, after a save: the buildings' objects point back to their buildings (+0x200) and the ids of the references table are turned into pointers again (0x505df0 character, 0x4fe980 object, 0x502020 building, 0x4e50a0 kind 9) @deep B: 40 code hunks, read in asmdiff (the equal stretches around them checked too): the kind order 1, 3, 2, 9, the offsets and the calls 0x505df0 / 0x4fe980 / 0x502020 / 0x4e50a0 match the original at 0x519aa0 exactly; the hunks are shapes - ours keeps the loop counter in another register of the first loop, the second loop's entry compares through ecx with jge where the original compares esi with jl, and the original reloads [esi] before the kind==3 / kind==2 pushes where ours forwards a register. Shape is match-deep's. Q2/Q7: the callees and globals are unnamed; Q6 is described. */


void ls_IdsToRefs(void)
{
    int saved = d3_CurrentScene;
    int k;
    int *p;
    char *b;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(char **)(*(int *)(b + 0x61) + 0x200) = b;
    }
    d3if_SelectScene(saved, 1);
    for (p = ls_RefTargets; (int)p < (int)ls_RefTargets + 0x50000; p = (int *)((char *)p + 10)) {
        if (*p != 0) {
            if (*((char *)p - 5) == 1)
                *p = (int)gm_FindChar(*p);
            if (*((char *)p - 5) == 3)
                *p = gm_FindObjectInst(*p);
            if (*((char *)p - 5) == 2)
                *p = gm_FindBuildingById(*p);
            if (*((char *)p - 5) == 9)
                *p = cm_PartyById(*p);
        }
    }
}
/* 0x519b50, after a load: the buildings' objects point back to their buildings, the scene is restored (0x44aa10) and the pointers are set again */


int ls_RelinkScene(int f)
{
    int saved = d3_CurrentScene;
    int k;
    AIBLD *b;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = (AIBLD *)(ai_Blds + k);
        if (b->type != 0 && b->object != 0)
            ((D3OBJECT *)b->object)->owner = (void *)b->id;
    }
    d3_SaveScene(0, f);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = (AIBLD *)(ai_Blds + k);
        if (b->type != 0 && b->object != 0)
            ((D3OBJECT *)b->object)->owner = b;
    }
    d3if_SelectScene(saved, 1);
    return 1;
}
/* 0x519bf0, the object list ai_Objects (0x5008 records of 0x43 bytes; the used ones have a nonzero first word): the count, then each used record in pieces (2, 4, 4, 4, 4, 1, 1 bytes and 0x1f bytes at +0x1c) */


int ls_WriteObjects(int f)
{
    int n = 0;
    int i, off;
    AIOBJ *p = ai_Objects;
    short *q;
    char *r;

    for (i = 0x5008; i != 0; i--) {
        if (p->id != 0)
            n++;
        p++;
    }
    if (vfs_fwrite(&n, 4, 1, f) <= 0)
        return 0;
    off = 0;
    do {
        r = (char *)(off + (int)ai_Objects);
        q = (short *)(off + (int)ai_Objects);
        if (*q != 0) {
            if (vfs_fwrite(r, 2, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 2, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 6, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 10, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0xe, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x12, 1, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x13, 1, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x1c, 0x1f, 1, f) <= 0)
                return 0;
        }
        off += 0x43;
    } while (off < 0x14f218);
    return 1;
}
/* 0x519d20, the buildings: the count of the used ones (0x100 AIBLD records of 0xa9 bytes, ai_Blds), then each field by field (+0 .. +0x5c, then by type: 0x1e its 64 vegetation records at AIBLD.plants, 0x48 .. 0x4a gv_WriteGraveyard, 0x4b .. 0x4d ahm_WriteRecord, the others the 0x30 bytes at +0x65), the work settings at +0x99; at the end the building sites (ai_NBauplaetze, then ai_Bauplaetze's name, minimum and maximum corner). Returns 1, 0 when a write fails */


int ls_WriteBuildings(int f)
{
    int n = 0, off, i, j;
    char *p;
    AIBLD *b;
    char c;

    p = ai_Blds;
    for (i = 0x100; i != 0; i--) {
        if (*p != 0)
            n++;
        p += 0xa9;
    }
    LS_W(&n, 4)
    off = 0;
    do {
        b = (AIBLD *)(ai_Blds + off);
        if (ai_Blds[off] != 0) {
            LS_W(&b->type, 1)
            LS_W(&b->id, 4)
            LS_W(b->name, 0x20)
            LS_W(&b->masterId, 2)
            LS_W(&b->owner, 2)
            LS_W(&b->mainRoom, 2)
            LS_W(&b->saved2b, 4)
            LS_W(&b->saved2f, 1)
            LS_W(&b->side, 1)
            LS_W(&b->saved35, 4)
            LS_W(&b->budget, 4)
            LS_W(&b->wageRate, 4)
            LS_W(&b->workRate, 4)
            LS_W(&b->saved45, 4)
            LS_W(&b->priceFactor, 4)
            LS_W(&b->roundSums[0], 4)
            LS_W(&b->roundSums[1], 4)
            LS_W(&b->roundSums[2], 4)
            LS_W(&b->flags5aw, 2)
            LS_W(&b->level, 1)
            c = b->type;
            if (c == 0x1e) {
                for (j = 0; j < 0x40; j++) {
                    LS_W(&((AIMAPENT *)b->plants)[j].id, 4)
                    LS_W(&((AIMAPENT *)b->plants)[j].bldId, 4)
                    LS_W(&((AIMAPENT *)b->plants)[j].x, 1)
                    LS_W(&((AIMAPENT *)b->plants)[j].z, 1)
                    LS_W(&((AIMAPENT *)b->plants)[j].type, 2)
                    LS_W(&((AIMAPENT *)b->plants)[j].fallId, 4)
                    LS_W(&((AIMAPENT *)b->plants)[j].size, 1)
                    LS_W(&((AIMAPENT *)b->plants)[j].state, 1)
                }
            } else if (c == 'H' || c == 'I' || c == 'J') {
                if (gv_WriteGraveyard(f, b) == 0)
                    return 0;
            } else if (c == 'K' || c == 'L' || c == 'M') {
                if (ahm_WriteRecord(f, b) == 0)
                    return 0;
            } else {
                LS_W(b->kindData, 0x30)
            }
            LS_W(b->workSet, 0x10)
        }
        off += 0xa9;
    } while (off < 0xa900);
    LS_W(&ai_NBauplaetze, 4)
    ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, ai_NBauplaetze * 0x60);
    for (i = 0; i < ai_NBauplaetze; i++) {
        LS_W(ai_Bauplaetze[i].name, 0x40)
        LS_W(&ai_Bauplaetze[i].x0, 0x10)
        LS_W(&ai_Bauplaetze[i].x1, 0x10)
    }
    return 1;
}
/* 0x51a1a0, the records of 0x7d5f80 .. 0x7d6a42 (0xac bytes each): the id, name, ids, the floats and ints at the offsets below; the fields +0x52 .. +0x6a only when the save game's version 0x623650 is above 0x10013 @deep B: 135 code hunks, read at the start and sampled: the base pointer 0x7d5f82 lands in eax in ours and in esi in the original, so the first block is sub eax,0x2 vs lea eax,[esi-0x2] and every later block's lea/sub and pairing shifts; the offsets (-2, 0, 0x12, 0x26, 0x1a, 0x1e, 0x2a, 0x2e, 0x32, 0x36, 0x3e, 0x42, 0x46, 0x4a, 0x4e and the 0x10013 version tail +0x52 .. +0x6a), the sizes and vfs_fwrite calls match the original at 0x51a1a0 exactly. Shape is match-deep's. Q2/Q7: gm_PlayerRecordBytes and ls_Version unnamed; Q6 described. */


int ls_WritePlayerRecords(int f)
{
    char *p = gm_PlayerRecordBytes;

    for (;;) {
        LS_W(p - 2, 2)
        LS_W(p, 0x10)
        LS_W(p + 0x12, 4)
        LS_W(p + 0x26, 4)
        LS_W(p + 0x1a, 4)
        LS_W(p + 0x1e, 4)
        LS_W(p + 0x2a, 4)
        LS_W(p + 0x2e, 4)
        LS_W(p + 0x32, 4)
        LS_W(p + 0x36, 4)
        LS_W(p + 0x3e, 4)
        LS_W(p + 0x42, 4)
        LS_W(p + 0x46, 4)
        LS_W(p + 0x4a, 4)
        LS_W(p + 0x4e, 4)
        if (ls_Version > 0x10013) {
            LS_W(p + 0x52, 4)
            LS_W(p + 0x56, 4)
            LS_W(p + 0x5a, 4)
            LS_W(p + 0x3a, 4)
            LS_W(p + 0x62, 4)
            LS_W(p + 0x66, 4)
            LS_W(p + 0x6a, 4)
        }
        LS_W(p + 0x5e, 4)
        LS_W(p + 0x72, 0x11)
        LS_W(p + 0x86, 4)
        LS_W(p + 0x8a, 0x10)
        LS_W(p + 0x9a, 0xc)
        LS_W(p + 0xa6, 4)
        LS_W(p + 0x6e, 4)
        p += 0xac;
        if ((int)p > ((unsigned)&gm_TreeNodeCount + 0x1))
            return 1;
    }
}
/* 0x51a4b0, writes the characters (the counterpart of ls_ReadCharacters 0x51de70): the player character, the count
   of the used AICHAR records of ai_Chars (a record is used when its id is not 0xffff), the two judges and the eight
   characters of ai_KindBChars as netIds (-1 for none), then each used record field by field (the building, job and
   scene pointers as their ids, ref54 and picture less their bases 0x59a / 0x639). Returns 1, 0 when a write fails. */


int ls_WriteCharacters(int f)
{
    int none;
    int cnt;
    int model;
    int tmp;
    int i;
    AICHAR *c;

    none = -1;
    LS_W(&ai_PlayerId, 2)
    cnt = 0;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff)
            cnt++;
    }
    LS_W(&cnt, 4)
    if (ai_JudgeCatholic != 0) {
        LS_W(&ai_JudgeCatholic->netId, 4)
    } else {
        LS_W(&none, 4)
    }
    if (ai_JudgeProtestant != 0) {
        LS_W(&ai_JudgeProtestant->netId, 4)
    } else {
        LS_W(&none, 4)
    }
    for (i = 0; i < 8; i++) {
        tmp = -1;
        if (ai_KindBChars[i] != 0) {
            LS_W(&((AICHAR *)ai_KindBChars[i])->netId, 4)
        } else {
            LS_W(&tmp, 4)
        }
    }
    for (i = 0; i < 768; i++) {
        c = &ai_CharsT[i];
        if (c->id != 0xffff) {
            LS_W(&c->id, 2)
            LS_W(&c->partyId, 4)
            LS_W(&c->kind, 1)
            LS_W(&c->netId, 4)
            LS_W(&c->health, 1)
            LS_W(&c->sex, 1)
            LS_W(&c->w00a, 2)
            LS_W(&c->b00c, 1)
            LS_W(&c->b00d, 1)
            LS_W(&c->f010, 4)
            LS_W(&c->f014, 4)
            LS_W(&c->f018, 4)
            LS_W(&c->f01c, 4)
            LS_W(&c->f020, 4)
            LS_W(&c->v024, 4)
            LS_W(&c->w028, 2)
            LS_W(&c->v02c, 4)
            LS_W(c->first, 0x10)
            LS_W(c->family, 0x10)
            LS_W(&c->w050, 2)
            cnt = c->ref54 - 0x59a;
            LS_W(&cnt, 4)
            LS_W(&c->b058, 1)
            LS_W(c->relatives, 0x20)
            LS_W(&c->f07c, 4)
            LS_W(c->skills, 5)
            LS_W(c->mood, 0xa8)
            LS_W(&c->jobId, 1)
            LS_W(&c->title, 1)
            LS_W(&c->officeKind, 1)
            LS_W(&c->prevOffice, 1)
            LS_W(&c->appliedOffice, 1)
            LS_W(&c->secondOffice, 1)
            if (c->bld != 0) {
                LS_W(&c->bld->id, 4)
            } else {
                tmp = -1;
                LS_W(&tmp, 4)
            }
            if (c->bld2 != 0) {
                LS_W(&c->bld2->id, 4)
            } else {
                tmp = -1;
                LS_W(&tmp, 4)
            }
            LS_W(&c->v174, 4)
            if (c->job != 0) {
                LS_W(&c->job->id, 4)
            } else {
                tmp = -1;
                LS_W(&tmp, 4)
            }
            LS_W(&c->jobKind, 1)
            cnt = c->picture - 0x639;
            LS_W(&cnt, 4)
            LS_W(&c->v190, 4)
            LS_W(&c->money, 4)
            LS_W(&c->spent198, 4)
            LS_W(&c->wage, 4)
            LS_W(&c->v1a0, 4)
            LS_W(&c->v1a4, 4)
            LS_W(&c->v1a8, 4)
            LS_W(&c->v1ac, 4)
            LS_W(&c->b1b0, 1)
            LS_W(&c->heldRounds, 1)
            LS_W(&c->flags1b4, 0x10)
            LS_W(&c->flags1c8, 4)
            LS_W(&c->flags1cc, 4)
            LS_W(&c->v1d0, 4)
            LS_W(c->block1d4, 0x10)
            LS_W(&c->mood3Rate, 4)
            LS_W(&c->v1e8, 4)
            LS_W(&c->goodsMask, 4)
            LS_W(&c->v1f0, 4)
            LS_W(&c->b1c5, 1)
            if (c->scene != 0)
                model = c->scene->id;
            else
                model = -1;
            LS_W(&model, 4)
            LS_W(c->ownName, 0x18)
            LS_W(&c->partner, 4)
            LS_W(&c->relationBias, 1)
            LS_W(&c->b215, 1)
            LS_W(&c->b216, 1)
            LS_W(&c->b217, 1)
            LS_W(&c->mask, 1)
        }
    }
    return 1;
}
/* 0x51ad30, the index of the action queue entry p in ch_Queue, -1 for NULL (the unsigned byte distance divided by
   sizeof(CHACTION): the original's mul by the reciprocal) */


unsigned ls_ActionIndex(CHACTION *p)
{
    return p ? ((unsigned)p - (unsigned)ch_Queue) / sizeof(CHACTION) : 0xffffffff;
}
/* 0x51ad60, the action queue entry n of ch_Queue, NULL for -1 */


CHACTION *ls_ActionAt(int n)
{
    if (n == -1)
        return 0;
    return &ch_Queue[n];
}
/* 0x51ad80, the record r of a character scene (+0 / +8 .. , the names +0x170 / +0x130, the object's bounding box from +0x34, the word `id`, the 0x40 bytes of +0x64 / the 0x40 bytes of its parent's +0x68): writes the pieces; returns 1 when all was written */


int unk_Unk51ad80(char *r, int id, int f)
{
    char blank[0x40];
    char *q;

    LS_W(r, 4)
    LS_W(r + 8, 0x20)
    LS_W(r + 0x2c, 4)
    LS_W(r + 0x30, 4)
    LS_W(r + 0x8c, 2)
    LS_W(r + 0x170, 0x30)
    LS_W(r + 0x130, 0x40)
    if (*(int *)(r + 0x88) == 0 || *(char *)(*(int *)(r + 0x88) + 0x3d5) != 0) {
        int zero[7] = {0};

        LS_W(zero, 0x10)
        LS_W((char *)zero + 0xc, 0xc)
    } else {
        LS_W(*(char **)(r + 0x34) + 0x4c, 0x10)
        LS_W(*(char **)(r + 0x34) + 0x84, 0xc)
    }
    LS_W(*(char **)(r + 0x34) + 0x218, 1)
    LS_W(&id, 4)
    q = *(char **)(r + 100);
    if (q != 0) {
        LS_W(q, 0x40)
    }
    else {
        blank[0] = ch_EmptyName[0];
        memset(blank + 1, 0, 0x3f);
        LS_W(blank, 0x40)
    }
    q = *(char **)(r + 0x74);
    if (q != 0 && q != *(char **)(r + 0x70) && *(int *)(q + 0x68) != 0) {
        if (vfs_fwrite(*(char **)(q + 0x68), 1, 0x40, f) != 0x40)
            return 0;
    } else {
        blank[0] = ch_EmptyName[0];
        memset(blank + 1, 0, 0x3f);
        if (vfs_fwrite(blank, 1, 0x40, f) != 0x40)
            return 0;
    }
    return 1;
}


/* 0x51aff0, the characters' scene records and the action queue ch_Queue (CH_QUEUE CHACTION records, type != 0: used): the count and the records of the characters (0x770d04, 0x21c each) through 0x51ad80, the same for the objects of the kind 0x1d in the list 0x7e3f34, then the queue's pointers are turned into indices (ch: the character's id, prev / next through ls_ActionIndex), each record whose character (ch_List) has a body owner is written in pieces (the type byte, the vectors, the names, the 0xa0-byte union u and stop), and the pointers are restored (ch_List, ls_ActionAt). Returns 1. ROUGH: the pieces of the record copy (local_188 .. local_4) are the fields of the 0x65-word copy */
extern char *ai_Goods;              /* 0x91aaf0, the object prototypes (0x41 bytes) */
int mem_IsValid();                     /* 0x4225e0 */
/* 0x51aff0 */
int ls_WriteSceneRecords(int f)
{
    int cnt = 0, i, k, n;
    int *p;
    CHACTION *a;
    CHARACTER *c;
    int rec[0x65];
    char flag;

    for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182); p += 0x87) {
        if (*p)
            cnt++;
    }
    LS_W(&cnt, 4)
    for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182); p += 0x87) {
        if (*p != 0 && unk_Unk51ad80(*p, p[-0x60], f) == 0)
            return 0;
    }
    cnt = 0;
    {
        unsigned short *q = (unsigned short *)ai_Objects;

        for (i = 0x5008; i != 0; i--) {
            if (*(char *)(ai_Goods + *q * 0x41) == 0x1d)
                cnt++;
            q = (unsigned short *)((char *)q + 0x43);
        }
    }
    LS_W(&cnt, 4)
    for (k = 0; k < 0x14f218; k += 0x43) {
        char *q = (char *)ai_Objects + k;

        if (*(char *)(ai_Goods + *(unsigned short *)q * 0x41) == 0x1d
            && unk_Unk51ad80(*(int *)(q + 0x3b), *(int *)(q + 2), f) == 0)
            return 0;
    }
    for (i = 0; i < CH_QUEUE; i++) {
        a = &ch_Queue[i];
        if (a->type == 0) {
            memset(a, 0, sizeof(CHACTION));
        } else {
            a->ch = (CHARACTER *)a->ch->id;
            a->prev = (CHACTION *)ls_ActionIndex(a->prev);
            a->next = (CHACTION *)ls_ActionIndex(a->next);
        }
    }
    flag = 0;
    memset(rec, 0, sizeof(rec));
    for (i = 0; i < CH_QUEUE; i++) {
        a = &ch_Queue[i];
        n = (int)a->ch;
        if (n > -1 && n < CH_MAX) {
            c = ch_List[n];
            if (mem_IsValid(c) == 0)
                __asm int 3
            if (c != 0 && c->obj->owner != 0) {
                flag = a->type;
                memcpy(rec, a, sizeof(CHACTION));
            }
            LS_W(&flag, 1)
            LS_W((char *)rec + 0xc, 4)
            LS_W((char *)rec + 0x14, 4)
            LS_W((char *)rec + 0x30, 0x20)
            LS_W((char *)rec + 0x50, 0x20)
            LS_W((char *)rec + 0xf0, 0xa0)
            LS_W((char *)rec + 0x190, 1)
            LS_W((char *)rec + 0x24, 4)
            LS_W((char *)rec + 0x28, 4)
        }
    }
    for (i = 0; i < CH_QUEUE; i++) {
        a = &ch_Queue[i];
        if (a->type != 0) {
            a->ch = ch_List[(int)a->ch];
            a->prev = ls_ActionAt((int)a->prev);
            a->next = ls_ActionAt((int)a->next);
        }
    }
    return 1;
}
/* 0x51b340, writes the five stock blocks of gm_StockBlocks and the four player slots of ai_PlayerSlots: first the
   record count 0x55, then per block its four header words and its 0x55 goods records of 0x80 bytes (each the fields
   +0 (2 bytes), +4, +8, +0xc, +0x20, +0x24, +0x30, +0x34, +0x38, +0x3c (2 bytes), +0x2c), then per player slot of
   0x2f4 bytes its fields in the order of the reader 0x51f080: the name (0x20), the map position +0x40 (8), +0x48,
   +0x4c, +0x50, the currency +0x54 (2), +0x58 (8), +0x60, +0x61, the ten pairs +0x64 and the ten pairs +0xb4 (8
   bytes each), +0x104, +0x105, the four words +0x108 and +0x118, +0x128, the eleven privilege bytes +0x12c, +0x137,
   the seven bytes +0x138, +0x13f .. +0x142, the eight 0x12-byte entries +0x144, +0x1d4, +0x1d8, the 0xd0 bytes
   +0x1dc and the map offset +0x2ec (8). Returns 1, 0 when a write fails.
   @deep read whole against 0x51b340..0x51b882: the count 0x55, the five stock blocks (4 header words, 0x55
   records of 0x80 with the 11 fields +0 (2) +4 +8 +0xc +0x20 +0x24 +0x30 +0x34 +0x38 +0x3c (2) +0x2c) and
   the four player slots (every field, size and loop of 0x51b513..0x51b87c) match; instruction count equal
   (479 vs 479), 32 bytes differ. Left: only the strength-reduced record pointer's bias in the record loop
   0x51b3de..0x51b4f1 - ours add edi, 0x14 (fields at edi-4, edi, edi+4 .. edi+0x38), the original's add
   edi, 0x18 (edi-8, edi-4, edi .. edi+0x34); every lea displacement in that loop shifts by 4, nothing else
   differs. Tried: a char pointer walked by 0x80 (bias 0x10), char + j*0x80 index form and
   GMBLOCK/GMSTOCK struct index (bias 0x14), a struct pointer incremented / indexed through the
   global view (larger), the int fields as one array w[14] (no change): needs a generated variant round */


int ls_WritePlayerSlots(int f)
{
    int n;
    int j;
    char *rec;
    char *slot;
    GMBLOCK *b;

    n = 0x55;
    if (vfs_fwrite(&n, 4, 1, f) <= 0)
        return 0;
    for (n = 0; n < 5; n++) {
        b = &gm_StockBlocks[n];
        LS_W(&b->demandIndex, 4)
        LS_W(&b->demandFactor, 4)
        LS_W(&b->indexB, 4)
        LS_W(&b->factorB, 4)
        for (j = 0; j < 0x55; j++) {
            LS_W(&b->goods[j].item, 2)
            LS_W(&b->goods[j].amount, 4)
            LS_W(&b->goods[j].rate, 4)
            LS_W(&b->goods[j].v00c, 4)
            LS_W(&b->goods[j].demand, 4)
            LS_W(&b->goods[j].mean, 4)
            LS_W(&b->goods[j].baseStock, 4)
            LS_W(&b->goods[j].stock, 4)
            LS_W(&b->goods[j].price, 4)
            LS_W(&b->goods[j].marketKind, 2)
            LS_W(&b->goods[j].f2c, 4)
        }
    }
    for (n = 0; n < 4; n++) {
        slot = ai_PlayerSlots[n];
        LS_W(slot, 0x20)
        LS_W(slot + 0x40, 8)
        LS_W(slot + 0x48, 1)
        LS_W(slot + 0x4c, 4)
        LS_W(slot + 0x50, 4)
        LS_W(slot + 0x54, 2)
        LS_W(slot + 0x58, 8)
        LS_W(slot + 0x60, 1)
        LS_W(slot + 0x61, 1)
        for (j = 0, rec = slot + 0x64; j < 10; j++, rec += 8)
            LS_W(rec, 8)
        for (j = 0, rec = slot + 0xb4; j < 10; j++, rec += 8)
            LS_W(rec, 8)
        LS_W(slot + 0x104, 1)
        LS_W(slot + 0x105, 1)
        for (j = 0, rec = slot + 0x108; j < 4; j++, rec += 4)
            LS_W(rec, 4)
        for (j = 0, rec = slot + 0x118; j < 4; j++, rec += 4)
            LS_W(rec, 4)
        LS_W(slot + 0x128, 4)
        for (j = 0; j < 0xb; j++)
            LS_W(slot + 0x12c + j, 1)
        LS_W(slot + 0x137, 1)
        for (j = 0; j < 7; j++)
            LS_W(slot + 0x138 + j, 1)
        LS_W(slot + 0x13f, 1)
        LS_W(slot + 0x140, 1)
        LS_W(slot + 0x141, 1)
        LS_W(slot + 0x142, 1)
        for (j = 0, rec = slot + 0x144; j < 8; j++, rec += 0x12)
            LS_W(rec, 0x12)
        LS_W(slot + 0x1d4, 4)
        LS_W(slot + 0x1d8, 4)
        LS_W(slot + 0x1dc, 0xd0)
        LS_W(slot + 0x2ec, 8)
    }
    return 1;
}
/* 0x51b890, writes the two relation maps ai_Relations / ai_RelationMemory (0x300 x 0x300 bytes, row by row) */


int ls_WriteRelations(int f)
{
  int i;
  char *p;
  p = ((char *)ai_Relations);
  do {
    i = 0;
    do {
      LS_W(p + i, 1)
      i = i + 1;
    } while (i < 0x300);
    p = p + 0x300;
  } while ((int)p < (int)ai_Relations + 0x300 * 0x300);
  p = ((char *)ai_RelationMemory);
  do {
    i = 0;
    do {
      LS_W(p + i, 1)
      i = i + 1;
    } while (i < 0x300);
    p = p + 0x300;
  } while ((int)p < (int)ai_RelationMemory + 0x300 * 0x300);
  return 1;
}
/* 0x51b910, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer */


int ls_WriteStatistics(int f)
{
  int iVar1;
  char *puVar3;
  char *puVar4;
  char *puVar5;
  LS_W(((char *)(unsigned)&ord_Statistics), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x4)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x8)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0xc)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x10)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x14)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x18)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x1c)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x20)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x24)), 4)
  LS_W(((char *)(unsigned)&ord_RatingRate), 4)
  LS_W(((char *)(unsigned)&ord_StatisticsClock), 4)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0x4)), 2)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0x6)), 4)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0xa)), 4)
  LS_W(((char *)(unsigned)&ai_RatingBase), 4)
  LS_W(((char *)(unsigned)&ai_Rating), 4)
  LS_W(((char *)(unsigned)&ord_MessageRecord), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x4)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x8)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0xc)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x10)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x14)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x18)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x1c)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x20)), 4)
  puVar4 = ((char *)((unsigned)&ord_StatisticsHistory + 0x4));
  do {
    LS_W(puVar4 - 4, 4)
    LS_W(puVar4, 4)
    LS_W(puVar4 + 4, 4)
    LS_W(puVar4 + 8, 4)
    LS_W(puVar4 + 0xc, 4)
    puVar4 = puVar4 + 0x14;
  } while ((int)puVar4 < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x4));
  LS_W(((char *)(unsigned)&gm_IncomeHistory), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x44), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x88), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0xcc), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x110), 0x44)
  puVar3 = ((char *)((unsigned)&ai_Grid + 0x2));
  do {
    iVar1 = 0;
    puVar5 = puVar3;
    do {
      LS_W(puVar5 - 2, 2)
      LS_W(puVar5, 2)
      LS_W(puVar5 + 2, 1)
      LS_W(puVar5 + 6, 4)
      iVar1 = iVar1 + 1;
      puVar5 = puVar5 + 0x18;
    } while (iVar1 < 8);
    puVar3 = puVar3 + 0xc0;
  } while ((int)puVar3 < ((unsigned)((unsigned)ai_Grid + 0x600) + 0x2));
  return 1;
}
/* 0x51bdb0, the save game step that writes the avatar table (gv_WriteAvatars) */


int ls_WriteAvatars(void *f)
{
    return gv_WriteAvatars(f);
}
/* 0x51bdc0, the save game step that writes the event tables: the number of used he_Jobs records (kind != 0) and the
   size 0xa0 of their tail, then each used record in pieces (kind, id, chr, netId, from, the 12 words, the three
   clocks, stage, opts, the eight ids one by one, the text: its size and bytes, or a 0 when it has none, and the 0xa0
   bytes from +0xac); then the used carried-goods records (gm_CarryRecs, ref != 0: the id of the event ref points to,
   ttl, hasValue, owner, value, spec, data) and the used AP events (he_ApEvents with income, cost or a name). The
   reader is 0x51fb90. Returns 1, 0 when a write fails */
int ls_WriteEventTables(int f)
{
  int i;
  int n;
  int k;
  int zero;
  AIJOB *job;

  n = 0;
  for (i = 0; i < 0x400; i++) {
    if (he_Jobs[i].kind != 0)
      n++;
  }
  LS_W(&n, 4)
  i = 0xa0;                             /* the size of each record's tail from +0xac */
  LS_W(&i, 4)
  for (i = 0; i < 0x400; i++) {
    job = &he_Jobs[i];
    if (job->kind == 0)
      continue;
    LS_W(&job->kind, 1)
    LS_W(&job->id, 4)
    LS_W(&job->chr, 2)
    LS_W(&job->netId, 4)
    LS_W(&job->from, 4)
    LS_W(job->words14, 0x30)
    LS_W(&job->start, 0xe)
    LS_W(&job->end, 0xe)
    LS_W(&job->clock60, 0xe)
    LS_W(&job->stage, 4)
    LS_W(&job->opts, 1)
    for (k = 0; k < 8; k++) {
      LS_W(&job->ids[k], 4)
    }
    if (job->text != 0 && job->textSize != 0) {
      LS_W(&job->textSize, 4)
      LS_W(job->text, job->textSize)
    } else {
      zero = 0;
      LS_W(&zero, 4)
    }
    LS_W(&job->u, 0xa0)
  }

  n = 0;
  for (i = 0; i < 0x100; i++) {
    if (gm_CarryRecs[i].ref != 0)
      n++;
  }
  LS_W(&n, 4)
  for (i = 0; i < 0x100; i++) {
    if (gm_CarryRecs[i].ref == 0)
      continue;
    LS_W(&((AIJOB *)gm_CarryRecs[i].ref)->id, 4)
    LS_W(&gm_CarryRecs[i].ttl, 1)
    LS_W(&gm_CarryRecs[i].hasValue, 4)
    LS_W(&gm_CarryRecs[i].owner, 4)
    LS_W(&gm_CarryRecs[i].value, 4)
    LS_W(&gm_CarryRecs[i].spec, 0xe)
    LS_W(gm_CarryRecs[i].data, 0x80)
  }

  n = 0;
  for (i = 0; i < 0x100; i++) {
    if (he_ApEvents[i].income != 0 || he_ApEvents[i].cost != 0 || he_ApEvents[i].name[0] != 0)
      n++;
  }
  LS_W(&n, 4)
  for (i = 0; i < 0x100; i++) {
    if (he_ApEvents[i].income == 0 && he_ApEvents[i].cost == 0 && he_ApEvents[i].name[0] == 0)
      continue;
    LS_W(&he_ApEvents[i].counter, 1)
    LS_W(&he_ApEvents[i].key, 4)
    LS_W(&he_ApEvents[i].income, 4)
    LS_W(&he_ApEvents[i].cost, 4)
    LS_W(he_ApEvents[i].name, 0x40)
  }
  return 1;
}
/* 0x51c2f0, writes the used scene party records (cm_Parties) to the save file f: their count, then per record the
   id, kind, thing, flags, used, the 16 member netIds, the offer kind, the 0x98-byte kind payload (+0x7c), the clock
   time and the host */


int ls_WriteParties(int f)
{
  G28REC *p;
  int n;
  int i;

  n = 0;
  for (i = 0; i < 96; i++) {
    if (cm_Parties[i].id != -1)
      n++;
  }
  LS_W(&n, 4)
  for (i = 0; i < 96; i++) {
    p = &cm_Parties[i];
    if (p->id != -1) {
      LS_W(&p->id, 4)
      LS_W(&p->kind, 1)
      LS_W(&p->thing, 4)
      LS_W(&p->flags, 1)
      LS_W(&p->used, 1)
      if (vfs_fwrite(p->members, 4, 0x10, f) <= 0)
        return 0;
      LS_W(&p->offerKind, 1)
      LS_W(&p->eventId, 0x98)
      LS_W(&p->time, 0xe)
      LS_W(&p->host, 4)
    }
  }
  return 1;
}
/* 0x51c450, saves the history (hst.c) part of a save game: the texts-on byte hst_TextsOn, the offsets of the
   current text (hst_TextCurrent - hst_TextFirst) and of the next key-list entry (hst_KeyNext - hst_KeyFirst),
   the 0x10 bytes of the key hst_Key and the four 0x44-byte lists hst_Lists (used flag and eight id/value
   pairs each). Returns 1, or 0 when a write fails. */


int ls_WriteHistory(int f)
{
  int n;
  int i;
  int *list;
  int *entry;

  LS_W(&hst_TextsOn, 1)
  n = hst_TextCurrent - hst_TextFirst;
  LS_W(&n, 4)
  n = hst_KeyNext - hst_KeyFirst;
  LS_W(&n, 4)
  for (i = 0; i < 0x10; i++) {
    LS_W(hst_Key + i, 1)
  }
  /* list walks the lists by their e[0].v word (the word at +8 of each 0x44-byte record): list - 2 is the used
     flag, list - 1 and list are the id and value of each of the eight entries */
  for (list = &hst_Lists[0].e[0].v; (int)list < (int)&hst_Lists[4].e[0].v; list = list + 0x11) {
    LS_W(list - 2, 4)
    entry = list;
    for (i = 0; i < 8; i++) {
      LS_W(entry - 1, 4)
      LS_W(entry, 4)
      entry = entry + 2;
    }
  }
  return 1;
}
#define LS_MSG_INDEX(p, table) ((p) != 0 ? ((char *)(p) - (char *)(table)) / sizeof(G17MSG) : -1)
/* 0x51c560, writes cm's message queues: the 0x2000 records of cm_RecvPool and then the 0x8000 of cm_SendRing (each
   a copy of the 0x99-byte G17MSG with its prev / next pointers turned into record indices of its own table, -1 for
   none), after the first the heads cm_RecvQueue and cm_RecvFree, after the second cm_SendQueue (as indices too),
   then cm_LastSeq, the 0x8000 send states cm_SendStates (10 bytes each), the group bounds cm_GroupFirstSeq /
   cm_GroupEndSeq and the three ids cm_LastCharId / cm_LastBuildingId / cm_LastObjectId. Returns 1, 0 when a write
   fails. The reader is 0x520310. */


int ls_WriteMessageQueues(int f)
{
    int idx;
    G17MSG m;
    int i;

    for (i = 0; i < 0x2000; i++) {
        m = cm_RecvPool[i];
        idx = LS_MSG_INDEX(cm_RecvPool[i].prev, cm_RecvPool);
        m.prev = (G17MSG *)idx;
        idx = LS_MSG_INDEX(cm_RecvPool[i].next, cm_RecvPool);
        m.next = (G17MSG *)idx;
        LS_W(&m, sizeof(G17MSG))
    }
    idx = LS_MSG_INDEX(cm_RecvQueue, cm_RecvPool);
    LS_W(&idx, 4)
    idx = LS_MSG_INDEX(cm_RecvFree, cm_RecvPool);
    LS_W(&idx, 4)
    for (i = 0; i < 0x8000; i++) {
        m = cm_SendRing[i];
        idx = LS_MSG_INDEX(cm_SendRing[i].prev, cm_SendRing);
        m.prev = (G17MSG *)idx;
        idx = LS_MSG_INDEX(cm_SendRing[i].next, cm_SendRing);
        m.next = (G17MSG *)idx;
        LS_W(&m, sizeof(G17MSG))
    }
    idx = LS_MSG_INDEX(cm_SendQueue, cm_SendRing);
    LS_W(&idx, 4)
    LS_W(&cm_LastSeq, 4)
    for (i = 0; i < 0x8000; i++)
        LS_W(&cm_SendStates[i], sizeof(G17STAT))
    LS_W(&cm_GroupFirstSeq, 4)
    LS_W(&cm_GroupEndSeq, 4)
    LS_W(&cm_LastCharId, 4)
    LS_W(&cm_LastBuildingId, 4)
    return vfs_fwrite(&cm_LastObjectId, 4, 1, f) > 0;
}
/* 0x51c870, the table 0x7604e8 (three words per entry at +0x18 words apart; the last entry is followed by the two further blocks 0x5dbfc0 and 0x4ed480) @deep B3 code hunks 50 after the LS_W restructure (the draft mixed if(fwrite!=0) and direct calls; the original is the LS_W macro for all five writes): the rest is VC6's induction-variable base - the original keeps esi at the entry pointer 0x7604e8 (lea [esi-0x60]/[esi]/[esi+0x68]/[esi+0xc8], add esi,4, cmp 0x760548) while ours bases esi at 0x760550 (the +0x68 word) with offsets -0xc8/-0x68/0/+0x60, same addresses, and VC6 then merges the loop's return-0 blocks to the function end where the original reuses the first check's inline epilogue at 0x51c88c. Read the whole function (24 entries of four words, then 0x5dbfc0 and 0x4ed480); tried do-while and while spellings. Q1/Q2/Q3/Q5 open (0x7604e8 table unnamed, callees ai_SaveWind/gv_WriteEffects undeclared here) */


int ls_WriteEnvironment(int f)
{
    int *p;

    LS_W((char *)(unsigned)&sv_WinterWeather, 4)
    p = (int *)sv_Weather.storm;
    do {
        LS_W(p - 0x18, 4)
        LS_W(p, 4)
        LS_W(p + 0x1a, 4)
        LS_W(p + 0x32, 4)
        p = p + 1;
    } while ((int)p < (unsigned)((unsigned)sv_Weather.storm + 0x60));
    if (ai_SaveWind(f) != 0)
        return gv_WriteEffects(f) != 0;
    return 0;
}
/* 0x51c920, reads count items of size bytes into buf (vfs_fread, 0x430bd0, the counterpart of vfs_fwrite):
   true when the read returned a positive count */


int ls_Read(void *buf, int size, int count, int f)
{
    unsigned got = vfs_fread(buf, size, count, f);

    return got > 0;
}


LSSAVE ls_SavePath = {0};  /* 0x75e640 */
unsigned short ai_ScreenImage[19200] = {0};  /* 0x766320, the */
int ai_Unk76f920[0x40] = {0};  /* 0x76f920, the 0x100-byte header of the loaded save game (id, flag byte, name, ..; ls_ReadSaveHeader fills it up to +0xfc) */
/* 0x51c950, loads a save game: opens the file (0x430470, mode 0x5f1084), resets the game (0x503e80, 0x40b240, 0x50d490), reads the header (0x51cee0, versions 0x10026 .. 0x1005a only) and the fixed globals (each from its version: 0x6f1bcc from 0x10022, 0x61a658 from 0x10030, 0x6f1b88 from 0x1003d), then the readers of the subsystems in the order 0x51d3d0, 0x51d550, 0x51db00, 0x51de70, 0x51f080, 0x4ea7a0 (from 0x10045), 0x51d2d0, [unless the header flag +4 has bit 2 (a map only save): 0x4ea7a0, 0x4f2b30, 0x51f650 (from 0x10016), 0x51f720, 0x4ed270 (from 0x1002a), 0x51fb90, 0x520000, 0x520200, 0x520310, 0x5205f0 (from 0x1002e), 0x5206b0, then the references (0x511790 / 0x511840 / 0x51e900), the character pointers 0x6f1bfc / 0x6f1c00 (0x505df0), 0x4b63b0, 0x589b30 (from 0x10029), 0x5a8a20 (from 0x10047), 0x4ec140 (from 0x10049), 0x4ada80 (from 0x10052), 0x4c0040 (from 0x10053)]. The first one that fails closes the file, frees the buffer 0xf31140 and returns 0. At the end 0x528770, 0x511790, 0x513740, 0x513590(0x633738, 1) and the path 0x76f9bc is kept in 0x75e640. Returns 1. ROUGH */
/* ai_Unk76f924 / ai_Unk76f9b8 / ai_Unk76f9bc: views of ai_Unk76f920 (data_views.h) */
extern char * cm_LoadBuf;                /* 0xf31140 */
extern int ai_TimeCopy[4];  /* 0x75e130, the clock copy */
#define LS_RD(p, sz) ls_Read((int)(p), (sz), 1, h)
#define LS_LOADFAIL { vfs_fclose(h); mem_Free(cm_LoadBuf); cm_LoadBuf = 0; return 0; }   /* a late reader failed: the file closed, the load buffer freed */
/* 0x51c950 */
int ls_LoadGame(int name, int kind)
{
    int h;

    h = vfs_Tfopen(name, "rb", vfs_Root);
    if (h == 0)
        return 0;
    gm_ResetCharacters();
    ch_DeleteAll();
    ai_PlayerCharC = 0;
    ai_JudgeCatholic = 0;
    ai_JudgeProtestant = 0;
    init_FreeScenes();
    if (ls_ReadSaveHeader(h, ai_Unk76f920) == 0 || ai_Unk76f920[0] > 0x1005a || ai_Unk76f920[0] < 0x10026
        || !LS_RD(&ai_NextObjectId, 4) || !LS_RD(&ai_NextEventId, 4) || !LS_RD(&ai_Time, 0xe)
        || !LS_RD(&ai_OwnPlayer, 1) || !LS_RD(&ai_PlayerCharC, 4) || !LS_RD(&ai_NextCharNumber, 4)
        || !LS_RD(&ai_NextPlayerNumber, 4) || !LS_RD(&ai_CharCount, 4))
        goto close;
    if (ls_Version >= 0x10022)
        LS_RD(&ls_SpareInt, 4);
    LS_RD(&ai_Update, sizeof(AIUPDATE));   /* 0xf89580 */
    ai_Unk75e134 = *(int *)&ai_Time.hour;
    ai_TimeCopy[0] = ai_Time.year;
    ai_Unk75e138 = *(int *)((char *)&ai_Time + 8);
    ai_Unk75e13c = *(short *)((char *)&ai_Time + 0xc);
    if ((ai_Unk76f924 & 2) == 0)
        ai_Difficulty = ai_Unk76f9b8;
    if (ls_Version >= 0x10030)
        LS_RD(&ai_NextPartyId, 4);
    if (ls_Version >= 0x1003d)
        LS_RD(&ai_NextCrimeId, 4);
    else
        ai_NextCrimeId = 1000000;
    if (ls_ReadObjects(h) == 0 || ls_ReadBuildings(h) == 0 || ls_ReadPlayerRecords(h) == 0 || ls_ReadCharacters(h) == 0
        || ls_ReadPlayerSlots(h) == 0 || (ls_Version >= 0x10045 && amt_LoadAemter(h, ls_Version) == 0) || ls_LoadScene(h) == 0)
        goto close;
    if ((ai_Unk76f924 & 2) != 0) {          /* a map-only save ends here */
        vfs_fclose(h);
        ai_JudgeCatholic = 0;
        ai_JudgeProtestant = 0;
        if (ls_FixupAfterLoad() == 0)
            goto close;                     /* the original closes the file a second time on this path */
        return 1;
    }
    if ((ls_Version < 0x10045 && amt_LoadAemter(h, ls_Version) == 0) || gv_ReadCrimes(h) == 0
        || (ls_Version >= 0x10016 && ls_ReadRelations(h) == 0) || ls_ReadStatistics(h) == 0
        || (ls_Version >= 0x1002a && ls_ReadAvatars((void *)h) == 0) || ls_ReadEventTables(h) == 0
        || ls_ReadParties(h) == 0 || unk_Unk520200(h) == 0)
        goto close;
    if (unk_Unk520310(h) == 0) {
        vfs_fclose(h);
        ls_FixupAfterLoad();
        return 0;
    }
    if ((ls_Version >= 0x1002e && ls_ReadEnvironment(h) == 0) || ls_FixupAfterLoad() == 0)
        goto close;
    d3if_SelectScene(0, 1);
    init_PrepareTownScene();
    init_RecreateQuadTree();
    if (ls_ReadSceneRecords(h) == 0)
        LS_LOADFAIL
    ai_JudgeCatholic = gm_FindChar(ai_JudgeCatholic);
    ai_JudgeProtestant = gm_FindChar(ai_JudgeProtestant);
    if (ai_LoadMissions(h) == 0)
        LS_LOADFAIL
    if (ls_Version >= 0x10029 && ai_LoadBookmarks(h) == 0)
        LS_LOADFAIL
    if (ls_Version >= 0x10047 && ai_LoadSettings(h) == 0)
        LS_LOADFAIL
    if (ls_Version >= 0x10049 && ahm_LoadWalk(h) == 0)
        LS_LOADFAIL
    if (ls_Version >= 0x10052 && hmt_Load(h, ls_Version) == 0)
        LS_LOADFAIL
    if (ls_Version >= 0x10053 && rt_LoadRoutes(h) == 0)
        LS_LOADFAIL
    vfs_fclose(h);
    sv_RefreshBanners();
    init_PrepareTownScene();
    init_AfterTownLoad();
    init_SetDetailLevel(d3_Terrain, 1);
    strcpy(ls_SavePath.path, ai_Unk76f9bc);
    return 1;
close:
    vfs_fclose(h);
    return 0;
}
/* 0x51cee0, reads the header of a save game into h (an LSHEADER), each field from the version it came in
   with (the version itself goes to ls_Version); the 160 x 120 screen shot (24 bit, 0xe100 bytes) is converted
   into ai_ScreenImage. Returns 1, 0 when a read fails or the version is below 0x10025 @deep read whole against 0x51cee0..0x51d2c0 (433 vs 434 instructions): every read with its version gate, the early return 0 below 0x10025 (it keeps the 23 failed reads' inline returns apart), the reads-first branch order of 0x1002b / 0x10038 / 0x10039, the 120 x 160 shot through shot[k * 3 ..] match. Left: allocation - ours keeps &hd->difficulty in esi for the read and the default store (lea esi,[ebx+0x98] / mov [esi],2), the original recomputes it (lea edx,[ebx+0x98] / mov [ebx+0x98],2) and orders the 0x10039 test before add ebx,0x9c; the strcpy of 'Savegame' reads the original's merged literal tail 'game' (0x62366c) */


int ls_ReadSaveHeader(int f, int *h)
{
    LSHEADER *hd = (LSHEADER *)h;
    char *shot;
    int i, j, k = 0;

    shot = (char *)mem_Alloc(0xe100, "ls:tmp");
    LS_R(&hd->version, 4)
    ls_Version = hd->version;
    LS_R(&hd->flags, 1)
    LS_R(hd->name, 0x20)
    LS_R(hd->slotPair, 8)
    LS_R(&hd->slotByte, 1)
    if (ls_Version >= 0x10033)
        LS_R(&hd->numPlayers, 1)
    if (ls_Version >= 0x10028) {
        LS_R(hd->clock, 0xe)
        LS_R(hd->charText30, 0x10)
        if (ls_Version >= 0x10054) {
            LS_R(hd->charText40, 0x10)
            LS_R(&hd->charByte0c, 1)
            LS_R(&hd->charRef54, 4)
        }
        LS_R(&hd->charByte164, 1)
        LS_R(&hd->charByte166, 1)
        LS_R(&hd->charByte0d, 1)
        LS_R(&hd->worth, 4)
        LS_R(&hd->charByte09, 1)
        LS_R(&hd->saveSlot, 1)
        if (ls_Version >= 0x1002b) {
            LS_R(&hd->startBuilding, 4)
            LS_R(&hd->startObject, 4)
        } else {
            hd->startBuilding = -1;
            hd->startObject = -1;
        }
        if (ls_Version >= 0x10034) {
            for (i = 0; i < 8; i++)
                LS_R(&hd->party[i], 4)
        }
        LS_R(shot, 0xe100)
        for (j = 0x78; j != 0; j--) {               /* the 160 x 120 screen shot, 24 bit, into the 16-bit image */
            for (i = 0xa0; i != 0; i--) {
                ai_ScreenImage[k] = dd_RGB(shot[k * 3], shot[k * 3 + 1], shot[k * 3 + 2]);
                k++;
            }
        }
    }
    if (shot != 0)
        mem_Free(shot);
    if (ls_Version < 0x10025)
        return 0;
    if (ls_Version >= 0x10038)
        LS_R(&hd->difficulty, 4)
    else
        hd->difficulty = 2;
    if (ls_Version >= 0x10039)
        LS_R(hd->title, 0x60)
    else
        strcpy(hd->title, "Savegame");
    return 1;
}
/* 0x51d2d0, after a load: builds the scene (0x44be60 with the loader 0x537b60) and its height map (d3sm_Create,
   size 0x100, or 0x80 when the terrain's side is 0x40), sets the terrain's detail bits (D3FLMAP.lod, from
   init_Detail) and registers the two scene callbacks 0x51d4f0 (0x1c0) / 0x51d530 (0xc0) with 0x438e60. Returns 1
   @deep read whole against the original and rewrote it typed (d3_Scenes[0].hmap = (struct D3HMAP
   *)d3sm_Create(...) with the D3HMAP cast the other callers use, D3FLMAP.lod = init_Detail for the +0x1c70
   bits - the xor/and/xor is VC6's bitfield store, and the two d3_ForEachObject2 callbacks). Q gates
   settled (Q2 ai_Unkfd7c50 -> the d3_Scenes[0].hmap field, Q4's two raw accesses gone). 71 vs 73
   instructions, 28 differing, all scheduling: (1) the size argument of d3sm_Create - the original
   duplicates the argument pushes per branch (push 2 / push 0x80, jmp, push 2 / push 0x100) at 0x51d323,
   ours materialises the ternary in eax (mov eax,0x80 / je / mov eax,0x100) at 0x51d321 and pushes it once;
   (2) the esi save sits before that call in ours (0x51d32d) and after it in the original (0x51d341), and
   the hmap store follows the esi save instead of the call; (3) the terrain pointer in the detail block is
   eax in ours, ecx in the original (diffclass regs 6 + 4 at 0x51d35d / 0x51d370), which is also where the
   prover stops (a 1-byte store shift). Tried one build each: with and without a D3FLMAP *m local, the size
   computed in an int local first, and the declaration order - identical or unchanged. Same callees,
   arguments, constants, branches and stores on both sides. */


int ls_LoadScene(int f)
{
    int saved;

    d3if_SelectScene(1, 0);
    d3if_ClearScene(0);
    d3if_SelectScene(0, 0);
    d3_LoadScene(0, f, (void *)(unsigned)&main_LoadProgressCallback);
    d3if_SelectScene(1, 0);
    d3if_SelectScene(0, 0);
    d3_Scenes[0].hmap = (struct D3HMAP *)d3sm_Create((struct D3TERRAIN *)d3_Terrain,
                                (((D3FLMAP *)d3_Terrain)->size == 0x80 || ((D3FLMAP *)d3_Terrain)->size != 0x40) ? 0x100 : 0x80, 2);
    saved = d3_CurrentScene;
    d3if_SelectScene(0, 0);
    if (d3_Terrain != 0) {
        ((D3FLMAP *)d3_Terrain)->lod = init_Detail;
        d3fl_SetDetail(0x80);
    }
    d3_ForEachObject2(d3_DefaultScene, 0, 0x1c0, (void *)(unsigned)&ls_HookBuildingCB);
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, (void *)(unsigned)&ls_ClearObjectFlag);
    d3if_SelectScene(saved, 0);
    return 1;
}
/* 0x51d3d0, reads the object list 0x7e3f34 (after 0x4fe830 cleared it): the count, then each record in the pieces of the writer 0x519bf0; the count is kept in 0x6f1bdc. Returns 1 */


int ls_ReadObjects(int f)
{
    int n, i;
    char *r;

    gm_ClearObjects();
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        r = (char *)ai_Objects + i * 0x43;
        LS_R(r, 2)
        LS_R(r + 2, 4)
        LS_R(r + 6, 4)
        LS_R(r + 10, 4)
        LS_R(r + 0xe, 4)
        LS_R(r + 0x12, 1)
        LS_R(r + 0x13, 1)
        LS_R(r + 0x1c, 0x1f)
    }
    f3gm_ObjectCount = n;
    return 1;
}
/* 0x51d4f0, scene callback: runs init_HookObject (o, building, 2) for every used record of the 0x100 of ai_Blds
   (0xa9 bytes) whose id (+1) is the owner (+0x200) of the scene object o. Returns 1
   @deep Q4 blocked: the two remaining raw accesses (*(int *)(o + 0x200) the owner id of the scene object, *(int *)(p + 1) the building id) need typed fields - AIBLD from ai.h for the building record and a named D3OBJECT.+0x200 field from d3.h; ls.c includes ai.h now (the gv.h clash is gone), so AIBLD can type the building record, and d3.h's D3OBJECT has only char pad200[0x8] there, so the field name is a shared-header decision. Everything else done: byte match kept, Q1/Q2/Q3/Q5/Q6/Q7 pass after init_HookObject and ai_Blds */


char ls_HookBuildingCB(char *o)
{
    int i;
    char *p;

    for (i = 0; i < 0xa900; i += 0xa9) {
        p = ai_Blds + i;
        if (*p != 0 && *(int *)(o + 0x200) == *(int *)(p + 1))
            init_HookObject(o, (unsigned short *)p, 2);
    }
    return 1;
}
/* 0x51d530, scene callback: clears the flag 4 in +0x213 of the object */

char ls_ClearObjectFlag(char *o)
{
    o[0x213] &= ~4;
    return 1;
}
/* 0x51d550, reads the buildings ai_Blds from the save file f (the counterpart of the writer 0x519d20): for each of n records the fields (+0 .. +0x5c), for the type 0x1e the plant map ("f3g:PlantMap", 0x600 bytes of 0x18-byte plants, its pointer at +0x71), for the types 0x48 .. 0x4a / 0x4b .. 0x4d the type specific record at +0x65 (read by gv_ReadGraveyard / ahm_ReadRecord, or built by gv_AllocGraveyard / a new "AHM_INFO" record for the versions below 0x10047 / 0x10049), then the 0x10 flag bytes at +0x99 (for the versions below 0x10043: zero, 0xffffffff at +0xa5, bit 0 set) and the fixed +0x30 = 0x1388 and +0x95 = -1; at the end the building sites ai_Bauplaetze (ai_NBauplaetze records of 0x60 bytes). Returns 1, 0 when a read fails. */

int ls_ReadBuildings(int f)
{
    int n, k, off = 0, i, r;
    char *p;
    char *q;
    int *pi;
    unsigned *flags;
    int dummy;

    gm_ClearBuildings();
    LS_R(&n, 4)
    for (k = 0; k < n; k++) {
        p = ai_Blds + off;
        LS_R(p, 1)
        LS_R(p + 1, 4)
        LS_R(p + 5, 0x20)
        LS_R(p + 0x25, 2)
        LS_R(p + 0x27, 2)
        LS_R(p + 0x29, 2)
        LS_R(p + 0x2b, 4)
        LS_R(p + 0x2f, 1)
        LS_R(p + 0x34, 1)
        LS_R(p + 0x35, 4)
        LS_R(p + 0x39, 4)
        LS_R(p + 0x3d, 4)
        pi = (int *)(p + 0x41);
        LS_R(pi, 4)
        if (*pi > 4)
            *pi = 2;
        LS_R(p + 0x45, 4)
        if (ls_Version >= 0x10028)
            LS_R(p + 0x49, 4)
        if (ls_Version < 0x10032)
            LS_R(&dummy, 4)
        if (ls_Version >= 0x10057) {
            LS_R(p + 0x4d, 4)
            LS_R(p + 0x51, 4)
            LS_R(p + 0x55, 4)
        }
        LS_R(p + 0x5a, 2)
        LS_R(p + 0x5c, 1)
        if (*p == 0x1e) {
            *(char **)(p + 0x71) = mem_Alloc(0x600, "f3g:PlantMap");
            for (i = 0; i < 0x600; i += 0x18) {
                LS_R(*(char **)(p + 0x71) + i, 4)
                LS_R(*(char **)(p + 0x71) + i + 4, 4)
                LS_R(*(char **)(p + 0x71) + i + 8, 1)
                LS_R(*(char **)(p + 0x71) + i + 9, 1)
                LS_R(*(char **)(p + 0x71) + i + 10, 2)
                LS_R(*(char **)(p + 0x71) + i + 0x10, 4)
                LS_R(*(char **)(p + 0x71) + i + 0xc, 1)
                LS_R(*(char **)(p + 0x71) + i + 0xd, 1)
                *(int *)(*(char **)(p + 0x71) + i + 0x14) = 0;
            }
        } else {
            if (*p == 'H' || *p == 'I' || *p == 'J') {
                if (ls_Version < 0x10047) {
                    LS_R(p + 0x65, 0x30)
                    *(int *)(p + 0x65) = (int)gv_AllocGraveyard(p);
                    goto tail;
                }
                r = gv_ReadGraveyard(f, p);
            } else if (*p == 'K' || *p == 'L' || *p == 'M') {
                if (ls_Version < 0x10049) {
                    int *a;

                    LS_R(p + 0x65, 0x30)
                    a = (int *)mem_Alloc(0x3c, "AHM_INFO");
                    *(int **)(p + 0x65) = a;
                    *a = -1;
                    goto tail;
                }
                r = ahm_ReadRecord(f, p);
            } else {
                r = ls_Read((int)(p + 0x65), 0x30, 1, f);
            }
            if (r == 0)
                return 0;
        }
tail:
        flags = (unsigned *)(p + 0x99);
        if (ls_Version < 0x10043) {
            memset(p + 0x99, 0, 0x10);
            p[0xa5] = -1;
            p[0xa6] = -1;
            p[0xa7] = -1;
            p[0xa8] = -1;
            *flags |= 1;
        } else {
            LS_R(flags, 0x10)
        }
        p[0x30] = 0x88;
        p[0x31] = 0x13;
        p[0x32] = 0;
        p[0x33] = 0;
        p[0x95] = -1;
        p[0x96] = -1;
        p[0x97] = -1;
        p[0x98] = -1;
        off += 0xa9;
    }
    LS_R(&ai_NBauplaetze, 4)
    ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, ai_NBauplaetze * 0x60);
    for (i = 0; i < ai_NBauplaetze; i++) {
        LS_R(ai_Bauplaetze[i].name, 0x40)
        LS_R(&ai_Bauplaetze[i].x0, 0x10)
        LS_R(&ai_Bauplaetze[i].x1, 0x10)
    }
    return 1;
}
/* 0x51db00, reads the 16 player records gm_PlayerRecords (0xac bytes each; the counterpart of the writer 0x51a1a0): the fields, each version gated (+0x26 from 0x1002c, +0x52 .. +0x6a from 0x10014, +0x5e = 1.0 below 0x10056, the 0x11 (0x0e below 0x10046) name bytes at +0x72 from 0x10015, +0x6e from 0x10018). Returns 1, 0 when a read fails. */


int ls_ReadPlayerRecords(int f)
{
    char *p = gm_PlayerRecordBytes;
    int i, n;

    do {
        LS_R(p - 2, 2)
        LS_R(p, 0x10)
        LS_R(p + 0x12, 4)
        if (ls_Version >= 0x1002c)
            LS_R(p + 0x26, 4)
        LS_R(p + 0x1a, 4)
        LS_R(p + 0x1e, 4)
        LS_R(p + 0x2a, 4)
        LS_R(p + 0x2e, 4)
        LS_R(p + 0x32, 4)
        LS_R(p + 0x36, 4)
        LS_R(p + 0x3e, 4)
        LS_R(p + 0x42, 4)
        LS_R(p + 0x46, 4)
        LS_R(p + 0x4a, 4)
        LS_R(p + 0x4e, 4)
        if (ls_Version >= 0x10014) {
            LS_R(p + 0x52, 4)
            LS_R(p + 0x56, 4)
            LS_R(p + 0x5a, 4)
            LS_R(p + 0x3a, 4)
            LS_R(p + 0x62, 4)
            LS_R(p + 0x66, 4)
            LS_R(p + 0x6a, 4)
        }
        if (ls_Version >= 0x10056) {
            LS_R(p + 0x5e, 4)
        } else {
            *(int *)(p + 0x5e) = 0x3f800000;
        }
        if (ls_Version >= 0x10015) {
            n = (((ls_Version >= 0x10046) - 1) & 0xfffffffd) + 0x11;
            for (i = 0; i < n; i++)
                LS_R(p + i + 0x72, 1)
        }
        LS_R(p + 0x86, 4)
        LS_R(p + 0x8a, 0x10)
        LS_R(p + 0x9a, 0xc)
        LS_R(p + 0xa6, 4)
        if (ls_Version >= 0x10018)
            LS_R(p + 0x6e, 4)
        p += 0xac;
    } while ((int)p < (int)((char *)gm_PlayerRecords + 0xac2));  /* one past the 16th record (gm_PlayerRecordBytes + 0xac0; a byte offset, the array is int[]); the original's literal 0x7d6a42 is an absolute address that does not hold in the linked image, where it made the loop read ~70 records and the .cty load fail */
    return 1;
}
/* 0x51de70, reads the characters (the counterpart of the header writer 0x5194c0 and the character writer 0x51a4b0): ai_PlayerId, ai_CharCount, the two judge pointers ai_JudgeCatholic / ai_JudgeProtestant and the eight slots of ai_KindBChars (from 0x10016), then ai_CharCount records of the 0x21c bytes of ai_Chars (the id word first, it picks the record): each field from its version (the reference fields +0x54 / +0x18c get their base 0x59a / 0x639 added, +0x174 is 2 instead of 4 below 0x10020), some fields with their defaults for older versions (+0x20c = -1 and +0x190 = 4 below 0x1003e, +0x1cc = 0 below 0x10049); the last chunk +0x210 .. +0x218 from 0x10036. Returns 1, 0 when a read fails. */


int ls_ReadCharacters(int f)
{
    int *ref;               /* the reference field read (+0x54 / +0x18c): its saved index plus the base */
    int v, ok;
    unsigned idx;
    int *slots;
    int sz, k, cnt;
    unsigned id;            /* the id word read at the start of a record */
    int word;
    char *c;

  LS_R(&ai_PlayerId, 2)
  LS_R(&cnt, 4)
  ai_CharCount = cnt;
  LS_R(&ai_JudgeCatholic, 4)
  LS_R(&ai_JudgeProtestant, 4)
  if (0x10016 < ls_Version) {
    slots = ai_KindBChars;
    do {
      LS_R(slots, 4)
      slots = slots + 1;
    } while (slots < ai_KindBChars + 8);
  }
  k = 0;
  if (0 < cnt) {
    do {
      ok = ls_Read((int)&id, 2, 1, f);
      v = ls_Version;
      if (ok == 0) {
        return 0;
      }
      idx = id & 0xffff;
      c = ai_Chars[idx];
      *(short *)c = (short)id;
      if (v < 0x1003e) {
        *(int *)(c + 0x20c) = 0xffffffff;
      }
      else {
        LS_R((c + 0x20c), 4)
      }
      LS_R((c + 0x2), 1)
      LS_R((c + 0x4), 4)
      LS_R((c + 0x8), 1)
      LS_R((c + 0x9), 1)
      LS_R((c + 0xa), 2)
      LS_R((c + 0xc), 1)
      LS_R((c + 0xd), 1)
      LS_R((c + 0x10), 4)
      LS_R((c + 0x14), 4)
      LS_R((c + 0x18), 4)
      LS_R((c + 0x1c), 4)
      LS_R((c + 0x20), 4)
      LS_R((c + 0x24), 4)
      if (0x1003a < ls_Version) {
        LS_R((c + 0x28), 2)
        LS_R((c + 0x2c), 4)
      }
      LS_R((c + 0x30), 0x10)
      if ((0x10030 < ls_Version) &&
         ls_Read((int)((c + 0x40)), 0x10, 1, f) == 0) {
        return 0;
      }
      LS_R((c + 0x50), 2)
      ref = (int *)(c + 0x54);
      LS_R(ref, 4)
      *ref = *ref + 0x59a;
      LS_R((c + 0x58), 1)
      LS_R((c + 0x5c), 0x20)
      if ((0x10023 < ls_Version) &&
         ls_Read((int)((c + 0x7c)), 4, 1, f) == 0) {
        return 0;
      }
      LS_R((c + 0x80), 5)
      LS_R((c + 0x88), 0xa8)
      LS_R((c + 0x164), 1)
      LS_R((c + 0x165), 1)
      LS_R((c + 0x166), 1)
      LS_R((c + 0x167), 1)
      LS_R((c + 0x168), 1)
      LS_R((c + 0x169), 1)
      *(int *)(c + 0x16c) = 0;
      LS_R((c + 0x16c), 4)
      *(int *)(c + 0x170) = 0;
      LS_R((c + 0x170), 4)
      if (ls_Version < 0x10020) {
        sz = 2;
      }
      else {
        sz = 4;
      }
      LS_R((c + 0x174), sz)
      *(int *)(c + 0x17c) = 0;
      LS_R((c + 0x17c), 4)
      LS_R((c + 0x180), 1)
      ref = (int *)(c + 0x18c);
      LS_R(ref, 4)
      *ref = *ref + 0x639;
      LS_R((c + 0x190), 4)
      if (ls_Version < 0x1003e) {
        *(int *)((c + 0x190)) = 4;
      }
      LS_R((c + 0x194), 4)
      LS_R((c + 0x198), 4)
      LS_R((c + 0x19c), 4)
      LS_R((c + 0x1a0), 4)
      LS_R((c + 0x1a4), 4)
      LS_R((c + 0x1a8), 4)
      LS_R((c + 0x1ac), 4)
      LS_R((c + 0x1b0), 1)
      LS_R((c + 0x1b1), 1)
      LS_R((c + 0x1b4), 0x10)
      LS_R((c + 0x1c8), 4)
      if (ls_Version < 0x10049) {
        *(int *)(c + 0x1cc) = 0;
      }
      else {
        LS_R((c + 0x1cc), 4)
      }
      LS_R((c + 0x1d0), 4)
      LS_R((c + 0x1d4), 0x10)
      LS_R((c + 0x1e4), 4)
      LS_R((c + 0x1e8), 4)
      LS_R((c + 0x1ec), 4)
      if ((0x10029 < ls_Version) &&
         ls_Read((int)((c + 0x1f0)), 4, 1, f) == 0) {
        return 0;
      }
      if ((0x10020 < ls_Version) &&
         ls_Read((int)((c + 0x1c5)), 1, 1, f) == 0) {
        return 0;
      }
      LS_R(&word, 4)
      *(int *)(c + 0x184) = word;
      if ((0x10020 < ls_Version) &&
         ls_Read((int)((c + 0x1f4)), 0x18, 1, f) == 0) {
        return 0;
      }
      if (0x10035 < ls_Version) {
        LS_R((c + 0x210), 4)
        LS_R((c + 0x214), 1)
        LS_R((c + 0x215), 1)
        LS_R((c + 0x216), 1)
        LS_R((c + 0x217), 1)
        LS_R((c + 0x218), 1)
      }
      k = k + 1;
    } while (k < cnt);
  }
  return 1;
}
/* 0x51e750, reads one ch.c CHARACTER record from the save (allocated by ch_AllocAt from the id read first): its name (+8, 0x20 bytes), the group / script group (+0x2c / +0x30), the flag word +0x8c (bit 5 set when the flag byte read is 1), the full name +0x170 (0x30) and the body name +0x130 (0x40), +0x38 (0x10), +0x48 (0xc), the texts +0x1a8 (from 0x10013) and +0x1e8 (from 0x1005a), the flags &= 0xdffb, then the reference index read last goes to +0x12c (0x51e900 turns it into a pointer). Returns the record */


struct CHARACTER *ls_ReadCharacter(int f)
{
    int id, v;
    CHARACTER *c;
    char b;

    LS_R(&id, 4)
    c = ch_AllocAt(id);
    LS_R(c->name, 0x20)
    LS_R(&c->group, 4)
    LS_R(&c->scriptGroup, 4)
    LS_R(&c->flags.all, 2)
    LS_R(c->fullname, 0x30)
    LS_R(c->body, 0x40)
    LS_R(&c->pos, 0x10)
    LS_R(&c->rot, 0xc)
    LS_R(&b, 1)
    if (b == 1)
        c->flags.bytes.lo |= 0x20;
    LS_R(&id, 4)
    if (ls_Version >= 0x10013)
        LS_R(c->handObject, 0x40)
    if (ls_Version >= 0x1005a)
        LS_R(c->sitAnim, 0x40)
    v = id;
    c->flags.all &= 0xdffb;
    c->owner = (void *)v;
    return c;
}
/* 0x51e900, reads what ls_WriteSceneRecords writes: two lists of scene characters (ls_ReadCharacter; the owner id of
   the first turned back into its AICHAR with gm_FindChar, of the second into its AIOBJ with gm_Query), then for every
   scene character of ch_List: the universe of its building / object (group / scriptGroup) is selected, its mesh name
   "%s_%s" (fullname, body; "hozfaeller" is a misspelling in the data, made "holzfaeller_%s") is looked up as
   "*%s.bgf" and, when missing, taken from the owner's name record (gm_CharNameRecord, cut at its last '_'), the mesh
   is made (ch_CreateMesh), the ids go back into group / scriptGroup, the hand object and sit animation are set, and
   the body is hooked up with its owner: a character (ai_Chars) gets "sp_<netId>", the owner and pick id on the body
   and the hand object of its good (cm_UpdateHandObject); an object gets "sp_<objId>" and, for the carts 0x134 /
   0x135 / 0x136, a cart model "ob_KARREN_klein / mittel / GROSS" with the texture set of its keeper; hidden
   characters stay hidden. Then the action records of ch_Queue (0x300, from version 0x10044 0x500, from 0x10058 2000) and their links:
   the character, prev / next and the function of the action type; an action without prev becomes its character's
   queue head ("invalid stockptr" without one), '3' actions get their scene universe, walks are reset; an action
   with a broken link ("invalid next or prev-action pointer") is unlinked and cleared. Returns 1, 0 when a read
   fails.
   @deep read whole against 0x51e900..0x51f07b and rewritten on the typed records (CHARACTER pos / rot /
   owner / handObject / sitAnim, D3OBJECT pickId / owner, AICHAR jobKind / ref54 / ownName, AIOBJ sceneObj,
   CHACTION u.scene): the two ls_ReadCharacter lists, the ch_List pass (universe, mesh name, hozfaeller
   fix, gm_CharNameRecord fallback, ch_CreateMesh, hand object / sit anim, the character vs object owner
   with the original's redundant owner re-test at 0x51ec3a, the cart models and texture set), the
   action-record read (do-while; count 0x300 / 0x500 from 0x10044 / 2000 from 0x10058 - the draft had 0x300
   for the middle range, a meaning fix) and the link pass (the type held in a local as the original's bl,
   universe stored args[0] then u.scene.universe) all compared statement by statement; 619 vs 621
   instructions. Left: allocation only - the ch_List loop pointer lives in [esp+0x14] and is reloaded into
   edx at 0x51e9d6 (ours keeps it in eax), the original keeps the zero in ebp and the type in bl in the
   link pass (ours: zero in ebx, the type spilled to [esp+0x18], the offset counter in ebp instead of
   [esp+0x14] / edi, the 2 missing instructions), the eax/ecx/edx rotation through 0x51ea56..0x51ecd3 and
   0x51ed71..0x51eff4 (e.g. ours mov ecx,[esi+0x34]; lea eax,[esi+0x48] vs the original's mov
   edx,[esi+0x34]; lea ecx,[esi+0x48]), and the slot swaps [esp+0x34]/[esp+0x38], [esp+0x24]/[esp+0x28],
   [esp+0x14]/[esp+0x1c] */


int ls_ReadSceneRecords(int f)
{
    int n, i, j, saved, uni;
    CHARACTER *ch;
    AIBLD *bld;
    AIOBJ *part;
    char *u;
    void *own;
    AICHAR *c;
    AIOBJ *o;
    AICHAR *keeper;
    D3OBJECT *cart;
    unsigned char colour;
    unsigned char type;
    CHACTION *a;
    char buf[0x80], file[0x104], path[0x104];

    n = 0;
    saved = d3_CurrentScene;
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        ch = ls_ReadCharacter(f);
        ch->owner = gm_FindChar((int)ch->owner);
    }
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        ch = ls_ReadCharacter(f);
        ch->owner = gm_Query(0, 2, 7, 1, (int)ch->owner);
    }
    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch == 0)
            continue;
        part = 0;
        bld = (AIBLD *)gm_BuildQuery(1, 1, ch->group);
        if (bld != 0) {
            part = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 1, ch->scriptGroup);
            if (part != 0 && part->id != 0xfd)
                uni = init_ObjectSceneUniverse((unsigned short *)part);
            else
                uni = init_BuildingSceneUniverse((unsigned char *)bld);
        } else {
            uni = 0;
        }
        d3if_SelectScene(uni, 1);
        sprintf(buf, "%s_%s", ch->fullname, ch->body);
        if (strstr(buf, "hozfaeller") != 0)
            sprintf(buf, "holzfaeller_%s", ch->body);
        sprintf(file, "*%s.bgf", buf);
        _strupr(file);
        if (vfs_GetPath(file, vfs_Root, vfs_Root, path) == 0) {
            c = ch->owner;
            c->ownName[0] = 0;
            strcpy(ch->fullname, (char *)gm_CharNameRecord(c->id) + 4);
            u = strrchr(ch->fullname, '_');
            if (u != 0) {
                strcpy(ch->body, u + 1);
                *u = 0;
            }
            sprintf(buf, "%s_%s", ch->fullname, ch->body);
        }
        ch_CreateMesh(ch, buf, &ch->pos);
        if (bld != 0)
            ch->group = bld->id;
        if (part != 0)
            ch->scriptGroup = part->objId;
        if (ch->obj == 0)
            continue;
        if (ch->handObject[0] != 0)
            ch_SetHandObject(ch, 2, ch->handObject);
        if (ch->sitAnim[0] != 0) {
            u = strrchr(ch->sitAnim, '_');
            if (u != 0)
                *u = 0;
            ch_SetSitAnim(ch, ch->sitAnim);
        }
        own = ch->owner;
        if (own != 0 && (char *)own >= ai_Chars[0] && (char *)own < ai_Chars[768]) {
            c = own;
            d3_SetObjectRot(ch->obj, &ch->rot);
            sprintf(ch->obj->name, "sp_%i", c->netId);
            ch->obj->owner = c;
            ch->obj->pickId = c->id | 0x3000000;
            c->model = (AITASK *)ch;
            if (c->job != 0)
                c->jobKind = c->job->kind;
            cm_UpdateHandObject((char *)c);
        } else if (ch->owner != 0) {
            o = own;
            init_HookObject((char *)ch->obj, own, 1);
            sprintf(ch->obj->name, "sp_%i", o->objId);
            o->sceneObj = ch;
            if (o->id == 0x134)
                cart = d3_CreateObject(0, &ch->pos, &ch->rot, "ob_KARREN_klein");
            else if (o->id == 0x135)
                cart = d3_CreateObject(0, &ch->pos, &ch->rot, "ob_KARREN_mittel");
            else if (o->id == 0x136)
                cart = d3_CreateObject(0, &ch->pos, &ch->rot, "ob_KARREN_GROSS");
            else
                cart = 0;
            if (cart != 0) {
                keeper = (AICHAR *)gm_ObjectOwnerChar(own);
                if (keeper != 0 && keeper->ref54 != 0 && (keeper->kind == 5 || keeper->kind == 6 || keeper->kind == 7))
                    colour = (unsigned char)keeper->ref54 + 'g';
                else
                    colour = 0;
                d3if_SelectTextureSet(cart, (struct D3SURF *)cart->info, colour, 1);
                ch_AttachTransport(ch, cart);
                cart->mask = 1;
                init_HookObject((char *)cart, own, 1);
            }
        }
        if ((ch->flags.bytes.lo & 0x20) != 0)
            ch_SetVisible(ch, 0);
    }
    d3if_SelectScene(saved, 1);
    i = 0;
    ch_Queue[0].func = 0;
    if (ls_Version < 0x10044)
        n = 0x300;
    else
        n = ls_Version < 0x10058 ? 0x500 : CH_QUEUE;
    do {
        a = &ch_Queue[i];
        memset(a, 0, sizeof(CHACTION));
        LS_R(&type, 1)
        a->type = type;
        LS_R(&a->count, 4)
        LS_R(&a->ch, 4)
        LS_R(&a->args[0], 0x20)
        LS_R(&a->args[8], 0x20)
        LS_R(&a->u, 0xa0)
        LS_R(&a->stop, 1)
        LS_R(&a->prev, 4)
        LS_R(&a->next, 4)
        i++;
    } while (i < n);
    for (i = 0; i < CH_QUEUE; i++) {
        a = &ch_Queue[i];
        type = a->type;
        if (type != 0) {
            a->ch = ch_List[(int)a->ch];
            if (a->ch == 0) {
                memset(a, 0, sizeof(CHACTION));
                continue;
            }
            a->prev = ls_ActionAt((int)a->prev);
            a->next = ls_ActionAt((int)a->next);
            a->func = ch_ActDefs[type].func;
            a->count = 0;
            if (a->prev == 0) {
                if (a->ch != 0) {
                    if (a->ch->actions != 0)
                        ch_ClearActions(a->ch);
                    a->ch->actions = a;
                } else {
                    sprintf(path, "ls_LoadCharacter(): invalid stockptr for action-type:%i (character per...)", type);
                    a->type = 0;
                }
            }
            if (a->type == '3') {
                bld = (AIBLD *)gm_BuildQuery(1, 1, a->args[1]);
                if (bld != 0) {
                    part = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 1, a->args[2]);
                    if (part != 0 && part->id != 0xfd)
                        a->u.scene.universe = a->args[0] = init_ObjectSceneUniverse((unsigned short *)part);
                    else
                        a->u.scene.universe = a->args[0] = init_BuildingSceneUniverse((unsigned char *)bld);
                } else {
                    a->u.scene.universe = a->args[0] = 0;
                }
            } else if (a->type == '-') {
                a->u.walk.retryAt = -1;
                a->u.walk.path = 0;
            }
        }
        if ((int)a->prev == -1 || (int)a->next == -1) {
            sprintf(file, "ls_LoadCharacter(): invalid next or prev-action pointer for action-type:%i", a->type);
            a->prev = 0;
            a->next = 0;
            for (j = 0; j < CH_QUEUE; j++) {
                if (ch_Queue[j].next == a)
                    ch_Queue[j].next = 0;
                if (ch_Queue[j].prev == a)
                    ch_Queue[j].prev = 0;
            }
            memset(a, 0, sizeof(CHACTION));
        }
    }
    return 1;
}
/* 0x51f080, reads what ls_WritePlayerSlots writes: the record count of the stock blocks (from version 0x10048, else
   0x3e), the five blocks of gm_StockBlocks (four header words, then that many goods records with the fields +0 (2
   bytes), +4, +8, +0xc, +0x20, +0x24, +0x30, +0x34, +0x38, +0x3c (2 bytes), +0x2c), then the four player slots of
   ai_PlayerSlots field by field (the map offset +0x2ec from version 0x10037). Finally the town text of the first
   slot ("_STADTAUSWAHL_<name>_INFO+0", upper case, txt_FindKey) is copied to its +0x20, the slot's own name when
   there is none. Returns 1, 0 when a read fails.
   @deep read whole against 0x51f080..0x51f646: the count (0x3e, read from 0x10048), the five stock blocks
   (4 header words, n records with the 11 fields +0 (2) +4 +8 +0xc +0x20 +0x24 +0x30 +0x34 +0x38 +0x3c (2)
   +0x2c), the four player slots (every field, size and loop, +0x2ec from 0x10037) and the _STADTAUSWAHL_
   town text lookup match; 508 vs 508 instructions, 32 bytes differ. Left: only the strength-reduced record
   pointer's bias in the record loop 0x51f12c..0x51f24c - ours lea edi, [ebx + 0xc] (blk+0x14: fields at
   edi-4 .. edi+0x38), the original's lea edi, [ebx + 0x10] (blk+0x18: edi-8 .. edi+0x34); the same bias as
   ls_WritePlayerSlots 0x51b340 (same record type, same tries there: char pointer, index form, struct
   index, int array): needs a generated variant round */


int ls_ReadPlayerSlots(int f)
{
    int n;
    int b, j, k;
    GMBLOCK *blk;
    char *slot;
    char buf[0x100];

    n = 0x3e;
    if (ls_Version >= 0x10048)
        LS_R(&n, 4)
    for (b = 0; b < 5; b++) {
        blk = &gm_StockBlocks[b];
        LS_R(&blk->demandIndex, 4)
        LS_R(&blk->demandFactor, 4)
        LS_R(&blk->indexB, 4)
        LS_R(&blk->factorB, 4)
        for (j = 0; j < n; j++) {
            LS_R(&blk->goods[j].item, 2)
            LS_R(&blk->goods[j].amount, 4)
            LS_R(&blk->goods[j].rate, 4)
            LS_R(&blk->goods[j].v00c, 4)
            LS_R(&blk->goods[j].demand, 4)
            LS_R(&blk->goods[j].mean, 4)
            LS_R(&blk->goods[j].baseStock, 4)
            LS_R(&blk->goods[j].stock, 4)
            LS_R(&blk->goods[j].price, 4)
            LS_R(&blk->goods[j].marketKind, 2)
            LS_R(&blk->goods[j].f2c, 4)
        }
    }
    for (b = 0; b < 4; b++) {
        slot = ai_PlayerSlots[b];
        LS_R(slot, 0x20)
        LS_R(slot + 0x40, 8)
        LS_R(slot + 0x48, 1)
        LS_R(slot + 0x4c, 4)
        LS_R(slot + 0x50, 4)
        LS_R(slot + 0x54, 2)
        LS_R(slot + 0x58, 8)
        LS_R(slot + 0x60, 1)
        LS_R(slot + 0x61, 1)
        for (j = 0; j < 10; j++)
            LS_R(slot + 0x64 + j * 8, 8)
        for (j = 0; j < 10; j++)
            LS_R(slot + 0xb4 + j * 8, 8)
        LS_R(slot + 0x104, 1)
        LS_R(slot + 0x105, 1)
        for (j = 0; j < 4; j++)
            LS_R(slot + 0x108 + j * 4, 4)
        for (j = 0; j < 4; j++)
            LS_R(slot + 0x118 + j * 4, 4)
        LS_R(slot + 0x128, 4)
        for (j = 0; j < 0xb; j++)
            LS_R(slot + 0x12c + j, 1)
        LS_R(slot + 0x137, 1)
        for (j = 0; j < 7; j++)
            LS_R(slot + 0x138 + j, 1)
        LS_R(slot + 0x13f, 1)
        LS_R(slot + 0x140, 1)
        LS_R(slot + 0x141, 1)
        LS_R(slot + 0x142, 1)
        for (j = 0; j < 8; j++)
            LS_R(slot + 0x144 + j * 0x12, 0x12)
        LS_R(slot + 0x1d4, 4)
        LS_R(slot + 0x1d8, 4)
        LS_R(slot + 0x1dc, 0xd0)
        if (ls_Version >= 0x10037)
            LS_R(slot + 0x2ec, 8)
    }
    sprintf(buf, "_STADTAUSWAHL_%s_INFO+0", ai_PlayerSlots[0]);
    _strupr(buf);
    k = txt_FindKey(buf);
    if (k != -1)
        strcpy(ai_PlayerSlots[0] + 0x20, txt_Ptrs[k]);
    else
        strcpy(ai_PlayerSlots[0] + 0x20, ai_PlayerSlots[0]);
    return 1;
}
/* 0x51f650, reads the two relation maps ai_Relations / ai_RelationMemory (n x n bytes, a row of 0x300: n = 0x100 below 0x1003a, 0x100 below 0x10042 else 0x300; the second map from 0x10036). Returns 1 */


int ls_ReadRelations(int f)
{
    int n, i, j;
    char *p;

    if (ls_Version < 0x1003a)
        n = 0x100;
    else
        n = ((ls_Version >= 0x10042) - 1 & 0xffffff00) + 0x300;
    i = 0;
    if (n > 0) {
        p = (char *)ai_Relations;
        do {
            j = 0;
            do {
                LS_R(j + (int)p, 1)
                j++;
            } while (j < n);
            p += 0x300;
            i++;
        } while (i < n);
    }
    if (ls_Version >= 0x10036 && (i = 0, n > 0)) {
        p = (char *)ai_RelationMemory;
        do {
            j = 0;
            do {
                LS_R(j + (int)p, 1)
                j++;
            } while (j < n);
            p += 0x300;
            i++;
        } while (i < n);
    }
    return 1;
}
/* 0x51f720, reads what ls_WriteStatistics writes: the ten words of ord_Statistics, ord_RatingRate, the
   statistics clock (a word, a short and two words), ai_RatingBase, ai_Rating, the nine words of ord_MessageRecord,
   the 16 records of five words of ord_StatisticsHistory, the five 0x44-byte rows of gm_IncomeHistory and the
   8 x 8 cells of ai_Grid (a, b, used, average). Returns 1, 0 when a read fails. */


int ls_ReadStatistics(int f)
{
    int i, j;

    LS_R(&ord_Statistics.col[0], 4)
    LS_R(&ord_Statistics.col[1], 4)
    LS_R(&ord_Statistics.col[2], 4)
    LS_R(&ord_Statistics.col[3], 4)
    LS_R(&ord_Statistics.chars, 4)
    LS_R(&ord_Statistics.rating, 4)
    LS_R(&ord_Statistics.base, 4)
    LS_R(&ord_Statistics.sumC, 4)
    LS_R(&ord_Statistics.sumA, 4)
    LS_R(&ord_Statistics.score, 4)
    LS_R(&ord_RatingRate, 4)
    LS_R(&ord_StatisticsClock.year, 4)
    LS_R(&ord_StatisticsClock.hour, 2)
    LS_R(&ord_StatisticsClock.min, 4)
    LS_R(&ord_StatisticsClock.sec, 4)
    LS_R(&ai_RatingBase, 4)
    LS_R(&ai_Rating, 4)
    LS_R(&ord_MessageRecord.enabled, 4)
    LS_R(&ord_MessageRecord.count, 4)
    LS_R(&ord_MessageRecord.time, 4)
    LS_R(&ord_MessageRecord.base, 4)
    LS_R(&ord_MessageRecord.scale, 4)
    LS_R(&ord_MessageRecord.value, 4)
    LS_R(&ord_MessageRecord.limit, 4)
    LS_R(&ord_MessageRecord.phase, 4)
    LS_R(&ord_MessageRecord.stage, 4)
    for (i = 0; i < 16; i++) {
        LS_R(&ord_StatisticsHistory[i].noFirst, 4)
        LS_R(&ord_StatisticsHistory[i].combined, 4)
        LS_R(&ord_StatisticsHistory[i].noSecond, 4)
        LS_R(&ord_StatisticsHistory[i].score, 4)
        LS_R(&ord_StatisticsHistory[i].base, 4)
    }
    LS_R(gm_IncomeHistory, 0x44)
    LS_R(gm_IncomeHistory + 0x44, 0x44)
    LS_R(gm_IncomeHistory + 0x88, 0x44)
    LS_R(gm_IncomeHistory + 0xcc, 0x44)
    LS_R(gm_IncomeHistory + 0x110, 0x44)
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            LS_R(&ai_Grid[i][j].a, 2)
            LS_R(&ai_Grid[i][j].b, 2)
            LS_R(&ai_Grid[i][j].used, 1)
            LS_R(&ai_Grid[i][j].average, 4)
        }
    }
    return 1;
}
/* 0x51fb80, the save game's reader of the avatar table: forwards to gv_ReadAvatars (0x4ed270) */


int ls_ReadAvatars(void *f)
{
    return gv_ReadAvatars(f);
}
/* 0x51fb90, the load game step that reads the event tables (the counterpart of ls_WriteEventTables): the count
   and then each of the n used AIJOB records of he_Events (kind, id, chr, netId, from, the 12 words, the three
   clocks, stage, opts, the eight ids from version 0x1002f, the text's size and buffer (mem_Alloc "messagebox")
   or 0, and the kind-specific block +0xac: the kind 0xc gets a 0x58-byte "f3_alwin_t:d" record at +0xbc, the
   kind 0xd -1 in it and, for stage 6, the byte +0xd0 = 0); the records flagged 8 go to he_QuickList; then the
   used gm_CarryRecs records and the used he_ApEvents (counter, key, income, cost and name). Returns 1, 0 when a
   read fails */

int ls_ReadEventTables(int f)
{
    int n, size2, i, idx;
    AIJOB *rec;
    GMCARRYREC *cr;
    HEAPEVENT *ap;
    char *tb;

    he_FreeEvents();
    LS_R(&n, 4)
    LS_R(&size2, 4)
    he_EventTop = 0;
    he_QuickTop = 0;
    he_EventCount = 0;
    if (n > 0) {
        do {
            idx = he_EventCount;
            rec = &he_Jobs[idx];
            LS_R(&rec->kind, 1)
            LS_R(&rec->id, 4)
            LS_R(&rec->chr, 2)
            LS_R(&rec->netId, 4)
            LS_R(&rec->from, 4)
            LS_R(rec->words14, 0x30)
            LS_R(&rec->start, 0xe)
            LS_R(&rec->end, 0xe)
            LS_R(&rec->clock60, 0xe)
            LS_R(&rec->stage, 4)
            LS_R(&rec->opts, 1)
            if (ls_Version >= 0x1002f) {
                for (i = 0; i < 8; i++)
                    LS_R(&rec->ids[i], 4)
            }
            rec->text = 0;
            rec->textSize = 0;
            LS_R(&rec->textSize, 4)
            if (rec->textSize != 0) {
                tb = (char *)mem_Alloc(0x400, "messagebox");
                rec->text = tb;
                if (ls_Read((int)tb, rec->textSize, 1, f) == 0)
                    return 0;
            }
            if (rec->kind == 0xc) {
                LS_R(&rec->u, size2)
                rec->alwin = (char *)mem_Alloc(0x58, "f3_alwin_t:d");
            } else if (rec->kind == 0xd) {
                LS_R(&rec->u, size2)
                rec->idbc = -1;
                if (rec->stage == 6)
                    ((char *)rec)[0xd0] = 0;
            } else {
                LS_R(&rec->u, size2)
            }
            if ((rec->opts & 8) != 0) {
                he_QuickList[he_QuickTop] = (char *)rec;
                he_QuickTop++;
            }
            he_EventCount++;
        } while (he_EventCount < n);
    }
    he_EventTop = he_EventCount;
    LS_R(&n, 4)
    i = 0;
    if (n > 0) {
        cr = gm_CarryRecs;
        do {
            cr->ref = 0;
            LS_R(&cr->ref, 4)
            LS_R(&cr->ttl, 1)
            LS_R(&cr->hasValue, 4)
            LS_R(&cr->owner, 4)
            LS_R(&cr->value, 4)
            LS_R(&cr->spec, 0xe)
            LS_R(cr->data, 0x80)
            i++;
            cr++;
        } while (i < n);
    }
    LS_R(&n, 4)
    if (n > 0) {
        i = 0;
        ap = he_ApEvents;
        do {
            LS_R(&ap->counter, 1)
            LS_R(&ap->key, 4)
            LS_R(&ap->income, 4)
            LS_R(&ap->cost, 4)
            LS_R(ap->name, 0x40)
            i++;
            ap++;
        } while (i < n);
    }
    return 1;
}
/* 0x520000, reads the scene party records (cm_Parties) from the save file f, the counterpart of ls_WriteParties:
   the count, then per record the id, kind, thing, flags, used, the 16 member netIds (16 words of the old 0x40 bytes
   below version 0x10059), the offer kind, the 0x98-byte kind payload (+0x7c), the clock (the game clock plus i + 1
   hours below version 0x1003f) and the host (-1 below 0x10040). Returns 1 */


int ls_ReadParties(int f)
{
    char tmp[0x1000];
    int n, i;
    G28REC *r;

    LS_R(&n, 4)
    r = cm_Parties;
    for (i = 0; i < n; i++, r++) {
        LS_R(&r->id, 4)
        LS_R(&r->kind, 1)
        LS_R(&r->thing, 4)
        LS_R(&r->flags, 1)
        LS_R(&r->used, 1)
        if (ls_Version >= 0x10059) {
            if (ls_Read((int)r->members, 4, 0x10, f) == 0)
                return 0;
        } else {
            if (ls_Read((int)tmp, 0x40, 0x10, f) == 0)
                return 0;
            memcpy(r->members, tmp, 0x40);
        }
        LS_R(&r->offerKind, 1)
        LS_R(&r->eventId, 0x98)
        if (ls_Version >= 0x1003f) {
            LS_R(&r->time, 0xe)
        } else {
            memcpy(&r->time, &ai_Time, 0xe);
            gm_AddTime(&r->time, i + 1, 0, 0);
        }
        if (ls_Version >= 0x10040) {
            LS_R(&r->host, 4)
        } else {
            r->host = -1;
        }
    }
    return 1;
}
/* 0x520200, reads the mission table 0x6f0724 / 0x601058 .. 0x60106c, 0xf83520 (0x10 bytes), and the 0x11-word records from 0xf83548 (8 pairs of words each). Returns 1 */


int unk_Unk520200(int f)
{
    int n, i, j;
    int *p, *q;

    LS_R(&hst_TextsOn, 1)
    hst_LoadPlayerTexts();
    LS_R(&n, 4)
    hst_TextCurrent = hst_TextFirst + n;
    if (ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    hst_KeyNext = hst_KeyFirst + n;
    for (i = 0; i < 0x10; i++)
        LS_R((char *)(unsigned)&hst_Key + i, 1)
    p = (int *)((unsigned)&hst_Lists + 0x8);
    while (ls_Read((int)(p - 2), 4, 1, f) != 0) {
        q = p;
        for (j = 0; j < 8; j++) {
            LS_R(q - 1, 4)
            LS_R(q, 4)
            q += 2;
        }
        p += 0x11;
        if ((int)p > ((unsigned)((unsigned)hst_Lists + 0x114) + 0x3))
            return 1;
    }
    return 0;
}
/* 0x520310, reads the two record pools 0x9367e0 / 0xa69140 (0x99-byte records: the references at +0x91 / +0x95 are indices turned into pointers (-1 = NULL)), the pool heads 0xa68d24 / 0xa68c00 / 0xf81160, the reference table 0xf31160 (10 bytes each, 0x2000 or 0x8000 of them from 0x1003c) and the words 0xf81168, 0xf8121c, 0xa68c04, 0x60bd88 .. 0x60bd90. Returns true when all was read */


int unk_Unk520310(int f)
{
    int *p;
    int i, n;
    char *q;

    for (p = (int *)((unsigned)&cm_RecvPool + 0x91); (int)p < ((unsigned)&cm_SplitMsg + 0x71); p = (int *)((char *)p + 0x99)) {
        LS_R((char *)p - 0x91, 0x99)
        if (*p == -1)
            *p = 0;
        else
            *p = (unsigned)&cm_RecvPool + *p * 0x99;
        if (p[1] == -1)
            p[1] = 0;
        else
            p[1] = (unsigned)&cm_RecvPool + p[1] * 0x99;
    }
    LS_R(&n, 4)
    cm_RecvQueue = n == -1 ? 0 : (unsigned)&cm_RecvPool + n * 0x99;
    i = 0;
    LS_R(&n, 4)
    cm_RecvFree = n == -1 ? 0 : (unsigned)&cm_RecvPool + n * 0x99;
    n = ((ls_Version < 0x1003c) - 1 & 0x6000) + 0x2000;
    for (p = (int *)((unsigned)&cm_SendStates + 0x2); (int)p < ((unsigned)((unsigned)cm_SendStates + 0x50000) + 0x2); p = (int *)((char *)p + 10)) {
        p[1] = 0;
        *p = -1;
        *((char *)p - 2) = 1;
    }
    if (n != 0) {
        for (p = (int *)((unsigned)&cm_SendRing + 0x91); i < n; p = (int *)((char *)p + 0x99)) {
            LS_R((char *)p - 0x91, 0x99)
            if (*p == -1)
                *p = 0;
            else
                *p = (unsigned)&cm_SendRing + *p * 0x99;
            if (p[1] == -1)
                p[1] = 0;
            else
                p[1] = (unsigned)&cm_SendRing + p[1] * 0x99;
            i++;
        }
    }
    i = 0;
    if (ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    cm_SendQueue = n == -1 ? 0 : (unsigned)&cm_SendRing + n * 0x99;
    LS_R(&cm_LastSeq, 4)
    n = ((ls_Version < 0x1003c) - 1 & 0x6000) + 0x2000;
    if (n != 0) {
        q = (char *)(unsigned)&cm_SendStates;
        do {
            LS_R(q, 10)
            i++;
            q += 10;
        } while (i < n);
    }
    if (ls_Read((int)(unsigned)&cm_GroupFirstSeq, 4, 1, f) == 0)
        return 0;
    LS_R((unsigned)&cm_GroupEndSeq, 4)
    *(int *)(unsigned)&cm_LastCharId = 0;
    if (ls_Read((int)(unsigned)&cm_LastCharId, 4, 1, f) == 0)
        return 0;
    LS_R((unsigned)&cm_LastBuildingId, 4)
    return ls_Read((int)(unsigned)&cm_LastObjectId, 4, 1, f) != 0;
}
/* 0x5205f0, the reader of the weather environment ls_WriteEnvironment writes: the winter-weather flag
   sv_WinterWeather and the 24 hours' clouds, storm, windX and windY of sv_Weather; from version 0x10050 also the
   wind table (ai_LoadWind) and the effects (gv_ReadEffects). Returns 1, or 0 when a read fails */


int ls_ReadEnvironment(int f)
{
  int i;

  if (ls_Read((int)(unsigned)&sv_WinterWeather, 4, 1, f) == 0)
    return 0;
  for (i = 0; i < 24; i++) {
    LS_R(&sv_Weather.clouds[i], 4)
    LS_R(&sv_Weather.storm[i], 4)
    LS_R((int *)&sv_Weather.windX[i], 4)
    LS_R((int *)&sv_Weather.windY[i], 4)
  }
  if (ls_Version >= 0x10050 && ai_LoadWind(f) != 0 && gv_ReadEffects(f, ls_Version) != 0)
    return 1;
  return 0;
}
/* 0x5206b0, the fix-up after a load, the counterpart of 0x519a00 before a save: it turns the loaded ids back into pointers: the player's character (ai_PlayerChar = gm_FindChar, 0 = fail), the market building (ai_MarketBuilding = gm_BuildQuery (1, 5, 10)), the eight kind 0xb characters (ai_KindBChars), the reference fields of the ai_Chars records (gm_FindById into +0x16c and +0x170, he_FindEvent into +0x17c, +0x184 cleared), the objects of ai_Objects (an object with an owner is chained into the owner's list: the heads at +0x5d building / +0x14 object / +0x178 character, +0x3f the next link; objects of the good type 0x1d lose their +0x24 word), the event records of he_Events (the kinds 0x29 0x2d 0x2f 0x11 0x25 5 10 0x26 or flagged 8 are re-initialised through he_RunHandlers[kind] with ai_GameState = 2, the stored +0x70 saved and put back; the kinds 0xd / 0xf / 0x78 / 0x76 / 0x14 get their own fixes), the first word of the 0xa4-byte records at 0x74e1e0 (an event owner id, he_FindEvent (1, 1, id)) and the target words of the reference records (ls_RefTargets, by kind: 1 gm_FindChar, 3 gm_FindObjectInst, 2 gm_FindBuildingById, 9 cm_PartyById); it closes with gm_ResetStock. Returns 1, 0 when the player's character is gone */


int ls_FixupAfterLoad(void)
{
    int *p, *q;
    char *r;
    int i, saved, v, c;
    unsigned char kind, *pb;
    unsigned short *id, *otype;
    int *owner, *ev70, *ev10, *evb8, *list, *next;
    unsigned char **v24;

    ai_PlayerCharC = (char *)gm_FindChar((int)ai_PlayerCharC);
    if (ai_PlayerCharC == 0)
        return 0;
    ai_MarketBuilding = (int)gm_BuildQuery(1, 5, 10);
    if (ls_Version >= 0x10017) {
        for (q = (int *)ai_KindBChars; (int)q < (int)(ai_KindBChars + 8); q++)
            *q = (int)gm_FindChar(*q);
    }
    for (p = (int *)((char *)ai_Chars + 0x16c); (int)p < (int)(ai_Chars + 768) + 0x16c; p = (int *)((char *)p + 0x21c)) {
        id = (unsigned short *)((char *)p - 0x16c);
        if (*id != 0xffff) {
            if (p[0] == -1)
                p[0] = 0;
            else
                gm_FindById(p, 0, 0, p[0]);
            if (p[1] == -1)
                p[1] = 0;
            else
                gm_FindById(p + 1, 0, 0, p[1]);
            if (p[4] == -1)
                p[4] = 0;
            else
                p[4] = (int)he_FindEvent(1, 1, p[4]);
            p[6] = 0;
        }
    }
    for (i = 0; i < f3gm_ObjectCount; i++) {
        int a, b;

        r = (char *)ai_Objects + i * 0x43;
        otype = (unsigned short *)r;
        owner = (int *)(r + 6);
        if (ai_Goods[*otype * 0x41] == 0x1d)
            *(int *)(r + 0x24) = 0;
        if (*owner != -1 && gm_FindById(&a, &b, &c, *owner) != 0) {
            int link;

            if (a == 0) {
                if (b == 0) {
                    list = (int *)(c + 0x178);
                    link = *list;
                    if (link == 0) {
                        *list = (int)r;
                        continue;
                    }
                } else {
                    list = (int *)(b + 0x14);
                    link = *list;
                    if (link == 0) {
                        *list = (int)r;
                        continue;
                    }
                }
            } else {
                list = (int *)(a + 0x5d);
                link = *list;
                if (link == 0) {
                    *list = (int)r;
                    continue;
                }
            }
            next = (int *)(link + 0x3f);
            while (*next != 0) {
                link = *next;
                next = (int *)(link + 0x3f);
            }
            *next = (int)r;
        }
    }
    saved = d3_CurrentScene;
    d3if_SelectScene(0, 0);
    for (pb = he_Events; (int)pb < (int)he_QuickList; pb += 0x14c) {
        kind = *pb;
        if (kind != 0) {
            if ((kind == 0x29 || kind == 0x2d || kind == 0x2f || kind == 0x11 || kind == 0x25 || kind == 5
                 || kind == 10 || kind == 0x26 || (pb[0x78] & 8) != 0)
                && (v = *(ev70 = (int *)(pb + 0x70))) != 0) {
                *ev70 = 0;
                ai_GameState = 2;
                if (kind == 0x11)
                    pb[0xf0] &= 0xef;
                he_RunHandlers[kind](pb);
                ai_GameState = 0;
                *ev70 = v;
            }
            kind = *pb;
            if (kind != 0xc) {
                if (kind == 0xd) {
                    evb8 = (int *)(pb + 0xb8);
                    gm_FindById(&c, 0, 0, *evb8);
                    if (c != 0)
                        he_ShowConstruction((int)pb);
                } else if (kind == 0xf) {
                    ev10 = (int *)(pb + 0x10);
                    v = gm_FindObjectInst(*ev10);
                    if (v != 0) {
                        v24 = (unsigned char **)(v + 0x24);
                        *v24 = pb;
                    }
                } else if (kind == 0x78) {
                    memset(pb + 200, 0, 8);
                } else if (kind == 0x76) {
                    memset(pb + 0xb0, 0, 4);
                } else if (kind == 0x14) {
                    sv_CountGroupMembers((char *)pb);
                    pb[0xac] = pb[0x14] + pb[0x18];
                }
            }
        }
    }
    d3if_SelectScene(saved, 0);
    for (q = (int *)((unsigned)&he_QuickList + 0x6a0); (int)q < ((unsigned)&he_QuickList + 0xaaa0); q += 0x29) {
        if (*q != 0)
            *q = (int)he_FindEvent(1, 1, *q);
    }
    for (q = ls_RefTargets; (int)q < (int)ls_RefTargets + 0x50000; q = (int *)((char *)q + 10)) {
        if (*q != 0) {
            if (*((char *)q - 5) == 1)
                *q = (int)gm_FindChar(*q);
            if (*((char *)q - 5) == 3)
                *q = gm_FindObjectInst(*q);
            if (*((char *)q - 5) == 2)
                *q = (int)gm_FindBuildingById(*q);
            if (*((char *)q - 5) == 9)
                *q = cm_PartyById(*q);
        }
    }
    gm_ResetStock();
    return 1;
}
/* 0x520a30, sends the save game file `name` to the other players: opens it "rbn" (vfs_Tfopen), takes its length (vfs_ftell, rounded up to a multiple of 0x80), reads it into a mem_Alloc'd "ls_SendGame" buffer (vfs_fread) and hands it out in blocks of 0x80 bytes (cm_SendLoadBlock, the size first in cm_SendLoadSize; a Sleep(15) after every 16th block; the network pumps sv_PumpMessages / sv_PumpNet between), then waits until nothing is queued in cm_SendQueue. Closes the file and frees the buffer. Returns 1, 0 when the file or the buffer fails */


int ls_SendGame(char *name)
{
    VFSFILE *fh;
    int len, size, cnt = 0;
    char *buf, *p;

    fh = vfs_Tfopen(name, "rbn", (int)vfs_Root);
    if (fh == 0)
        return 0;
    vfs_fseek(fh, 0, SEEK_END);
    len = (int)vfs_ftell(fh);
    size = (len / 0x80 + 1) * 0x80;
    vfs_fseek(fh, 0, SEEK_SET);
    buf = (char *)mem_Alloc(size, "ls_SendGame");
    if (buf == 0) {
        vfs_fclose(fh);
        return 0;
    }
    if (vfs_fread(buf, len, 1, fh) == -1) {
        mem_Free(buf);
        vfs_fclose(fh);
        return 0;
    }
    cm_SendLoadSize(size);
    p = buf;
    if (size > 0) {
        int sent = 0;

        do {
            if (cm_SendLoadBlock(p) != -1) {
                p += 0x80;
                sent += 0x80;
                if (cnt++ % 16 == 0)
                    Sleep(0xf);
            }
            sv_PumpMessages();
            sv_PumpNet();
        } while (sent < size);
    }
    while (cm_SendQueue != 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    mem_Free(buf);
    vfs_fclose(fh);
    return 1;
}
/* 0x520b80, takes the save game that arrived over the network: waits (pumping messages and the network) until
   cm_LoadBuf holds all cm_LoadSize bytes, opens it as a memory file, reads its header and, only for a network start
   (header flag bit 1) of a version up to 0x1005a, the globals as ls_LoadGame does (the id counters, the clock and
   its copy ai_TimeCopy, the own player and its character, the counters, the update block, the party / crime ids
   by version), then the objects, buildings, player records and characters (the characters of kind 6 become kind
   5), the player slots, the Aemter (from 0x10045), the scene (unless header flag bit 2) and the fix-up. Closes the
   file and frees the buffer; returns 1, 0 when a step fails. */


int ls_ReceiveGame(void)
{
    int h;
    LSHDR hdr;
    int i;

    while (cm_LoadBuf == 0 || cm_LoadReceived < cm_LoadSize) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    h = (int)vfs_MemOpen(cm_LoadBuf, cm_LoadSize, "rb");
    if (h == 0) {
        mem_Free(cm_LoadBuf);
        cm_LoadBuf = 0;
        return 0;
    }
    ai_PlayerCharC = 0;
    ls_ReadSaveHeader(h, (int *)&hdr);
    if ((hdr.flags & 2) == 0 || hdr.version > 0x1005a)
        LS_LOADFAIL
    if (!LS_RD(&ai_NextObjectId, 4) || !LS_RD(&ai_NextEventId, 4) || !LS_RD(&ai_Time, 0xe)
        || !LS_RD(&ai_OwnPlayer, 1) || !LS_RD(&ai_PlayerCharC, 4) || !LS_RD(&ai_NextCharNumber, 4)
        || !LS_RD(&ai_NextPlayerNumber, 4) || !LS_RD(&ai_CharCount, 4))
        LS_LOADFAIL
    ai_TimeCopyT = ai_Time;
    LS_RD(&ls_SpareInt, 4);
    LS_RD(&ai_Update, sizeof(AIUPDATE));   /* 0xf89580 */
    if (ls_Version >= 0x10030)
        LS_RD(&ai_NextPartyId, 4);
    if (ls_Version >= 0x1003d)
        LS_RD(&ai_NextCrimeId, 4);
    else
        ai_NextCrimeId = 1000000;
    if (ls_ReadObjects(h) == 0)
        LS_LOADFAIL
    if (ls_ReadBuildings(h) == 0)
        LS_LOADFAIL
    if (ls_ReadPlayerRecords(h) == 0)
        LS_LOADFAIL
    if (ls_ReadCharacters(h) == 0)
        LS_LOADFAIL
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].kind == 6)
            ai_CharsT[i].kind = 5;
    }
    if (ls_ReadPlayerSlots(h) == 0)
        LS_LOADFAIL
    if (ls_Version >= 0x10045 && amt_LoadAemter(h, ls_Version) == 0)
        LS_LOADFAIL
    if ((hdr.flags & 4) == 0 && ls_LoadScene(h) == 0)
        LS_LOADFAIL
    if (ls_FixupAfterLoad() == 0)
        LS_LOADFAIL
    vfs_fclose(h);
    mem_Free(cm_LoadBuf);
    cm_LoadBuf = 0;
    return 1;
}

unsigned cm_LoadSize = 0;  /* 0xa68d28, the received bytes and the expected length (compared unsigned */
int cm_LastSeq = 0;  /* 0xf81168 .. */
unsigned cm_LoadReceived = 0;  /* 0xf81220 / */
