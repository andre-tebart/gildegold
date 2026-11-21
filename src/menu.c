/* menu.c -- the screen / front-end module (0x5d33b0 .. 0x5d9de0; no leaked name): the file selector,
   the save / load windows, the options reader / writer and screens, the options menu, credits, pause and
   message filter, the gypsy / tailor contact windows. The poker windows 0x5d9de0.. are their own object
   (pokerwin.c: the plain block ai_PokerUi 0x62dda0 follows this unit's last string, data layout s11).
   Start (plausible): its first literal "%s_%s" 0x62d7d8 follows the previous unit's last literal "special_uni"
   0x62d6bc and the subject changes from the action records of the table 0x62c420 to the screens. End: the
   end of the game code (the last function 0x5dd100 returns at 0x5dd3d6, nop fill to 0x5dd3e0); dinput.lib's
   c_dfDIKeyboard / c_dfDIMouse object tables follow, library data (config/symbols/game_range.toml).
   Open: d2.h is not included yet: the unit's own declarations of d2_StartFade / d2_ReleaseFade /
   d2_ScreenSurf / d2_Inputs / d2_LockSurface / d2_UnlockSurface disagree with d2.h's, so the unit reads the d2
   records through local views (D2INPUTVIEW, D2FADEVIEW, D2SURFVIEW ...).
   Open: not included yet (dcp headers conflicts menu HEADER): ch.h cm.h d2.h d3_int_io.h gfx.h; held by
   own declarations that differ from the owner's: d2_StartFade, d2_ReleaseFade, d2_ScreenSurf,
   d2_Inputs, d2_LockSurface, d2_UnlockSurface, ch_List, d3io_Lod.
   Data: the four floor option bytes 0x6f35c8 .. 0x6f35cb are objects of their own (ai_GfxFloor, ai_GfxCameraLimits,
   ai_GfxFloorLod, ai_GfxCharDetail): ai_ApplyGfxOptions reads 0x6f35c9 as a byte (mov al, byte ptr) where a union gives a dword load.
   unit of rea 8cef7d5b (src/game/unk_5d33b0.c); boundaries in decl/text.dcl */
extern int gm_PlayerRecords[688];
#include "txt_views.h"
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <string.h>
#include "vfs.h"
#include "di_input.h"
#include "f3snd.h"
#include "tb.h"
#include "d3.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "surf.h"
#include "menu.h"
#include "sv_t.h"
#include "lc.h"
#include "data_views.h"
#include "d2_types.h"                 /* D2OBJ, D2WIN, D2PIC: the records of d2_Objs, d2_LastWindowPtr, d2_Pics */
#include "ls.h"                      /* ls_SavePath */
#include "ahm.h"
#include "ai.h"
#include "amt.h"
#include "anm.h"
#include "bio.h"
#include "d3_interface.h"
#include "d3s.h"
#include "f3gm.h"
#include "f3init.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "msx.h"
#include "sblib.h"
#include "sv.h"
#include "tr.h"
#include "poker.h"
#include "mission.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "dd_vesa.h"
#include "ch.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define ai_MarketBuildingV (*(G16BLDVIEW **)&ai_MarketBuilding)  /* 0x6f1bc8, ai_MarketBuilding (ai.c: AIBLD *) as this unit's G16BLDVIEW (its +0x4c / object fields) */
#define ai_FallbackBuildingB (*(unsigned char **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_PlayerBuildingB (*(unsigned char **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_CharsC (*(char (*)[768 * 0x21c])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) read flat, as this unit does */
#define ai_CharsK ((AICHARKINDVIEW *)((char *)ai_Chars + 2))  /* 0x770b82, the records from their kind byte (ai_HasQuicksavePlayer walks them so) */

extern OPTIONBYTES ai_GfxResolution;
/* full rebuild: the symbols whose original addresses the code above spells literally */

extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2WIN *d2_LastWindowPtr;     /* 0x6ed980, d2.h: the window a screen works in (created last) */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_FrameMask;           /* 0x760638, the pump mask of the current frame (sv_RunFrame) */
extern struct D2OBJ *d2_Objs;              /* 0xf8f080, d2.h: D2OBJ * */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */
extern int sv_PopRequest;               /* 0x6f22a0, nonzero: MASTER (sv_RunFrame) pops this screen */

#define U_I(a) (*(int *)(a))
#define U_B(a) (*(unsigned char *)(a))

int sprintf(char *buf, const char *fmt, ...); /* 0x5de652, the CRT's sprintf (no CRT symbol available) */
/* 0x5d33b0, the file list of a directory (256 B): lists the files of dirPath whose extension equals ext
   (compared case-insensitively) into records (0x210 bytes each: the name at +9, the full
   "<dirPath>/<name>" path at +0x109 with its extension cut off; the 9 bytes in front of the name are the
   caller's slot header). vfs_FindDir (dirPath, vfs_Root) gives the directory; each name is copied and
   formatted with "%s/%s" (0x62d7d8). Returns the number of entries written. */

int ai_ListFiles(char *dirPath, const char *ext, char *records)
{
    VFSDIR *dir;
    VFSENTRY *entry;
    char foundExt[0x20];
    char *dot;
    char *out;
    int i;
    int count;

    dir = vfs_FindDir(dirPath, vfs_Root);
    if (dir == 0)
        return 0;
    count = 0;
    i = 0;
    if (i < dir->nentries) {
        out = records + 9;
        do {
            entry = &dir->entries[i];
            dot = strrchr(entry->name, '.');
            if (dot != 0)
                strncpy(foundExt, dot, 0x20);
            if (strcmpi(foundExt, ext) == 0) {
                strcpy(out, entry->name);
                sprintf(out + 0x100, "%s/%s", dirPath, entry->name);
                dot = strrchr(out, '.');
                if (dot != 0)
                    *dot = '\0';
                count++;
                out += 0x210;
            }
            i++;
        } while (i < dir->nentries);
    }
    return count;
}

void d2_CentreViewWindows(int view); void d2_SetPointerSprite(int a, int pic); int d2_SetForm(int view, int n);
void d2_CloseView(int view);
int io_PrintText(char *fmt, ...); int d2_GetWinObj(int view, int idx, int k);
int sv_CreateScrollButtons(int x, int y, int f, int g, int p5, int *b, int *a, int *c); /* 0x52e860, sv.c:
                                           the slider of the form f at (x, y) */
/* Callees whose units have no header here (type debt: the canonical prototypes live in d2.h, f3gm.c,
   gv.h, cm.h and he.h; the pointer views follow this TU's declarations). */
int d2_GetViewWin(int view, int i);         /* 0x475ff0, d2.h: window i of a view, -1 if out of range */
int gm_BuildingSkill(unsigned char *b, unsigned k, int flag); /* 0x5092e0, f3gm.c: the building skill (ai.h's
                                               prototype; f3snd.h's building view is unsigned char *) */
char *gv_FindAccused(char *c, int max, int *ids, int *cnt);  /* 0x4f41b0, gv.c: the character with the
                                               most records of its table not yet paired with c */
void cm_SendProofExecuted(int record, int character); /* 0x4c4ed0, cm.c: message 0x24, the pair (character,
                                               record) into the executed table 0x91b6e0 */
void cm_SendMoneyDelta(int character, int delta); /* 0x4c5f60, cm.c: message 0x5a, adds delta to the
                                               character's money (+0x194), clamped 0 .. 0x32 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2); /* 0x53dda0, he.c */
void d2_SetObjValue(int obj, ...); int d2_GetObjValue(int obj); int d2_CreateWinText(short x, short y, char *text, int win); void d2_SetObjText(int obj, char *text);
char *itoa(int, char *, int); void d2_SetObjFont(int obj, int font); int sv_RunFrame(int cb, unsigned mask);

/* 0x5d34b0, the file selector window "menu\fileselector": lists the files of dir with the extension ext
   (ai_ListFiles, up to 32) under the caption, with their count. With bit 0 of flags a click on a file writes
   "<dir>\<name><ext>" into out and returns 1; without it the click puts the name into the name input
   (preset txt_DefaultFileName) and the button 0x1ccc writes the input's name into out and returns 1. A right
   click leaves (0). mask is the frame pump's mask (sv_RunFrame) */
int ai_FilePickWindow(unsigned mask, int flags, char *caption, char *dir, char *ext, char *out)
{
    FILEREC recs[32];
    char num[0x20];
    int win;
    int n;
    int i;
    int ctrl;
    int input;
    int count;
    int okBtn;
    int slidA;
    int slidB;
    int direct;
    int ret;

    input = -1;
    count = -1;
    okBtn = -1;
    ret = 0;
    win = d2_OpenForm(0, 0, "menu\\fileselector");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    io_PrintText(caption);
    d2_SetForm(win, 0);
    sv_CreateScrollButtons(0x140, 0x100, d2_LastWindow, d2_GetViewWin(win, 1), 0x18, &slidA, &slidB, &count);
    memset(recs, 0, sizeof(recs));
    n = ai_ListFiles(dir, ext, (char *)recs);
    direct = flags & 1;
    if (direct == 0) {
        d2_SetForm(win, 3);
        okBtn = d2_GetWinObj(win, 3, io_PrintText(0x1ccc));
        d2_SetForm(win, 0);
        input = d2_GetWinObj(win, 0, 0);
        d2_SetObjValue(input, txt_DefaultFileName);
    }
    d2_SetForm(win, 1);
    for (i = 0; i < n; i++) {
        ctrl = d2_CreateWinText(0, i * 0x18, recs[i].name, d2_LastWindow);
        recs[i].ctrl = ctrl;
        d2_Objs[ctrl].interactive = 1;
        d2_Objs[recs[i].ctrl].centred = 1;
        d2_Objs[recs[i].ctrl].w = d2_LastWindowPtr->w;
        d2_Objs[recs[i].ctrl].style = 3;
    }
    itoa(n, num, 10);
    d2_SetObjText(count, num);
    d2_SetObjFont(count, 0x54);
    while (sv_RunFrame((int)ai_FilePickWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0) {
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == recs[i].ctrl) {
                    if (direct != 0) {
                        sprintf(out, "%s\\%s%s", dir, recs[i].name, ext);
                        sv_PopRequest = 1;
                        ret = 1;
                    } else {
                        d2_SetObjValue(input, recs[i].name);
                    }
                    break;
                }
            }
        }
        if (d2_ClickedId != -1 && d2_PointerObj == okBtn) {
            if (direct == 0)
                sprintf(out, "%s\\%s%s", dir, (char *)d2_GetObjValue(input), ext);
            sv_PopRequest = 1;
            ret = 1;
        }
    }
    d2_CloseView(win);
    return ret;
}

int d2_CreateRadioGroup(int n, ...); void d2_RadioAdd(int g, int obj); void d2_MenuNavigate(int g); void d2_RaiseViewWindows(int view); void d2_RadioClear(int g);

#define ai_MessageBoxOpen (ai_GfxResolution.byte[1])  /* 0x6f2f95, 1 while ai_MessageBox runs (a second box returns 0 at once) */

/* 0x5d3810, shows the message box with the text `text` (io_PrintText) and waits for its answer: the form by the
   flags (2 "misc\Messagebox_Autosize", 0x10 _BIG, 0x20 _VERY_BIG, 0x100 _NONE_PERGA on form page 1, else
   "misc\Messagebox"); 8 pumps with the mask 0x717c6 instead of the frame's own (less the bits 0x38), 0x40 raises
   the windows every frame, 4 closes the box 500 ms after it opened. Every control of the window but the type 0x40
   ones joins a radio group the keys walk; the button 0 answers 1 (yes), the button 1 answers 0, a right click
   closes it. Returns the answer, 0 when another box is open. */
int ai_MessageBox(int text, int flags)
{
    int win;
    int list;
    int id;
    int i;
    int ret;
    unsigned mask;
    unsigned long start;

    start = d3_Time;
    ret = 0;
    if (ai_MessageBoxOpen != 0)
        return 0;
    mask = sv_FrameMask & ~0x38;
    if ((flags & 2) != 0)
        win = d2_OpenForm(0, 0, "misc\\Messagebox_Autosize");
    else if ((flags & 0x10) != 0)
        win = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if ((flags & 0x20) != 0)
        win = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else if ((flags & 0x100) != 0) {
        win = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
        d2_SetForm(win, 1);
    } else
        win = d2_OpenForm(0, 0, "misc\\Messagebox");
    if ((flags & 8) != 0)
        mask = 0x717c6;
    io_PrintText(text);
    d2_CentreViewWindows(win);
    ai_MessageBoxOpen = 1;
    list = d2_CreateRadioGroup(0);
    for (i = 0; i < d2_LastWindowPtr->nobjs; i++) {
        id = d2_LastWindowPtr->objs[i];
        if (d2_Objs[id].type != 0x40)
            d2_RadioAdd(list, id);
    }
    while (sv_RunFrame((int)ai_MessageBox, mask) != 0) {
        d2_MenuNavigate(list);
        if ((flags & 0x40) != 0)
            d2_RaiseViewWindows(win);
        if (d2_ClickedId != 0) {
            if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
                ret = 0;
            }
        } else {
            sv_PopRequest = 1;
            ret = 1;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if ((flags & 4) != 0 && d3_Time > start + 500)
            sv_PopRequest = 1;
    }
    ai_MessageBoxOpen = 0;
    d2_RadioClear(list);
    d2_CloseView(win);
    return ret;
}
/* the two records 0x5d3ab0 builds: the request (a file entry of ai_ListFiles, or an empty slot) and one slot
   of the save / load window, which ai_RegisterSaveSlot (0x5d3a00) fills with a copy of the request */
#pragma pack(push, 1)
typedef struct SAVEREQUEST {                /* 0x210, a save file entry (ai_ListFiles) or an empty slot's request */
    char pad00[9];
    char name[0x80];                        /* +0x09, the slot caption (the file name without ".SAV") */
    char title[0x80];                       /* +0x89, the save's title */
    char path[0x107];                       /* +0x109, the file path (ai_FillSaveSlots opens it) */
} SAVEREQUEST;                              /* 0x210 */
typedef struct SAVESLOTVIEW {               /* 0x220, one slot of the save / load window */
    unsigned char kind;                     /* +0x00, the slot kind (ls_SaveSlot for a registered save) */
    char pad01[3];
    int obj;                                /* +0x04, the slot's caption text object, -1 = none */
    int win;                                /* +0x08, the slot's child window, -1 = free */
    unsigned char used;                     /* +0x0c, 1: a filled slot, 0: an empty one */
    char pad0d[3];
    union {
        SAVEREQUEST req;                    /* +0x10, the request ai_RegisterSaveSlot copied */
        struct {
            char reqpad[9];
            char name[0x80];                /* +0x19, the caption (the request's name) */
            char title[0x80];               /* +0x99, the save's title (the header's +0x9c) */
        };
    };
} SAVESLOTVIEW;                             /* 0x220 */
#pragma pack(pop)

/* 0x5d3a00, puts the save request req into its slot of the save / load window: the request named "QUICKSAVE"
   takes the slot 1 and "AUTOSAVE" the slot 0 whatever slot says, and the slots 0 / 1 take no other name (a
   case-insensitive compare); a free slot (win -1) gets the kind byte and a copy of the request. Returns the
   slot, -1 when it is taken, the name does not fit it or slot is -1. count (0x10) is not read.
   @deep deep s29: c2tail --tries on the goto form: the xjump merger deletes the copy before the jmp at the
   end of the slot == 0 arm (} else if (slot == 1)) and keeps the slot == 1 arm's compare, which falls into
   the join; the original keeps the slot == 0 copy, so in its flow graph the slot == 1 compare is the one
   ending in a jmp. Goto-free forms measured (slot = -1 in the failing arm, one if (slot != -1) fill, one
   return slot): if / else-if arms 17 (flag-inl, slots[slot] inline: 13 ins, every block right except that
   the QUICKSAVE compare stays a second full copy, no cross-jump: the return block is duplicated into both
   arms so neither ends in a jmp); (slot == 0 || slot == 1) + strcmpi(name, slot == 1 ? QUICKSAVE :
   AUTOSAVE): 14 ins with the original's layout but the string as a mov eax / push eax value (the original
   pushes per arm: two calls); || / && conditions 13-14; ?: condition 10 (neg / sbb setcc); if (slot != 0)
   {..} else {AUTOSAVE} nesting 24-41. slots[slot] instead of s in the goto form: 10 ins but draft (M9),
   not kept. prior: insensitive (32 samples). Meaning equal. Now shape 14 ins (was draft 17): the arms 'if
   (slot == 0) { if (strcmpi(name, AUTOSAVE) == 0) goto fill; } else if (slot == 1) {.. QUICKSAVE ..} else
   if (slot == -1) goto done; else goto fill; slot = -1; goto done;' make VC6 cross-jump the two compare
   tails into one push edi / call _strcmpi / or ebx,-1 block as the original does, but VC6 keeps the
   slot==1 arm's copy (the slot==0 arm pushes AUTOSAVE and jumps forward) where the original keeps the
   slot==0 arm's copy (falls into the call) and places 'cmp ebx,1; jne; push QUICKSAVE; jmp back' after the
   failure return (+0x61). Left: that block placement (code 10) plus mov edx|ecx for slots at +0x72/+0x80
   (regs 4). Tried (dcp try, rs1-rs7): per-arm 'return -1' / 'slot = -1; goto done' / 'goto fail' / inline
   fail in each arm, 25 combinations x both arm orders: no cross-jump (two compare blocks with their own
   epilogue, 16-28 ins) or the wrong copy; arm order 1-then-0: 12 ins but tests slot==1 first (not the
   original's order, not kept); slot != 0 nesting: 41-46; switch: 40-48; ?: condition: 34; && / ||
   condition: 18; goto-structured layouts (autosave label after the chain, fail label inside arm 0): 18.
   The previous want-local form (17) is gone. (tiebreak: insensitive) */

int ai_RegisterSaveSlot(int count, SAVESLOTVIEW *slots, int slot, SAVEREQUEST *req)
{
    char *name;
    SAVESLOTVIEW *s;

    name = req->name;
    if (strcmpi(name, "QUICKSAVE") == 0)
        slot = 1;
    else if (strcmpi(name, "AUTOSAVE") == 0)
        slot = 0;
    if (slot == 0) {
        if (strcmpi(name, "AUTOSAVE") == 0)
            goto fill;
    } else if (slot == 1) {
        if (strcmpi(name, "QUICKSAVE") == 0)
            goto fill;
    } else if (slot == -1)
        goto done;
    else
        goto fill;
    slot = -1;
    goto done;
fill:
    s = &slots[slot];
    if (s->win != -1)
        return -1;
    s->kind = slot;
    s->req = *req;
done:
    return slot;
}

/* views of d2.h's D2OBJ / D2INPUT (d2.h is not included yet: the unit's own d2_* declarations disagree with
   d2.h's, see the lead comment) */
typedef struct D2INPUTVIEW { char pad0[0x10]; int w; char pad1[0x148]; int maxw; } D2INPUTVIEW;

extern D2INPUTVIEW d2_Inputs[];         /* 0xfa6020, d2.h: D2INPUT [0x80] */
extern int d2_FontHeight;               /* 0xf8f084, d2.h */
extern struct D2INPUT *d2_FocusedInput;           /* 0x6ed9dc, d2.h: D2INPUT * */
extern unsigned char kb_Last;           /* 0x1468410, d2.h */
int d2_WinCreateSlider(int x, int y, int pic, int unk, int win);     /* 0x46bf10, d2.h: D2OBJ *d2_Objs */
void d2_CloseView(int view);                                         /* 0x4758f0, d2.h */
void d2_SetObjWidth(int obj, short w);                               /* 0x46c260, d2.h */
int d2_OpenInput(int x, int y, int w, int h, unsigned char flags);   /* 0x46a110 */
void d2_RemoveObj(int obj);                                          /* 0x46e710 */
void d2_SetObjFont(int obj, int font);                               /* 0x4760b0 */
void d2_SetObjValue(int obj, ...);                                               /* 0x475cb0, d2.c (varargs: the call sites pass 1, 3 or 4 values) */
void d2_SetObjMax(int obj, int v);                                   /* 0x475f40 */

/* d2.h's D2FADE (the fade record of 0x4767d0): only the flags byte is read here (bit 2 = done, the
   original tests the byte, not the dword); d2.h cannot be included, see the lead comment */
typedef struct D2FADEVIEW { unsigned char flags; char pad1[0x63]; } D2FADEVIEW;
/* d2's window records, as d2.c defines them (D2WIN, d2_types.h) */
extern D2WIN d2_Wins[];             /* 0xf8f120, d2.h: D2WIN d2_Wins[0x61] */
extern float sv_FramesPerTick;          /* 0x62378c, sv.c: the frames drawn since the last timer tick
                                           (the frame loop counts them in 0x623790, the tick callback
                                           0x520ec0 copies the count as a float and clears it) */
int d2_CreateWindow(short x, short y, short w, short h, unsigned int flags); /* 0x472390, d2.c */
#define d2_CreateWindowI ((int (*)(int, int, int, int, unsigned int))d2_CreateWindow)  /* @original the callers push ints although the definition names short x / y / w / h (ai_CreditsScreen 0x5d7ba0, byte-matched, pushes d2_ScreenH sign-extended: movsx ecx) */
void d2_PlaceWindowScaled(int win, int flags); /* 0x475730, d2.c */
int d2_RemoveWindow(int win);           /* 0x472ce0, d2.c */
int io_PrintText(char *fmt, ...);          /* 0x515960, io_text.c: prints a text (id or literal) with its
                                           tokens into the current window and redraws it */
extern short d2_ScreenW;                /* 0xfb9072, d2.h */
extern short d2_ScreenH;                /* 0xfb9070, d2.h */
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
D2FADEVIEW *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */
void d2_ReleaseFade(D2FADEVIEW *f);        /* 0x476860, d2.h */
void d2_ClearScreen(void);              /* 0x46e990, d2.h */
void d2_ShowPicture(int pic);           /* 0x46cfd0, d2.h */
void sv_SetPanelEnabled(int on);        /* 0x522100, sv.c: disables the panel widgets when on is 0 */
void sv_DisableVisibleWidgets(int on);  /* 0x52e600, sv.c: flags the widgets of the visible forms */
void sv_RedrawScreen(void);             /* 0x524010, sv.c */
extern int ai_GameOver;                 /* 0x6f259c, cm.h: nonzero when the game is over / left */
extern int ai_FrameWait;                /* 0x6f22bc, sv.c: frames still to wait (sv_RunFrame counts it down) */
extern int sv_PopRequest;               /* 0x6f22a0, cm.h / gv.h: the dialog close request */
extern int d2_CutsceneActive;           /* 0x6ed9d4, cm.h / gfx.h */
extern unsigned char kb_Last;           /* 0x1468410, d2.h: the last key (di_input.c) */
void d2_FocusInput(int obj);                                           /* 0x478320 */

#pragma pack(push, 1)
/* the header ls_ReadSaveHeader (0x51cee0) fills, the file layout of the writer 0x5194c0 (ls.c) */
typedef struct SAVEHEADERVIEW {             /* 0xfc */
    int version;                            /* +0x00, the save version (also ls_Version) */
    unsigned char flags;                    /* +0x04, bit 2: a map only save */
    char player[0x20];                       /* +0x05, the player's name */
    char pad25[0x0d];
    int year;                               /* +0x32, the game clock's year (0x578 + it is the calendar year) */
    unsigned short hour;                    /* +0x36, its hour */
    int minute;                             /* +0x38, its minute */
    char pad3c[4];
    char scenario[0x10];                    /* +0x40, the character's scenario record */
    char character[0x0c];                   /* +0x50, the character's saved bytes */
    char pad5c[4];
    unsigned char nameTextOffset;           /* +0x60, added to the text id 0x130 (0x18f compact) of the character line */
    unsigned char officeTextOffset;         /* +0x61, added to the text id 0x253 (0x27b compact) of the character line */
    unsigned char traitTextOffset;          /* +0x62, added to the text id 0x114 (0x11e compact) of the character line */
    char pad63[5];
    int money;                              /* +0x68, the character's money ("%i%c") */
    unsigned char compactTexts;             /* +0x6c, nonzero: the three text ids shift by +10 / +0x5f / +0x28 */
    unsigned char slot;                     /* +0x6d, ls_SaveSlot of the save */
    short pad6e;
    int playerBuilding;                     /* +0x70, the player's building id, -1 = none */
    int otherBuilding;                      /* +0x74, the second building id, -1 = none */
    int party[8];                           /* +0x78, the table 0x76fa24 */
    int difficulty;                         /* +0x98, ai_Difficulty (2 for a save older than 0x10038) */
    char title[0x60];                       /* +0x9c, the title shown in the slot */
} SAVEHEADERVIEW;                           /* 0xfc */
#pragma pack(pop)

extern int ls_Version;                      /* 0x623650, ls.c: the version of the save read last */
int ls_ReadSaveHeader(int f, int *h);      /* 0x51cee0, ls.c: reads a save header into h (no header there) */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win); /* 0x472aa0, d2.h */
int d2_WinOpenRaw(int x, int y, int w, int h, void *src, int win);  /* 0x46c390, d2.h */
int d2_WinOpenPic(short x, short y, int pic, int win); /* 0x4731d0, d2.h: creates a picture object */
/* ai_EmptySlotNameFormat: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_DifficultyTexts: view of txt_Ptrs (build/mk_txt_views.py) */
extern int d2_HitWindow;                    /* 0xf8aad0, the window the pointer hit (gfx.c sets it from
                                               d2_PointerWin; also the "double click" id of the slots) */
/* ai_LoadGameConfirmText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_SaveProgressText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_SaveOverwriteText: view of txt_Ptrs (build/mk_txt_views.py) */
extern int ai_GameFlags;                /* 0x6f23f8, the game flags: the load window writes the load mode 10 whole */
void d2_EnableObject(int obj, int on);      /* 0x475aa0, d2.h: enables / disables an object and its items */
int sv_OpenMessageBox(int text, unsigned char flags); /* 0x521900, sv.c: opens the waiting box with text */

OPTIONBYTES ai_GfxResolution = {0};  /* 0x6f2f94: byte 0 cur_res */
/* ai_ContactTextProductionTailor: view of txt_Ptrs (build/mk_txt_views.py) */
MENUOPTIONS ai_Options;  /* 0x6f35c0 .. 0x6f3670, the profile's option values (menu_types.h: the old names are its field macros) */
extern int ai_Opponents[7];
extern char ai_OptionsFile[288];
typedef struct SCREENMODE {                 /* 8 bytes, a screen mode of the table 0x62d7b8 */
    int w;                                  /* +0x0, the width */
    int h;                                  /* +0x4, the height */
} SCREENMODE;
/* ai_ContactTextCardGame: view of txt_Ptrs (build/mk_txt_views.py) */
unsigned char menu_UnreadData[240] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d6c8, menu's first .data datum (after lcamt's last literal "special\uni" 0x62d6bc): no instruction operand or image dword points into it */
SCREENMODE ai_ScreenModes[3] = { { 800, 600 }, { 1024, 768 }, { 1152, 864 } };  /* 0x62d7b8, the 800x600 / 1024x768 / 1152x864 modes */
int ai_ScreenModeW = 800;  /* 0x62d7d0, the width of the mode ai_GfxResolution picks */
int ai_ScreenModeH = 600;  /* 0x62d7d4, its height */
char *itoa(int value, char *buf, int base); /* 0x5e56c4, the library's integer to text (base 10 here) */

/* 0x5d3ab0, fills the 16 slots of a save / load window (1488 B): view / form the window's form
   (d2_SetForm), slots the 16 slot records, files the 0x210-byte file list buffer and dir the save
   directory. The slots are cleared and marked free (-1), then ai_ListFiles (dir, ".SAV") lists the
   saved games (at most 16): each entry's file (the path at +0x109) is opened, its header read
   (ls_ReadSaveHeader); a map-only save (flags bit 2) is skipped. ai_RegisterSaveSlot puts the entry
   into a free slot, which gets a child window, its caption (the save's title) and the save's date,
   money and difficulty (the texts 0x1cc5 / 0x1cc6, the difficulty name 0x14413f0). A second pass over
   the 16 records gives every still free slot an empty-slot caption ("Slot %i" 0x14414e0), the screen
   image 0x766320 and the text 0x1cc7. Returns nothing. */
void ai_FillSaveSlots(int view, int form, SAVESLOTVIEW *slots, char *files, char *dir, int reserved)
{
    SAVEHEADERVIEW hdr;
    char timebuf[0x40];
    SAVEREQUEST req;
    SAVEREQUEST *rec;
    int n;
    int i;
    int slot;
    int file;
    int k;

    (void)reserved;
    memset(slots, 0, 0x2100);
    for (i = 0; i < 0x10; i++) {
        slots[i].obj = -1;
        slots[i].win = -1;
    }
    n = ai_ListFiles(dir, ".SAV", files);
    if (n > 0x10)
        n = 0x10;
    for (k = 0; k < n; k++) {
        rec = (SAVEREQUEST *)files + k;
        file = vfs_Tfopen(rec->path, "rb", vfs_Root);
        if (file == 0)
            continue;
        if (ls_ReadSaveHeader(file, &hdr) == 0 || (hdr.flags & 2) != 0) {
            vfs_fclose(file);
            continue;
        }
        vfs_fclose(file);
        slot = ai_RegisterSaveSlot(0x10, slots, hdr.slot, rec);
        if (slot == -1)
            continue;
        d2_SetForm(view, form);
        slots[slot].win = d2_CreateChildWindow(0, slot * 0x82, d2_LastWindowPtr->w - 0x10, 0x82, 0x10,
                                             d2_LastWindow);
        d2_Wins[slots[slot].win].font = 0x54;
        slots[slot].obj = d2_CreateWinText(0xa8, -2, hdr.title, d2_LastWindow);
        d2_SetObjFont(slots[slot].obj, 0x53);
        {
            char *src = hdr.title;
            char *dst = slots[slot].title;

            while ((*dst++ = *src++))
                ;
        }
        slots[slot].used = 1;
        if (ls_Version >= 0x10028) {
            for (i = 0; i < 0x78; i++) {
                ai_ScreenImage[i * 0xa0] = 0;
                ai_ScreenImage[i * 0xa0 + 0x9f] = 0;
            }
            for (i = 0; i < 0xa0; i++) {
                ai_ScreenImage[i] = 0;
                ai_ScreenImage[0x4a60 + i] = 0;
            }
            d2_WinOpenRaw(0, 0, 0xa0, 0x78, ai_ScreenImage, d2_LastWindow);
        }
        d2_Objs[slots[slot].obj].interactive = 1;
        d2_Objs[slots[slot].obj].w = d2_LastWindowPtr->w - 0xa0;
        d2_Objs[slots[slot].obj].style = 3;
        d2_CreateChildWindow(0xa4, 0x12, d2_LastWindowPtr->w - 0xb0, 0x6e, 0x10, d2_LastWindow);
        d2_Wins[d2_LastWindow].font = 0x54;
        d2_Wins[d2_LastWindow].mt = 0;
        sprintf(timebuf, "%i%c", hdr.money, 0x11);
        io_PrintText(0x1cc5, hdr.player, hdr.year + 0x578, hdr.hour, hdr.minute % 0x3c);
        io_PrintText(0x1cc6,
                     (hdr.compactTexts ? 0x11e : 0x114) + hdr.traitTextOffset,
                     hdr.scenario,
                     (hdr.compactTexts ? 0x18f : 0x130) + hdr.nameTextOffset,
                     (hdr.compactTexts ? 0x27b : 0x253) + hdr.officeTextOffset,
                     timebuf,
                     ai_DifficultyTexts[hdr.difficulty]);
    }
    memset(ai_ScreenImage, 0, 0x9600);
    for (i = 0; i < 0x10; i++) {
        if (slots[i].win != -1)
            continue;
        memset(&req, 0, 0x210);
        sprintf(req.name, ai_EmptySlotNameFormat, i);
        slot = ai_RegisterSaveSlot(0x10, slots, i, &req);
        if (slot == -1)
            continue;
        d2_SetForm(view, form);
        slots[slot].used = 0;
        slots[slot].win = d2_CreateChildWindow(0, slot * 0x82, d2_LastWindowPtr->w - 0x10, 0x78, 0x10,
                                              d2_LastWindow);
        d2_Wins[slots[slot].win].font = 0x54;
        slots[slot].obj = d2_CreateWinText(0xa8, 0, slots[slot].name, d2_LastWindow);
        d2_SetObjFont(slots[slot].obj, 0x53);
        d2_WinOpenRaw(0, 0, 0xa0, 0x78, ai_ScreenImage, d2_LastWindow);
        d2_WinOpenPic(0x3c, 0x20, 0x4fb, d2_LastWindow);
        d2_Objs[slots[slot].obj].interactive = 1;
        d2_Objs[slots[slot].obj].w = d2_LastWindowPtr->w - 0xa0;
        d2_Objs[slots[slot].obj].style = 3;
        d2_CreateChildWindow(0xa4, 0x18, d2_LastWindowPtr->w - 0xb0, 0x60, 0x10, d2_LastWindow);
        d2_Wins[d2_LastWindow].font = 0x54;
        io_PrintText(0x1cc7);
    }
}
/* 0x5d4080, the load-game window (558 B): opens the form "menu\loadgame_new" (d2_OpenForm), places
   its views (d2_PlaceViewWindows 1 / d2_PlaceMainWindow 2), fits the form (sv_FitForm), sets the
   pointer sprite (d2_SetPointerSprite 0, 0), prints the caption 0x1c57 and builds the slider
   (sv_CreateScrollButtons 0x214 x 0x168, step 0x82) of the list control d2_GetViewWin (win, 1); then
   ai_FillSaveSlots (win, 1) fills the 16 slots from "gamedata/saves". The pump sv_RunFrame
   (0x5309a0) runs with this function and the mask of the frame; per frame a mouse event asks the
   screen to pop and, when dm_L.lclick is set, the 16 slots are walked: the slot is hit when the click
   is on its caption object (d2_PointerObj), in its window (d2_HitWindow) or in a control of it (the
   parent object of the clicked window belongs to the slot's window). A hit asks for confirmation
   (ai_MessageBox 0x101) while a game runs, then writes "Gamedata\Saves\<name>.SAV" into main_NetSaveName,
   sets the load mode (ai_GameFlags 10) and the pop request and returns 1. Returns 0 otherwise. */

int ai_LoadGameWindow(void)
{
    int ret;
    unsigned mask;
    int win;
    int up, bar, down;
    char path[0x104];
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int i;

    ret = 0;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindow(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c57);
    d2_SetForm(0, 0);
    sv_CreateScrollButtons(0x214, 0x168, d2_LastWindow, d2_GetViewWin(win, 1), 0x82, &down, &up, &bar);
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/saves", 0); /* @original: the original call pushes
        a sixth argument 0 that 0x5d3ab0 never reads */
    while (sv_RunFrame((int)ai_LoadGameWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0) {
            for (i = 0; i < 0x10; i++) {
                if (slots[i].used != 0 && slots[i].win != -1
                    && (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                        || (d2_HitWindow != -1
                            && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win]))) {
                    if (ai_GameState == 0 || ai_MessageBox(ai_LoadGameConfirmText, 0x101) != 0) {
                        sprintf(path, "Gamedata\\Saves\\%s.SAV", slots[i].name);
                        strcpy(main_NetSaveName, path);
                        ai_GameFlags = 10;
                        sv_PopRequest = 1;
                        ret = 1;
                    }
                    break;
                }
            }
        }
    }
    d2_CloseView(win);
    return ret;
}
/* 0x5d42b0, the window that picks a saved network game to join (520 B): the twin of ai_LoadGameWindow
   (the same form "menu\loadgame_new", the view placement, the fitted form, the pointer sprite, the
   caption 0x1c57, the slider 0x214 x 0x168 (step 0x82) of the list control d2_GetViewWin (win, 1)) with
   ai_FillSaveSlots (win, 1, "gamedata/network", mode 1). The pump sv_RunFrame runs with this function
   and the frame's mask; a mouse event asks the screen to pop, and when dm_L.lclick is set the 16 slots
   are walked (the same hit test as ai_LoadGameWindow: the caption object, the slot's window, or a
   control of it). A hit asks for confirmation (ai_MessageBox 0x101) while a game runs, then copies the
   slot's name into main_NetSaveName (the caller opens "gamedata/network/<name>.SAV"), asks the screen to
   pop and returns 1. Returns 0 otherwise. */

int ai_JoinGameWindow(void)
{
    int ret;
    unsigned mask;
    int win;
    int up, bar, down;
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int i;

    ret = 0;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindow(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c57);
    d2_SetForm(0, 0);
    sv_CreateScrollButtons(0x214, 0x168, d2_LastWindow, d2_GetViewWin(win, 1), 0x82, &down, &up, &bar);
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/network", 1);
    while (sv_RunFrame((int)ai_JoinGameWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0) {
            for (i = 0; i < 0x10; i++) {
                if (slots[i].used != 0 && slots[i].win != -1
                    && (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                        || (d2_HitWindow != -1
                            && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win]))) {
                    if (ai_GameState == 0 || ai_MessageBox(ai_LoadGameConfirmText, 0x101) != 0) {
                        strcpy(main_NetSaveName, slots[i].name);
                        sv_PopRequest = 1;
                        ret = 1;
                    }
                    break;
                }
            }
        }
    }
    d2_CloseView(win);
    return ret;
}
/* 0x5d44c0, an edit field dialog (261 B): opens an input at (x, y) with the font height, font 0x53,
   the text and the focus; the width / max width of its D2INPUT become 0x80 / 0x110 and its maximum
   value 0x1f. The pump runs with this function; Enter (kb_Last 0x1c) copies the edited text
   (d2_GetObjValue) into text, the mouse event or a lost focus (d2_FocusedInput == 0) only closes the
   dialog; sv_PopRequest asks MASTER to pop the screen. d2_RemoveObj destroys the control at the end.
   Returns 1 when the text was taken over, else 0. */


int ai_EditText(int x, int y, char *text)
{
    int id;
    int ret;
    int *input;

    ret = 0;
    id = d2_OpenInput(x, y, 0, d2_FontHeight, 1);
    d2_SetObjFont(id, 0x53);
    d2_SetObjValue(id, text);
    d2_FocusInput(id);
    input = &d2_Objs[id].input;
    d2_Inputs[*input].w = 0x80;
    d2_Inputs[*input].maxw = 0x110;
    d2_SetObjMax(id, 0x1f);
    while (sv_RunFrame((int)ai_EditText, sv_FrameMask) != 0) {
        if (kb_Last == 0x1c) {
            strcpy(text, (char *)d2_GetObjValue(id));
            sv_PopRequest = 1;
            ret = 1;
        }
        if (dm_L.rup != 0 || d2_FocusedInput == 0)
            sv_PopRequest = 1;
    }
    d2_RemoveObj(id);
    return ret;
}
/* 0x5d4c70, the options writer (2509 B): every option is written with WritePrivateProfileStringA into
   the INI ai_OptionsFile (the sections "Gfx", "Sound", "Game") after a _itoa of a byte or word of
   the option globals (the 12 screen tuning floats scaled by 100, the sound options, the speed keys,
   the switches, the panel options, the seven opponents and the town name written as it is). */

void ai_SaveOptions(void)
{
    char buf[0x20];

    itoa(ai_GfxTextureScale, buf, 10);
    WritePrivateProfileStringA("Gfx", "texture_scale", buf, ai_OptionsFile);
    itoa(ai_GfxDetail, buf, 10);
    WritePrivateProfileStringA("Gfx", "details", buf, ai_OptionsFile);
    itoa(ai_GfxLodHandling, buf, 10);
    WritePrivateProfileStringA("Gfx", "lod_handling", buf, ai_OptionsFile);
    itoa(ai_GfxShadowDetail, buf, 10);
    WritePrivateProfileStringA("Gfx", "shadow_detail", buf, ai_OptionsFile);
    itoa(ai_GfxFloor, buf, 10);
    WritePrivateProfileStringA("Gfx", "floor_mipmapping", buf, ai_OptionsFile);
    itoa(ai_GfxSet.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "gfx_set", buf, ai_OptionsFile);
    itoa(ai_GfxCameraLimits, buf, 10);
    WritePrivateProfileStringA("Gfx", "camera_limits", buf, ai_OptionsFile);
    itoa(ai_GfxFloorLod, buf, 10);
    WritePrivateProfileStringA("Gfx", "floor_lod", buf, ai_OptionsFile);
    itoa(ai_GfxCharDetail, buf, 10);
    WritePrivateProfileStringA("Gfx", "character_detail", buf, ai_OptionsFile);
    itoa(ai_GfxFog.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "fog_plane", buf, ai_OptionsFile);
    itoa(ai_GfxResolution.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "cur_res", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[0] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_r", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[1] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_g", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[2] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_b", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[3] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_a", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[4] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_r", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[5] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_g", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[6] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_b", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[7] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_a", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[8] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_r", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[9] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_g", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[10] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_b", buf, ai_OptionsFile);
    itoa((int)(ai_ScreenTuning[11] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_a", buf, ai_OptionsFile);
    itoa(ai_SoundOptions.masterVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "master_vol", buf, ai_OptionsFile);
    itoa(ai_SoundOptions.sfxVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "sfx_vol", buf, ai_OptionsFile);
    itoa(ai_SoundOptions.msxVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "msx_vol", buf, ai_OptionsFile);
    itoa(ai_SoundOptions.voiceVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "speech_vol", buf, ai_OptionsFile);
    itoa(ai_SoundOptions.msxFreq & 0xff, buf, 10);
    WritePrivateProfileStringA("Sound", "msx_freq", buf, ai_OptionsFile);
    itoa(ai_OptionSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "speed", buf, ai_OptionsFile);
    itoa(ai_OptionMouseSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "mouse_speed", buf, ai_OptionsFile);
    itoa(ai_OptionScrollSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "scroll_speed", buf, ai_OptionsFile);
    itoa(ai_OptionCameraSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "camera_speed", buf, ai_OptionsFile);
    itoa(ai_GameSwitches.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "invert_mouse", buf, ai_OptionsFile);
    itoa(ai_GameSwitches.byte[1], buf, 10);
    WritePrivateProfileStringA("Game", "nachtwaechter", buf, ai_OptionsFile);
    WritePrivateProfileStringA("Game", "stadt", ai_TownName, ai_OptionsFile);
    itoa(ai_OptionHistorie, buf, 10);
    WritePrivateProfileStringA("Game", "historie", buf, ai_OptionsFile);
    itoa(ai_OptionMission, buf, 10);
    WritePrivateProfileStringA("Game", "mission", buf, ai_OptionsFile);
    itoa(ai_OptionNetMission, buf, 10);
    WritePrivateProfileStringA("Game", "net_mission", buf, ai_OptionsFile);
    itoa(ai_GameSwitches.byte[2], buf, 10);
    WritePrivateProfileStringA("Game", "show_cursor_txt", buf, ai_OptionsFile);
    itoa(ai_GameSwitches.byte[3], buf, 10);
    WritePrivateProfileStringA("Game", "show_geb_info", buf, ai_OptionsFile);
    itoa(ai_GamePanel.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "panel_mode", buf, ai_OptionsFile);
    itoa(ai_GamePanel.byte[1], buf, 10);
    WritePrivateProfileStringA("Game", "help_events", buf, ai_OptionsFile);
    itoa(ai_GamePanel.byte[2], buf, 10);
    WritePrivateProfileStringA("Game", "difficulty", buf, ai_OptionsFile);
    itoa(ai_GamePanel.byte[3], buf, 10);
    WritePrivateProfileStringA("Game", "hints", buf, ai_OptionsFile);
    itoa(ai_OptionPanelHelp.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "panel_help", buf, ai_OptionsFile);
    itoa(ai_Opponents[0], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_a", buf, ai_OptionsFile);
    itoa(ai_Opponents[1], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_b", buf, ai_OptionsFile);
    itoa(ai_Opponents[2], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_c", buf, ai_OptionsFile);
    itoa(ai_Opponents[3], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_d", buf, ai_OptionsFile);
    itoa(ai_Opponents[4], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_e", buf, ai_OptionsFile);
    itoa(ai_Opponents[5], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_f", buf, ai_OptionsFile);
    itoa(ai_Opponents[6], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_g", buf, ai_OptionsFile);
}
/* 0x5d5640, the options reader (1523 B), the twin of ai_SaveOptions: every option is read with
   GetPrivateProfileIntA from the INI ai_OptionsFile (the sections "Gfx", "Sound", "Game") into the
   option globals: texture_scale / details / lod_handling / shadow_detail into the bytes 1 / 0 / 2 / 3
   of ai_GfxDetail, floor_mipmapping / camera_limits / floor_lod / character_detail into the bytes of
   ai_GfxFloor (character_detail defaults to 1), fog_plane, the 12 screen tuning floats (the key is the
   value * 100, brightness r / g / b default 0 and a 0x32, contrast and gamma 100), gfx_set, cur_res
   (it also picks the screen mode record: ai_ScreenModeW / H), the sound volumes and msx_freq, the
   speed keys (invert_mouse is forced to 0), the town name ("Augsburg" when the key is missing), the
   historic / mission numbers, the panel options and the seven opponents (default 2).
   The mode record is picked by the stored byte (ai_GfxResolution reread after the store, as the original does). */


void ai_LoadOptions(void)
{
    unsigned v;

    v = GetPrivateProfileIntA("Gfx", "texture_scale", 0, ai_OptionsFile);
    ai_GfxTextureScale = v;
    v = GetPrivateProfileIntA("Gfx", "details", 0, ai_OptionsFile);
    ai_GfxDetail = v;
    v = GetPrivateProfileIntA("Gfx", "lod_handling", 0, ai_OptionsFile);
    ai_GfxLodHandling = v;
    v = GetPrivateProfileIntA("Gfx", "shadow_detail", 0, ai_OptionsFile);
    ai_GfxShadowDetail = v;
    v = GetPrivateProfileIntA("Gfx", "floor_mipmapping", 0, ai_OptionsFile);
    ai_GfxFloor = v;
    v = GetPrivateProfileIntA("Gfx", "camera_limits", 0, ai_OptionsFile);
    ai_GfxCameraLimits = v;
    v = GetPrivateProfileIntA("Gfx", "floor_lod", 0, ai_OptionsFile);
    ai_GfxFloorLod = v;
    v = GetPrivateProfileIntA("Gfx", "character_detail", 1, ai_OptionsFile);
    ai_GfxCharDetail = v;
    v = GetPrivateProfileIntA("Gfx", "fog_plane", 0, ai_OptionsFile);
    ai_GfxFog.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "brightness_r", 0, ai_OptionsFile);
    ai_ScreenTuning[0] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "brightness_g", 0, ai_OptionsFile);
    ai_ScreenTuning[1] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "brightness_b", 0, ai_OptionsFile);
    ai_ScreenTuning[2] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "brightness_a", 50, ai_OptionsFile);
    ai_ScreenTuning[3] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_r", 100, ai_OptionsFile);
    ai_ScreenTuning[4] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_g", 100, ai_OptionsFile);
    ai_ScreenTuning[5] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_b", 100, ai_OptionsFile);
    ai_ScreenTuning[6] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "contrast_a", 100, ai_OptionsFile);
    ai_ScreenTuning[7] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_r", 100, ai_OptionsFile);
    ai_ScreenTuning[8] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_g", 100, ai_OptionsFile);
    ai_ScreenTuning[9] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_b", 100, ai_OptionsFile);
    ai_ScreenTuning[10] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gamma_a", 100, ai_OptionsFile);
    ai_ScreenTuning[11] = (float)v * 0.01f;
    v = GetPrivateProfileIntA("Gfx", "gfx_set", 0, ai_OptionsFile);
    ai_GfxSet.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "cur_res", 0, ai_OptionsFile);
    ai_GfxResolution.byte[0] = v;
    ai_ScreenModeW = ai_ScreenModes[ai_GfxResolution.byte[0]].w;
    ai_ScreenModeH = ai_ScreenModes[ai_GfxResolution.byte[0]].h;
    v = GetPrivateProfileIntA("Sound", "master_vol", 0, ai_OptionsFile);
    ai_SoundOptions.masterVolume = v;
    v = GetPrivateProfileIntA("Sound", "sfx_vol", 0, ai_OptionsFile);
    ai_SoundOptions.sfxVolume = v;
    v = GetPrivateProfileIntA("Sound", "msx_vol", 0, ai_OptionsFile);
    ai_SoundOptions.msxVolume = v;
    v = GetPrivateProfileIntA("Sound", "speech_vol", 0, ai_OptionsFile);
    ai_SoundOptions.voiceVolume = v;
    v = GetPrivateProfileIntA("Sound", "msx_freq", 0, ai_OptionsFile);
    ai_SoundOptions.msxFreqByte[0] = v;
    v = GetPrivateProfileIntA("Game", "speed", 0, ai_OptionsFile);
    ai_OptionSpeed = v;
    v = GetPrivateProfileIntA("Game", "mouse_speed", 0, ai_OptionsFile);
    ai_OptionMouseSpeed = v;
    v = GetPrivateProfileIntA("Game", "camera_speed", 0, ai_OptionsFile);
    ai_OptionCameraSpeed = v;
    v = GetPrivateProfileIntA("Game", "scroll_speed", 0, ai_OptionsFile);
    ai_OptionScrollSpeed = v;
    v = GetPrivateProfileIntA("Game", "invert_mouse", 0, ai_OptionsFile);
    ai_GameSwitches.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "nachtwaechter", 1, ai_OptionsFile);
    ai_GameSwitches.byte[1] = v;
    v = GetPrivateProfileIntA("Game", "show_cursor_txt", 1, ai_OptionsFile);
    ai_GameSwitches.byte[2] = v;
    v = GetPrivateProfileIntA("Game", "show_geb_info", 1, ai_OptionsFile);
    ai_GameSwitches.byte[3] = v;
    ai_GameSwitches.byte[0] = 0;
    GetPrivateProfileStringA("Game", "stadt", "Augsburg", ai_TownName, 0x40, ai_OptionsFile);
    v = GetPrivateProfileIntA("Game", "historie", 0, ai_OptionsFile);
    ai_OptionHistorie = v;
    v = GetPrivateProfileIntA("Game", "mission", 1, ai_OptionsFile);
    ai_OptionMission = v;
    v = GetPrivateProfileIntA("Game", "net_mission", 1, ai_OptionsFile);
    ai_OptionNetMission = v;
    v = GetPrivateProfileIntA("Game", "panel_mode", 1, ai_OptionsFile);
    ai_GamePanel.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "help_events", 1, ai_OptionsFile);
    ai_GamePanel.byte[1] = v;
    v = GetPrivateProfileIntA("Game", "difficulty", 1, ai_OptionsFile);
    ai_GamePanel.byte[2] = v;
    v = GetPrivateProfileIntA("Game", "hints", 1, ai_OptionsFile);
    ai_GamePanel.byte[3] = v;
    v = GetPrivateProfileIntA("Game", "panel_help", 1, ai_OptionsFile);
    ai_OptionPanelHelp.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "opponent_a", 2, ai_OptionsFile);
    ai_Opponents[0] = v;
    v = GetPrivateProfileIntA("Game", "opponent_b", 2, ai_OptionsFile);
    ai_Opponents[1] = v;
    v = GetPrivateProfileIntA("Game", "opponent_c", 2, ai_OptionsFile);
    ai_Opponents[2] = v;
    v = GetPrivateProfileIntA("Game", "opponent_d", 2, ai_OptionsFile);
    ai_Opponents[3] = v;
    v = GetPrivateProfileIntA("Game", "opponent_e", 2, ai_OptionsFile);
    ai_Opponents[4] = v;
    v = GetPrivateProfileIntA("Game", "opponent_f", 2, ai_OptionsFile);
    ai_Opponents[5] = v;
    v = GetPrivateProfileIntA("Game", "opponent_g", 2, ai_OptionsFile);
    ai_Opponents[6] = v;
}

extern unsigned int d3s_TexSize;         /* 0xfb9260, d3s.c */
extern unsigned int d3s_MaxTexSize;      /* 0xfb91dc, d3s.c */
extern float d3o_FogScale;               /* 0x5fbd04, d3_interface.h */
extern float sv_CameraAngleFactor;               /* 0x6f22d8, sv.c */
extern unsigned char ai_GameState;       /* 0x6f25ac */
extern int d3_CurrentScene;              /* 0x633444 */
extern D3SCENE *d3_DefaultScene;         /* 0x5fb5b0 */
extern D3SCENE d3_Scenes[];              /* 0xfd7ba0 */
extern char d3fl_Mips;                   /* 0x63373c, d3_fl.h */
extern char ch_ShadowEnabled;            /* 0x5f122c, the shadow flag ch.c copies into the bodies' +0x211 bit 3 */
extern float sv_CameraLimits[12];        /* 0x6237d0, the camera limit table (height / angle pairs, sv.c) */
extern float sv_CameraHeightNear;        /* 0x623800, the camera height at the near / far blend (sv.c) */
extern float sv_CameraAngleNear;         /* 0x623804, the camera angle at the near / far blend */
extern float sv_CameraHeightFar;         /* 0x623808 */
extern float sv_CameraAngleFar;          /* 0x62380c */
void d3s_ResetShadows(struct D3OBJECT *obj);    /* 0x45ce00, d3s.c */
void d3s_FreeShadows(struct D3OBJECT *obj);     /* 0x45ccd0, d3s.c */
void d3_UpdateAll(unsigned char full);          /* 0x43b6e0, d3_engine.c */
char d3if_SelectScene(int index, char keep);    /* 0x4478e0, d3_interface.c */
void d3_ReloadMeshes(unsigned int mask, int type, void (*progress)(int a, int i, int n));  /* 0x43e090, d3_engine.c */
void d3_ApplyGraphicsOptions(int a, unsigned char b, unsigned char c, unsigned char mode, int e); /* 0x43c4e0 */
#define d3_ApplyGraphicsOptionsB ((void (*)(unsigned char, unsigned char, unsigned char, unsigned char, int))d3_ApplyGraphicsOptions)  /* @original ai_ApplyGfxOptions (0x5d5d4d) pushes the floor option as a byte (mov dl, byte ptr; push edx) */
void ch_SetLowPoly(unsigned char mode);         /* 0x402af0, ch.c */
void d3fl_SetMips(char a);                      /* 0x43f930, d3_fl.c */
void ai_CheckBuildSites(void);                  /* 0x58fb60, lc.c */
void d3_SetRLights(unsigned char on);           /* 0x43e180, d3_engine.c */
struct D3FLMAP;
void init_SetDetailLevel(struct D3FLMAP *s, char f);       /* 0x513590, f3init.c */
void sv_PlaceCamera(float f);                   /* 0x525dd0, sv.c */
char d3_ForEachObject2(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, void (*cb)(struct D3OBJECT *obj)); /* 0x438e60, d3_engine.h */
/* the LOD mask at 0x5fbf78 as this unit stores it (the whole dword, d3_int_io.h's D3IOLOD bits) */
typedef union INIT_LOD {
    unsigned int all;               /* the whole dword, which is what the original compares and stores */
    struct {
        unsigned int mode : 7;      /* 1: all three LODs, 2: the best one found */
        unsigned int shadow : 1;    /* the "_s" shadow variant */
    } bits;
} INIT_LOD;
extern INIT_LOD d3io_Lod;          /* 0x5fbf78 */

/* 0x5d5c40, applies the graphics options to the scene: the shadow detail (ai_GfxShadowDetail) picks the
   character shadow flag ch_ShadowEnabled and the shadow texture size (0x20 / 0x40), a changed size resets / frees
   every shadow and updates the scene, the map's detail level is set (init_SetDetailLevel), the LOD handling
   (ai_GfxLodHandling) selects d3io_Lod.mode and reloads the meshes with the scene deselected, d3_ApplyGraphicsOptions gets
   the floor mipmapping (ai_GfxFloor), the texture scale (2 - ai_GfxTextureScale) and the changed flag, the fog scale follows ai_GfxFog
   while no building is shown, the bodies of the town scene's characters get the shadow bit (+0x211 bit 3),
   ch_SetLowPoly gets the character detail (ai_GfxCharDetail), the camera limit pair of the camera_limits byte
   (ai_GfxCameraLimits) is copied to sv_CameraHeightNear .. sv_CameraAngleFar, the game state re-places the
   camera, the mipmaps follow the floor setting and the scene is selected around the build-site check and the
   red lights */
void ai_ApplyGfxOptions(void)
{
    unsigned char level, changed;
    unsigned size;
    int lod, scene, i;
    CHARACTER *c;

    lod = 0;
    level = ai_GfxShadowDetail;
    changed = 0;
    size = d3s_TexSize;
    if (level == 0) {
        changed = 0;
        ch_ShadowEnabled = 1;
        size = 0x20;
    } else if (level == 1) {
        ch_ShadowEnabled = 1;
        size = 0x20;
        changed = 1;
    } else if (level == 2) {
        ch_ShadowEnabled = 0;
        size = 0x40;
        changed = 1;
    }
    if (size != d3s_TexSize) {
        d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3s_ResetShadows);
        d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3s_FreeShadows);
        d3s_TexSize = size;
        if (size >= d3s_MaxTexSize)
            d3s_TexSize = d3s_MaxTexSize;
        d3_UpdateAll(0);
    }
    init_SetDetailLevel(ai_MapRefs, 0);
    if (ai_GfxLodHandling == 0)
        lod = 2;
    else if (ai_GfxLodHandling == 1)
        lod = 4;
    if (lod != (int)d3io_Lod.all) {
        scene = d3_CurrentScene;
        d3io_Lod.all = lod;
        d3if_SelectScene(0, 0);
        d3_ReloadMeshes(0x40, 3, 0);
        d3if_SelectScene(scene, 0);
    }
    d3_ApplyGraphicsOptionsB(ai_GfxFloor, 0, 2 - ai_GfxTextureScale, changed, 0);
    if (ai_PlayerBuildingB == 0 && ai_FallbackBuildingB == 0)
        d3o_FogScale = 1.0 - ai_GfxFog.byte[0] * 0.01;
    for (i = 0; i < 0x300; i++) {
        c = ch_List[i];
        if (c != 0 && c->scene == d3_Scenes)
            c->obj->objectFlags = ((ch_ShadowEnabled << 3 ^ c->obj->objectFlags) & 8) ^ c->obj->objectFlags;
    }
    ch_SetLowPoly(ai_GfxCharDetail);
    sv_CameraHeightNear = sv_CameraLimits[ai_GfxCameraLimits * 2];
    sv_CameraAngleNear = sv_CameraLimits[ai_GfxCameraLimits * 2 + 1];
    sv_CameraHeightFar = sv_CameraLimits[ai_GfxCameraLimits * 2 + 6];
    sv_CameraAngleFar = sv_CameraLimits[ai_GfxCameraLimits * 2 + 7];
    if (ai_GameState != 0)
        sv_PlaceCamera(sv_CameraAngleFactor);
    if (d3fl_Mips != (char)ai_GfxFloor)
        d3fl_SetMips(ai_GfxFloor);
    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    ai_CheckBuildSites();
    if (ai_GfxDetail == 0)
        d3_SetRLights(0);
    else
        d3_SetRLights(1);
    d3if_SelectScene(scene, 1);
}
/* 0x5d5e90, applies the option values to the run time (117 B): the game speed factor cm_SpeedFactor
   from the "speed" option (ai_OptionSpeed * 0.00625 + 0.25; in a network game (ai_GameFlags & 4) from
   the shared speed level cm_SpeedLevel: (level * 0.25 + 0.5) * 0.8), the input base kb_SetBase
   (ai_OptionMouseSpeed - 0x40), the camera step ai_CameraStep = 100 - the low byte of the
   "camera_speed" option and di_SwapLR = 0. Called on every option apply, at the game start and by the
   speed keys (0x589cf0). */


void ai_ApplyOptions(void)
{
    double speed;

    di_SwapLR = 0;
    if ((ai_GameFlags & 4) != 0)
        speed = ((double)(unsigned int)cm_SpeedLevel * 0.25 + 0.5) * 0.8;
    else
        speed = (float)ai_OptionSpeed * 0.00625f + 0.25;
    cm_SpeedFactor = (float)speed;
    kb_SetBase(ai_OptionMouseSpeed - 0x40);
    ai_CameraStep = 100 - (ai_OptionCameraSpeed);
}
/* 0x5d5f10, applies the sound option volumes to the mixer (192 B): the master volume scales the sound
   driver (sb_SetMasterVolume, sfx * master / 127) and the music volume (msx_SetVolume, msx * master /
   127); the ambient driver gets the sfx volume (msx_SetAmbVolume). snd_VoiceVolume = speech / 127 and
   snd_MusicVolume = the low byte of the msx_freq option * 0.25. Called on every option apply. */

void ai_ApplyVolumes(void)
{
    float master;

    master = (float)ai_SoundOptions.masterVolume * (1.0f / 127.0f);
    sb_SetMasterVolume((int)((float)ai_SoundOptions.sfxVolume * master));
    msx_SetVolume((int)((float)ai_SoundOptions.msxVolume * master));
    msx_SetAmbVolume((int)((float)ai_SoundOptions.sfxVolume * master));
    snd_VoiceVolume = (float)ai_SoundOptions.voiceVolume * (1.0f / 127.0f);
    snd_MusicVolume = (float)(ai_SoundOptions.msxFreq & 0xff) * 0.25f;
}

#define U_W(a) (*(unsigned short *)(a))

/* d2.h's D2SURF (the 0x40-byte screen surface): the two fields the screen shot reads. Type debt: d2.c
   defines d2_ScreenSurf as void * (D2SURF itself is in d2_types.h). */
typedef struct D2SURFVIEW {
    char pad0[0x10];
    int pitch;                          /* +0x10, in pixels */
    char pad1[8];
    unsigned short *bits;               /* +0x1c, pixels (system memory, or while locked) */
} D2SURFVIEW;
extern D2SURFVIEW *d2_ScreenSurf;       /* 0x6ed924, d2.h: D2SURF * */
int d2_LockSurface(D2SURFVIEW *surf);   /* 0x479020, d2.h: D2SURF *, locks the surface (pixels in bits) */
int d2_UnlockSurface(D2SURFVIEW *surf); /* 0x4790e0, d2.h: D2SURF * */
void *mem_Alloc(int size, char *tag);   /* 0x4223c0, d3.h */
void mem_Free(void *p);                 /* 0x422620, d3.h */
extern unsigned int dd_RMask;           /* 0x1464fa4, dd_vesa.h: the red channel mask of the display */
extern unsigned int dd_GMask;           /* 0x1464fb0, dd_vesa.h */
extern unsigned int dd_BMask;           /* 0x146359c, dd_vesa.h */

int ls_Read(void *buf, int size, int count, int f);   /* 0x51c920, ls.c: vfs_fread items */
void sv_SetStatusText(char *s);   /* 0x52e330, sv.c */
int cm_SendExSysMessage(unsigned char id, void *msg);   /* 0x4c4db0, as in cm.h */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);   /* 0x41e670, as in dd_vesa.h */
unsigned int util_Hash(unsigned char *buf, unsigned int len);      /* 0x401000, as in bio.h */
void sv_PumpNet(void);                                             /* 0x52fe00, sv.c */
void sv_BroadcastText(unsigned fl, char *t);  /* 0x52fcf0, sv.c: sends t as the network message 0xe and
                                                 shows it until the answer, then clears the status line */
int ls_WriteSaveHeader(int f, unsigned flags); /* 0x5194c0, ls.c: writes the save header (the flag word) */
int ls_ReadSaveHeader(int f, int *h);         /* 0x51cee0, ls.c: reads the save header into h */

/* cm.h's save-request state (the 0x0f the network message carries) and the acknowledgement slots. */
extern int cm_RequestedSaveSlot;        /* 0x6f25c8, cm.h: the save slot the request asked for */
extern char cm_RequestedSaveName[];     /* 0x6f25d0, cm.h: the save name the request carried */
extern int cm_SaveAcks[][2];            /* 0x76fa20, cm.h: 8 slots; [0] = the player's netId, [1] = the
                                           save name hash, -1 when empty (message 0x10 fills the first free) */
extern unsigned char ai_NumPlayers;     /* 0x62489c, the player count (ai.c / cm.c / main.c) */
extern unsigned short ai_PlayerId;      /* 0x6f25b8, the local player (cm.h / am.h / gv.h) */
/* ai_SaveWaitText: view of txt_Ptrs (build/mk_txt_views.py) */

/* ai.h's AICHAR (0x21c bytes, ai_CharsC[0x300] at 0x770b80): the fields this unit reads. f3snd.h declares
   ai_CharsC as char[], so ai.h cannot be included here; type debt. */
typedef struct AICHARVIEW {
    unsigned short id;                  /* +0x000, 0xffff when free */
    unsigned char kind;                 /* +0x002, 6 / 7 = a player */
    char pad003;
    unsigned int netId;                 /* +0x004 */
    char pad008[0x15e];
    unsigned char officeKind;           /* +0x166, the kind of the office held (amt.h AMTCHARVIEW.officeKind) */
    char pad167[0x21];
    char selected;                      /* +0x188, the group selection flag (sv.c ai_Selected, cleared by sv_ClearSelection) */
    char pad189[3];
    int pic;                            /* +0x18c, the picture of the character the poker panel shows (reasoned: the picture index) */
    char pad190[0x3c];
    unsigned int flags1cc;             /* +0x1cc, ai.h's flags1cc (bit 0x20000 = the blood revenge) */
    char pad1d0[0x3c];
    int partyId;                        /* +0x20c, the scene party the character belongs to, -1 = none (ai_char.h AICHAR.partyId) */
    char pad210[0xc];
} AICHARVIEW;

/* the same records from their kind byte (ai_CharsC + 2), the base ai_HasQuicksavePlayer walks */
#pragma pack(push, 1)
typedef struct AICHARKINDVIEW {
    unsigned char kind;                 /* +0x000 (record +2), 6 / 7 = a player */
    char pad001[5];
    unsigned char health;               /* +0x006 (record +8), > 0 = alive (ai_char.h AICHAR.health) */
    char pad007[0x203];
    int partyId;                        /* +0x20a (record +0x20c), -1 = none (ai_char.h AICHAR.partyId) */
    char pad20e[0xe];
} AICHARKINDVIEW;
#pragma pack(pop)

/* 0x5d6f40, creates one menu button of an options dialog (84 B): a control of the dialog (picture
   0xbf, the caption texts text1 / text2) at x 0x20 and the row's y, centred (d2_Objs[id].centred,
   d2_SetObjWidth re-centres it on the 300 width). The twin of main_MenuButton (row stride 0x2b).
   Returns the control id. */
int ai_MenuButton(int row, int text1, int text2)
{
    int id;

    id = d2_WinCreateSlider(0x20, row * 0x2e + 10, 0xbf, text1, text2);
    d2_Objs[id].centred = 1;
    d2_SetObjWidth(id, 300);
    return id;
}
/* 0x5d6fa0, the black fade in of a screen change (259 B): d2_StartFade (0, 0, screen width
   0xfb9072, height 0xfb9070, "BLACK" 0x5fea38, 0x1e, 1) runs until the fade record's flags byte has bit 2,
   pumping sv_RunFrame with the frame flags 0x760638 & ~0x2000 | 0x100000. Then the dirty-guard rectangle
   0xfb9060 .. 0xfb906c takes the current screen size (right = top = 0, so every d2_AddDirty test hits and
   the game screen redraws fully), 0x760638 = 0x24087, and the engine is reset
   (sv_DisableVisibleWidgets (1), d2_ClearScreen, d2_ShowPicture (0x794), d2_ReleaseFade (fade),
   sv_SetPanelEnabled (0)) and a second black fade (0x32, 10) is started as the new screen's black cover. */

void ai_LeaveGameScreen(void)
{
    D2FADEVIEW *fade;
    int flags;
    short w, h;

    flags = (sv_FrameMask & ~0x2000) | 0x100000;
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x1e, 1);
    while ((fade->flags & 4) == 0) {
        sv_RunFrame(0, flags);
    }
    d2_DirtyRectRight = 0;
    w = d2_ScreenW;
    d2_DirtyRectLeft[0] = w;
    d2_DirtyRectTop = 0;
    h = d2_ScreenH;
    d2_DirtyRectBottom = h;
    sv_FrameMask = 0x24087;
    sv_DisableVisibleWidgets(1);
    d2_ClearScreen();
    d2_ShowPicture(0x794);
    d2_ReleaseFade(fade);
    sv_SetPanelEnabled(0);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x32, 10);
}
/* 0x5d7080, the black fade out back to the game (222 B), the twin of 0x5d6fa0: d2_StartFade
   (0, 0, width, height, "BLACK", 0x1e, 1) runs until bit 2, pumping sv_RunFrame (0, 0x24087). When the
   game is not over (ai_GameOver 0x6f259c == 0) the dirty-guard rectangle 0xfb9060 .. 0xfb906c is restored
   from sv_ViewRectLeft .. sv_ViewRectBottom 0x6248a4 .. 0x6248b0, the engine resumes
   (d2_ClearScreen, sv_SetPanelEnabled (1), sv_RedrawScreen, sv_DisableVisibleWidgets (0),
   d2_ReleaseFade, then the black cover d2_StartFade (.., 0x32, 10)); when the game is over only
   d2_ClearScreen / d2_ReleaseFade run and ai_FrameWait 0x6f22bc = 3 keeps the frame for 3 more rounds. */

void ai_ResumeGameScreen(void)
{
    D2FADEVIEW *fade;

    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x1e, 1);
    while ((fade->flags & 4) == 0) {
        sv_RunFrame(0, 0x24087);
    }
    if (ai_GameOver != 0) {
        d2_ClearScreen();
        d2_ReleaseFade(fade);
        ai_FrameWait = 3;
        return 0;
    }
    d2_DirtyRectRight = sv_ViewRectRight;
    d2_DirtyRectLeft[0] = sv_ViewRectLeft;
    d2_DirtyRectTop = sv_ViewRectTop;
    d2_DirtyRectBottom = sv_ViewRectBottom;
    d2_ClearScreen();
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    sv_DisableVisibleWidgets(0);
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x32, 10);
    return 0;
}
/* 0x5d7160, the screenshot into the small screen image ai_ScreenImage (690 B): locks the screen
   surface (d2_ScreenSurf), samples its pixels (+0x1c, pitch +0x10) into a 0x25800 byte 16-bit buffer
   (mem_Alloc "!f3:ScreenShot") at the scale 0.003125 per source pixel - the original keeps one float
   step, width * 0.003125, and advances it over the 0x140 columns and the 0xf0 rows with an x / y
   accumulator (the row and column are the float -> int converts 0x5de518). Then surf_ScaleBlit
   0x420140 shrinks the buffer into the 0x78 x 0xa0 image ai_ScreenImage 0x766320 (pitch 0x140; the
   source desc carries the display masks dd_RMask / dd_GMask / dd_BMask, both descs are 16 bpp) and the
   buffer is freed. Void; when the surface does not lock it returns early. */

void ai_Screenshot(void)
{
    DDSURFACEDESC2 src, dst;
    unsigned short *buf;
    float step, xf;
    int i = 0, x, y;
    float yf = 0.0f;

    step = (float)d2_ScreenW * 0.003125f;
    if (d2_LockSurface(d2_ScreenSurf) == 0)
        return;
    buf = mem_Alloc(0x25800, "!f3:ScreenShot");
    for (y = 0xf0; y != 0; y--) {
        int row;

        xf = 0.0f;
        row = (int)yf * d2_ScreenSurf->pitch;
        for (x = 0x140; x != 0; x--) {
            int col = (int)xf;
            xf += step;
            buf[i++] = d2_ScreenSurf->bits[col + row];
        }
        yf += step;
    }
    d2_UnlockSurface(d2_ScreenSurf);
    D3_ZERO(&src, sizeof(src));
    src.dwSize = sizeof(src);
    src.dwWidth = 0x140;
    src.dwHeight = 0xf0;
    src.lPitch = 0x280;                 /* 0x140 pixels of 16 bits */
    src.lpSurface = buf;
    src.ddpfPixelFormat.dwRBitMask = dd_RMask;
    src.ddpfPixelFormat.dwGBitMask = dd_GMask;
    src.ddpfPixelFormat.dwBBitMask = dd_BMask;
    src.ddpfPixelFormat.dwRGBBitCount = 0x10;
    D3_ZERO(&dst, sizeof(dst));
    dst.dwSize = sizeof(dst);
    dst.dwWidth = 0xa0;
    dst.dwHeight = 0x78;
    dst.lPitch = 0x140;                 /* 0xa0 pixels of 16 bits */
    dst.lpSurface = ai_ScreenImage;
    dst.ddpfPixelFormat.dwRGBBitCount = 0x10;
    surf_ScaleBlit(&dst, &src);
    mem_Free(buf);
}
/* 0x5d7320, loads the loading screen image (275 B): opens path (vfs_Tfopen, "rb", vfs_Root), reads
   0xe100 bytes (ls_Read 0x51c920, 0x78 x 0xa0 RGB triples) and converts them to the 16-bit pixels of
   ai_ScreenImage 0x766320 (dd_RGB, the pixel index runs across both loops), then frees the buffer and
   closes. Returns nothing (the original returns no value; the callers ignore it). */

void ai_LoadScreenImage(char *path)
{
    unsigned char *buf;
    VFSFILE *h;
    int i = 0, x, y;

    h = vfs_Tfopen(path, "rb", vfs_Root);
    if (h == 0)
        return;
    buf = (unsigned char *)mem_Alloc(0xe100, "ls:tmp");
    ls_Read(buf, 0xe100, 1, (int)h);
    for (y = 0; y < 0x78; y++) {
        for (x = 0; x < 0xa0; x++) {
            ai_ScreenImage[i] = dd_RGB(buf[i * 3], buf[i * 3 + 1], buf[i * 3 + 2]);
            i++;
        }
    }
    if (buf != 0)
        mem_Free(buf);
    vfs_fclose(h);
}
/* 0x5d73e0, is there a character that can be quicksaved (78 B): 0 -> no game (ai_GameState 0x6f25ac
   == 0); otherwise walks the character records (ai_Chars + 2, stride 0x21c) for a living player (kind
   6 / 7) that belongs to a scene party (partyId != -1); 0 if found, 1 when the walk passes
   gm_PlayerRecordBytes 0x7d5f82 */

int ai_HasQuicksavePlayer(void)
{
    AICHARKINDVIEW *p;
    int r;

    r = 1;
    if (ai_GameState == 0)
        r = 0;
    else {
        p = ai_CharsK;
        do {
            if (p->health > 0 && (p->kind == 6 || p->kind == 7) && p->partyId != -1)
                return 0;
            p++;
        } while ((int)p < (int)gm_PlayerRecordBytes);
    }
    return r;
}
/* 0x5d7430, the quicksave: in a normal game (ai_GameFlags bit 2 clear, bit 7 clear) it shows the
   "Quicksave" status text (sv_SetStatusText 0x52e330), pumps one frame (sv_RunFrame 0x5309a0 (0,
   0x657c7)), takes the screenshot (ai_Screenshot 0x5d7160), sets the save slot (ls_SaveSlot = 1),
   writes the save (ls_SaveGame 0x5190b0: "Gamedata\Saves\Quicksave.SAV" 0x62db54, "Quicksave"
   0x62db74, 1) and clears the status text (ch_EmptyName 0x631090). In a network game (bit 2): without
   bit 4 it shows ai_QuickSaveDisabledText; when ai_HasQuicksavePlayer finds no saveable player it
   shows ai_QuickSaveNoPlayerText; otherwise it sends the 0xf request (CMQUICKSAVE: slot 1, name
   "QUICKSAVE" 0x624528) through cm_SendExSysMessage, then waits for the message slot to change
   (cm_GetSlotState) while pumping the net (sv_PumpNet 0x52fe00). Void; the callers ignore the result. */

void ai_Quicksave(void)
{
    CMQUICKSAVE rec;
    char flags;
    int msg;
    int r;

    flags = ai_GameFlags;
    if ((flags & 4) != 0) {
        if ((flags & 0x10) == 0) {
            sv_SetStatusText(ai_QuickSaveDisabledText);
            return;
        }
        if (ai_HasQuicksavePlayer() == 0) {
            sv_SetStatusText(ai_QuickSaveNoPlayerText);
            return;
        }
        memset(&rec, 0, sizeof(rec));
        rec.slot = 1;
        strcpy(rec.name, "QUICKSAVE");
        /* @original cm.h declares cm_SendExSysMessage void, but the original caller reads the call's
           eax as the message handle (0x4c4db0 returns 0x4c39c0's int); the call goes through a
           prototype that returns int and the handle is passed to cm_GetSlotState */
        msg = ((int (__cdecl *)(unsigned char, void *))cm_SendExSysMessage)(0xf, &rec);
        r = cm_GetSlotState(msg);
        if (r == 0) {
            do {
                sv_PumpNet();
                r = cm_GetSlotState(msg);
            } while (r == 0);
        }
        return;
    }
    if ((flags & 0x80) != 0)
        return;
    sv_SetStatusText("Quicksave");
    sv_RunFrame(0, 0x657c7);
    ai_Screenshot();
    ls_SaveSlot = 1;
    ls_SaveGame("Gamedata\\Saves\\Quicksave.SAV", "Quicksave", 1);
    sv_SetStatusText(ch_EmptyName);
    return;
}
/* 0x5d7530, saves a network game (0x237 B): saveName is the file name under Gamedata/network, name the
   save name recorded in the header. sv_FrameMask gets 0x20000 and the 8 acknowledgement slots cm_SaveAcks
   are cleared; only in a network game (ai_GameFlags bit 2): the other players are told the status text
   ai_SaveWaitText (sv_BroadcastText), the player count (kind 6 / 7 records with an id) goes into
   ai_NumPlayers, name into ls_SavePath, ls_SaveSlot takes the requested slot cm_RequestedSaveSlot, the
   standard city screen is shown, then a .SAV file is created with the save header (flag 5) and read back
   to hash the header's 0x78 bytes (util_Hash). The hash and the local player's netId (ai_Chars) go into
   the 0x7c-byte system message 0x10, which the peers acknowledge by filling cm_SaveAcks; the function
   pumps the network until as many slots are filled as there are players, then writes the .SRV (flag 6)
   and .SAV (flag 5) saves (ls_SaveGame) and clears the status line. Void; outside a network game it only
   clears the slots. */

void ai_SaveNetworkGame(char *saveName, char *name)
{
    char path[MAX_PATH];
    char hdr[0xfc];
    int rec[0x1f];
    VFSFILE *h;
    unsigned int hash = 0;
    AICHARVIEW *c;
    int i, n;

    sv_FrameMask |= 0x20000;
    for (i = 0; i < 8; i++)
        cm_SaveAcks[i][0] = -1;
    if ((ai_GameFlags & 4) == 0)
        return;
    sv_BroadcastText(sv_FrameMask, ai_SaveWaitText);
    ai_NumPlayers = 0;
    for (i = 0; i < 768; i++) {
        c = &((AICHARVIEW *)ai_CharsC)[i];
        if (c->id != 0xffff && (c->kind == 6 || c->kind == 7))
            ai_NumPlayers++;
    }
    strcpy(ls_SavePath.path, name);
    ls_SaveSlot = cm_RequestedSaveSlot;
    ai_LoadScreenImage("Gamedata/cities/standard.scr");
    sprintf(path, "Gamedata\\network\\%s.SAV", saveName);
    h = vfs_Tfopen(path, "wb", vfs_Root);
    if (h != 0) {
        ls_WriteSaveHeader(h, 5);
        vfs_fclose(h);
    }
    h = vfs_Tfopen(path, "rb", vfs_Root);
    if (h != 0) {
        ls_ReadSaveHeader(h, hdr);
        hash = util_Hash((unsigned char *)hdr, 0x78);
        vfs_fclose(h);
    }
    memset(rec, 0, sizeof(rec));
    rec[0] = ((AICHARVIEW *)ai_CharsC)[ai_PlayerId].netId;
    rec[1] = hash;
    cm_SendExSysMessage(0x10, rec);
    if (ai_NumPlayers > 0) {
        do {
            n = 0;
            for (i = 0; i < 8; i++)
                if (cm_SaveAcks[i][0] != -1)
                    n++;
            sv_PumpNet();
        } while (n < ai_NumPlayers);
    }
    sv_BroadcastText(sv_FrameMask, ai_SaveWaitText);
    sprintf(path, "Gamedata\\network\\%s.SRV", saveName);
    ls_SaveGame(path, name, 6);
    sprintf(path, "Gamedata\\network\\%s.SAV", saveName);
    ls_SaveGame(path, name, 5);
    sv_SetStatusText(ch_EmptyName);
}
#define U_F(a) (*(float *)(a))

/* the callees of the front-end stretch (their units' headers are not included here, so their
   prototypes are repeated as the definitions spell them; the building pointers are this TU's ints) */
struct AIBLD; int ai_ProductionWindow(struct AIBLD *bld); /* 0x58bd60, lc.c: "Handel\Handel_PRODUKTION" */
int ai_OpenTransport(int rec);             /* 0x5af1a0, lc.c */
void ai_TradeSearchWindow(unsigned char *cfg, int arg); /* 0x595480, lc.c: "HANDEL\HANDEL_SUCHEN" */
void ai_MasterLetterWindow(struct AIBLD *bld); /* 0x5c3200, lc.c: "ob_MEISTERBRIEF" */
void ai_MarketContact(struct AIBLD *rec);  /* 0x58f510, lc.c */
int sv_CheckMoney(char *o, int n);         /* 0x520f90, sv.c: 1 when the object's money (+0x194)
                                              covers n, else a message box with the text 0x74 */
int sv_MessageBox(int text, unsigned flags); /* 0x521150, sv.c: the modal message box */
void sv_CameraFollow(void);                /* 0x52d7a0, sv.c: the per-frame camera follow */
int d2_RemoveWindow(); int io_PrintText(char *fmt, ...);
int d2_GetObjValue();

/* 0x5d7770, the options menu screen (1112 B): param none, void. The game screen 0x5d7160 is copied
   into the local 0x2580-dword image, the screen fades black (0x5d6fa0), the window "menu\options"
   0x62dbb8 (0x4741f0 / 0x475870 (1) / 0x475850 (2) / d2_SetForm (0)) with the screen 0x52ea30
   (0x6ed938) and 0x520f20 is shown and the seven buttons are built by 0x5d6f40 (the texts 0x1441330
   .. 0x1441348, the font 0x6ed938): ids 0 / 1 (load / save: 0x475e40 off in a network game), 2 (leave
   the game, confirmed in the 0x200-byte text buffer), 3 (graphics options 0x5d5fd0), 4 (sound options
   0x5d6580), 5 (game options 0x5d6950) and 7 (back), then the radio group 0x46c490 (7, id0, id1, id2,
   id3, id4, id5, id7), 0x423a50 (0) and the frame flags 0x6f0b60 saved / cleared, 0x4c3140. The pump
   0x5309a0 (0x5d7770, 0x24087) loops: d2_MenuNavigate (group); a pending screen 0x6f22a0 = 1 on
   0x14673a0 / 0x1468410; a click 0x5fe710: id0 -> 0x475590 (wnd, 0), 0x5d4080 () -> the game start
   (0x6f25a0 = 0x6f259c = 1, 0x52fb50 (0x53bb20)); id1 -> 0x5d4920 / 0x5d45d0 (the image); id5 ->
   0x5d6950; id2 -> the confirmation text 0x5176d0 (id 0x1ca4) + the box 0x5d3810 -> 0x4c3f90 /
   0x4c3ae0 wait, 0x6f259c = 1, 0x52fb50; id3 -> 0x5d5fd0; id4 -> 0x5d6580; id7 -> 0x6f22a0 = 1.
   The click chain order is id0, id1, id5, id2, id3, id4, id7 (the original's compare order, not the
   creation order). On a zero pump: 0x5d7080 (fade out), 0x6333e0 = 0, 0x6f25ae = 0, d2_RadioClear,
   d2_CloseView, the ambient volume restored from the sfx option byte, 0x6f0b60 restored, 0x4c30e0,
   and with the sign bit of the game flags set and no game end (0x6f259c) 0x576030 (tail call). */
void ai_OptionsMenu(void)
{
    int image[0x2580];
    char text[0x200];
    int wnd, group, pump;
    int id0, id1, id2, id3, id4, id5, id7;
    int saved;
    int click;

    ai_Screenshot();
    memcpy(image, ai_ScreenImage, sizeof(image));
    ai_LeaveGameScreen();
    wnd = d2_OpenForm(0, 0, "menu\\options");
    sv_SetScreenCenter(0x24087);
    d2_PlaceViewWindows(wnd, 1);
    d2_PlaceMainWindow(wnd, 2);
    d2_SetForm(wnd, 0);
    sv_FitForm(d2_LastWindow);
    sv_ResetScreenClip();
    d2_SetForm(0, 0);
    id0 = ai_MenuButton(0, (int)ai_TextLoadGame, d2_LastWindow);
    id1 = ai_MenuButton(1, (int)ai_TextSaveGame, d2_LastWindow);
    if ((ai_GameFlags & 4) != 0) {
        d2_SetObjEnabled(id0, 0);
        if ((ai_GameFlags & 0x10) == 0)
            d2_SetObjEnabled(id1, 0);
    }
    if ((ai_GameFlags & 0x80) != 0) {
        unk_ClearEndGameGate();
        d2_SetObjEnabled(id0, 0);
        d2_SetObjEnabled(id1, 0);
    }
    sv_MenuOpen = 1;
    id2 = ai_MenuButton(2, (int)ai_TextGameOptions, d2_LastWindow);
    id3 = ai_MenuButton(3, (int)ai_TextGfxOptions, d2_LastWindow);
    id4 = ai_MenuButton(4, (int)ai_TextSfxOptions, d2_LastWindow);
    id5 = ai_MenuButton(5, (int)ai_TextLeaveGame, d2_LastWindow);
    if ((ai_GameFlags & 4) == 0)
        tb_Paused = 1;
    id7 = ai_MenuButton(7, (int)ai_TextBack, d2_LastWindow);
    group = d2_CreateRadioGroup(7, id0, id1, id2, id3, id4, id5, id7);
    msx_SetAmbVolume(0);
    saved = snd_AmbientOn;
    snd_AmbientOn = 0;
    snd_StopMarketAmbience();
    pump = sv_RunFrame(ai_OptionsMenu, 0x24087);
    while (pump != 0) {
        d2_MenuNavigate(group);
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0 && (click = d2_PointerObj) != -1) {
            if (click == id0) {
                d2_SetViewEnabled(wnd, 0);
                if (ai_LoadGameWindow() != 0) {
                    ai_LeaveGame = 1;
                    ai_GameOver = 1;
                    sv_PopTo((int)main_GameLoop);
                } else {
                    d2_SetViewEnabled(wnd, 1);
                }
            } else if (click == id1) {
                d2_SetViewEnabled(wnd, 0);
                if ((ai_GameFlags & 4) == 0)
                    ai_SaveOptionWindow(image);
                else
                    ai_SaveNameWindow(image);
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id5) {
                d2_SetViewEnabled(wnd, 0);
                txt_Format(text, 0x1ca4);       /* 0x1ca4: the "leave the game?" text of the text table */
                if (ai_MessageBox((int)text, 1) != 0) {
                    int r = cm_SendPlayerId();
                    if (r != -1) {
                        do {
                            if (cm_GetSlotState(r) != 0)
                                break;
                            sv_PumpNet();
                        } while (1);
                    }
                    ai_GameOver = 1;
                    sv_PopTo((int)main_GameLoop);
                } else {
                    d2_SetViewEnabled(wnd, 1);
                }
            } else if (click == id2) {
                d2_SetViewEnabled(wnd, 0);
                ai_OptionsGameScreen();
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id3) {
                d2_SetViewEnabled(wnd, 0);
                ai_OptionsGfxScreen();
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id4) {
                d2_SetViewEnabled(wnd, 0);
                ai_OptionsSfxScreen();
                d2_SetViewEnabled(wnd, 1);
            } else if (click == id7) {
                sv_PopRequest = 1;
            }
        }
        pump = sv_RunFrame(ai_OptionsMenu, 0x24087);
    }
    ai_ResumeGameScreen();
    if ((ai_GameFlags & 4) == 0)
        tb_Paused = 0;
    sv_MenuOpen = 0;
    d2_RadioClear(group);
    d2_CloseView(wnd);
    msx_SetAmbVolume(ai_SoundOptions.sfxVolume);
    snd_AmbientOn = saved;
    snd_StartMarketAmbience();
    if ((ai_GameFlags & 0x80) != 0 && ai_GameOver == 0)
        unk_SetEndGameGate();
}

/* ---- the two save / load window bodies and the three option screen stubs are defined below
   0x5d7770 (out of address order: 0x5d45d0 / 0x5d4920 / 0x5d5fd0 / 0x5d6580 / 0x5d6950 belong before
   it). VC6 drops a call whose result is unused to a function whose side-effect-free body it has
   already seen (an empty stub, or a `return 0;` stub); the original's caller 0x5d7770 was compiled
   against the real bodies. Move them back into address order once they have their bodies. ---- */

/* 0x5d45d0, the save window (839 B): the twin of ai_LoadGameWindow (the same form "menu\loadgame_new",
   the view placement, the fitted form, the pointer sprite, the caption 0x1c58, the slider 0x214 x 0x168
   of the list control, ai_FillSaveSlots (win, 1, "gamedata/saves", mode 0)), but the two autosave /
   quicksave slots (0 / 1) are disabled (d2_SetObjEnabled 0) and only the slots 2 .. 15 are clickable.
   The pump sv_RunFrame runs with this function and the frame's mask; a mouse event asks the screen to
   pop and, when dm_L.lclick is set and the list is not scrolling (the window's scrollRemY is 0), the
   slots 2 .. 15 are walked with the hit test of ai_LoadGameWindow. A hit disables the slot object and
   opens the name editor (ai_EditText) at the caption's position on the slot's title; a name that is
   not cancelled (ai_MessageBox 0x101 asks first when "Gamedata\Saves\<name>.SAV" already exists) is
   saved: the file name "Gamedata/saves/GILDE_SAVEGAME_<i>.SAV", the screen image image (0x9600 bytes)
   into ai_ScreenImage, ls_SaveSlot = i, the message box 0x14414e8, ls_SaveGame (name, title, 1), the
   pop request. The slot object is enabled again. Returns 0. */
int ai_SaveOptionWindow(int image)
{
    unsigned mask;
    int win;
    int obj;
    int down, up, bar;
    int exists;
    VFSDIR *found;
    int i;
    char path[0x104];
    char name[0x104];
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int view;

    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindow(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c58);
    d2_SetForm(0, 0);
    sv_CreateScrollButtons(0x214, 0x168, d2_LastWindow, d2_GetViewWin(win, 1), 0x82, &down, &up, &bar);
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/saves", 0);
    d2_SetObjEnabled(slots[0].obj, 0);
    d2_SetObjEnabled(slots[1].obj, 0);
    obj = d2_GetViewWin(win, 1);
    while (sv_RunFrame((int)ai_SaveOptionWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0 && d2_Wins[obj].scrollRemY == 0) {
            for (i = 2; i < 0x10; i++) {
                if (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                    || (d2_HitWindow != -1
                        && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win])) {
                    exists = 0;
                    sprintf(path, "Gamedata\\Saves\\%s.SAV", slots[i].name);
                    if (vfs_FindFile(path, vfs_Root, &found) != 0)
                        exists = 1;
                    d2_EnableObject(slots[i].obj, 0);
                    if (ai_EditText(d2_Objs[slots[i].obj].x, d2_Objs[slots[i].obj].y,
                                    slots[i].title) == 0) {
                        d2_EnableObject(slots[i].obj, 1);
                        break;
                    }
                    if (exists == 0 || ai_MessageBox(ai_SaveOverwriteText, 0x101) != 0) {
                        sprintf(name, "Gamedata/saves/GILDE_SAVEGAME_%i.SAV", i);
                        memcpy(ai_ScreenImage, (void *)image, 0x9600);
                        ls_SaveSlot = i;
                        view = sv_OpenMessageBox(ai_SaveProgressText, 0);
                        ls_SaveGame(name, slots[i].title, 1);
                        d2_CloseView(view);
                        sv_PopRequest = 1;
                    }
                    d2_EnableObject(slots[i].obj, 1);
                }
            }
        }
    }
    d2_CloseView(win);
    return 0;
}
/* 0x5d4920, the save window of a network game (836 B): the twin of ai_SaveOptionWindow (the same form
   "menu\loadgame_new", the view placement, the fitted form, the pointer sprite, the caption 0x1c58, the
   slider 0x214 x 0x168 of the list control, the one disabled autosave slot 0, the list control and its
   scrollRemY test, the slot walk 2 .. 15 with the same hit test, the name editor ai_EditText on the
   slot's title) but the save is the network request: "Gamedata\network\<name>.SAV" is looked up first
   (vfs_FindFile) and ai_MessageBox 0x101 asks when it exists; a saveable player
   (ai_HasQuicksavePlayer) sends the 0xf request with the CMQUICKSAVE record (slot i, the title as the
   name) through cm_SendExSysMessage, else the status text 0x14414a4 says there is no player to save.
   The slot object is enabled again. Returns 0. */

int ai_SaveNameWindow(int image)
{
    unsigned mask;
    int win;
    int obj;
    int down, up, bar;
    int exists;
    VFSDIR *found;
    int i;
    char path[0x104];
    CMQUICKSAVE rec;
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];

    (void)image;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindow(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c58);
    d2_SetForm(0, 0);
    sv_CreateScrollButtons(0x214, 0x168, d2_LastWindow, d2_GetViewWin(win, 1), 0x82, &down, &up, &bar);
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/network", 1);
    d2_SetObjEnabled(slots[0].obj, 0);
    obj = d2_GetViewWin(win, 1);
    while (sv_RunFrame((int)ai_SaveNameWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0 && d2_Wins[obj].scrollRemY == 0) {
            for (i = 2; i < 0x10; i++) {
                if (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                    || (d2_HitWindow != -1
                        && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win])) {
                    exists = 0;
                    sprintf(path, "Gamedata\\network\\%s.SAV", slots[i].name);
                    if (vfs_FindFile(path, vfs_Root, &found) != 0)
                        exists = 1;
                    d2_EnableObject(slots[i].obj, 0);
                    if (ai_EditText(d2_Objs[slots[i].obj].x, d2_Objs[slots[i].obj].y,
                                    slots[i].title) == 0) {
                        d2_EnableObject(slots[i].obj, 1);
                        break;
                    }
                    if (exists == 0 || ai_MessageBox(ai_SaveOverwriteText, 0x101) != 0) {
                        if (ai_HasQuicksavePlayer() == 0) {
                            sv_SetStatusText(ai_QuickSaveNoPlayerText);
                        } else {
                            memset(&rec, 0, sizeof rec);
                            rec.slot = i;
                            strcpy(rec.name, slots[i].title);
                            cm_SendExSysMessage(0xf, &rec);
                        }
                        sv_PopRequest = 1;
                    }
                    d2_EnableObject(slots[i].obj, 1);
                }
            }
        }
    }
    d2_CloseView(win);
    return 0;
}
/* 0x5d5fd0, the graphics options screen "menu\options_gfx" 0x62daf8 (1454 B): opens the form, fills its nine controls
   of the view 1 (0 the resolution list, 1 .. 3 the detail / shadow / texture lists, 4 .. 6 the floor options, 7 the fog
   slider (shown as 100 - the stored fog), 8 the last list) from the option bytes (ai_GfxResolution, ai_GfxDetail,
   ai_GfxFloor, ai_GfxFog) with their labels and item lists, disables the resolution control while a game runs, and pumps the
   screen (sv_RunFrame, this function as the callback): a click or the right button pops it, the first button of the row
   (d2_PointerObj == ids[0]) applies. On apply it reads the controls back into the option bytes and writes the INI
   (ai_SaveOptions) and applies the gfx options (ai_ApplyGfxOptions); a changed resolution is stored, saved, flags the
   game for a restart (ai_LeaveGame 0, ai_GameOver 1, main_ScreenRestart 1) and reloads the options (ai_LoadOptions). Void; opened by the
   options menu 0x5d7770 and main.c.
   Text slots (txt_Ptrs 0x1c67 ..): the labels of the nine controls 0x1c67 .. 0x1c6f, the item lists 0x1c70 .. 0x1c75 and
   0x1c7b / 0x1c7c, the two buttons 0x1ca2 / 0x1ca3 (read off the original's loads, not named by the text files).
   @original without the 1024x768 mode the resolution default is dflt, never assigned: the original reads an
   uninitialised stack slot there. */

void ai_OptionsGfxScreen(void)
{
    unsigned mask;
    int form, apply, mode;
    int res, obj1, obj2, obj3, obj4, obj5, obj6, obj8, obj7;
    int ids[4];
    int okId, cancelId;
    int dflt;

    apply = 0;
    mask = sv_FrameMask & 0xffffdfff;
    form = d2_OpenForm(0, 0, "menu\\options_gfx");
    sv_SetScreenCenter(mask);
    d2_PlaceViewWindows(form, 1);
    d2_PlaceMainWindow(form, 2);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 2);
    io_PrintText(0x1c5a);
    d2_SetForm(form, 1);
    res = d2_GetWinObj(form, 1, 0);
    obj1 = d2_GetWinObj(form, 1, 1);
    obj2 = d2_GetWinObj(form, 1, 2);
    obj3 = d2_GetWinObj(form, 1, 3);
    obj4 = d2_GetWinObj(form, 1, 4);
    obj5 = d2_GetWinObj(form, 1, 5);
    obj6 = d2_GetWinObj(form, 1, 6);
    obj7 = d2_GetWinObj(form, 1, 7);
    obj8 = d2_GetWinObj(form, 1, 8);
    if (dd_Has1024x768 != 0)
        mode = 1;
    else
        mode = dflt;
    if (dd_Has1152x864 != 0)
        mode = 2;
    d2_SetObjValue(res, 0, mode, ai_GfxResolution.word & 0xff);
    d2_SetSliderFlags(res, 0x82);
    d2_SetLabel(res, txt_Ptrs[0x1c67]);
    d2_SetList(res, 3, txt_Ptrs[0x1c73], txt_Ptrs[0x1c74], txt_Ptrs[0x1c75]);
    d2_SetObjValue(obj1, 0, 2, ai_GfxDetail);
    d2_SetSliderFlags(obj1, 0x82);
    d2_SetLabel(obj1, txt_Ptrs[0x1c68]);
    d2_SetList(obj1, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue(obj2, 0, 2, ai_GfxTextureScale);
    d2_SetSliderFlags(obj2, 0x82);
    d2_SetLabel(obj2, txt_Ptrs[0x1c69]);
    d2_SetList(obj2, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue(obj3, 0, 2, ai_GfxFloorLod);
    d2_SetSliderFlags(obj3, 0x82);
    d2_SetLabel(obj3, txt_Ptrs[0x1c6a]);
    d2_SetList(obj3, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue(obj4, 0, 1, ai_GfxFloor);
    d2_SetSliderFlags(obj4, 0x82);
    d2_SetLabel(obj4, txt_Ptrs[0x1c6b]);
    d2_SetList(obj4, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_SetObjValue(obj5, 0, 1, ai_GfxLodHandling);
    d2_SetSliderFlags(obj5, 0x82);
    d2_SetLabel(obj5, txt_Ptrs[0x1c6c]);
    d2_SetList(obj5, 2, txt_Ptrs[0x1c70], txt_Ptrs[0x1c72]);
    d2_SetObjValue(obj6, 0, 2, ai_GfxShadowDetail);
    d2_SetSliderFlags(obj6, 0x82);
    d2_SetLabel(obj6, txt_Ptrs[0x1c6d]);
    d2_SetList(obj6, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue(obj7, 0x32, 100, 100 - (ai_GfxFog.word & 0xff));
    d2_SetSliderFlags(obj7, 0x882);
    d2_SetLabel(obj7, txt_Ptrs[0x1c6e]);
    d2_SetObjValue(obj8, 0, 2, ai_GfxCameraLimits);
    d2_SetSliderFlags(obj8, 0x82);
    d2_SetLabel(obj8, txt_Ptrs[0x1c6f]);
    d2_SetList(obj8, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    if (ai_GameState != 0)
        d2_EnableObject(res, 0);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x1ca2], txt_Ptrs[0x1ca3]);
    okId = ids[0];
    cancelId = ids[1];
    while (sv_RunFrame((int)ai_OptionsGfxScreen, mask) != 0) {
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == okId) {
                apply = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancelId)
                sv_PopRequest = 1;
        }
    }
    if (apply != 0) {
        ai_GfxDetail = d2_GetObjValue(obj1);
        ai_GfxTextureScale = d2_GetObjValue(obj2);
        ai_GfxLodHandling = d2_GetObjValue(obj5);
        ai_GfxShadowDetail = d2_GetObjValue(obj6);
        ai_GfxFloor = d2_GetObjValue(obj4);
        ai_GfxCameraLimits = d2_GetObjValue(obj8);
        ai_GfxFloorLod = d2_GetObjValue(obj3);
        ai_GfxFog.byte[0] = 100 - d2_GetObjValue(obj7);
        ai_SaveOptions();
        ai_ApplyGfxOptions();
    }
    if (d2_GetObjValue(res) != (ai_GfxResolution.word & 0xff) && apply != 0) {
        ai_GfxResolution.byte[0] = d2_GetObjValue(res);
        ai_SaveOptions();
        ai_LeaveGame = 0;
        ai_GameOver = 1;
        main_ScreenRestart = 1;
        ai_LoadOptions();
    }
    d2_CloseView(form);
}
/* 0x5d6580, the sound options screen "menu\options_sfx" 0x62db1c (974 B): the same window family as
   0x5d5fd0. Fills five controls of the view 1 from ai_SoundOptions: four volume sliders (0 .. 0x7f: master, sfx, msx,
   speech) and the msx_freq list (5 items), then pumps the screen (sv_RunFrame, this function as the callback): a click or the
   right button pops it, the first button of the row applies. While the left button is down on a volume slider the new
   values are previewed live (sb_SetMasterVolume of sfx * master / 100, msx_SetVolume of msx * master / 100; master is the
   slider * 0.01) and the first press on the sfx slider plays the sample "NeuesUpgrade_s" 0x62db0c of the system bank
   (once per visit). On apply the five controls are read back into ai_SoundOptions, the INI is written (ai_SaveOptions)
   and the volumes applied (ai_ApplyVolumes). Void; opened by the options menu 0x5d7770.
   Text slots (txt_Ptrs): the labels 0x1c91 .. 0x1c95, the msx_freq items 0x1c87 .. 0x1c8b, the buttons 0x1ca2 / 0x1ca3. */

void ai_OptionsSfxScreen(void)
{
    unsigned mask;
    int form, apply, sample;
    int master, sfx, msx, speech, freq;
    int ids[2];
    int okId, cancelId;
    float fMaster, fSfx, fMsx;

    apply = 0;
    sample = 0;
    mask = sv_FrameMask & 0xffffdfff;
    form = d2_OpenForm(0, 0, "menu\\options_sfx");
    sv_SetScreenCenter(mask);
    d2_PlaceViewWindows(form, 1);
    d2_PlaceMainWindow(form, 2);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 2);
    io_PrintText(0x1c5b);
    d2_SetForm(form, 1);
    master = d2_GetWinObj(form, 1, 0);
    sfx = d2_GetWinObj(form, 1, 1);
    msx = d2_GetWinObj(form, 1, 2);
    speech = d2_GetWinObj(form, 1, 3);
    freq = d2_GetWinObj(form, 1, 4);
    d2_SetObjValue(master, 0, 0x7f, ai_SoundOptions.masterVolume);
    d2_SetSliderFlags(master, 0x882);
    d2_SetLabel(master, txt_Ptrs[0x1c91]);
    d2_SetObjValue(sfx, 0, 0x7f, ai_SoundOptions.sfxVolume);
    d2_SetSliderFlags(sfx, 0x882);
    d2_SetLabel(sfx, txt_Ptrs[0x1c92]);
    d2_SetObjValue(msx, 0, 0x7f, ai_SoundOptions.msxVolume);
    d2_SetSliderFlags(msx, 0x882);
    d2_SetLabel(msx, txt_Ptrs[0x1c93]);
    d2_SetObjValue(speech, 0, 0x7f, ai_SoundOptions.voiceVolume);
    d2_SetSliderFlags(speech, 0x882);
    d2_SetLabel(speech, txt_Ptrs[0x1c94]);
    d2_SetObjValue(freq, 0, 4, ai_SoundOptions.msxFreq & 0xff);
    d2_SetSliderFlags(freq, 0x82);
    d2_SetLabel(freq, txt_Ptrs[0x1c95]);
    d2_SetList(freq, 5, txt_Ptrs[0x1c87], txt_Ptrs[0x1c88], txt_Ptrs[0x1c89], txt_Ptrs[0x1c8a], txt_Ptrs[0x1c8b]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x1ca2], txt_Ptrs[0x1ca3]);
    okId = ids[0];
    cancelId = ids[1];
    while (sv_RunFrame((int)ai_OptionsSfxScreen, mask) != 0) {
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == okId) {
                apply = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancelId)
                sv_PopRequest = 1;
        }
        if (dm_L.ldown != 0 && (d2_PointerObj == master || d2_PointerObj == sfx || d2_PointerObj == msx
                                || d2_PointerObj == speech)) {
            fMaster = (float)d2_GetObjValue(master) * 0.01f;
            fSfx = (float)d2_GetObjValue(sfx);
            fMsx = (float)d2_GetObjValue(msx);
            sb_SetMasterVolume((int)(fSfx * fMaster));
            msx_SetVolume((int)(fMsx * fMaster));
            if (d2_PointerObj == sfx && sample == 0)
                sample = (int)sb_PlaySample(snd_SystemBank, "NeuesUpgrade_s", 1, 0x3f, 0x7f, 0, 0);
        }
    }
    if (apply != 0) {
        ai_SoundOptions.masterVolume = d2_GetObjValue(master);
        ai_SoundOptions.sfxVolume = d2_GetObjValue(sfx);
        ai_SoundOptions.msxVolume = d2_GetObjValue(msx);
        ai_SoundOptions.voiceVolume = d2_GetObjValue(speech);
        ai_SoundOptions.msxFreqByte[0] = d2_GetObjValue(freq);
        ai_SaveOptions();
        ai_ApplyVolumes();
    }
    d2_CloseView(form);
}
/* 0x5d6950, the game options screen "menu\options_game" 0x62db30 (1514 B): the same window family as 0x5d5fd0. Fills
   eleven controls of the view 1: four sliders (the game speed ai_OptionSpeed 0 .. 0xa0, the mouse speed 0 .. 0x1f4, the scroll
   speed 0 .. 100, the camera speed byte 0 .. 100) and seven two-state / list options from ai_GameSwitches (bytes 0, 2, 3), ai_GamePanel
   (the panel list 0 .. 4, bytes 1 and 3) and ai_OptionPanelHelp; the switch byte 0 and the panel byte 1 are shown disabled. Pumps
   the screen (sv_RunFrame, this function as the callback): a click or the right button pops it, the first button of the row applies.
   On apply the controls are read back into the option globals (switch byte 0 is cleared, its control is not read) and, when the
   panel list changed, the stored panel is updated, the view is disabled and, while a game runs, the screen is rebuilt
   (sv_DestroyScreen, sv_BuildScreenLayout of sv_PanelImage, the fade ai_LeaveGameScreen, sv_ScreenDirty = 1); then the INI is
   written (ai_SaveOptions) and the speed factor applied (ai_ApplyOptions). Void; opened by the options menu 0x5d7770 and main.c.
   Text slots (txt_Ptrs): the labels 0x1c96 .. 0x1ca1 (0x1c9f unused), the two-state items 0x1c7b / 0x1c7c, the panel items
   0x1c76 .. 0x1c7a, the buttons 0x1ca2 / 0x1ca3. */

void ai_OptionsGameScreen(void)
{
    unsigned mask;
    int form, apply;
    int ids[2];
    int okId, cancelId;
    int speed, mouse, scroll, camera, sw0, sw2, sw3, panel, panel1, help, panel3;

    apply = 0;
    mask = sv_FrameMask & 0xffffdfff;
    form = d2_OpenForm(0, 0, "menu\\options_game");
    sv_SetScreenCenter(mask);
    d2_PlaceViewWindows(form, 1);
    d2_PlaceMainWindow(form, 2);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 2);
    io_PrintText(0x1c59);
    d2_SetForm(form, 1);
    speed = d2_GetWinObj(form, 1, 0);
    mouse = d2_GetWinObj(form, 1, 1);
    scroll = d2_GetWinObj(form, 1, 2);
    camera = d2_GetWinObj(form, 1, 3);
    sw0 = d2_GetWinObj(form, 1, 4);
    sw2 = d2_GetWinObj(form, 1, 5);
    sw3 = d2_GetWinObj(form, 1, 6);
    panel = d2_GetWinObj(form, 1, 9);
    panel1 = d2_GetWinObj(form, 1, 10);
    panel3 = d2_GetWinObj(form, 1, 11);
    help = d2_GetWinObj(form, 1, 12);
    d2_SetObjValue(speed, 0, 0xa0, ai_OptionSpeed);
    d2_SetSliderFlags(speed, 0x882);
    d2_SetLabel(speed, txt_Ptrs[0x1c96]);
    d2_SetObjValue(mouse, 0, 0x1f4, ai_OptionMouseSpeed);
    d2_SetSliderFlags(mouse, 0x882);
    d2_SetLabel(mouse, txt_Ptrs[0x1c97]);
    d2_SetObjValue(scroll, 0, 100, ai_OptionScrollSpeed);
    d2_SetSliderFlags(scroll, 0x882);
    d2_SetLabel(scroll, txt_Ptrs[0x1c98]);
    d2_SetObjValue(camera, 0, 100, ai_OptionCameraSpeed);
    d2_SetSliderFlags(camera, 0x882);
    d2_SetLabel(camera, txt_Ptrs[0x1c99]);
    d2_SetObjValue(sw0, 0, 1, ai_GameSwitches.word & 0xff);
    d2_SetSliderFlags(sw0, 0x82);
    d2_SetLabel(sw0, txt_Ptrs[0x1c9a]);
    d2_SetList(sw0, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_EnableObject(sw0, 0);
    d2_SetObjValue(sw2, 0, 1, ai_GameSwitches.byte[2]);
    d2_SetSliderFlags(sw2, 0x82);
    d2_SetLabel(sw2, txt_Ptrs[0x1c9b]);
    d2_SetList(sw2, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_SetObjValue(sw3, 0, 1, ai_GameSwitches.byte[3]);
    d2_SetSliderFlags(sw3, 0x82);
    d2_SetLabel(sw3, txt_Ptrs[0x1c9c]);
    d2_SetList(sw3, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_SetObjValue(panel, 0, 4, ai_GamePanel.word & 0xff);
    d2_SetSliderFlags(panel, 0x82);
    d2_SetLabel(panel, txt_Ptrs[0x1c9d]);
    d2_SetList(panel, 5, txt_Ptrs[0x1c76], txt_Ptrs[0x1c77], txt_Ptrs[0x1c78], txt_Ptrs[0x1c79], txt_Ptrs[0x1c7a]);
    d2_SetObjValue(panel1, 0, 1, ai_GamePanel.byte[1]);
    d2_SetSliderFlags(panel1, 0x82);
    d2_SetLabel(panel1, txt_Ptrs[0x1c9e]);
    d2_SetList(panel1, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_EnableObject(panel1, 0);
    d2_SetObjValue(help, 0, 1, ai_OptionPanelHelp.word & 0xff);
    d2_SetSliderFlags(help, 0x82);
    d2_SetLabel(help, txt_Ptrs[0x1ca1]);
    d2_SetList(help, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_SetObjValue(panel3, 0, 1, ai_GamePanel.byte[3]);
    d2_SetSliderFlags(panel3, 0x82);
    d2_SetLabel(panel3, txt_Ptrs[0x1ca0]);
    d2_SetList(panel3, 2, txt_Ptrs[0x1c7b], txt_Ptrs[0x1c7c]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x1ca2], txt_Ptrs[0x1ca3]);
    okId = ids[0];
    cancelId = ids[1];
    while (sv_RunFrame((int)ai_OptionsGameScreen, mask) != 0) {
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == okId) {
                apply = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancelId)
                sv_PopRequest = 1;
        }
    }
    if (apply != 0) {
        ai_OptionSpeed = d2_GetObjValue(speed);
        ai_OptionCameraSpeed = d2_GetObjValue(camera);
        ai_OptionMouseSpeed = d2_GetObjValue(mouse);
        ai_OptionScrollSpeed = d2_GetObjValue(scroll);
        ai_GameSwitches.byte[0] = 0;
        ai_GameSwitches.byte[2] = d2_GetObjValue(sw2);
        ai_GameSwitches.byte[3] = d2_GetObjValue(sw3);
        ai_GamePanel.byte[1] = d2_GetObjValue(panel1);
        ai_GamePanel.byte[3] = d2_GetObjValue(panel3);
        ai_OptionPanelHelp.byte[0] = d2_GetObjValue(help);
        if (d2_GetObjValue(panel) != (ai_GamePanel.word & 0xff)) {
            ai_GamePanel.byte[0] = d2_GetObjValue(panel);
            d2_SetViewEnabled(form, 0);
            if (ai_GameState != 0) {
                sv_DestroyScreen();
                sv_BuildScreenLayout(sv_PanelImage());
                ai_LeaveGameScreen();
                sv_ScreenDirty = 1;
            }
        }
        ai_SaveOptions();
        ai_ApplyOptions();
    }
    d2_CloseView(form);
}
/* 0x5d7ba0, the credits screen (649 B), opened by the credits button of the main menu (main.c
   0x533850, "ZumGutenEnd.mp3"): a black cover fades in (d2_StartFade 0x4767d0 (0, 0, width, height,
   "BLACK", 0x3c, 1), its flags byte bit 2 = done, pumped with sv_RunFrame 0x5309a0 (0, 0x24087)),
   then the screen switches (d2_ReleaseFade 0x476860, d2_ClearScreen 0x46e990, d2_ShowPicture 0x46cfd0
   (0x7ad), a new black cover (.., 0x3c, 10)) and the credits scroll in a 600-wide window at (100, 0)
   (d2_CreateWindow 0x472390, d2_PlaceWindowScaled 0x475730 (bar, 1)): the text (io_PrintText 0x515960
   (0x2004)) starts one screen height above the window (scrollY = -height) and the window is pumped
   (sv_RunFrame (this, 0x24087)) until the pump returns 0, a key / the mouse or the text bottom
   (height * 1.5 + scrollY > contentHeight) sets the screen close request sv_PopRequest; the frame
   skip of the scroll is the frame rate sv_FramesPerTick 0x62378c (>= 80: 4 rows, >= 30: 2, below: 1
   per frame). A second fade loop (the new black cover, .., 0x3c, 1) scrolls on until that cover ends;
   then the window is removed (d2_RemoveWindow 0x472ce0), the picture 0x797 is shown, the cover
   released and the final black cover (.., 0x3c, 10) drawn with d2_CutsceneActive 0x6ed9d4 cleared. */

void ai_CreditsScreen(void)
{
    D2FADEVIEW *fade;
    int bar, frames, pump, step;

    step = 4;
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 1);
    while ((fade->flags & 4) == 0)
        sv_RunFrame(0, 0x24087);
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 10);
    d2_ClearScreen();
    d2_ShowPicture(0x7ad);
    bar = d2_CreateWindowI(100, 0, 600, d2_ScreenH, 0x10);
    d2_PlaceWindowScaled(bar, 1);
    io_PrintText(0x2004);
    d2_LastWindowPtr->scrollY = -d2_ScreenH;
    frames = 0;
    d2_CutsceneActive = 1;
    pump = sv_RunFrame(ai_CreditsScreen, 0x24087);
    while (pump != 0) {
        if (dm_L.rup != 0 || kb_Last != 0)
            sv_PopRequest = 1;
        d2_LastWindowPtr = &d2_Wins[bar];
        if (frames % step == 0)
            d2_LastWindowPtr->scrollY = d2_LastWindowPtr->scrollY + 1;
        frames++;
        if ((double)d2_LastWindowPtr->h * 1.5 + (double)d2_LastWindowPtr->scrollY >
            (double)d2_LastWindowPtr->contentHeight)
            sv_PopRequest = 1;
        if (sv_FramesPerTick >= 80.0f)
            step = 4;
        else if (sv_FramesPerTick >= 30.0f)
            step = 2;
        else if (sv_FramesPerTick < 30.0f)
            step = 1;
        pump = sv_RunFrame(ai_CreditsScreen, 0x24087);
    }
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 1);
    while ((fade->flags & 4) == 0) {
        sv_RunFrame(0, 0x24087);
        d2_LastWindowPtr = &d2_Wins[bar];
        if (frames % step == 0)
            d2_LastWindowPtr->scrollY++;
        if (sv_FramesPerTick >= 80.0f)
            step = 4;
        else if (sv_FramesPerTick >= 30.0f)
            step = 2;
        else if (sv_FramesPerTick < 30.0f)
            step = 1;
        frames++;
    }
    d2_RemoveWindow(bar);
    d2_ShowPicture(0x797);
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 10);
    d2_CutsceneActive = 0;
}
/* 0x5d7e90, the pause loop (120 B): sets tb_Paused, shows the "Pause" text (sv_SetStatusText) and
   clears ai_GameState; the pump sv_RunFrame (frame flags sv_FrameMask & ~0x2000 | 0x100000) runs
   while it returns nonzero and the last key (kb_Last) is not '9'. Then the status text is reset
   (ch_EmptyName), tb_Paused cleared and ai_GameState set. */

void ai_PauseGame(void)
{
    int flags;
    int r;

    flags = sv_FrameMask & ~0x2000 | 0x100000;
    tb_Paused = 1;
    sv_SetStatusText("Pause");
    ai_GameState = 0;
    r = sv_RunFrame(0, flags);
    while (r != 0 && kb_Last != '9')
        r = sv_RunFrame(0, flags);
    sv_SetStatusText(ch_EmptyName);
    tb_Paused = 0;
    ai_GameState = 1;
    return 0;
}

/* The message filter's row captions (the text system fills the 0x1441500 .. slots at run time) and
   the getter / setter of its mask (he.c: message kind 0x605 .. 0x60d <-> bit 0 .. 0x100 of 0x6f263c). */
/* ai_MessageFilterTexts: view of txt_Ptrs (build/mk_txt_views.py) */
int he_IsMessageKindFiltered(int kind); /* 0x575640, he.c: nonzero when the kind is filtered */
void he_SetMessageKindFiltered(int kind, int filtered); /* 0x575710, he.c: sets / clears the kind's bit */
int cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60, cm.c: message 0x19 (the field block) */
void d2_SetCaption(int obj, char *text);           /* 0x473540, d2.h: sets the caption and its width */

/* 0x5d7f10, the message filter window "menu\messagefilter" 0x62dbc8 (390 B), opened from the message
   box's menu: the nine message kinds 0x605 .. 0x60d become a column of checkbox rows (d2_WinOpenPic
   0x4731d0 at x 8, y i * 0x1c + 10, the picture i + 0x605, on the window 0x6ed938): each row is a
   checkbox (checkable and interactive), gets the font 0x55 and the caption ai_MessageFilterTexts[i],
   a 0x78 x 0x1e text box (picW, captionX 0x1e, captionY 3) and is checked while the kind is not
   filtered (he_IsMessageKindFiltered 0x575640). The screen is pumped (sv_RunFrame 0x5309a0 (this,
   0x677c7)) until the pump returns 0; a mouse event / the key 1 closes it (sv_PopRequest 0x6f22a0)
   after every row's checkbox state has been written back into the filter mask
   (he_SetMessageKindFiltered 0x575710, checked = not filtered), and d2_CloseView 0x4758f0 frees the
   view. Void. */
void ai_MessageFilterScreen(void)
{
    int wnd, pump, i, id;
    int rows[16];                       /* the nine checkbox rows (the frame holds 16) */

    wnd = d2_OpenForm(0, 0, "menu\\messagefilter");
    d2_CentreViewWindows(wnd);
    d2_SetForm(wnd, 2);
    i = 0;
    do {
        id = d2_WinOpenPic(8, i * 0x1c + 10, i + 0x605, d2_LastWindow);
        rows[i] = id;
        d2_Objs[id].checkable = 1;
        d2_Objs[id].interactive = 1;
        d2_SetObjFont(id, 0x55);
        d2_SetCaption(id, ai_MessageFilterTexts[i]);
        d2_Objs[id].picW = 0x78;
        d2_Objs[id].captionX = 0x1e;
        d2_Objs[id].captionY = 3;
        d2_SetObjValue(id, he_IsMessageKindFiltered(i + 0x605) == 0);
        i++;
    } while (i < 9);
    pump = sv_RunFrame(ai_MessageFilterScreen, 0x677c7);
    while (pump != 0) {
        if (dm_L.rup != 0 || kb_Last == 1) {
            sv_PopRequest = 1;
            i = 0;
            do {
                he_SetMessageKindFiltered(i + 0x605, d2_GetObjValue(rows[i]) == 0);
                i++;
            } while (i < 9);
        }
        pump = sv_RunFrame(ai_MessageFilterScreen, 0x677c7);
    }
    d2_CloseView(wnd);
}

/* The request record of the screen callbacks (0x5c bytes, opened by 0x5c1130): the id word, the
   kind byte (6 / 7 = a player character) and the player handle. */
typedef struct AIREQ {
    unsigned short id;                  /* +0x00 */
    unsigned char kind;                 /* +0x02 */
    char pad03;
    int handle;                         /* +0x04 */
    char pad08[0x54];
} AIREQ;


/* 0x5d8080, the tailor chat request window "locations\schneider\plausch" 0x62dbf8 (733 B, a
   Ghidra-missed start; the contact prompt 0x5d8360 opens it through 0x5c1130): the window lists the
   character with the most unclaimed graveyard evidence records (gv_FindAccused, up to the building
   skill 0x5092e0 + 1 record ids) and prints its name (the request id word, the count). A click on
   the list marks those records executed for the player (message 0x24, cm_SendProofExecuted), clears the
   +0x194 value of the player and of the request handle (message 0x5a, cm_SendMoneyDelta) and sends the
   event texts 0x422f / (for the request kinds 6 / 7) 0x4230 + "_NACHRICHTEN_NEU_15"; without a
   found character only the request handle is cleared. The mouse event 0x14673a0 / the answer
   0xf8a668 == 1 sets sv_PopRequest 1, a click on the control (d2_PointerObj == obj) 3. The pump
   0x5309a0 (this function, 0x67fff) ends with 0x4758f0 (wnd). Void. */
void ai_TailorChatWindow(int request)
{
    AIREQ *req;
    AICHARVIEW *rec;
    int count;
    int obj;
    int found;
    int ids[4];                         /* the record ids the query fills (its max is 4) */
    char buf[0x400];                    /* both event texts are built here, one after the other */
    int limit;
    int wnd;
    int pump;
    int i;

    req = (AIREQ *)request;
    rec = (AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c);
    wnd = d2_OpenForm(0, 0, "locations\\schneider\\plausch");
    found = gv_FindAccused((char *)(ai_CharsC + ai_PlayerId * 0x21c), 4, ids, &count);
    if (ai_PlayerBuildingB != 0)
        limit = gm_BuildingSkill((struct AIBLD *)ai_PlayerBuildingB, 0x24, 1);
    else
        limit = 1;
    if (count >= limit + 1)
        count = limit + 1;
    sv_FitForm(d2_GetViewWin(wnd, 0));
    d2_CentreViewWindows(wnd);
    sv_ReleaseNamePlates();
    d2_SetForm(wnd, 1);
    io_PrintText(0x4227);
    io_PrintText(0x422b, req->id, count, 0x422c, req->id, 1);
    d2_SetForm(wnd, 2);
    obj = d2_GetWinObj(wnd, 2, io_PrintText("%ia[%s]$N", 0x422d));
    io_PrintText("%in[%s]", 0x422e);
    if (found == 0 || count == 0)
        d2_SetObjEnabled(obj, 0);
    pump = sv_RunFrame(ai_TailorChatWindow, 0x67fff);
    while (pump != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_PointerObj == obj) {
            if (found != 0) {
                for (i = 0; i < count; i++)
                    cm_SendProofExecuted(ids[i], ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId);
                cm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -1);
                cm_SendMoneyDelta(req->handle, -1);
                cm_SendSetField(rec->netId, 0x1cc, 4, 0x20000, 0);
                txt_Format(buf, 0x422f, req->id);
                he_ShowNews(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -1, buf, 0, 0x5ec,
                            0);
                if (req->kind == 6 || req->kind == 7) {
                    txt_Format(buf, 0x4230, ai_PlayerId, 1);
                    he_ShowNews(req->handle, -1, buf, 0, 0x5ec, "_NACHRICHTEN_NEU_15");
                }
                sv_PopRequest = 3;
            } else {
                cm_SendMoneyDelta(req->handle, -1);
                sv_PopRequest = 3;
            }
        }
        pump = sv_RunFrame(ai_TailorChatWindow, 0x67fff);
    }
    d2_CloseView(wnd);
}

/* The 0x50-byte request record 0x5c1130 sends: the kind at +0, the flags / id at +4 and the event
   kind at +0x10. */
typedef struct CONTACTREQ {
    int type;                           /* +0x00 */
    int a;                              /* +0x04 */
    char pad08[8];
    int b;                              /* +0x10 */
    char pad14[0x3c];
} CONTACTREQ;

/* 0x5d8360, the tailor contact prompt (198 B), reached from the production-building contact
   listener 0x5d8420 (its contact_PLAUSCH branch): when the character's flags1cc (+0x1cc of
   ai_Chars[ai_PlayerId]) has bit 0x20000, the message box 0x521150 shows ai_TailorBlockedText
   (flags 4) and the function returns. Otherwise sv_CheckMoney (the character, 1) asks whether its
   money covers the chat; when it does, a D3_ZEROed request record (0x50 bytes: +0 = 1, +4 = 0x400,
   +0x10 = 6) goes through the amount menu ai_AmountMenu (the caption ai_TailorAmountText,
   ch_EmptyName as the second text, the callback ai_TailorChatWindow). Ends with the camera follow
   sv_CameraFollow. Void; the two arguments the contact dispatch passes are not read. */
void ai_TailorContact(int building, int arg)
{
    CONTACTREQ req;

    if ((((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 0x20000) != 0) {
        sv_MessageBox((int)ai_TailorBlockedText, 4);
        return;
    }
    if (sv_CheckMoney((char *)(ai_CharsC + ai_PlayerId * 0x21c), 1) != 0) {
        D3_ZERO(&req, sizeof(req));
        req.type = 1;
        req.a = 0x400;
        req.b = 6;
        ai_AmountMenu((int)&req, (int)ai_TailorAmountText, (int)ch_EmptyName, (int)ai_TailorChatWindow);
    }
    sv_CameraFollow();
}

/* ai.h's AIBLD (0xa9 bytes, packed) as the contact listener reads it: the head of the building's object
   list. */
#pragma pack(push, 1)
typedef struct AIBLDVIEW {
    char pad0[0x5d];
    int objList;                        /* +0x5d, AIBLD.objList: the objects of the building */
} AIBLDVIEW;
#pragma pack(pop)

/* 0x5d8420, the contact listener of the production buildings (429 B): building the building record,
   arg the caller's argument. The contacts 0x52e1c0 are created on the flags 0x6f231c: bit 0x200
   -> contact_PRODUKTION_SCHNEIDER 0x62dc48 (0x144aa84, 0x13), contact_ROHSTOFFE_SUCHEN_SCHNEIDER
   0x62dc24 (0x14401b4, 0x16) and contact_PLAUSCH 0x62dc14 (0x144aa58, 0xc); bit 0x400 ->
   contact_LAGER, contact_TRANSPORT, ob_PERSONALBUCH, ob_MEISTERBRIEF. The click 0x6f22e4 dispatches:
   LAGER -> 0x5b26b0 (param_1, the dword +0x5d), SCHNEIDER -> 0x58bd60 (param_1), TRANSPORT ->
   0x5af1a0 (param_1), PERSONALBUCH -> 0x5aab80 (param_1, "ob_PERSONALBUCH"), MEISTERBRIEF ->
   0x5c3200 (param_1), ROHSTOFFE -> 0x595480 (param_1, param_2), PLAUSCH -> 0x5d8360. Then
   0x58f510 (param_1). The pump 0x5309a0 (0x5d8420, 0x67fff) runs between 0x52e190 calls. */
void ai_ProductionContact(int building, int arg)
{
    int lager, schn, trans, personal, meister, roh, plausch;
    int pump;
    int click;

    lager = 0;
    schn = 0;
    trans = 0;
    personal = 0;
    meister = 0;
    roh = 0;
    plausch = 0;
    sv_ReleaseNamePlates();
    pump = sv_RunFrame(ai_ProductionContact, 0x67fff);
    while (pump != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            schn = sv_RegisterNamePlate("contact_PRODUKTION_SCHNEIDER", ai_ContactTextProductionTailor, 0x13);
            roh = sv_RegisterNamePlate("contact_ROHSTOFFE_SUCHEN_SCHNEIDER", ai_ContactTextRawMaterialSearch, 0x16);
            plausch = sv_RegisterNamePlate("contact_PLAUSCH", ai_ContactTextChat, 0xc);
        }
        if ((ai_BuildingPerms & 0x400) != 0) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            trans = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
        }
        if ((click = ai_ClickedObject) != 0) {
            if (click == lager) {
                ai_StorageWindow(building, ((AIBLDVIEW *)building)->objList);
            } else if (click == schn) {
                ai_ProductionWindow((struct AIBLD *)building);
            } else if (click == trans) {
                ai_OpenTransport(building);
            } else if (click == personal) {
                ai_PersonnelBook((struct AIBLD *)building, "ob_PERSONALBUCH");
            } else if (click == meister) {
                ai_MasterLetterWindow((struct AIBLD *)building);
            } else if (click == roh) {
                ai_TradeSearchWindow((unsigned char *)building, arg);
            } else if (click == plausch) {
                ai_TailorContact(building, arg);
            }
        }
        ai_MarketContact((struct AIBLD *)building);
        pump = sv_RunFrame(ai_ProductionContact, 0x67fff);
    }
    sv_ReleaseNamePlates();
    return 0;
}

/* ---- the contact listeners are defined above the stub bodies they open (out of address order:
   0x5d9a80 / 0x5d9b20 / 0x5d9b90 belong after 0x5d9890). VC6 drops a call to a function whose
   body it has already seen when that body has no side effect: the ROUGH stub bodies of 0x5d85e0
   .. 0x5d9890 make the call vanish, the original's callers were compiled against the real
   bodies. Move the listeners back into address order once those have their bodies. ---- */

/* 0x5d9a80, the gypsy contact listener (97 B): building the gypsy building. contact_BLUTRACHE
   0x62dce0 (ai_ContactTextBloodRevenge 0x144a884, 0x16) opens the blood revenge window 0x5d85e0
   (ai_BloodRevengeWindow), contact_EHRERBIETUNG 0x62dcc8 (ai_ContactTextHomage 0x144a9d8, 0x16) the
   homage window 0x5d95a0 (ai_HomageWindow), between sv_ReleaseNamePlates calls and the pump 0x5309a0
   (this function, 0x67fff). The clicked object (ai_ClickedObject) selects one of the plates. */
void ai_GypsyContact(int building)
{
    int blutrache = 0;
    int ehrerbietung = 0;
    int clicked;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GypsyContact, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            blutrache = sv_RegisterNamePlate("contact_BLUTRACHE", ai_ContactTextBloodRevenge, 0x16);
            ehrerbietung = sv_RegisterNamePlate("contact_EHRERBIETUNG", ai_ContactTextHomage, 0x16);
        }
        clicked = ai_ClickedObject;
        if (clicked != 0) {
            if (clicked == blutrache)
                ai_BloodRevengeWindow(building);
            else if (clicked == ehrerbietung)
                ai_HomageWindow(building);
        }
    }
}
/* 0x5d9b20, the "Fluch wirken" (cast curse) contact listener (76 B): buildingArg the building.
   contact_FLUCH_WIRKEN 0x62dcf4 (ai_ContactTextCurse 0x144a8c0, 0x16) opens the curse window 0x5d8c90
   (ai_CurseWindow) when its plate is the clicked object (ai_ClickedObject), between
   sv_ReleaseNamePlates calls and the pump 0x5309a0 (this function, 0x67fff). */

void ai_CurseContact(int buildingArg)
{
    int fluch;
    int building;
    int clicked;

    /* the original keeps the building in a callee-saved register across the plate call */
    building = buildingArg;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_CurseContact, 0x67fff) != 0) {
        fluch = sv_RegisterNamePlate("contact_FLUCH_WIRKEN", ai_ContactTextCurse, 0x16);
        clicked = ai_ClickedObject;
        if (clicked != 0 && clicked == fluch)
            ai_CurseWindow(building);
    }
    return 0;
}
/* 0x5d9b90, the theatre / card game contact listener (127 B): building the building.
   contact_THEATERSTUECK 0x62dd20 (ai_ContactTextTheatre 0x144a98c, 0x16) opens the curse target picker
   0x5d90e0 (ai_CurseTargetWindow), contact_KARTENSPIEL 0x62dd0c (ai_ContactTextCardGame 0x144aa9c,
   0x16) the poker game 0x5da030 (ai_PokerGame), between sv_ReleaseNamePlates calls and the pump
   0x5309a0 (this function, 0x67fff). */

void ai_TheatreContact(int building)
{
    int theater = 0;
    int karten;
    int clicked;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_TheatreContact, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) != 0)
            theater = sv_RegisterNamePlate("contact_THEATERSTUECK", ai_ContactTextTheatre, 0x16);
        karten = sv_RegisterNamePlate("contact_KARTENSPIEL", ai_ContactTextCardGame, 0x16);
        clicked = ai_ClickedObject;
        if (clicked != 0) {
            if (clicked == theater)
                ai_CurseTargetWindow(building);
            else if (clicked == karten)
                ai_PokerGame();
        }
    }
}

typedef struct GYPSYROW {               /* 0x38 bytes: one row of the special-amount list (ai_AmountRowStatus / ai_AmountRow read it as ints) */
    AICHARVIEW *rec;                    /* +0x00, the character the row shows (0 = empty) */
    int ctl;                            /* +0x04, the row's control object (d2_Objs index) */
    int pad08[2];
    int x;                              /* +0x10, the position of the row */
    int y;                              /* +0x14 */
    int pad18[8];
} GYPSYROW;

extern struct D2PIC *d2_Pics;              /* 0x6ed918, d2.h: D2PIC * */

/* 0x5d85e0, the gypsy blood revenge window "Locations\zigeuner\blutrache" 0x62dc68 (1709 B); the building is the
   gypsy house. When the player's status bit 8 (+0x1cc of its record) is set the green message box 0x41b2 / 0x41b3 is shown
   and the window ends. Otherwise the form is opened and five rows are laid out in a pentagon (the top row at the middle,
   the others at thirds of the width, the rows of the list ai_AmountRow). Row 0 is the avenger (the first record), rows 1 .. 4
   are the chosen targets: each is picked with the special-amount menu ai_AmountMenu (a config of 0x14 dwords: +0 = 1, +4 =
   0x400, +8 / +0xc the 64-bit filter 0x1a7fdfbb6, +0x10 = 6, +0x14 = 2, +0x1c the character ids of the other rows, 0xffff for
   an empty one). The price of the revenge is the sum over the rows of the level of the record's office / 3 + 2 (row 0 and
   empty rows cost nothing), shown through the texts "$C" and 0x41b6. Every 0x8c ms the rows are refreshed: a record that is no
   longer in use or whose net id changed is dropped. The first button of the row (ids[0]) confirms when the player can pay
   (sv_CheckMoney): the avenger's text 0x41bf, the relation of every target to the avenger drops by 50 + 50 * the mix of
   the two (gm_Mix, cm_SendRelations), the price is paid (gm_SendMoneyDelta), the power bit 8 of +0x1cc is set (cm_SendSetField) and the
   delivery text for the chosen targets is made (ai_GypsyDeliveryText) and shown (0x41b9 with 0x41b2); the second button and
   the right mouse button leave. The pump ends with d2_CloseView and the camera follow; a refused window
   returns right after the green box. */
void ai_BloodRevengeWindow(int building)
{
    char msg[0x800];
    GYPSYROW rows[5];
    AMOUNTCFG cfg;                      /* the config of the amount menu */
    int sel[9];                         /* 0 .. 4 the net id of the record in each row (-1 empty), 5 .. 8 the chosen records */
    int ids[2];                         /* the two buttons the row of buttons creates */
    int tick, refresh, form, total;
    unsigned short *idp;
    GYPSYROW *row;
    int n, i, k, r;

    refresh = 1;
    memset(rows, 0, sizeof(rows));
    memset(sel, 0xff, 5 * sizeof(int));
    total = 0;
    tick = 0;
    if (((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 8) {
        sv_MessageBoxGreen((int)txt_Ptrs[0x41b2], (int)txt_Ptrs[0x41b3], 0);
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    rows[0].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 2;
    rows[0].y = 0x1e;
    rows[1].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 3 - 10;
    rows[1].y = 0x7d;
    rows[2].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) * 2 / 3 + 10;
    rows[2].y = 0x7d;
    rows[3].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 3 - 100;
    rows[3].y = 0xdc;
    rows[4].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) * 2 / 3 + 100;
    rows[4].y = 0xdc;
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41b7], txt_Ptrs[0x41b8]);
    do {
        if (refresh != 0) {
            n = gm_BathObjectCount((struct AIBLD *)building) + 1;
            tick = d3_Time;
            refresh = 0;
            d2_SetForm(form, 1);
            io_PrintText("$C");
            io_PrintText((char *)0x41b2);
            total = 0;
            for (i = 0; i < 5; i++) {
                row = &rows[i];
                if (i >= n) {
                    row->rec = 0;
                    sel[i] = -1;
                }
                ai_AmountRowStatus((int *)row);
                ai_AmountRow(row->x, row->y, d2_LastWindow, 0x54, (int *)row, 1);
                if (i < n) {
                    d2_SetObjEnabled(row->ctl, 1);
                    d2_Objs[row->ctl].interactive = 1;
                    d2_Objs[row->ctl].style = 3;
                    d2_Objs[row->ctl].user = row->rec;
                    d2_Objs[row->ctl].greyed = 0;
                } else {
                    d2_Objs[row->ctl].greyed = 1;
                }
                if (row->rec == 0 || i == 0) {
                    r = 0;
                } else {
                    r = amt_GetKindLevel(row->rec->officeKind);
                    r = r / 3 + 2;
                }
                total += r;
            }
            d2_SetForm(form, 2);
            io_PrintText("$C", total);
            io_PrintText((char *)0x41b6, total);
            d2_SetObjEnabled(ids[0], rows[0].rec != 0 && total != 0);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, total) == 1) {
                    AICHARVIEW *av = rows[0].rec;

                    txt_Format(msg, 0x41bf, av->id);
                    k = 0;
                    for (i = 1; i < 5; i++) {
                        if (rows[i].rec != 0) {
                            r = 0x32 - (int)(gm_Mix((int)(ai_CharsC + ai_PlayerId * 0x21c), (int)rows[i].rec, 4) * -50.0f);
                            cm_SendRelations(rows[0].rec->id, rows[i].rec->id, -r, 1);
                            sel[5 + k] = (int)rows[i].rec;
                            k++;
                        }
                    }
                    gm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -total);
                    cm_SendSetField(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, 0x1cc, 4, 8, 0);
                    ai_GypsyDeliveryText((struct AIPER *)av, k, (struct AIPER **)&sel[5]);
                    txt_Format(msg, 0x41b9, 0x41b2);
                    sv_MessageBox((int)msg, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            } else {
                for (i = 0; i < 5; i++) {
                    if (d2_PointerObj == rows[i].ctl) {
                        memset(&cfg, 0, sizeof(cfg));
                        for (k = 0, idp = cfg.ids; k < 5; k++) {
                            if (k != i) {
                                *idp = rows[k].rec != 0 ? rows[k].rec->id : 0xffff;
                                idp++;
                            }
                        }
                        cfg.filter = gypsyFilter;
                        cfg.option14 = 2;
                        cfg.flags = 1;
                        cfg.mode = 0x400;
                        cfg.kind = 6;
                        rows[i].rec = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x41b4 + (i != 0)], 0, 0);
                        sel[i] = rows[i].rec != 0 ? rows[i].rec->netId : -1;
                        refresh = 1;
                        break;
                    }
                }
            }
        } else if (d3_Time > (unsigned)(tick + 0x8c)) {
            for (i = 0; i < 5; i++) {
                if (rows[i].rec != 0 && (rows[i].rec->id == 0xffff || rows[i].rec->netId != (unsigned)sel[i])) {
                    rows[i].rec = 0;
                    sel[i] = -1;
                }
            }
            refresh = 1;
        }
    } while (sv_RunFrame((int)ai_BloodRevengeWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
/* batch 3: the gypsy / poker / particle stretch 0x5d8c90 .. 0x5dd3e0 */
void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_CheckGoodAmount(char *o, int n, unsigned char t);
struct AITIME;
int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int ai_PokerStart();
unsigned short ai_PokerStep(); int gm_BuildingSkill(); float ai_NormalizeEffectScale(float x);

/* 0x5d8c90, the "Fluch wirken" (curse) window "Locations\zigeuner\blutrache" 0x62dc68 (905 B): the building is the
   gypsy house. When the player's status bit 0x10 (+0x1cc of its record) is set the voice "RAUSWURF" is played and the green
   message box 0x41c1 / 0x41c2 refuses. Otherwise the target is chosen with the special-amount menu ai_AmountMenu (the same
   config as the blood revenge window: +0 = 1, +4 = 0x400, +8 / +0xc the filter 0x1a7fdfbb6, +0x10 = 6, the id list from +0x1c all
   0xffff); no target ends the window. The form shows a slider (0 .. 5 at first, then 0 .. the limit gm_CurseObjectCount - 1) with
   the six strength names 0x41ca .. 0x41cf and the two buttons 0x41d8 / 0x41d9. Each pass the strength v is clamped to the limit,
   the texts 0x41d0 + v (with the target's id), 0x41d6 / 0x41c1 / 0x41c4 + v and, for a building of another owner, the price
   max (v, 1) * 32000 (0x41d7) are shown. The right mouse button and the second button leave; the first button curses when the
   player has the goods (sv_CheckGoodAmount): the status bit 0x10 is set (cm_SendSetField), another owner is paid the price
   (cm_SendMoveGoodsUnchecked), the message 0x9d (arg 0 = the strength, arg 2 = the target's net id, the game time, flags 2) is sent
   (cm_SendLongMessage), the voice "_WAHRSAGERIN_FLUCH_0<v>" is played and the text 0x41da is shown. The pump ends with
   d2_CloseView and the camera follow. */
void ai_CurseWindow(int building)
{
    AMOUNTCFG cfg;                      /* the config of the amount menu */
    int ids[2];                         /* the two buttons */
    int slider, form;
    AICHARVIEW *target;
    int limit, v;

    if (((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 0x10) {
        snd_PlayVoice(-7, 0, "RAUSWURF", -1);
        sv_MessageBoxGreen((int)txt_Ptrs[0x41c1], (int)txt_Ptrs[0x41c2], 0);
        return;
    }
    memset(&cfg, 0, sizeof(cfg));
    memset(cfg.ids, 0xff, sizeof(cfg.ids));
    cfg.flags = 1;
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    target = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x41c3], 0, 0);
    if (target == 0) {
        sv_CameraFollow();
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    slider = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue(slider, 0, 5, 0, 1);
    d2_SetList(slider, 6, txt_Ptrs[0x41ca], txt_Ptrs[0x41cb], txt_Ptrs[0x41cc], txt_Ptrs[0x41cd], txt_Ptrs[0x41ce],
               txt_Ptrs[0x41cf]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41d8], txt_Ptrs[0x41d9]);
    do {
        char text[0x200];

        limit = gm_CurseObjectCount((struct AIBLD *)building) - 1;
        v = d2_GetObjValue(slider);
        if (v >= limit)
            v = limit;
        d2_SetObjValue(slider, 0, limit, v, 1);
        txt_Format(text, v + 0x41d0, target->id);
        d2_SetForm(form, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x41d6, 0x41c1, v + 0x41c4, text);
        if (ai_PlayerId != ((CURSEBLD *)building)->owner)
            io_PrintText((char *)0x41d7, (v > 1 ? v : 1) * 32000);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckGoodAmount(ai_CharsC + ai_PlayerId * 0x21c, v, ai_OwnPlayer) != 0) {
                    char name[100];
                    CURSEMSG msg;

                    cm_SendSetField(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, 0x1cc, 4, 0x10, 0);
                    if (ai_PlayerId != ((CURSEBLD *)building)->owner)
                        cm_SendMoveGoodsUnchecked(-1, ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, ai_OwnPlayer,
                                                  (v > 1 ? v : 1) * 32000);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x9d;
                    msg.fromId = -1;
                    msg.flags = 2;
                    msg.arg[0] = v;
                    msg.to = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId;
                    memcpy(&msg.time, &ai_Time, sizeof(AITIME));
                    msg.arg[1] = target->netId;
                    cm_SendLongMessage((char *)&msg);
                    txt_Format(text, 0x41da, 0x41c1);
                    sprintf(name, "_WAHRSAGERIN_FLUCH_0%d", v);
                    snd_PlayVoice(-7, 0, name, -1);
                    sv_MessageBox((int)text, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_CurseWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
/* 0x5d90e0, the curse target picker "Locations\zigeuner\blutrache" (1216 B): the building is the gypsy house. A clock is
   made for the next day (the game time + 24 hours, the hour 21 + a random 0 / 1, the minute 0). The form has two sliders: the
   share (progress of a running curse, 0 .. 100, drawn as a wide bar at the top) and the victim (0 .. gm_CurseTargetCount - 1,
   the strength of the new curse) and two buttons 0x41f6 / 0x41f7. Every 0x8c ms or when the victim value changes the state is
   rebuilt: with no running curse (ai_FindCurseEvent of the building is 0) the clock is made again, the share bar goes back to 0
   and the text 0x41f5 shows the strength class (2 for a value below 4, 3 for 4 .. 6, 4 above) with the victim name 0x41fb + the
   value and the due clock; with a running curse the clock is its due time (two hours later while it is not yet done), the share
   is the minutes from its start to now over those to the due time (at most 1, times 100) and the text 0x41f8 (the victim
   0x41fb + its id) or 0x41f9 (done) is shown. The buttons are enabled and the victim slider is used only without a running
   curse, the share bar only with one. The right mouse button and the second button leave; the first button pays the class
   (sv_CheckMoney, gm_SendMoneyDelta) and sends the message 0xa0 (the building id, the game time, arg 0 = the strength,
   flags 2) with cm_SendLongMessage. The pump ends with d2_CloseView and the camera follow. */

void ai_CurseTargetWindow(int building)
{
    CURSEMSG msg;
    AITIME when;
    int ids[2];                         /* the two buttons */
    int share, victim, form;
    int value, cls;
    unsigned tick;
    CURSEEVENT *ev;
    int elapsed, total;

    tick = 0;
    cls = 2;
    value = 0;
    memcpy(&when, &ai_Time, sizeof(AITIME));
    gm_AddTime((struct AITIME *)&when, 0x18, 0, 0);
    when.hour = util_RandomU(cls) + 0x15;
    when.min = 0;
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    share = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2, 10, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    d2_SetObjValue(share, 0, 100, 0);
    d2_SetForm(form, 2);
    victim = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    gm_CurseTargetCount((char *)building);
    d2_SetObjValue(victim, 0, gm_CurseTargetCount((char *)building) - 1, 0, 1);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41f6], txt_Ptrs[0x41f7]);
    do {
        if (d3_Time > tick + 0x8c || d2_GetObjValue(victim) != value) {
            tick = d3_Time;
            ev = (CURSEEVENT *)ai_FindCurseEvent((struct AIBLD *)building);
            if (ev != 0) {
                when = ev->due;
                when.hour += ev->done ? 0 : 2;
                elapsed = gm_MinutesBetween((struct AITIME *)&ev->from, (struct AITIME *)&ai_Time);
                total = gm_MinutesBetween((struct AITIME *)&ev->from, (struct AITIME *)&when);
                d2_SetObjValue(share, 0, 100, (int)(min((float)elapsed / total, 1.0f) * 100.0f));
            } else {
                memcpy(&when, &ai_Time, sizeof(AITIME));
                gm_AddTime((struct AITIME *)&when, 0x18, 0, 0);
                when.hour = util_RandomU(2) + 0x15;
                when.min = 0;
                d2_SetObjValue(share, 0, 100, 0);
            }
            d2_SetForm(form, 1);
            io_PrintText("$C");
            if (ev != 0) {
                if (ev->done == 0)
                    io_PrintText((char *)0x41f8, 0x41f4, ev->victim + 0x41fb, &when);
                else
                    io_PrintText((char *)0x41f9, 0x41f4);
            } else {
                value = d2_GetObjValue(victim);
                if (value < 4)
                    cls = 2;
                else
                    cls = (value >= 7) + 3;
                io_PrintText((char *)0x41f5, 0x41f4, cls, value + 0x41fb, &when);
            }
            d2_SetObjEnabled(ids[0], ev == 0);
            d2_SetObjEnabled(ids[1], ev == 0);
            d2_SetForm(form, 2);
            d2_EnableObject(share, ev != 0);
            d2_EnableObject(victim, ev == 0);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, cls) == 1) {
                    gm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -cls);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0xa0;
                    msg.flags = 2;
                    msg.to = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId;
                    msg.fromId = ((CURSEBLD *)building)->id;
                    memcpy(&msg.time, &ai_Time, sizeof(AITIME));
                    msg.arg[0] = value;
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_CurseTargetWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
/* 0x5d95a0, the "Ehrerbietung" (homage) window "Locations\zigeuner\blutrache" (745 B): the building is the gypsy house.
   A building without an owner (owner 0xffff) ends it at once (camera follow). When the player's status bit 0x20 (+0x1cc of
   its record) is set the green message box 0x4207 / 0x4208 refuses. Otherwise the person to honour is chosen with the
   special-amount menu ai_AmountMenu (the config of the other gypsy windows, the text 0x4209; none ends the window) and the form
   shows a slider (the gift, 2 .. 8, starting at 2) and the two buttons 0x420c / 0x420d. Each pass the gift value v is read
   and the text 0x420a (the person's id, v) is shown. The right mouse button and the second button leave; the first button
   pays when the player has the money (sv_CheckMoney of v): the status bit 0x20 is set (cm_SendSetField), v is paid
   (gm_SendMoneyDelta) and the person's relation to the player rises by v * 5 (cm_SendRelations); a player character (kind 6 / 7)
   gets the news 0x420e ("_NACHRICHTEN_ZIGEUNER_04", channel 0x5f0) with the id of the player (he_ShowNews). The pump ends
   with d2_CloseView and the camera follow. */

void ai_HomageWindow(int building)
{
    char text[0x400];
    AMOUNTCFG cfg;                      /* the config of the amount menu */
    int ids[2];                         /* the two buttons */
    int slider, form;
    AICHARVIEW *target;
    int value;

    if (((CURSEBLD *)building)->owner == 0xffff) {
        sv_CameraFollow();
        return;
    }
    if (((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 0x20) {
        sv_MessageBoxGreen((int)txt_Ptrs[0x4207], (int)txt_Ptrs[0x4208], 0);
        return;
    }
    memset(&cfg, 0, sizeof(cfg));
    memset(cfg.ids, 0xff, sizeof(cfg.ids));
    cfg.flags = 1;
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    target = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x4209], 0, 0);
    if (target == 0) {
        sv_CameraFollow();
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    slider = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue(slider, 2, 8, 2, 1);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x420c], txt_Ptrs[0x420d]);
    do {
        value = d2_GetObjValue(slider);
        d2_SetForm(form, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x420a, 0x4207, target->id, value);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, value) == 1) {
                    cm_SendSetField(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, 0x1cc, 4, 0x20, 0);
                    gm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -value);
                    cm_SendRelations(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, target->netId, value * 5, 1);
                    if (target->kind == 6 || target->kind == 7) {
                        txt_Format(text, 0x420e, ai_PlayerId);
                        he_ShowNews(target->netId, -1, text, 0, 0x5f0, "_NACHRICHTEN_ZIGEUNER_04");
                    }
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_HomageWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
/* 0x5d9890, the gypsy group window "Locations\zigeuner\blutrache" form (661 B): the building is the gypsy house.
   Opens the form (the player bar sv_OpenPlayerBar first), shows the texts 0x41e7 / 0x41e6 / 0xed6 and a row of two buttons
   (txt_Ptrs[0x41e8] / [0x41e9]). The right mouse button and the second button leave; the first button collects up to four
   characters of the group selection (flag +0x188, ai_Selected): none shows the message box txt_Ptrs[0x41ea], otherwise the long message
   0xa1 (flags 1) is sent (cm_SendLongMessage) with the sender (the player's net id), the building id, the game clock and the net
   ids of the collected characters; the window then ends. The pump ends with d2_CloseView, the player bar close and the camera
   follow. Reasoned: the name and the purpose follow from the message contents (the building id, the clock, the chosen players),
   the form is the one the other gypsy windows use; message 0xa1 is not read at its receiver. Body read whole
   against the disassembly. */

void ai_GypsyBeggingWindow(int building)
{
    CURSEMSG msg;
    int chosen[4];                      /* the indices of the collected characters */
    int ids[2];                         /* the two buttons */
    int form, count, i;

    sv_OpenPlayerBar(building, 0);
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    io_PrintText("$C");
    io_PrintText((char *)0x41e7, 0x41e6, 0xed6);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41e8], txt_Ptrs[0x41e9]);
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                count = 0;
                for (i = 0; i < 768 && count < 4; i++) {
                    if (((AICHARVIEW *)(ai_CharsC + i * 0x21c))->selected != 0) {
                        chosen[count] = i;
                        count++;
                    }
                }
                if (count != 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0xa1;
                    msg.flags = 1;
                    msg.to = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId;
                    msg.fromId = ((CURSEBLD *)building)->id;
                    memcpy(&msg.time, &ai_Time, sizeof(AITIME));
                    for (i = 0; i < count; i++)
                        msg.ids[i] = ((AICHARVIEW *)(ai_CharsC + chosen[i] * 0x21c))->netId;
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                } else {
                    sv_MessageBox((int)txt_Ptrs[0x41ea], 0);
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GypsyBeggingWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}
/* 0x5d9c30, the contact listener of the gypsy production building (348 B): building the building record. The contact
   plates (sv_RegisterNamePlate) are created each frame from the permission bits ai_BuildingPerms: bit 0x200 ->
   contact_ZUTATEN_SUCHEN_ZIGEUNER (0x144aa34, 0x16), contact_TRANSPORT (0x15), contact_PRODUKTION_ZIGEUNER (
   0x1441f98, 0x13), contact_LAGER (0xe) and contact_BETTELN (0x144a954, 0x2c); bit 0x400 -> contact_PERSONALBUCH and
   contact_MEISTERBRIEF. The click (ai_ClickedObject) opens the helpers window 0x584d70 for the ingredient search, the
   transport, the production, the store (second argument 0), the master letter or the begging window
   (ai_GypsyBeggingWindow) of the building; the personnel book is checked after the chain from a fresh read of the click.
   Every frame ends with the trade window checks ai_TradeContactChecks. The pump (this function, 0x67fff) runs between the
   sv_ReleaseNamePlates call and the loop; the loop is entered by the pump result. Void. Reasoned: the plate names are
   the literal strings, the callees are read off the calls (the ingredient search window is the "HANDEL_SUCHEN" helper). */

void ai_GypsyProductionContact(int building)
{
    int zutaten = 0;
    int transport = 0;
    int produktion = 0;
    int lager = 0;
    int betteln;
    int personal;
    int meister;
    int pump;

    personal = 0;
    meister = 0;
    betteln = 0;
    sv_ReleaseNamePlates();
    pump = sv_RunFrame(ai_GypsyProductionContact, 0x67fff);
    while (pump != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            zutaten = sv_RegisterNamePlate("contact_ZUTATEN_SUCHEN_ZIGEUNER", txt_Ptrs[0x421d], 0x16);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            produktion = sv_RegisterNamePlate("contact_PRODUKTION_ZIGEUNER", txt_Ptrs[0x1f76], 0x13);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            betteln = sv_RegisterNamePlate("contact_BETTELN", txt_Ptrs[0x41e5], 0x2c);
        }
        if ((ai_BuildingPerms & 0x400) != 0) {
            personal = sv_RegisterNamePlate("contact_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meister = sv_RegisterNamePlate("contact_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
        }
        if (ai_ClickedObject != 0) {
            if ((ai_BuildingPerms & 0x200) != 0) {
                if (ai_ClickedObject == zutaten)
                    ai_GypsyHelperWindow((struct AIBLD *)building);
                else if (ai_ClickedObject == transport)
                    ai_OpenTransport(building);
                else if (ai_ClickedObject == produktion)
                    ai_ProductionWindow((struct AIBLD *)building);
                else if (ai_ClickedObject == lager)
                    ai_StorageWindow(building, 0);
                else if (ai_ClickedObject == meister)
                    ai_MasterLetterWindow((struct AIBLD *)building);
                else if (ai_ClickedObject == betteln)
                    ai_GypsyBeggingWindow(building);
            }
            if (ai_ClickedObject == personal)
                ai_PersonnelBook((struct AIBLD *)building, "ob_PERSONALBUCH");
        }
        ai_TradeContactChecks();
        pump = sv_RunFrame(ai_GypsyProductionContact, 0x67fff);
    }
}

/* dcp unit split: the declarations that moved to pokerwin (copies: drop what menu does not use) */
typedef struct PLAYERRECVIEW {          /* f3gm.h's player record (gm_PlayerRecord, 0x56 words): only the float the poker game reads */
    char pad00[0x60];
    float luck;                         /* +0x60, the chance that the player sits down to play (decays by * 0.66 - 0.1 per game; reasoned) */
} PLAYERRECVIEW;

/* dcp unit split: the declarations that moved to shot (copies: drop what menu does not use) */
#include <math.h>
#undef SIGN
#undef DRAG
#undef WINDDRAG
#include <math.h>
