/* unk_5d33b0.c -- the screen / front-end module (0x5d33b0 .. 0x5dd3e0; no leaked name): the file selector,
   the save / load windows, the options reader / writer and screens, the options menu, credits, pause and
   message filter, the gypsy / tailor contact windows, the poker windows, the wind and the cannon ball.
   Start (plausible): its first literal "%s_%s" 0x62d7d8 follows the previous unit's last literal "special_uni"
   0x62d6bc and the subject changes from the action records of the table 0x62c420 to the screens. End: the
   end of the game code (the last function 0x5dd100 returns at 0x5dd3d6, nop fill to 0x5dd3e0); dinput.lib's
   c_dfDIKeyboard / c_dfDIMouse object tables follow, library data (config/symbols/game_range.toml).
   Open: d2.h is not included yet: the unit's own declarations of d2_StartFade / d2_ReleaseFade / d2_WinOpenPic /
   d2_ScreenSurf / d2_Inputs / d2_LockSurface / d2_UnlockSurface disagree with d2.h's, so the unit reads the d2
   records through local views (D2INPUTVIEW, D2FADEVIEW, D2SURFVIEW ...).
   Open: ai_SaveOptionWindow / ai_SaveNameWindow / ai_LoadGameWindow / ai_JoinGameWindow still cast
   d2_GetViewWin to six arguments; as in ai_FilePickWindow the original likely nests d2_GetViewWin (win, 1)
   in the 8-argument sv_CreateFormSlider call (their @deep "cleanup split" notes).
   Open: not included yet (dcp headers conflicts unk_5d33b0 HEADER): ch.h cm.h d2.h d3_int_io.h gfx.h; held by
   own declarations that differ from the owner's: d2_StartFade, d2_ReleaseFade, d2_WinOpenPic, d2_ScreenSurf,
   d2_Inputs, d2_LockSurface, d2_UnlockSurface, ch_List, d3io_Lod.
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
#include "unk_5d33b0.h"
#define d2_SetObjValue2 ((void (*)(int, char *))d2_SetObjValue)  /* 0x475cb0 (d2.c: five parameters): @original the file selector ai_FilePickWindow pushes 2 arguments (the input, the name) */
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.c: int view): @original the callers pass the placement flags too, which the original's tail jump leaves on the stack for d2_PlaceWindowScaled */
#include "unk_575860.h"
#include "unk_58bbd0.h"
#include "data_views.h"
#include "time_view.h"
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
#include "unk_4a7140.h"
#include "unk_4b39f0.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "dd_vesa.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define sv_CheckGoodAmountB ((int (*)(char *, int, unsigned char))sv_CheckGoodAmount)  /* 0x521040 (sv.c: unsigned t): @original ai_CurseWindow (0x5d8c90) passes the good as a byte (mov cl, [ai_OwnPlayer], no zero extension) */
#define ai_MarketBuildingV (*(G16BLDVIEW **)&ai_MarketBuilding)  /* 0x6f1bc8, ai_MarketBuilding (ai.c: AIBLD *) as this unit's G16BLDVIEW (its +0x4c / object fields) */
#define ai_FallbackBuildingB (*(unsigned char **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_PlayerBuildingB (*(unsigned char **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_CharsC (*(char (*)[768 * 0x21c])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) read flat, as this unit does */

extern OPTIONBYTES ai_GfxDetail, ai_GfxFloor, ai_GfxSet, ai_GfxFog, ai_GfxResolution, ai_GameSwitches, ai_GamePanel, ai_OptionPanelHelp;
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
            if (_strcmpi(foundExt, ext) == 0) {
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
int sv_CreateFormSlider(int x, int y, int f, int g, int p5, int *b, int *a, int *c); /* 0x52e860, sv.c:
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
void d2_SetObjValue(); int d2_GetObjValue(int obj); int d2_CreateWinText(short x, short y, char *text, int win); void d2_SetObjText(int obj, char *text);
char *_itoa(int, char *, int); void d2_SetObjFont(int obj, int font); int sv_RunFrame(int cb, unsigned mask);

/* 0x5d34b0, the file selector window "s_menu_fileselector": lists the files of dir with the extension ext
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

    ret = 0;
    input = -1;
    count = -1;
    okBtn = -1;
    win = d2_OpenForm(0, 0, "s_menu_fileselector");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    io_PrintText(caption);
    d2_SetForm(win, 0);
    sv_CreateFormSlider(0x140, 0x100, d2_LastWindow, d2_GetViewWin(win, 1), 0x18, &slidA, &slidB, &count);
    memset(recs, 0, sizeof(recs));
    n = ai_ListFiles(dir, ext, (char *)recs);
    direct = flags & 1;
    if (direct == 0) {
        d2_SetForm(win, 3);
        okBtn = d2_GetWinObj(win, 3, io_PrintText(0x1ccc));
        d2_SetForm(win, 0);
        input = d2_GetWinObj(win, 0, 0);
        d2_SetObjValue2(input, txt_DefaultFileName);
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
    _itoa(n, num, 10);
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
                        d2_SetObjValue2(input, recs[i].name);
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

/* 0x5d3810, a message box (481 B): param_1 the text id (0x515960), param_2 the flags: 2 -> the
   "misc\Messagebox_Autosize", 0x10 -> _BIG, 0x20 -> _VERY_BIG, 0x100 -> _NONE_PERGA (pump form 1),
   else "misc\Messagebox"; bit 3 (8) replaces the pump flags with 0x717c6, bit 0x40 calls 0x4741a0 per
   frame and bit 2 (4) closes the box when 0x6333cc has advanced 500 past the start. The box is created
   with 0x4741f0 / 0x475600; the control list 0x46c490 walks the 0x6ed980 + 0x18 ids (the count +0x1c)
   and adds every control record 0xf8f080 + id * 0x2ec whose +0x18 is not 0x40 (0x46c560). The pump
   0x5309a0 runs with this function as its callback (0x46c6c0 per frame); a click id 0 (0x475600?) means
   yes -> 1, id 1 -> 0. 0x6f2f95 byte 1 marks the box open (a second box returns 0 at once).
   Returns 1 / 0.
   @deep meaning matches the original: the six messagebox names by flags 2/0x10/0x20/0x100, the 0x717c6 override, 0x46c490/0x46c560 control filter (!= 0x40), the pump loop (0x46c6c0, 0x4741a0 on 0x40, 0xf8a668 0/1 -> ret 1/0, 0x14673a0, the +500 ms timeout on 0x6333cc) and the 0x6f2f95 open flag all match 0x5d3810's disassembly; the loop bound is re-read each iteration now. 273 code hunks remain from codegen only: frame 0x0c vs 0x10 (a 4th original slot with no reader), register allocation (list ebp, flags ebx, name edi vs the original's esi loop index) and the store/scheduling order of the flag tests */
int ai_MessageBox(int param_1, int param_2)
{
    int win;
    int list;
    int id;
    int i;
    int n;
    int ret;
    int pump_flags;
    unsigned int start;
    char *name;

    start = (unsigned int)U_I((unsigned)&ai_ClockMs);
    ret = 0;
    if (U_B(((unsigned)&ai_GfxResolution + 0x1)) != 0)
        return 0;
    pump_flags = U_I((unsigned)&sv_FrameMask) & 0xffffffc7;
    if ((param_2 & 2) == 0) {
        if ((param_2 & 0x10) == 0) {
            if ((param_2 & 0x20) == 0) {
                if ((param_2 & 0x100) != 0) {
                    win = d2_OpenForm(0, 0, "misc\Messagebox_NONE_PERGA");
                    d2_SetForm(win, 1);
                    goto created;
                }
                name = "misc\Messagebox";
            } else {
                name = "misc\Messagebox_VERY_BIG";
            }
        } else {
            name = "misc\Messagebox_BIG";
        }
    } else {
        name = "misc\Messagebox_Autosize";
    }
    win = d2_OpenForm(0, 0, name);
created:
    if ((param_2 & 8) != 0)
        pump_flags = 0x717c6;
    io_PrintText(param_1);
    d2_CentreViewWindows(win);
    U_B(((unsigned)&ai_GfxResolution + 0x1)) = 1;
    list = d2_CreateRadioGroup(0);
    for (i = 0; i < *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 0x1c); i++) {
        id = *(int *)(*(int *)(U_I((unsigned)&d2_LastWindowPtr) + 0x18) + i * 4);
        if (U_I(U_I((unsigned)&d2_Objs) + 0x18 + id * 0x2ec) != 0x40)
            d2_RadioAdd(list, id);
    }
    while (sv_RunFrame((int)ai_MessageBox, pump_flags) != 0) {
        d2_MenuNavigate(list);
        if ((param_2 & 0x40) != 0)
            d2_RaiseViewWindows(win);
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if ((param_2 & 4) != 0 && start + 500 < (unsigned int)U_I((unsigned)&ai_ClockMs))
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    U_B(((unsigned)&ai_GfxResolution + 0x1)) = 0;
    d2_RadioClear(list);
    d2_CloseView(win);
    return ret;
}
/* 0x5d3a00, writes a save slot (161 B): param_1 unused, param_2 the two slot records (0x220 bytes each),
   param_3 the slot (-1 -> -1; 0 = AUTOSAVE, 1 = QUICKSAVE, any other value writes without a name
   check), param_4 the request record (the name at +9). The name must equal the slot's string
   ("QUICKSAVE" 0x624528 / "AUTOSAVE" 0x62451c, case-insensitive). A free slot (+8 == -1) gets the byte
   +0 = the slot and the 0x84 dwords of param_4 copied to +0x10. Returns the slot, -1 when taken / the
   name does not match.
   @deep meaning matches the original/Ghidra: the QUICKSAVE/AUTOSAVE name rules over param_4+9, param_3 0/1/-1 handling, the second case-insensitive compare, the slot = param_2 + param_3*0x220 free check at +8 == -1, the byte store and the 0x84-dword copy from param_4; the 100 code hunks are control-flow layout (the two strcmpi paths with the inline -1 returns at 0x5d3a46/0x5d3a56 and 0x5d3a6d) and register allocation (param_4 in edi vs esi, param_3 in ebx) only */

int ai_RegisterSaveSlot(int param_1, int param_2, int param_3, void *param_4)
{
    char *name;
    char *want;
    char *slot;
    int i;

    (void)param_1;
    name = (char *)param_4 + 9;
    if (_strcmpi(name, "QUICKSAVE") == 0) {
        param_3 = 1;
        want = "QUICKSAVE";
    } else if (_strcmpi(name, "AUTOSAVE") == 0) {
        param_3 = 0;
        want = "AUTOSAVE";
    } else if (param_3 == 0) {
        want = "AUTOSAVE";
    } else if (param_3 == 1) {
        want = "QUICKSAVE";
    } else if (param_3 == -1) {
        return -1;
    } else {
        goto write;
    }
    if (_strcmpi(name, want) != 0)
        return -1;
write:
    slot = (char *)(param_2 + param_3 * 0x220);
    if (*(int *)(slot + 8) != -1)
        return -1;
    *slot = (char)param_3;
    for (i = 0; i < 0x84; i++)
        ((int *)(slot + 0x10))[i] = ((int *)param_4)[i];
    return param_3;
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
void d2_SetObjValue();                                               /* 0x475cb0, K&R: d2.c's definition takes 5 parameters, the call sites push 2, 4 or 5 */
void d2_SetObjMax(int obj, int v);                                   /* 0x475f40 */

/* d2.h's D2FADE (the fade record of 0x4767d0): only the flags byte is read here (bit 2 = done, the
   original tests the byte, not the dword); d2.h cannot be included, see the lead comment */
typedef struct D2FADEVIEW { unsigned char flags; char pad1[0x63]; } D2FADEVIEW;
/* d2's window records, as d2.c defines them (D2WIN, d2_types.h) */
extern D2WIN d2_Wins[];             /* 0xf8f120, d2.h: D2WIN d2_Wins[0x61] */
extern float sv_FramesPerTick;          /* 0x62378c, sv.c: the frames drawn since the last timer tick
                                           (the frame loop counts them in 0x623790, the tick callback
                                           0x520ec0 copies the count as a float and clears it) */
int d2_CreateWindow(int x, int y, int w, int h, unsigned int flags); /* 0x472390, d2.c: the callers
                                           push ints although the K&R definition names short parameters */
void d2_PlaceWindowScaled(int win, int flags); /* 0x475730, d2.c */
int d2_RemoveWindow(int win);           /* 0x472ce0, d2.c */
int io_PrintText(char *fmt, ...);          /* 0x515960, io_text.c: prints a text (id or literal) with its
                                           tokens into the current window and redraws it */
extern short d2_ScreenW;                /* 0xfb9072, d2.h */
extern short d2_ScreenH;                /* 0xfb9070, d2.h */
extern int d2_DirtyRectLeft;            /* 0xfb9060, d2.h: the rectangle d2_AddDirty tests against */
extern int d2_DirtyRectTop;             /* 0xfb9064, d2.h */
extern int d2_DirtyRectRight;           /* 0xfb9068, d2.h */
extern int d2_DirtyRectBottom;          /* 0xfb906c, d2.h */
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
extern int sv_ViewRectLeft;             /* 0x6248a4, sv.c: the game view rectangle */
extern int sv_ViewRectTop;              /* 0x6248a8, sv.c */
extern int sv_ViewRectRight;            /* 0x6248ac, sv.c */
extern int sv_ViewRectBottom;           /* 0x6248b0, sv.c */
void d2_FocusInput(int obj);                                           /* 0x478320 */

/* the two records 0x5d3ab0 builds: one slot of the save / load window and the request
   ai_RegisterSaveSlot (0x5d3a00) copies into a free one */
#pragma pack(push, 1)
typedef struct SAVESLOTVIEW {               /* 0x220, one slot of the save / load window */
    unsigned char kind;                     /* +0x00, the slot kind (ls_SaveSlot for a registered save) */
    char pad01[3];
    int obj;                                /* +0x04, the slot's caption text object, -1 = none */
    int win;                                /* +0x08, the slot's child window, -1 = free */
    unsigned char used;                     /* +0x0c, 1: a filled slot, 0: an empty one */
    char pad0d[0x0c];
    char name[0x80];                        /* +0x19, the caption (the request's name) */
    char title[0x80];                       /* +0x99, the save's title (the header's +0x9c) */
    char pad119[0x107];
} SAVESLOTVIEW;                             /* 0x220 */
typedef struct SAVEREQUEST {                /* 0x210, the request of ai_RegisterSaveSlot */
    char pad00[9];
    char name[0x77];                        /* +0x09, the slot caption */
    char pad80[0x190];
} SAVEREQUEST;                              /* 0x210 */
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
    unsigned char nameTextOffset;           /* +0x5c, added to the text id 0x253 of the character line */
    char pad5d[4];
    unsigned char officeTextOffset;         /* +0x61, added to the text id 0x130 of the character line */
    unsigned char traitTextOffset;          /* +0x62, added to the text id 0x114 of the character line */
    char pad63[5];
    int money;                              /* +0x68, the character's money ("%i%c") */
    unsigned char compactTexts;             /* +0x6c, nonzero: the three text ids shift by -10 / -0x5f / -0x28 */
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
int d2_WinOpenPic(int x, int y, int pic, int win); /* 0x4731d0, d2.h: creates a picture object; @original the original call at 0x5d7f52 pushes the int arguments (push eax / ebx / edx / 8) against the short-parameter definition */
/* ai_EmptySlotNameFormat: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_DifficultyTexts: view of txt_Ptrs (build/mk_txt_views.py) */
extern int d2_HitWindow;                    /* 0xf8aad0, the window the pointer hit (gfx.c sets it from
                                               d2_PointerWin; also the "double click" id of the slots) */
/* ai_LoadGameConfirmText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_SaveProgressText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_SaveOverwriteText: view of txt_Ptrs (build/mk_txt_views.py) */
extern int ai_GameFlagsWord;                /* 0x6f23f8, the dword view of ai_GameFlags: the load window
                                               writes the load mode 10 whole */
void d2_EnableObject(int obj, int on);      /* 0x475aa0, d2.h: enables / disables an object and its items */
int sv_OpenMessageBox(int text, unsigned char flags); /* 0x521900, sv.c: opens the waiting box with text */

int ai_LeaveGame = 0;  /* 0x6f25a0, set with ai_GameOver when the menu leaves the game (the
                                       load window / the game loop end); main.c clears it when a game
                                       starts ( */
OPTIONBYTES ai_GfxResolution = {0};  /* 0x6f2f94: byte 0 cur_res */
/* ai_ContactTextProductionTailor: view of txt_Ptrs (build/mk_txt_views.py) */
EFFECTSTATE ai_EffectState = {0};  /* 0x6f3300 .. 0x6f34f0, the wind state ai_InitWind builds */
POKERGAME ai_PokerState = {0};  /* 0x6f3500 .. 0x6f35c0, the poker game state (unk_4a7140.c steps it) */
OPTIONBYTES ai_GfxSet = {0};  /* 0x6f35c0: byte 0 gfx_set */
/* the option globals the writer 0x5d4c70 (and the reader 0x5d5640) use: each dword holds four option
   bytes, the INI keys name the bytes (see the key order in 0x5d4c70) */
OPTIONBYTES ai_GfxDetail = {0}; /* 0x6f35c4: byte 1 texture_scale, 0 details, 2 lod_handling, 3 shadow_detail */
OPTIONBYTES ai_GfxFloor = {0};  /* 0x6f35c8: byte 0 floor_mipmapping, 1 camera_limits, 2 floor_lod, 3 character_detail */
OPTIONBYTES ai_GfxFog = {0};  /* 0x6f35cc: byte 0 fog_plane */
float ai_ScreenTuning[0xc] = {0};  /* 0x6f35d0: brightness r, g, b, a, contrast r, g, b, a, gamma
                                               r, g, b, a (0 .. 1, the keys are the value * 100) */
SNDOPTIONBYTES ai_SoundOptions = {0};  /* 0x6f3600 */
int ai_OptionSpeed = 0;  /* 0x6f3608, the "speed" key: the game speed setting ( */
int ai_OptionCameraSpeed = {0}; /* 0x6f360c, the "camera_speed" key (the reader stores the low byte) */
int ai_OptionMouseSpeed = 0;  /* 0x6f3610, the "mouse_speed" key (the input base is this - */
OPTIONBYTES ai_GameSwitches = {0};  /* 0x6f3618: byte 0 invert_mouse, 1 nachtwaechter, 2 show_cursor_txt, 3 show_geb_info */
extern OPTIONBYTES ai_GamePanel;
extern int ai_OptionScrollSpeed;            /* 0x6f3614, the "scroll_speed" key */
char ai_TownName[64] = {0};  /* 0x6f361c, the "stadt" key (written as a string) */
int ai_OptionHistorie = 0;  /* 0x6f365c, the "historie" key */
int ai_OptionMission = 0;  /* 0x6f3660, the "mission" key */
int ai_OptionNetMission = 0;  /* 0x6f3664, the "net_mission" key */
OPTIONBYTES ai_GamePanel = {0};  /* 0x6f3668: byte 0 panel_mode, 1 help_events, 2 difficulty, 3 hints */
OPTIONBYTES ai_OptionPanelHelp = {0};  /* 0x6f366c, the "panel_help" key */
int ai_Opponents[7] = {0};  /* 0x75e160 .. */
char ai_OptionsFile[288] = {0};  /* 0x75e480, the INI the options are read from and written to */
extern unsigned char ai_OptionCameraSpeedByte; /* 0x6f360c, the byte the reader stores (the writer and the
                                               run time read ai_OptionCameraSpeed as an int) */
typedef struct SCREENMODE {                 /* 8 bytes, a screen mode of the table 0x62d7b8 */
    int w;                                  /* +0x0, the width */
    int h;                                  /* +0x4, the height */
} SCREENMODE;
/* ai_ContactTextCardGame: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_CameraStep = 50;  /* 0x623814, the camera move step, 100 - camera speed; sv.c converts it
                                       with fild at the float uses (the original loads an int) */
SCREENMODE ai_ScreenModes[3] = { { 800, 600 }, { 1024, 768 }, { 1152, 864 } };  /* 0x62d7b8, the 640 / 800 / 1024 modes */
int ai_ScreenModeW = 800;  /* 0x62d7d0, the width of the mode ai_GfxResolution picks */
int ai_ScreenModeH = 600;  /* 0x62d7d4, its height */
char *_itoa(int value, char *buf, int base); /* 0x5e56c4, the library's integer to text (base 10 here) */

/* 0x5d3ab0, fills the 16 slots of a save / load window (1488 B): view / form the window's form
   (d2_SetForm), slots the 16 slot records, files the 0x210-byte file list buffer and dir the save
   directory. The slots are cleared and marked free (-1), then ai_ListFiles (dir, ".SAV") lists the
   saved games (at most 16): each entry's file (the path at +0x109) is opened, its header read
   (ls_ReadSaveHeader); a map-only save (flags bit 2) is skipped. ai_RegisterSaveSlot puts the entry
   into a free slot, which gets a child window, its caption (the save's title) and the save's date,
   money and difficulty (the texts 0x1cc5 / 0x1cc6, the difficulty name 0x14413f0). A second pass over
   the 16 records gives every still free slot an empty-slot caption ("Slot %i" 0x14414e0), the screen
   image 0x766320 and the text 0x1cc7. Returns nothing.
   @deep body written from tools/disasm 0x5d3ab0 (the .SAV listing, the ls_ReadSaveHeader header, ai_RegisterSaveSlot, the child window + caption + the texts 0x1cc5 / 0x1cc6 with the difficulty name 0x14413f0, the second pass with the empty-slot name 0x14414e0 and 0x1cc7) and the meaning matches: the 121 differing instructions of 5 regions are regalloc / scheduling only - the loop prologue order (ebp=0x54, cmp/jle, the rec spill), the io_PrintText arg computation (our lea folding vs the original add/add; the select in ecx vs edx), the memset(&req) / format-load order, the d2_SetForm argument registers (eax/ecx vs ecx/edx) and the d2_Wins index / name pointer pair (edx/ecx swapped). Tried this session: the select polarity was a real meaning bug ((compact ? N : 0) + base, fixed), the loop as for/while over the parameter itself (157 hunks, worse, reverted), the strcpy pointer loop (matches); spell.py: order, reload, dup-tail - the reload offsets are the reused locals of the two passes */
void ai_FillSaveSlots(int view, int form, SAVESLOTVIEW *slots, char *files, char *dir, int reserved)
{
    SAVEHEADERVIEW hdr;
    char timebuf[0x40];
    SAVEREQUEST req;
    char *rec;
    int n;
    int i;
    int slot;
    int file;

    (void)reserved;
    memset(slots, 0, 0x2100);
    for (i = 0; i < 0x10; i++) {
        slots[i].obj = -1;
        slots[i].win = -1;
    }
    n = ai_ListFiles(dir, ".SAV", files);
    if (n > 0x10)
        n = 0x10;
    for (rec = files; n > 0; rec += 0x210, n--) {
        file = vfs_Tfopen(rec + 0x109, "rb", vfs_Root);
        if (file == 0)
            continue;
        if (ls_ReadSaveHeader(file, &hdr) == 0 || (hdr.flags & 2) != 0) {
            vfs_fclose(file);
            continue;
        }
        vfs_fclose(file);
        slot = ai_RegisterSaveSlot(0x10, (int)slots, hdr.slot, rec);
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
                     (hdr.compactTexts ? 10 : 0) + 0x114 + hdr.traitTextOffset,
                     hdr.scenario,
                     (hdr.compactTexts ? 0x5f : 0) + 0x130 + hdr.nameTextOffset,
                     (hdr.compactTexts ? 0x28 : 0) + 0x253 + hdr.officeTextOffset,
                     timebuf,
                     ai_DifficultyTexts[hdr.difficulty]);
    }
    memset(ai_ScreenImage, 0, 0x9600);
    for (i = 0; i < 0x10; i++) {
        if (slots[i].win != -1)
            continue;
        memset(&req, 0, 0x210);
        sprintf(req.name, ai_EmptySlotNameFormat, i);
        slot = ai_RegisterSaveSlot(0x10, (int)slots, i, &req);
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
   (sv_CreateFormSlider 0x214 x 0x168) of the list control d2_GetViewWin (win, 1, 0x82); then
   ai_FillSaveSlots (win, 1) fills the 16 slots from "gamedata/saves". The pump sv_RunFrame
   (0x5309a0) runs with this function and the mask of the frame; per frame a mouse event asks the
   screen to pop and, when dm_L.lclick is set, the 16 slots are walked: the slot is hit when the click
   is on its caption object (d2_PointerObj), in its window (d2_HitWindow) or in a control of it (the
   parent object of the clicked window belongs to the slot's window). A hit asks for confirmation
   (ai_MessageBox 0x101) while a game runs, then writes "Gamedata\Saves\<name>.SAV" into ls_SavePath,
   sets the load mode (ai_GameFlags 10) and the pop request and returns 1. Returns 0 otherwise. */

int ai_LoadGameWindow(void)
{
    int ret;
    unsigned mask;
    int win;
    int ctrl;
    int x14, x18, x1c;
    char path[0x104];
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int i;

    ret = 0;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindowF(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c57);
    d2_SetForm(0, 0);
    ctrl = ((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &x1c, &x14,
                                                                      &x18); /* @original: the original pushes six
        arguments (win, 1, 0x82, &x1c, &x14, &x18) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    ((int (*)(int, int, int, int))sv_CreateFormSlider)(0x214, 0x168, d2_LastWindow, ctrl); /* @original:
        the original pushes four of the eight arguments of sv.c's slider builder */
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
                    if (ai_GameState == 0 || ai_MessageBox(0x101, ai_LoadGameConfirmText) != 0) {
                        sprintf(path, "Gamedata\\Saves\\%s.SAV", slots[i].name);
                        strcpy(ls_SavePath.path, path);
                        ai_GameFlagsWord = 10;
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
   caption 0x1c57, the slider 0x214 x 0x168 of the list control d2_GetViewWin (win, 1, 0x82)) with
   ai_FillSaveSlots (win, 1, "gamedata/network", mode 1). The pump sv_RunFrame runs with this function
   and the frame's mask; a mouse event asks the screen to pop, and when dm_L.lclick is set the 16 slots
   are walked (the same hit test as ai_LoadGameWindow: the caption object, the slot's window, or a
   control of it). A hit asks for confirmation (ai_MessageBox 0x101) while a game runs, then copies the
   slot's name into ls_SavePath (the caller opens "gamedata/network/<name>.SAV"), asks the screen to
   pop and returns 1. Returns 0 otherwise. */

int ai_JoinGameWindow(void)
{
    int ret;
    unsigned mask;
    int win;
    int ctrl;
    int x14, x18, x1c;
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int i;

    ret = 0;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindowF(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c57);
    d2_SetForm(0, 0);
    ctrl = ((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &x1c, &x14,
                                                                      &x18); /* @original: the original pushes six
        arguments (win, 1, 0x82, &x1c, &x14, &x18) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    ((int (*)(int, int, int, int))sv_CreateFormSlider)(0x214, 0x168, d2_LastWindow, ctrl); /* @original:
        the original pushes four of the eight arguments of sv.c's slider builder */
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
                    if (ai_GameState == 0 || ai_MessageBox(0x101, ai_LoadGameConfirmText) != 0) {
                        strcpy(ls_SavePath.path, slots[i].name);
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
    char *src;
    char *dst;
    int *input;

    ret = 0;
    id = d2_OpenInput(x, y, 0, d2_FontHeight, 1);
    d2_SetObjFont(id, 0x53);
    d2_SetObjValue(id, text);  /* @original: this call site pushes 2 arguments (0x5d4500) against the 5-parameter definition */
    d2_FocusInput(id);
    input = &d2_Objs[id].input;
    d2_Inputs[*input].w = 0x80;
    d2_Inputs[*input].maxw = 0x110;
    d2_SetObjMax(id, 0x1f);
    while (sv_RunFrame((int)ai_EditText, sv_FrameMask) != 0) {
        if (kb_Last == 0x1c) {
            src = (char *)d2_GetObjValue(id);
            dst = text;
            while ((*dst++ = *src++) != '\0')
                ;
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

    _itoa(ai_GfxDetail.byte[1], buf, 10);
    WritePrivateProfileStringA("Gfx", "texture_scale", buf, ai_OptionsFile);
    _itoa(ai_GfxDetail.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "details", buf, ai_OptionsFile);
    _itoa(ai_GfxDetail.byte[2], buf, 10);
    WritePrivateProfileStringA("Gfx", "lod_handling", buf, ai_OptionsFile);
    _itoa(ai_GfxDetail.byte[3], buf, 10);
    WritePrivateProfileStringA("Gfx", "shadow_detail", buf, ai_OptionsFile);
    _itoa(ai_GfxFloor.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "floor_mipmapping", buf, ai_OptionsFile);
    _itoa(ai_GfxSet.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "gfx_set", buf, ai_OptionsFile);
    _itoa(ai_GfxFloor.byte[1], buf, 10);
    WritePrivateProfileStringA("Gfx", "camera_limits", buf, ai_OptionsFile);
    _itoa(ai_GfxFloor.byte[2], buf, 10);
    WritePrivateProfileStringA("Gfx", "floor_lod", buf, ai_OptionsFile);
    _itoa(ai_GfxFloor.byte[3], buf, 10);
    WritePrivateProfileStringA("Gfx", "character_detail", buf, ai_OptionsFile);
    _itoa(ai_GfxFog.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "fog_plane", buf, ai_OptionsFile);
    _itoa(ai_GfxResolution.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Gfx", "cur_res", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[0] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_r", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[1] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_g", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[2] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_b", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[3] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "brightness_a", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[4] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_r", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[5] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_g", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[6] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_b", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[7] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "contrast_a", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[8] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_r", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[9] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_g", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[10] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_b", buf, ai_OptionsFile);
    _itoa((int)(ai_ScreenTuning[11] * 100.0f), buf, 10);
    WritePrivateProfileStringA("Gfx", "gamma_a", buf, ai_OptionsFile);
    _itoa(ai_SoundOptions.masterVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "master_vol", buf, ai_OptionsFile);
    _itoa(ai_SoundOptions.sfxVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "sfx_vol", buf, ai_OptionsFile);
    _itoa(ai_SoundOptions.msxVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "msx_vol", buf, ai_OptionsFile);
    _itoa(ai_SoundOptions.voiceVolume, buf, 10);
    WritePrivateProfileStringA("Sound", "speech_vol", buf, ai_OptionsFile);
    _itoa(ai_SoundOptions.msxFreq & 0xff, buf, 10);
    WritePrivateProfileStringA("Sound", "msx_freq", buf, ai_OptionsFile);
    _itoa(ai_OptionSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "speed", buf, ai_OptionsFile);
    _itoa(ai_OptionMouseSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "mouse_speed", buf, ai_OptionsFile);
    _itoa(ai_OptionScrollSpeed, buf, 10);
    WritePrivateProfileStringA("Game", "scroll_speed", buf, ai_OptionsFile);
    _itoa(ai_OptionCameraSpeed & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "camera_speed", buf, ai_OptionsFile);
    _itoa(ai_GameSwitches.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "invert_mouse", buf, ai_OptionsFile);
    _itoa(ai_GameSwitches.byte[1], buf, 10);
    WritePrivateProfileStringA("Game", "nachtwaechter", buf, ai_OptionsFile);
    WritePrivateProfileStringA("Game", "stadt", ai_TownName, ai_OptionsFile);
    _itoa(ai_OptionHistorie, buf, 10);
    WritePrivateProfileStringA("Game", "historie", buf, ai_OptionsFile);
    _itoa(ai_OptionMission, buf, 10);
    WritePrivateProfileStringA("Game", "mission", buf, ai_OptionsFile);
    _itoa(ai_OptionNetMission, buf, 10);
    WritePrivateProfileStringA("Game", "net_mission", buf, ai_OptionsFile);
    _itoa(ai_GameSwitches.byte[2], buf, 10);
    WritePrivateProfileStringA("Game", "show_cursor_txt", buf, ai_OptionsFile);
    _itoa(ai_GameSwitches.byte[3], buf, 10);
    WritePrivateProfileStringA("Game", "show_geb_info", buf, ai_OptionsFile);
    _itoa(ai_GamePanel.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "panel_mode", buf, ai_OptionsFile);
    _itoa(ai_GamePanel.byte[1], buf, 10);
    WritePrivateProfileStringA("Game", "help_events", buf, ai_OptionsFile);
    _itoa(ai_GamePanel.byte[2], buf, 10);
    WritePrivateProfileStringA("Game", "difficulty", buf, ai_OptionsFile);
    _itoa(ai_GamePanel.byte[3], buf, 10);
    WritePrivateProfileStringA("Game", "hints", buf, ai_OptionsFile);
    _itoa(ai_OptionPanelHelp.word & 0xff, buf, 10);
    WritePrivateProfileStringA("Game", "panel_help", buf, ai_OptionsFile);
    _itoa(ai_Opponents[0], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_a", buf, ai_OptionsFile);
    _itoa(ai_Opponents[1], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_b", buf, ai_OptionsFile);
    _itoa(ai_Opponents[2], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_c", buf, ai_OptionsFile);
    _itoa(ai_Opponents[3], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_d", buf, ai_OptionsFile);
    _itoa(ai_Opponents[4], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_e", buf, ai_OptionsFile);
    _itoa(ai_Opponents[5], buf, 10);
    WritePrivateProfileStringA("Game", "opponent_f", buf, ai_OptionsFile);
    _itoa(ai_Opponents[6], buf, 10);
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
   @deep the body is transcribed from build/ghidra/5d5640.c and the disassembly (every GetPrivateProfileIntA key, its default and its target byte/word, the 12 tuning floats * 0.01f, the town string, invert_mouse forced to 0, the mode table); readable, 15 code + 30 order hunks left. Two roots: (1) the cur_res mode table: the original digits i = (v & 0xff) * 8 as an integer and loads [eax+0x62d7b8] / [eax+0x62d7bc], i.e. a char-offset index into the table; a typed SCREENMODE index compiles to [eax*8+base] instead, and the char-offset spelling needs a dereferenced pointer cast (Q4) - a shared-header view decision. Tried: SCREENMODE ai_ScreenModes[i] (this), an int * view with i*2. (2) the rest: the store of each GetPrivateProfileIntA result is scheduled before / after the call's stack cleanup (mov ds:X,eax|al), ours consistently after. Tried: v = call; target = v (this, 15 hunks) vs target = call inlined (20 hunks, reverted). spell.py: no menu rule; the writer ai_SaveOptions (the twin) is byte */


void ai_LoadOptions(void)
{
    unsigned v;
    int i;

    v = GetPrivateProfileIntA("Gfx", "texture_scale", 0, ai_OptionsFile);
    ai_GfxDetail.byte[1] = v;
    v = GetPrivateProfileIntA("Gfx", "details", 0, ai_OptionsFile);
    ai_GfxDetail.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "lod_handling", 0, ai_OptionsFile);
    ai_GfxDetail.byte[2] = v;
    v = GetPrivateProfileIntA("Gfx", "shadow_detail", 0, ai_OptionsFile);
    ai_GfxDetail.byte[3] = v;
    v = GetPrivateProfileIntA("Gfx", "floor_mipmapping", 0, ai_OptionsFile);
    ai_GfxFloor.byte[0] = v;
    v = GetPrivateProfileIntA("Gfx", "camera_limits", 0, ai_OptionsFile);
    ai_GfxFloor.byte[1] = v;
    v = GetPrivateProfileIntA("Gfx", "floor_lod", 0, ai_OptionsFile);
    ai_GfxFloor.byte[2] = v;
    v = GetPrivateProfileIntA("Gfx", "character_detail", 1, ai_OptionsFile);
    ai_GfxFloor.byte[3] = v;
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
    i = v & 0xff;
    ai_ScreenModeW = ai_ScreenModes[i].w;
    ai_ScreenModeH = ai_ScreenModes[i].h;
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
    ai_OptionCameraSpeedByte = v;
    v = GetPrivateProfileIntA("Game", "scroll_speed", 0, ai_OptionsFile);
    ai_OptionScrollSpeed = v;
    v = GetPrivateProfileIntA("Game", "invert_mouse", 0, ai_OptionsFile);
    ai_GameSwitches.byte[0] = v;
    v = GetPrivateProfileIntA("Game", "nachtwaechter", 1, ai_OptionsFile);
    ai_GameSwitches.byte[1] = v;
    v = GetPrivateProfileIntA("Game", "show_cursor_txt", 1, ai_OptionsFile);
    ai_GameSwitches.byte[2] = v;
    v = GetPrivateProfileIntA("Game", "show_geb_info", 1, ai_OptionsFile);
    ai_GameSwitches.byte[0] = 0;
    ai_GameSwitches.byte[3] = v;
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
    v = GetPrivateProfileIntA("Game", "opponent_f", 2, ai_OptionsFile);
    ai_Opponents[5] = v;
    v = GetPrivateProfileIntA("Game", "opponent_f", 2, ai_OptionsFile);
    ai_Opponents[5];
    v = GetPrivateProfileIntA("Game", "opponent_g", 2, ai_OptionsFile);
    ai_Opponents[6] = v;
}

/* the character list this unit walks for the shadow bit: ch.h's CHARACTER (only the fields used here; ch.h
   cannot be included in this unit) */
typedef struct INIT_CHARACTER {
    char pad000[0x34];              /* 0x00 .. 0x33 */
    unsigned char *obj;             /* 0x34, CHARACTER.obj: the body (its +0x211 bit 3 is the shadow bit) */
    char pad038[0x50];              /* 0x38 .. 0x87 */
    void *scene;                    /* 0x88, CHARACTER.scene */
} INIT_CHARACTER;
extern INIT_CHARACTER *ch_List[0x300];   /* 0x1468ae0, ch.h's CHARACTER *ch_List[CH_MAX] */
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
void ch_SetLowPoly(unsigned char mode);         /* 0x402af0, ch.c */
void d3fl_SetMips(char a);                      /* 0x43f930, d3_fl.c */
void ai_CheckBuildSites(void);                  /* 0x58fb60, unk_58bbd0.c */
void d3_SetRLights(unsigned char on);           /* 0x43e180, d3_engine.c */
void init_SetDetailLevel(int *s, char f);       /* 0x513590, f3init.c */
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

/* 0x5d5c40, applies the graphics options to the scene: the shadow detail (ai_GfxDetail.byte[3]) picks the
   character shadow flag ch_ShadowEnabled and the shadow texture size (0x20 / 0x40), a changed size resets / frees
   every shadow and updates the scene, the map's detail level is set (init_SetDetailLevel), the LOD handling
   (byte[2]) selects d3io_Lod.mode and reloads the meshes with the scene deselected, d3_ApplyGraphicsOptions gets
   the floor mipmapping, the texture scale (2 - byte[1]) and the changed flag, the fog scale follows ai_GfxFog
   while no building is shown, the bodies of the town scene's characters get the shadow bit (+0x211 bit 3),
   ch_SetLowPoly gets the character detail (ai_GfxFloor.byte[3]), the camera limit pair of the camera_limits byte
   (ai_GfxFloor.byte[1]) is copied to sv_CameraHeightNear .. sv_CameraAngleFar, the game state re-places the
   camera, the mipmaps follow the floor setting and the scene is selected around the build-site check and the
   red lights. Returns 0
   @deep behaviour from the ROUGH stub rewritten against the whole original 0x5d5c40..0x5d5e8b (the stub was 16 B): the shadow detail (ai_GfxDetail.byte[3]) picks ch_ShadowEnabled + the 0x20 / 0x40 size and the changed flag, a changed size resets / frees the shadows (mask 0x40) and clamps d3s_TexSize to d3s_MaxTexSize before d3_UpdateAll(0); init_SetDetailLevel(ai_MapRefs, 0); the LOD handling (byte[2]) picks d3io_Lod (2 / 4), deselects the scene, reloads the meshes (0x40, 3) and reselects; d3_ApplyGraphicsOptions(floor byte, 0, 2 - texture_scale, changed, 0); the fog scale 1.0 - fog*0.01 (double constants) only while neither ai_PlayerBuilding nor ai_FallbackBuilding is set; the town-scene character bodies get the shadow bit (+0x211 bit 3, the xor idiom); ch_SetLowPoly(character_detail); the camera limit pair of the camera_limits byte (table 0x6237d0, 6 pairs, the pair +3) into sv_CameraHeightNear / sv_CameraAngleNear / ...Far; sv_PlaceCamera when ai_GameState; d3fl_SetMips on a floor change; then select the scene, ai_CheckBuildSites, d3_SetRLights(detail != 0), reselect. Q1/Q2/Q5/Q7 cleared (INIT_CHARACTER + ch_List, INIT_LOD union for the whole-dword d3io_Lod, INIT_MAPREF via data_views' ai_MapRefs, prototypes for all 13 callees). Left: 6 hunks / 98 differing instructions before this session, all codegen shape: the camera limit copy - ours did float copies (fld/fstp) from (0x623800 - 0x30) where the original copies the four dwords from the table symbol 0x6237d0; that table was missing from the C, so the copies read whatever the linker put before sv_CameraHeightNear (the camera angle became 8.2e20 and the whole 3D view culled blue); it is now sv_CameraLimits in sv.c and the copies come from it. Still open: the changed flag / lod / scene registers (slots hunk [esp+4]/[esp+8]). Renames: ch_Unk5f122c -> ch_ShadowEnabled, sv_Unk525dd0 -> sv_PlaceCamera, ai_Unk623800/04/08/0c -> sv_CameraHeightNear / sv_CameraAngleNear / sv_CameraHeightFar / sv_CameraAngleFar, ai_Unk6f22d4 / d8 -> sv_CameraHeightFactor / sv_CameraAngleFactor. */
int ai_ApplyGfxOptions(void)
{
    unsigned char level, changed;
    unsigned size;
    int lod, scene, i;
    INIT_CHARACTER *c;
    float *lim;

    changed = 0;
    level = ai_GfxDetail.byte[3];
    size = d3s_TexSize;
    if (level == 0) {
        ch_ShadowEnabled = 1;
        size = 0x20;
    } else if (level == 1) {
        ch_ShadowEnabled = level;
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
    init_SetDetailLevel((int *)ai_MapRefs, 0);
    lod = 0;
    if (ai_GfxDetail.byte[2] == 0)
        lod = 2;
    else if (ai_GfxDetail.byte[2] == 1)
        lod = 4;
    if (lod != (int)d3io_Lod.all) {
        scene = d3_CurrentScene;
        d3io_Lod.all = lod;
        d3if_SelectScene(0, 0);
        d3_ReloadMeshes(0x40, 3, 0);
        d3if_SelectScene(scene, 0);
    }
    d3_ApplyGraphicsOptions(ai_GfxFloor.byte[0], 0, 2 - ai_GfxDetail.byte[1], changed, 0);
    if (ai_PlayerBuildingB == 0 && ai_FallbackBuildingB == 0)
        d3o_FogScale = 1.0 - (double)ai_GfxFog.byte[0] * 0.01;
    for (i = 0; i < 0x300; i++) {
        c = ch_List[i];
        if (c != 0 && c->scene == d3_Scenes)
            c->obj[0x211] = ((ch_ShadowEnabled << 3 ^ c->obj[0x211]) & 8) ^ c->obj[0x211];
    }
    ch_SetLowPoly(ai_GfxFloor.byte[3]);
    lim = sv_CameraLimits;
    i = ai_GfxFloor.byte[1];
    sv_CameraHeightNear = lim[i * 2];
    sv_CameraAngleNear = lim[i * 2 + 1];
    sv_CameraHeightFar = lim[i * 2 + 6];
    sv_CameraAngleFar = lim[i * 2 + 7];
    if (ai_GameState != 0)
        sv_PlaceCamera(sv_CameraAngleFactor);
    if (d3fl_Mips != ai_GfxFloor.byte[0])
        d3fl_SetMips(ai_GfxFloor.byte[0]);
    scene = d3_CurrentScene;
    d3if_SelectScene(1, 0);
    ai_CheckBuildSites();
    d3_SetRLights(ai_GfxDetail.byte[0] != 0);
    d3if_SelectScene(scene, 1);
    return 0;
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
    ai_CameraStep = 100 - (ai_OptionCameraSpeed & 0xff);
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
    char pad1d0[0x4c];
} AICHARVIEW;

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
    d2_DirtyRectLeft = w;
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
    d2_DirtyRectLeft = sv_ViewRectLeft;
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
        int row = (int)yf * d2_ScreenSurf->pitch;
        xf = 0.0f;
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
    src.dwHeight = 0xf0;
    src.dwWidth = 0x140;
    src.lPitch = 0x280;                 /* 0x140 pixels of 16 bits */
    src.lpSurface = buf;
    src.ddpfPixelFormat.dwRGBBitCount = 0x10;
    src.ddpfPixelFormat.dwRBitMask = dd_RMask;
    src.ddpfPixelFormat.dwGBitMask = dd_GMask;
    src.ddpfPixelFormat.dwBBitMask = dd_BMask;
    D3_ZERO(&dst, sizeof(dst));
    dst.dwSize = sizeof(dst);
    dst.dwHeight = 0x78;
    dst.dwWidth = 0xa0;
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
   == 0); otherwise walks the character records (0x770b82, ai_Chars + 2, stride 0x21c) for one with the
   record flag +6 > 0, the kind +0 in {6, 7} and the dword +0x20a != -1; 0 if found, 1 when the walk
   passes gm_PlayerRecordBytes 0x7d5f82.
   @deep behaviour: 41 code hunks against the 78-byte original, 37 vs 35 instructions; the differences are VC6 register allocation (our state byte al + pointer eax, the original loads cl before mov eax,1 and keeps the pointer in ecx) and constant scheduling (our -1 materialized in edx with or edx,-1 where the original compares with the imm8 -1), not a missing statement: the record walk, the p[6] > 0 unsigned test (now jbe as the original), the kind 6/7 test and the -1 sentinel test all match. Meaning-class leads tried: the do-while pointer walk form, the return-1 placement (early constant, late constant, variable), the state test spelling (U_B(0x6f25ac) == 0 vs > 0), the p[6] > 0 vs != 0 (fixed one instruction). tools/spell.py: 38 of 72 differing, no hunk matched the menu - the allocation/scheduling byte work is match-deep's. Q4 blocked: the only raw access left is U_I(p + 0x20a), the dword at character +0x20a that this function tests for -1; no evidence for its meaning anywhere (ai.h's AICHAR pads 0x1e8..0x210, the only other reader is the raw save slice of ls.c 0x51a5b4), so a typed view field would be an invented name (Q2 flags unk20a). Q1/Q2/Q3/Q6 are done (ai_HasQuicksavePlayer, ai_GameState, ai_Chars + 2, gm_PlayerRecordBytes 0x7d5f82). */

int ai_HasQuicksavePlayer(void)
{
    unsigned char *p;

    if (ai_GameState == 0)
        return 0;
    p = (unsigned char *)ai_CharsC + 2;
    do {
        if (p[6] > 0 && (p[0] == 6 || p[0] == 7) && U_I(p + 0x20a) != -1)
            return 0;
        p += 0x21c;
    } while ((int)p < (int)gm_PlayerRecordBytes);
    return 1;
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
   clears the slots.
   @deep one code hunk, 1 instruction: the original stores ai_NumPlayers = 0 before the record walk (mov BYTE ds:0x62489c,dl with dl=0 at 0x5d7589) and the count again after it; our VC6 removes the first store as dead. Leads tried (one edit + build each): separate init (count = 0; ai_NumPlayers = count;), chained init (ai_NumPlayers = count = 0), for-loop vs do-while record walk, volatile declaration of ai_NumPlayers (keeps the store but turns the wait-loop compare into mov dl/and edx,0xff instead of the original's xor edx,edx/mov dl), wait-loop spellings n < ai_NumPlayers / (int) cast / + 0 / reversed compare / int temp. Read all hunks: the rest is instruction-equal (register renames, and the scheduling of rec[0]/rec[1], which matches with rec[0] written first); only a fill tail (6 nop bytes) remains. Readability done (Q1 name, Q2 named - AICHARVIEW for ai.h's AICHAR because f3snd.h's char ai_Chars[] view blocks ai.h, Q3/Q4/Q5/Q6/Q7 clean). Left is the dead-store codegen: a declaration/view that keeps the store without changing the wait loop, or an inlined-helper form - match-deep's toolbox */

void ai_SaveNetworkGame(char *saveName, char *name)
{
    char path[MAX_PATH];
    char hdr[0xfc];
    int rec[0x1f];
    VFSFILE *h;
    unsigned int hash = 0;
    unsigned char count;
    unsigned char *p;
    AICHARVIEW *c;
    int i, n;

    sv_FrameMask |= 0x20000;
    for (i = 0; i < 8; i++)
        cm_SaveAcks[i][0] = -1;
    if ((ai_GameFlags & 4) == 0)
        return;
    sv_BroadcastText(sv_FrameMask, ai_SaveWaitText);
    count = 0;
    ai_NumPlayers = count;
    p = (unsigned char *)ai_CharsC + 2;
    do {
        c = (AICHARVIEW *)(p - 2);
        if (c->id != 0xffff && (c->kind == 6 || c->kind == 7))
            count++;
        p += 0x21c;
    } while ((int)p < (int)gm_PlayerRecordBytes);
    ai_NumPlayers = count;
    strcpy(ls_SavePath.path, name);
    ls_SaveSlot = cm_RequestedSaveSlot;
    ai_LoadScreenImage("Gamedata/cities/standard.scr");
    sprintf(path, "Gamedata/network/%s.SAV", saveName);
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
    sprintf(path, "Gamedata/network/%s.SRV", saveName);
    ls_SaveGame(path, name, 6);
    sprintf(path, "Gamedata/network/%s.SAV", saveName);
    ls_SaveGame(path, name, 5);
    sv_SetStatusText(ch_EmptyName);
}
#define U_F(a) (*(float *)(a))

/* the callees of the front-end stretch (their units' headers are not included here, so their
   prototypes are repeated as the definitions spell them; the building pointers are this TU's ints) */
int ai_StorageWindow(int building, int list); /* 0x5b26b0, unk_58bbd0.c */
int ai_ProductionWindow(unsigned char *building); /* 0x58bd60, unk_58bbd0.c: "Handel\Handel_PRODUKTION" */
int ai_OpenTransport(int rec);             /* 0x5af1a0, unk_58bbd0.c */
int ai_PersonnelBook(int building, char *text); /* 0x5aab80, unk_58bbd0.c: "ob_PERSONALBUCH" */
void ai_TradeSearchWindow(unsigned char *cfg, int arg); /* 0x595480, unk_58bbd0.c: "HANDEL\HANDEL_SUCHEN" */
void ai_MasterLetterWindow(unsigned char *building); /* 0x5c3200, unk_58bbd0.c: "ob_MEISTERBRIEF" */
void ai_MarketContact(struct AIBLD *rec);  /* 0x58f510, unk_58bbd0.c */
int sv_CheckMoney(char *o, int n);         /* 0x520f90, sv.c: 1 when the object's money (+0x194)
                                              covers n, else a message box with the text 0x74 */
int sv_MessageBox(int text, unsigned flags); /* 0x521150, sv.c: the modal message box */
void sv_CameraFollow(void);                /* 0x52d7a0, sv.c: the per-frame camera follow */
int ai_AmountMenu(int record, int text, int text2, int callback); /* 0x5c1130, unk_58bbd0.c: the
                                              "special_amt1" amount menu, returns the chosen value */
int d2_CreateWindow(); int d2_RemoveWindow(); int io_PrintText(char *fmt, ...);
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
    d2_PlaceMainWindowF(wnd, 2);
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
    saved = snd_AmbientOnByte;
    snd_AmbientOnByte = 0;
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
    snd_AmbientOnByte = saved;
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
   quicksave slots (0 / 1) are disabled (d2_EnableObject 0) and only the slots 2 .. 15 are clickable.
   The pump sv_RunFrame runs with this function and the frame's mask; a mouse event asks the screen to
   pop and, when dm_L.lclick is set and the list is not scrolling (the window's scrollRemY is 0), the
   slots 2 .. 15 are walked with the hit test of ai_LoadGameWindow. A hit disables the slot object and
   opens the name editor (ai_EditText) at the caption's position on the slot's title; a name that is
   not cancelled (ai_MessageBox 0x101 asks first when "Gamedata\Saves\<name>.SAV" already exists) is
   saved: the file name "Gamedata/saves/GILDE_SAVEGAME_<i>.SAV", the screen image image (0x9600 bytes)
   into ai_ScreenImage, ls_SaveSlot = i, the message box 0x14414e8, ls_SaveGame (name, title, 1), the
   pop request. The slot object is enabled again. Returns 0.
   @deep body written from tools/disasm 0x5d45d0 and the meaning matches (the twin setup with the caption 0x1c58, ai_FillSaveSlots gamedata/saves mode 0, the two disabled autosave/quicksave slots, the list control d2_GetViewWin(win, 1) and its scrollRemY test, the slot walk 2 .. 15, the hit test, the name editor ai_EditText on the slot title, the overwrite question 0x14414ec, the file GILDE_SAVEGAME_%i.SAV, the 0x9600-byte image copy, ls_SaveSlot, sv_OpenMessageBox 0x14414e8, ls_SaveGame, the pop request, the re-enable). The 19 remaining code hunks have ONE root: the original splits the pending argument cleanup of the d2_GetViewWin call as add esp,8 right after it (and then add esp,0x40 / add esp,0x18 later), ours cleans the same 24 dwords as add esp,0x40 / add esp,0x20, which shifts every later [esp+..] by 8 - VC6's deferred push cleanup / argument-slot reuse (the same split as ai_LoadGameWindow / ai_JoinGameWindow, which grade behaviour). Tried: the exists = 0 + if(vfs_FindFile(...)) exists = 1 spelling (a real gain, 36 -> 21 hunks), the exists = 0 placement before the sprintf (21 -> 19), the path/name buffer sizes to make the frame 0x6628 exactly, the 2-argument d2_GetViewWin prototype + a cast for the six-argument calls (byte-neutral, Q7 fixed); the push 0x101 / push ecx order is the cdecl argument order and is not chased. spell.py: no menu rule for the cleanup split */
int ai_SaveOptionWindow(int image)
{
    unsigned mask;
    int win;
    int obj;
    int x20, x1c, x24;
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
    d2_PlaceMainWindowF(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c58);
    d2_SetForm(0, 0);
    obj = ((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &x20, &x1c,
                                                                     &x24); /* @original: the original pushes six
        arguments (win, 1, 0x82, &x20, &x1c, &x24) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    ((int (*)(int, int, int, int))sv_CreateFormSlider)(0x214, 0x168, d2_LastWindow, obj); /* @original:
        the original pushes four of the eight arguments of sv.c's slider builder */
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/saves", 0);
    d2_EnableObject(slots[0].obj, 0);
    d2_EnableObject(slots[1].obj, 0);
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
                    if (exists == 0 || ai_MessageBox(0x101, ai_SaveOverwriteText) != 0) {
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
   The slot object is enabled again. Returns 0.
   @deep body written from tools/disasm 0x5d4920 (the twin of ai_SaveOptionWindow with ai_FillSaveSlots gamedata/network mode 1, the one disabled slot 0, the list control + scrollRemY test, the slot walk 2 .. 15 and hit test, ai_EditText on the title, the exists check of Gamedata\network\<name>.SAV, the overwrite box 0x14414ec, ai_HasQuicksavePlayer -> sv_SetStatusText 0x14414a4 else the CMQUICKSAVE 0xf request through cm_SendExSysMessage, sv_PopRequest = 1, the re-enable) and the meaning matches. The 17 remaining code hunks have ONE root: the original splits the pending argument cleanup as add esp,8 right after the d2_GetViewWin call (then add esp,0x40 / add esp,0x10), ours cleans the same 32 dwords as add esp,0x40 / add esp,0x18, shifting every later [esp+..] by 8 - VC6's deferred push cleanup / argument-slot reuse (the same split in ai_LoadGameWindow / ai_JoinGameWindow, which grade behaviour). Tried: the sv_PopRequest = 1 in the save path (a real gain), the image parameter named, the path/rec buffer sizes to make the frame 0x65a0 exact, the 2-argument d2_GetViewWin prototype + a cast for the six-argument call (Q7). push 0x101 before push ecx is the cdecl evaluation order and is not chased. spell.py: no menu rule for the cleanup split */

int ai_SaveNameWindow(int image)
{
    unsigned mask;
    int win;
    int obj;
    int x20, x1c, x24;
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
    d2_PlaceMainWindowF(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c58);
    d2_SetForm(0, 0);
    obj = ((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &x20, &x1c,
                                                                     &x24); /* @original: the original pushes six
        arguments (win, 1, 0x82, &x20, &x1c, &x24) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    ((int (*)(int, int, int, int))sv_CreateFormSlider)(0x214, 0x168, d2_LastWindow, obj); /* @original:
        the original pushes four of the eight arguments of sv.c's slider builder */
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/network", 1);
    d2_EnableObject(slots[0].obj, 0);
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
                    if (exists == 0 || ai_MessageBox(0x101, ai_SaveOverwriteText) != 0) {
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
   0x1c7c / 0x1c7d, the two buttons 0x1ca2 / 0x1ca3 (read off the original's loads, not named by the text files).
   The resolution default reads ids[1] before sv_ButtonRow has filled it (the original does the same). @deep body reconstructed from the disassembly (nine controls of view 1, ids[2] row, ok/cancel copies, apply read-back in the original order, resolution change path); 449 = 449 instructions, ONE hunk of 6 differing instructions left at 0x5d60c5: the original stores the obj8 d2_GetWinObj result at once (mov [esp+0x50],eax) and reads the dd_Has1024x768 flag into al (mov al,[flag]; add esp,0x30; test al,al; mov eax,1), ours keeps eax live, reads the flag into cl and stores obj8 after the cleanup (mov [esp+0x20],eax). Tried: declaration order of obj7/obj8, ids[2] vs ids[4] (ids[4] fixes the frame 0x30 -> 0x38), mode from a separate uninit local vs ids[1] (the original reads the uninitialised slot of the second id), 'if (flag == 0)' forms. Allocation / scheduling only. */

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
    d2_PlaceMainWindowF(form, 2);
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
    mode = 1;
    if (dd_Has1024x768 == 0)
        mode = dflt;
    if (dd_Has1152x864 != 0)
        mode = 2;
    d2_SetObjValue4(res, 0, mode, ai_GfxResolution.word & 0xff);
    d2_SetSliderFlags(res, 0x82);
    d2_SetLabel(res, txt_Ptrs[0x1c67]);
    d2_SetList(res, 3, txt_Ptrs[0x1c73], txt_Ptrs[0x1c74], txt_Ptrs[0x1c75]);
    d2_SetObjValue4(obj1, 0, 2, ai_GfxDetail.word & 0xff);
    d2_SetSliderFlags(obj1, 0x82);
    d2_SetLabel(obj1, txt_Ptrs[0x1c68]);
    d2_SetList(obj1, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj2, 0, 2, ai_GfxDetail.byte[1]);
    d2_SetSliderFlags(obj2, 0x82);
    d2_SetLabel(obj2, txt_Ptrs[0x1c69]);
    d2_SetList(obj2, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj3, 0, 2, ai_GfxFloor.byte[2]);
    d2_SetSliderFlags(obj3, 0x82);
    d2_SetLabel(obj3, txt_Ptrs[0x1c6a]);
    d2_SetList(obj3, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj4, 0, 1, ai_GfxFloor.word & 0xff);
    d2_SetSliderFlags(obj4, 0x82);
    d2_SetLabel(obj4, txt_Ptrs[0x1c6b]);
    d2_SetList(obj4, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(obj5, 0, 1, ai_GfxDetail.byte[2]);
    d2_SetSliderFlags(obj5, 0x82);
    d2_SetLabel(obj5, txt_Ptrs[0x1c6c]);
    d2_SetList(obj5, 2, txt_Ptrs[0x1c70], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj6, 0, 2, ai_GfxDetail.byte[3]);
    d2_SetSliderFlags(obj6, 0x82);
    d2_SetLabel(obj6, txt_Ptrs[0x1c6d]);
    d2_SetList(obj6, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj7, 0x32, 100, 100 - (ai_GfxFog.word & 0xff));
    d2_SetSliderFlags(obj7, 0x882);
    d2_SetLabel(obj7, txt_Ptrs[0x1c6e]);
    d2_SetObjValue4(obj8, 0, 2, ai_GfxFloor.byte[1]);
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
        ai_GfxDetail.byte[0] = d2_GetObjValue(obj1);
        ai_GfxDetail.byte[1] = d2_GetObjValue(obj2);
        ai_GfxDetail.byte[2] = d2_GetObjValue(obj5);
        ai_GfxDetail.byte[3] = d2_GetObjValue(obj6);
        ai_GfxFloor.byte[0] = d2_GetObjValue(obj4);
        ai_GfxFloor.byte[1] = d2_GetObjValue(obj8);
        ai_GfxFloor.byte[2] = d2_GetObjValue(obj3);
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
    d2_PlaceMainWindowF(form, 2);
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
    d2_SetObjValue4(master, 0, 0x7f, ai_SoundOptions.masterVolume);
    d2_SetSliderFlags(master, 0x882);
    d2_SetLabel(master, txt_Ptrs[0x1c91]);
    d2_SetObjValue4(sfx, 0, 0x7f, ai_SoundOptions.sfxVolume);
    d2_SetSliderFlags(sfx, 0x882);
    d2_SetLabel(sfx, txt_Ptrs[0x1c92]);
    d2_SetObjValue4(msx, 0, 0x7f, ai_SoundOptions.msxVolume);
    d2_SetSliderFlags(msx, 0x882);
    d2_SetLabel(msx, txt_Ptrs[0x1c93]);
    d2_SetObjValue4(speech, 0, 0x7f, ai_SoundOptions.voiceVolume);
    d2_SetSliderFlags(speech, 0x882);
    d2_SetLabel(speech, txt_Ptrs[0x1c94]);
    d2_SetObjValue4(freq, 0, 4, ai_SoundOptions.msxFreq & 0xff);
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
                sample = (int)sb_PlaySampleI(snd_SystemBank, "NeuesUpgrade_s", 1, 0x3f, 0x7f, 0, 0);
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
   Text slots (txt_Ptrs): the labels 0x1c96 .. 0x1ca1 (0x1c9f unused), the two-state items 0x1c7c / 0x1c7d, the panel items
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
    d2_PlaceMainWindowF(form, 2);
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
    help = d2_GetWinObj(form, 1, 11);
    panel3 = d2_GetWinObj(form, 1, 12);
    d2_SetObjValue4(speed, 0, 0xa0, ai_OptionSpeed);
    d2_SetSliderFlags(speed, 0x882);
    d2_SetLabel(speed, txt_Ptrs[0x1c96]);
    d2_SetObjValue4(mouse, 0, 0x1f4, ai_OptionMouseSpeed);
    d2_SetSliderFlags(mouse, 0x882);
    d2_SetLabel(mouse, txt_Ptrs[0x1c97]);
    d2_SetObjValue4(scroll, 0, 100, ai_OptionScrollSpeed);
    d2_SetSliderFlags(scroll, 0x882);
    d2_SetLabel(scroll, txt_Ptrs[0x1c98]);
    d2_SetObjValue4(camera, 0, 100, ai_OptionCameraSpeed & 0xff);
    d2_SetSliderFlags(camera, 0x882);
    d2_SetLabel(camera, txt_Ptrs[0x1c99]);
    d2_SetObjValue4(sw0, 0, 1, ai_GameSwitches.word & 0xff);
    d2_SetSliderFlags(sw0, 0x82);
    d2_SetLabel(sw0, txt_Ptrs[0x1c9a]);
    d2_SetList(sw0, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_EnableObject(sw0, 0);
    d2_SetObjValue4(sw2, 0, 1, ai_GameSwitches.byte[2]);
    d2_SetSliderFlags(sw2, 0x82);
    d2_SetLabel(sw2, txt_Ptrs[0x1c9b]);
    d2_SetList(sw2, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(sw3, 0, 1, ai_GameSwitches.byte[3]);
    d2_SetSliderFlags(sw3, 0x82);
    d2_SetLabel(sw3, txt_Ptrs[0x1c9c]);
    d2_SetList(sw3, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(panel, 0, 4, ai_GamePanel.word & 0xff);
    d2_SetSliderFlags(panel, 0x82);
    d2_SetLabel(panel, txt_Ptrs[0x1c9d]);
    d2_SetList(panel, 5, txt_Ptrs[0x1c76], txt_Ptrs[0x1c77], txt_Ptrs[0x1c78], txt_Ptrs[0x1c79], txt_Ptrs[0x1c7a]);
    d2_SetObjValue4(panel1, 0, 1, ai_GamePanel.byte[1]);
    d2_SetSliderFlags(panel1, 0x82);
    d2_SetLabel(panel1, txt_Ptrs[0x1c9e]);
    d2_SetList(panel1, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_EnableObject(panel1, 0);
    d2_SetObjValue4(help, 0, 1, ai_OptionPanelHelp.word & 0xff);
    d2_SetSliderFlags(help, 0x82);
    d2_SetLabel(help, txt_Ptrs[0x1ca1]);
    d2_SetList(help, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(panel3, 0, 1, ai_GamePanel.byte[3]);
    d2_SetSliderFlags(panel3, 0x82);
    d2_SetLabel(panel3, txt_Ptrs[0x1ca0]);
    d2_SetList(panel3, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
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
        ai_OptionCameraSpeedByte = d2_GetObjValue(camera);
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
    bar = d2_CreateWindow(100, 0, 600, d2_ScreenH, 0x10);
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
void cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60, cm.c: message 0x19 (the field block) */
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
   view. Void.
   @deep meaning matches (read against build/ghidra/5d7f10.c and the disassembly 0x5d7f10..0x5d8098: the nine checkbox rows, the mask get/set, both loops and the close path are the same statements); readable done (Q1-Q7). The 77 code hunks are VC6's induction-variable strength reduction - ours turns 'i * 0x1c + 10' and 'i + 0x605' into incremented IVs (mov ebp,0x605 / mov [esp+0x10],0xa / add esi,0x1c / add ebx,4) where the original recomputes the lea chain per iteration (lea ecx,[ebp*8]; sub ecx,ebp; lea edx,[ecx*4+0xa]) - plus the stack frame (ours 0x2c vs the original 0x44: the original spills wnd at [esp+0x10] and re-reads it before d2_CloseView, ours keeps it in a register; rows[i] at [esp+0x14]). tools/spell.py: no menu rule on the code hunks; the three reload leads read as that same slot allocation (keep the value in a local / drop the local re-read) - register/slot/IV allocation is match-deep's toolbox */
void ai_MessageFilterScreen(void)
{
    int wnd, pump, i, id;
    int rows[9];

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
        d2_SetObjValue(id, he_IsMessageKindFiltered(i + 0x605) == 0); /* @original: this call site
            pushes 2 arguments against the 5-parameter K&R definition (0x5d7fe5) */
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

    lager = schn = trans = personal = meister = roh = plausch = 0;
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
                ai_ProductionWindow((unsigned char *)building);
            } else if (click == trans) {
                ai_OpenTransport(building);
            } else if (click == personal) {
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            } else if (click == meister) {
                ai_MasterLetterWindow((unsigned char *)building);
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
   (sv_CheckMoney): the avenger's text 0x41bf, the relation of every target to the avenger drops by 50 - the mix of the two
   (cm_SendRelations), the price is paid (gm_SendMoneyDelta), the power bit 8 of +0x1cc is set (cm_SendSetField) and the
   delivery text for the chosen targets is made (ai_GypsyDeliveryText) and shown (0x41b9 with 0x41b2); the second button and
   the right mouse button leave. The pump ends with d2_CloseView and the camera follow. Returns the result of the green box
   when the window is refused. @deep body reconstructed from the disassembly (the pentagon layout, the five-row refresh with the amount-row callees, the 0x8c ms record check, the confirm branch with relations / money / power bit / delivery text, the row click building the 0x14-dword amount menu config, the pump and the tail call to the camera follow); 492 vs 496 instructions, meaning read whole, locals in the original slot order (msg, rows, cfg, sel, ids, tick, refresh, form, total). What is left is register allocation and constant hoisting: the original keeps the constants 1 / 0 in ebx / esi for the whole body (mov ebx,1 / xor esi,esi, the loop is entered past the xor esi,esi, sel[] = -1 stored through eax, form in edi with a spill at [esp+0x10], the row count in ebp, refresh at [esp+0x14]) while ours hoists -1 into ebp and keeps form in ebx, so every later store / compare differs (asmdiff: 250 instructions in 4 big code hunks, 14 order, 4 slots). Also left: the filter constant cfg[2..3] = 0xa7fdfbb6 / 1 is loaded by the original as an 8-byte copy from .rdata 0x5eed68 (mov eax,[0x5eed68]; mov ecx,[0x5eed6c]) - ours stores two immediates (tried *(__int64 *)&cfg[2] = 0x1a7fdfbb6: same code, Q4 worse, reverted). Tried: local declaration orders (slots now right), -(0x32 - x) via a temp so the neg stays (fixed that hunk), 0x5da picture index. */
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

    memset(rows, 0, sizeof(rows));
    sel[0] = -1;
    sel[1] = -1;
    sel[2] = -1;
    sel[3] = -1;
    sel[4] = -1;
    refresh = 1;
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
    rows[0].y = 0x1e;
    rows[0].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 2;
    rows[1].y = 0x7d;
    rows[1].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 3 - 10;
    rows[2].y = 0x7d;
    rows[2].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) * 2 / 3 + 10;
    rows[3].y = 0xdc;
    rows[3].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) / 3 - 100;
    rows[4].y = 0xdc;
    rows[4].x = (d2_Wins[d2_LastWindow].w - d2_Pics[0x5da].w) * 2 / 3 + 100;
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
        } else if (d2_ClickedId == -1) {
            if (d3_Time > (unsigned)(tick + 0x8c)) {
                for (i = 0; i < 5; i++) {
                    if (rows[i].rec != 0 && (rows[i].rec->id == 0xffff || rows[i].rec->netId != (unsigned)sel[i])) {
                        rows[i].rec = 0;
                        sel[i] = -1;
                    }
                }
                refresh = 1;
            }
        } else if (d2_PointerObj == ids[0]) {
            if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, total) == 1) {
                txt_Format(msg, 0x41bf, rows[0].rec->id);
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
                ai_GypsyDeliveryText((struct AIPER *)rows[0].rec, k, (struct AIPER **)&sel[5]);
                txt_Format(msg, 0x41b9, 0x41b2);
                sv_MessageBox((int)msg, 0);
                sv_PopRequest = 1;
            }
        } else if (d2_PointerObj == ids[1]) {
            sv_PopRequest = 1;
        } else {
            for (i = 0; i < 5; i++) {
                if (d2_PointerObj == rows[i].ctl)
                    break;
            }
            if (i < 5) {
                memset(&cfg, 0, sizeof(cfg));
                idp = cfg.ids;
                for (k = 0; k < 5; k++) {
                    if (k != i) {
                        *idp = rows[k].rec != 0 ? rows[k].rec->id : 0xffff;
                        idp++;
                    }
                }
                cfg.filter = gypsyFilter;
                cfg.flags = 1;
                cfg.mode = 0x400;
                cfg.kind = 6;
                cfg.option14 = 2;
                rows[i].rec = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x41b4 + (i != 0)], 0, 0);
                if (rows[i].rec != 0)
                    sel[i] = rows[i].rec->netId;
                else
                    sel[i] = -1;
                refresh = 1;
            }
        }
    } while (sv_RunFrame((int)ai_BloodRevengeWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}
/* batch 3: the gypsy / poker / particle stretch 0x5d8c90 .. 0x5dd3e0 */
void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_CheckGoodAmount();
struct GCLOCK;
int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int ai_PokerStart();
unsigned short ai_PokerStep(); int gm_BuildingSkill(); float ai_NormalizeEffectScale(float x);

/* 0x5d8c90, the "Fluch wirken" (curse) window "Locations\zigeuner\blutrache" 0x62dc68 (905 B): the building is the
   gypsy house. When the player's status bit 0x10 (+0x1cc of its record) is set the voice "RAUSWURF" is played and the green
   message box 0x41c1 / 0x41c2 refuses. Otherwise the target is chosen with the special-amount menu ai_AmountMenu (the same
   config as the blood revenge window: +0 = 1, +4 = 0x400, +8 / +0xc the filter 0x1a7fdfbb6, +0x10 = 6, the id list from +0x1c all
   0xffff); no target ends the window. The form shows a slider (0 .. 5 at first, then 0 .. the limit gm_CurseObjectCount - 1) with
   the six strength names 0x41ca .. 0x41cf and the two buttons 0x41d4 / 0x41d5. Each pass the strength v is clamped to the limit,
   the texts 0x41d0 + v (with the target's id), 0x41d6 / 0x41c1 / 0x41c4 + v and, for a building of another owner, the price
   max (v, 1) * 32000 (0x41d7) are shown. The right mouse button and the second button leave; the first button curses when the
   player has the goods (sv_CheckGoodAmount): the status bit 0x10 is set (cm_SendSetField), another owner is paid the price
   (cm_SendMoveGoodsUnchecked), the message 0x9d (arg 0 = the strength, arg 2 = the target's net id, the game time, flags 2) is sent
   (cm_SendLongMessage), the voice "_WAHRSAGERIN_FLUCH_0<v>" is played and the text 0x41da is shown. The pump ends with
   d2_CloseView and the camera follow. @deep body reconstructed and read whole against the disassembly: 316 = 316 instructions, 1103 = 1103 bytes, the amount-menu config (AMOUNTCFG, the 64-bit filter loaded from a const volatile object like the original's .rdata pair), form / slider / buttons, the strength loop, the confirm branch with the 0xf8-byte message and the tail jumps all match. ONE hunk of 4 differing instructions is left at 0x5d9026: the original stores the last word of the game time copy (mov [esp+0xa8],ax) after the push of the message pointer (mov ecx,[ebp+4]; lea edx,[esp+0x70]; push edx; mov [esp+0xa8],ax; mov [esp+0xd0],ecx) while ours stores it before (mov [esp+0xa4],ax; ...; push edx), a pure scheduling difference of one store across the push. Tried the order of msg.to / the time copy / msg.arg[2] (three orders tried: this one is the best, the others 16 and 49 instructions), memcpy vs a struct copy (the struct copy needs the Q4 cast of the char ai_Time[]). */
void ai_CurseWindow(int building)
{
    char text[0x200];
    char name[100];
    CURSEMSG msg;
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
    d2_SetObjValue5(slider, 0, 5, 0, 1);
    d2_SetList(slider, 6, txt_Ptrs[0x41ca], txt_Ptrs[0x41cb], txt_Ptrs[0x41cc], txt_Ptrs[0x41cd], txt_Ptrs[0x41ce],
               txt_Ptrs[0x41cf]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41d4], txt_Ptrs[0x41d5]);
    do {
        limit = gm_CurseObjectCount((struct AIBLD *)building) - 1;
        v = d2_GetObjValue(slider);
        if (v >= limit)
            v = limit;
        d2_SetObjValue5(slider, 0, limit, v, 1);
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
                if (sv_CheckGoodAmountB(ai_CharsC + ai_PlayerId * 0x21c, v, ai_OwnPlayer) != 0) {
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
                    memcpy(&msg.time, ai_Time, sizeof(GAMECLOCK));
                    msg.arg[2] = target->netId;
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
   flags 2) with cm_SendLongMessage. The pump ends with d2_CloseView and the camera follow. @deep body reconstructed and read whole against the disassembly: 363 vs 365 instructions, the clock (GAMECLOCK, memcpy of ai_Time + gm_AddTime, hour 21 + random, minute 0), the two sliders, the 0x8c ms / value-change rebuild with the running-curse (ai_FindCurseEvent) and the new-curse branches in the original order, the buttons, the pay + 0xa0 message and the tail jump all match. The diff has 7 slot hunks (a 4 byte shift of the ids / tick locals: ours keeps a float local for the share ratio, the original has none) and ONE code hunk of 28 instructions at 0x5d92c7, the share ratio: the original stores the first gm_MinutesBetween result as an int ([esp+0x44]) and later does fild first; fidiv second (the second result also in memory), then fcom 1.0 / fnstsw / test ah,5 / jnp; ours converts the first result at once (fild; fstp float temp before the second call), then fild second; fdivr temp. Tried: one expression (float)f() / g() (this), named int locals total / elapsed (same early fild), the clamp as a ternary with > and >= (changes the compare to and eax,0x4100 / 0x100 and does not remove the early conversion), swapping the if / else order (fixed three other hunks). Allocation / scheduling of the FPU expression only. */

void ai_CurseTargetWindow(int building)
{
    CURSEMSG msg;
    GAMECLOCK when;
    int ids[2];                         /* the two buttons */
    int share, victim, form;
    int value, cls;
    unsigned tick;
    CURSEEVENT *ev;
    float ratio;

    tick = 0;
    cls = 2;
    value = 0;
    memcpy(&when, ai_Time, sizeof(GAMECLOCK));
    gm_AddTime((struct GCLOCK *)&when, 0x18, 0, 0);
    when.hour = util_RandomU(cls) + 0x15;
    when.min = 0;
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    share = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2, 10, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    d2_SetObjValue4(share, 0, 100, 0);
    d2_SetForm(form, 2);
    victim = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    gm_CurseTargetCount((char *)building);
    d2_SetObjValue5(victim, 0, gm_CurseTargetCount((char *)building) - 1, 0, 1);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41f6], txt_Ptrs[0x41f7]);
    do {
        if (d3_Time > tick + 0x8c || d2_GetObjValue(victim) != value) {
            tick = d3_Time;
            ev = (CURSEEVENT *)ai_FindCurseEvent((struct AIBLD *)building);
            if (ev != 0) {
                when = ev->due;
                when.hour += ev->done ? 0 : 2;
                ratio = (float)gm_MinutesBetween((struct AITIME *)&ev->from, (struct AITIME *)ai_Time)
                        / gm_MinutesBetween((struct AITIME *)&ev->from, (struct AITIME *)&when);
                if (ratio > 1.0f)
                    ratio = 1.0f;
                d2_SetObjValue4(share, 0, 100, (int)(ratio * 100.0f));
            } else {
                memcpy(&when, ai_Time, sizeof(GAMECLOCK));
                gm_AddTime((struct GCLOCK *)&when, 0x18, 0, 0);
                when.hour = util_RandomU(2) + 0x15;
                when.min = 0;
                d2_SetObjValue4(share, 0, 100, 0);
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
                    memcpy(&msg.time, ai_Time, sizeof(GAMECLOCK));
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
   with d2_CloseView and the camera follow. @deep body reconstructed and read whole against the disassembly: 223 = 223 instructions, 745 = 745 bytes; the owner / status guards, the amount-menu config (AMOUNTCFG with the const volatile 64-bit filter), the form with the 2 .. 8 gift slider and the buttons, the per-pass text, the confirm branch (status bit 0x20, money, relation v * 5, the news for the player characters through he_ShowNews) and the tail jumps all match. ONE hunk of 6 differing instructions is left at 0x5d9604: the original loads the hi dword of the filter (mov edx,[0x5eed6c]) before the first rep stosd and pushes the hoisted 0 (the amount menu's last arguments) after the second lea edi, ours pushes the 0 before the first rep stosd and loads the hi dword after the second; cfg is then 4 bytes lower ([esp+0x30] vs [esp+0x2c]). Scheduling of constant loads / pushes around the two inline memsets. Tried: the filter assigned first (8 instructions), the id clearing as a dword loop instead of memset (same code). */

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
    d2_SetObjValue5(slider, 2, 8, 2, 1);
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
                    memcpy(&msg.time, ai_Time, sizeof(GAMECLOCK));
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
                    ai_GypsyHelperWindow((char *)building);
                else if (ai_ClickedObject == transport)
                    ai_OpenTransport(building);
                else if (ai_ClickedObject == produktion)
                    ai_ProductionWindow((unsigned char *)building);
                else if (ai_ClickedObject == lager)
                    ai_StorageWindow(building, 0);
                else if (ai_ClickedObject == meister)
                    ai_MasterLetterWindow((unsigned char *)building);
                else if (ai_ClickedObject == betteln)
                    ai_GypsyBeggingWindow(building);
            }
            if (ai_ClickedObject == personal)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
        }
        ai_TradeContactChecks();
        pump = sv_RunFrame(ai_GypsyProductionContact, 0x67fff);
    }
}
/* 0x5d9de0, opens the poker table window "Locations\zigeuner\poker" 0x62de10 (273 B): returns 0 when a window is open
   already (ai_PokerUi.window != -1) or the form cannot be opened, else 1. The pointer sprite is cleared, the view
   centred, and the panels are initialised from the windows the forms 1, 2 and 3 made (each takes d2_LastWindow and clears its
   five slots to -1; the form 3 panel, first in memory, is marked shown); the form 6 row of five buttons (the texts 0x144aae8 ..
   0x144aaf8, ids into ai_PokerUi.buttons) gets its third button disabled. The poker family 0x5d9de0 ..
   0x5db110 (the data block 0x62dda0 .. 0x62de10 is one POKERUI); betSlider (0x62ddb8) starts at -1. @deep Body read whole against the disassembly (open guard, form, three panels from the windows of the forms 1 / 2 / 3, button row of five on the form 6, third button disabled; data block 0x62dda0 reconstructed as POKERUI). 91 vs 93 instructions, no meaning difference left: the original loads the -1 once more into eax (mov eax,edi) and stores it through eax for the slots of the first two panels (and the shown flag 0 / 1 interleaved: slot0, slot1, win, slot2, shown, slot3, slot4) while ours stores through the hoisted -1 register directly; the compare constant sits in edi in the original (esi in ours); the five button texts are loaded in a different order for the push. asmdiff: hunks at 0x5d9e37 (80 ins) and 0x5d9f19 (30 ins) are pure scheduling / allocation of the constant stores. Tried: chained assignment of the slots, a named local int none = -1 for the compare and stores, natural per-slot order. */

int ai_OpenPokerWindow(void)
{
    if (ai_PokerUi.window != -1)
        return 0;
    ai_PokerUi.window = d2_OpenForm(0, 0, "Locations\\zigeuner\\poker");
    if (ai_PokerUi.window == -1)
        return 0;
    ai_PokerUi.betSlider = -1;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(ai_PokerUi.window);
    d2_SetForm(ai_PokerUi.window, 1);
    ai_PokerUi.panel[1].win = d2_LastWindow;
    ai_PokerUi.panel[1].slot[0] = -1;
    ai_PokerUi.panel[1].slot[1] = -1;
    ai_PokerUi.panel[1].slot[2] = -1;
    ai_PokerUi.panel[1].slot[3] = -1;
    ai_PokerUi.panel[1].slot[4] = -1;
    ai_PokerUi.panel[1].shown = 0;
    d2_SetForm(ai_PokerUi.window, 2);
    ai_PokerUi.panel[2].win = d2_LastWindow;
    ai_PokerUi.panel[2].slot[0] = -1;
    ai_PokerUi.panel[2].slot[1] = -1;
    ai_PokerUi.panel[2].slot[2] = -1;
    ai_PokerUi.panel[2].slot[3] = -1;
    ai_PokerUi.panel[2].slot[4] = -1;
    ai_PokerUi.panel[2].shown = 0;
    d2_SetForm(ai_PokerUi.window, 3);
    ai_PokerUi.panel[0].win = d2_LastWindow;
    ai_PokerUi.panel[0].slot[0] = -1;
    ai_PokerUi.panel[0].slot[1] = -1;
    ai_PokerUi.panel[0].slot[2] = -1;
    ai_PokerUi.panel[0].slot[3] = -1;
    ai_PokerUi.panel[0].slot[4] = -1;
    ai_PokerUi.panel[0].shown = 1;
    d2_SetForm(ai_PokerUi.window, 6);
    sv_ButtonRow(d2_LastWindow, 0, ai_PokerUi.buttons, 5, txt_Ptrs[0x424a], txt_Ptrs[0x424b], txt_Ptrs[0x424c], txt_Ptrs[0x424d], txt_Ptrs[0x424e]);
    d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
    return 1;
}
/* 0x5d9f50, shows the poker bet slider (159 B): arg on, nothing happens for 0. The slider is made once, on the first
   call, in the form 5 of the poker window (ai_PokerUi.window): d2_WinOpenSlider, as wide as the window less 0xb4, 0x7b
   picture, flags 2, scale 0 .. 100) and stored in ai_PokerUi.betSlider; its range is the bet limits converted to the
   player's money (gm_ConvertMoney of ai_PokerState.minBet, ai_PokerState.maxBet and ai_PokerState.minBet again as the start value, step 1).
   Later calls enable / disable the slider (d2_EnableObject). The limits are the poker game's POKERGAME.minBet / maxBet (0x6f35a4 / 0x6f35a8). */

void ai_PokerBetControl(int on)
{
    int slider;

    if (on != 0) {
        if (ai_PokerUi.betSlider == -1) {
            d2_SetForm(ai_PokerUi.window, 5);
            slider = d2_WinOpenSlider(0, 0, d2_Wins[d2_LastWindow].w - 0xb4, 0, 100, 0x7b, 2, d2_LastWindow);
            ai_PokerUi.betSlider = slider;
            d2_SetObjValue5(ai_PokerUi.betSlider, gm_ConvertMoney(ai_PokerState.minBet, ai_OwnPlayer), gm_ConvertMoney(ai_PokerState.maxBet, ai_OwnPlayer), gm_ConvertMoney(ai_PokerState.minBet, ai_OwnPlayer), 1);
        } else {
            d2_EnableObject(ai_PokerUi.betSlider, on);
        }
    }
}
/* 0x5da010, closes the poker window (24 B): closes the window's view (ai_PokerUi) and clears
   the handle (-1). */

void ai_ClosePokerWindow(void)
{
    d2_CloseView(ai_PokerUi.window);
    ai_PokerUi.window = -1;
}

typedef struct PLAYERRECVIEW {          /* f3gm.h's player record (gm_PlayerRecord, 0x56 words): only the float the poker game reads */
    char pad00[0x60];
    float luck;                         /* +0x60, the chance that the player sits down to play (decays by * 0.66 - 0.1 per game; reasoned) */
} PLAYERRECVIEW;

/* 0x5da030, the poker game (the card game against two opponents) of the gypsy poker table (2752 B). The player's record
   (gm_PlayerRecord) decides whether he plays: the record's float at +0x60 must exceed a random number, it must be past
   11 o'clock, and the player's worth (gm_GetWorth, converted to the player's money) at least 10000. Then the float is
   decayed (* 0.66 - 0.1), the key state 0x1e is cleared, the poker window (ai_OpenPokerWindow) opened and the game started
   (ai_PokerStart fills ai_PokerState; 0 closes the window and ends). Otherwise the form "Misc\Messagebox" shows the text
   0x4239 (with 0x4238) until the right button or the key 1 pops it. Returns 1 when a game was played, else 0.
   The pump runs sv_RunFrame (this function, 0x777c7); every frame the buttons follow the mask the game step
   (ai_PokerStep) returns: bit 1 the first button (ai_PokerUi.buttons[0]), bit 8 the second, bit 0x10 the fourth; bits 1 and
   0x20 show the bet slider (ai_PokerBetControl). The second button asks (text 0x4263, message box 0x101) and leaves, the
   fifth opens the rules window ai_PokerWindow2 with the poker view disabled. Then ai_PokerState.phase (1 .. 11) selects the
   step: the seats are redrawn with ai_PokerSeatPanel (mode 0 .. 5) when the phase changes, the pot text 0x4252 and a random
   remark 0x4253 .. 0x4255 (with the hand text 0x4256 / 0x4257 / 0x4258) are printed, the first button takes the bet from the
   slider (gm_PlayerFactor of its value) and steps the game; phase 6 lets the player mark one of the first two cards to
   exchange (the objects' checked flag, ai_Unk4a8630), phase 7 shows the two opponents' texts (0x425c when the seat put a card away in the exchange, else 0x425d: seat dropped),
   phase 10 shows the result (the winner's names and the hand text 0x4266 + the rank), phase 11 waits for the fourth button.
   Reasoned: the button roles and the phase meanings follow from the texts and the mask bits; the text ids are not read
   from the text files. @deep Body reconstructed whole from the disassembly (prelude with the record / worth / chance gate, the Misc\Messagebox refusal loop, the pump with the mask-driven buttons and the 11-phase switch, ai_PokerState as a POKERGAME at 0x6f3500 with 25 of 991 instructions differing). After the build asmdiff has 2 hunks left: (1) regs 2 at 0x5da290, the byte-copy store of the texts [edx+eax] vs the original's [eax+edx] (s[d] / d[s] compile the same); (2) code 23 at 0x5dadbd, which is not code: the 11-entry jump table of the switch plus its 3-byte alignment fill (lea ecx,[ecx]) that our object keeps inside the function symbol (size 3515 vs 3469) while the original's walker ends the function at 0x5dadbc and files the fill and the table at 0x5dadc0 apart (dump: func@0x5da030..0x5dadbd fill@0x5dadbd jumptable@0x5dadc0); B1 compares 1 of 3520 bytes (the sib byte). Tried: print inside / after the branches (fixed two hunks), branch polarity of the opponent texts (fixed), s[d] vs d[s]. Nothing in the C is left to change: the table range needs a tool decision (exclude our trailing jump table from the code hunks when the original's range stops before its own fill). */
int ai_PokerGame(void)
{
    char msg[0x100];
    char text[0x100];
    int lastPhase;
    unsigned short mask;
    PLAYERRECVIEW *rec;
    int worth, form;
    char *s, c;
    int d;

    lastPhase = -1;
    rec = (PLAYERRECVIEW *)gm_PlayerRecord(ai_CharsC + ai_PlayerId * 0x21c);
    if (rec == 0)
        return 0;
    worth = gm_ConvertMoney(gm_GetWorth(ai_PlayerId), ai_OwnPlayer);
    if (util_RandomF() < rec->luck && ai_TimeHour > 11 && worth >= 10000) {
        rec->luck = rec->luck * 0.66f - 0.1f;
        kb_Down[0x1e] = 0;
        if (ai_OpenPokerWindow() == 0)
            return 0;
        if (ai_PokerStart((struct AIPER *)(ai_CharsC + ai_PlayerId * 0x21c), (struct G10GAME *)&ai_PokerState) == 0) {
            ai_ClosePokerWindow();
            return 0;
        }
        mask = ai_PokerStep();
        d2_ClickedId = -1;
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            d2_SetObjEnabled(ai_PokerUi.buttons[0], mask & 1);
            d2_SetObjEnabled(ai_PokerUi.buttons[1], mask & 8);
            d2_SetObjEnabled(ai_PokerUi.buttons[3], mask & 0x10);
            ai_PokerBetControl((mask & 1) != 0 && (mask & 0x20) != 0);
            if (d2_ClickedId != -1) {
                if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                    txt_Format(msg, 0x4263);
                    if (sv_MessageBox((int)msg, 0x101) != 0)
                        sv_PopRequest = 1;
                }
                if (ai_PokerUi.buttons[4] == d2_PointerObj) {
                    d2_SetViewEnabled(ai_PokerUi.window, 0);
                    ai_PokerWindow2();
                    d2_SetViewEnabled(ai_PokerUi.window, 1);
                }
            }
            switch (ai_PokerState.phase) {
            case 1:
                if (lastPhase != 1) {
                    lastPhase = 1;
                    ai_PokerSeatPanel(1, 0, 0);
                    ai_PokerSeatPanel(2, 0, 0);
                    ai_PokerSeatPanel(0, 0, 0);
                }
                d2_SetForm(ai_PokerUi.window, 4);
                if (ai_PokerState.actor == 0) {
                    s = txt_Ptrs[0x4250];
                    d = text - s;
                    do {
                        c = *s;
                        s[d] = c;
                        s++;
                    } while (c != 0);
                    io_PrintText(text);
                    if (d2_ClickedId != -1 && ai_PokerUi.buttons[0] == d2_PointerObj) {
                        ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                    }
                } else {
                    sprintf(text, txt_Ptrs[0x424f], ai_PokerState.seat[ai_PokerState.actor].who->vname, ai_PokerState.seat[ai_PokerState.actor].who->nname);
                    io_PrintText(text);
                    txt_Format(text, 0x4251, ai_PokerState.minBet);
                    io_PrintText(text);
                    if (d2_ClickedId != -1 && ai_PokerUi.buttons[3] == d2_PointerObj)
                        mask = ai_PokerStep();
                }
                break;
            case 2:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_RandomU(3) + 0x4253, 0x4256);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                ai_PokerSeatPanel(1, 3, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 3, ai_PokerState.seat[2].card);
                ai_PokerSeatPanel(0, 3, ai_PokerState.seat[0].card);
                mask = ai_PokerStep();
                break;
            case 3:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 4:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_RandomU(3) + 0x4253, 0x4257);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                if (lastPhase != 4) {
                    lastPhase = 4;
                    ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                    ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                }
                mask = ai_PokerStep();
                break;
            case 5:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 6:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                io_PrintText(txt_Ptrs[0x425b]);
                d2_Objs[ai_PokerUi.panel[0].slot[0]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[2]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[3]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].interactive = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].style = 3;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].interactive = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].style = 3;
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.panel[0].slot[0] == d2_PointerObj) {
                        d2_Objs[ai_PokerUi.panel[0].slot[0]].checked = d2_Objs[ai_PokerUi.panel[0].slot[0]].checked == 0;
                        d2_Objs[ai_PokerUi.panel[0].slot[1]].checked = 0;
                        if (d2_Objs[ai_PokerUi.panel[0].slot[0]].checked != 0)
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 1);
                        else
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                    } else if (ai_PokerUi.panel[0].slot[1] == d2_PointerObj) {
                        d2_Objs[ai_PokerUi.panel[0].slot[1]].checked = d2_Objs[ai_PokerUi.panel[0].slot[1]].checked == 0;
                        d2_Objs[ai_PokerUi.panel[0].slot[0]].checked = 0;
                        if (d2_Objs[ai_PokerUi.panel[0].slot[1]].checked != 0)
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 1);
                        else
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                    } else if (ai_PokerUi.buttons[2] == d2_PointerObj) {
                        d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                        if (d2_Objs[ai_PokerUi.panel[0].slot[0]].checked != 0)
                            ai_PokerExchange(0);
                        else if (d2_Objs[ai_PokerUi.panel[0].slot[1]].checked != 0)
                            ai_PokerExchange(1);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[3] == d2_PointerObj) {
                        d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 7:
                d2_Objs[ai_PokerUi.panel[0].slot[0]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[2]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[3]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].interactive = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].style = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].interactive = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].style = 0;
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                if (ai_PokerState.seat[1].dropped != -1) {
                    sprintf(text, txt_Ptrs[0x425c], ai_PokerState.seat[1].who->vname, ai_PokerState.seat[1].who->nname);
                    io_PrintText(text);
                } else {
                    sprintf(text, txt_Ptrs[0x425d], ai_PokerState.seat[1].who->vname, ai_PokerState.seat[1].who->nname);
                    io_PrintText(text);
                }
                if (ai_PokerState.seat[2].dropped != -1) {
                    sprintf(text, txt_Ptrs[0x425c], ai_PokerState.seat[2].who->vname, ai_PokerState.seat[2].who->nname);
                    io_PrintText(text);
                } else {
                    sprintf(text, txt_Ptrs[0x425d], ai_PokerState.seat[2].who->vname, ai_PokerState.seat[2].who->nname);
                    io_PrintText(text);
                }
                if (d2_ClickedId != -1 && ai_PokerUi.buttons[3] == d2_PointerObj)
                    mask = ai_PokerStep();
                break;
            case 8:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_RandomU(3) + 0x4253, 0x4258);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                mask = ai_PokerStep();
                break;
            case 9:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 10:
                ai_PokerStep();
                ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                d2_SetForm(ai_PokerUi.window, 4);
                if (ai_PokerState.winner == 0)
                    txt_Format(text, 0x4265, ai_PokerState.winRank + 0x4266, ai_PokerState.pot);
                else
                    txt_Format(text, 0x4264, ai_PokerState.seat[(unsigned short)ai_PokerState.winner].who->vname, ai_PokerState.seat[(unsigned short)ai_PokerState.winner].who->nname, ai_PokerState.winRank + 0x4266, ai_PokerState.pot);
                io_PrintText(text);
                break;
            case 11:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[3] == d2_PointerObj)
                        sv_PopRequest = 1;
                }
                break;
            }
        } while (sv_RunFrame((int)ai_PokerGame, 0x777c7) != 0);
        ai_ClosePokerWindow();
        return 1;
    } else {
        form = d2_OpenForm(0, 0, "Misc\\Messagebox");
        if (form == 0)
            return 0;
        d2_CentreViewWindows(ai_PokerUi.window);
        txt_Format(text, 0x4239, 0x4238);
        io_PrintText(text);
        while (sv_RunFrame((int)ai_PokerGame, 0x777c7) != 0) {
            if (dm_L.rup != 0 || kb_Down[1] != 0)
                sv_PopRequest = 1;
        }
        d2_CloseView(form);
        return 0;
    }
}
/* 0x5dadf0, builds the panel of one poker seat (445 B): seat 0 is the player, 1 and 2 the opponents; n the number of
   cards to show, cards the seat's card array (ai_PokerState.seat[seat].card, indices into the deck). The panel's window
   (ai_PokerUi.panel[seat].win) becomes the current one and its text is cleared; the seat's title and names are printed
   with "%s %s %s" (the title text txt_Ptrs[0x114 + 10 * female + title], the first and the family name) into a centred
   label (font 0x54) at the window's top; the character's picture (the player's own, or the seat's model) is placed left
   of the cards. Then each of the n cards becomes a picture object 0x20 apart, stored in the panel's slot i: the card
   back (picture 0x775 + ai_CardBack) when the seat has folded (foldPhase >= 0), or while an opponent's first two
   cards are shown at the end of the game (winner 4); else the card's face (0x775 + the card index), and the player's
   first two cards are greyed (they are chosen for the exchange in phase 6). Void. Reasoned: the picture and text
   roles follow from the offsets (0x775 + index into d2_Pics, the label with the centred flag). @deep Body reconstructed whole from the disassembly (label, centred title text, portrait, then one picture per card: face, card back for a folded seat or an opponent's first two cards at the end, the player's first two greyed); 231 vs 230 instructions, no meaning difference left. asmdiff (49 instructions, hunks at 0x5dae76 / 0x5daf9e / 0x5db00a / 0x5db0d7): (1) the original reads the card-back constant 0x5ed1bc once per picture (mov ecx,[0x5ed1bc]; lea edx,[ecx+0x775] and the picture height index from the same ecx) while ours, with ai_CardBack as const volatile int (the original does not fold it), reads it twice in each of the two back blocks; a local copy (pic = ai_CardBack + 0x775) makes the compiler merge the two identical back blocks (size 704, the original keeps two copies), so the original's two blocks must differ in a way not visible here; (2) the title index scheduling (push edx before sbb ecx,ecx; and ecx,0xa / add ecx,0x114 order); (3) 6 order instructions in the loop increment (inc eax / add edx,ecx vs add edi,0x20). Tried: win / mid locals (the call in both seat branches without a window local fixed 130 instructions), branch order (seat != 0 first), pic local. */

void ai_PokerSeatPanel(int seat, int n, int *cards)
{
    char text[0x100];
    POKERWHO *who;
    int o, pic, x, w, h, i;

    d2_LastWindow = ai_PokerUi.panel[seat].win;
    io_PrintText("$C");
    w = d2_Wins[d2_LastWindow].w;
    h = d2_Wins[d2_LastWindow].h;
    x = (w - 0xc4) / 2;
    who = ai_PokerState.seat[seat].who;
    sprintf(text, "%s %s %s", txt_Ptrs[0x114 + (who->female != 0 ? 10 : 0) + who->title], who->vname, who->nname);
    o = d2_CreateWinText(0, 5, text, ai_PokerUi.panel[seat].win);
    d2_Objs[o].w = w;
    d2_Objs[o].centred = 1;
    d2_SetObjFont(o, 0x54);
    if (seat != 0) {
        pic = ai_PokerState.seat[seat].model;
        d2_WinOpenPic(x - 0x37, h / 2 - d2_Pics[pic].h / 2, pic, ai_PokerUi.panel[seat].win);
    } else {
        pic = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->pic;
        d2_WinOpenPic(x - 0x37, h / 2 - d2_Pics[pic].h / 2, pic, ai_PokerUi.panel[0].win);
    }
    for (i = 0; i < n; i++) {
        if (ai_PokerState.seat[seat].foldPhase >= 0) {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_Pics[ai_CardBack + 0x775].h / 2, ai_CardBack + 0x775, ai_PokerUi.panel[seat].win);
        } else if (seat != 0 && i < 2 && ai_PokerState.winner == 4) {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_Pics[ai_CardBack + 0x775].h / 2, ai_CardBack + 0x775, ai_PokerUi.panel[seat].win);
        } else {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_Pics[cards[i] + 0x775].h / 2, cards[i] + 0x775, ai_PokerUi.panel[seat].win);
            if (seat == 0 && i < 2)
                d2_Objs[ai_PokerUi.panel[0].slot[i]].greyed = 1;
        }
        x += 0x20;
    }
}
/* 0x5db110, the second poker window "Locations\\zigeuner\\poker2" (the poker hands table, opened by the fifth button of the poker
   table 0x5da030 with its view disabled; 2806 B of straight-line picture rows). Opens the form (-1: nothing), clears
   the pointer sprite, centres the view and shows the title text 0x423e; the form 3 gets a button row of one (the text
   0x4249) whose id ends the window; the form 4 gets a slider (sv_CreateFormSlider, 0x35) in the form 2 window. In the form 2 the
   texts 0x4247 / 0x4248 are printed and the table of the eight poker hands (the texts 0x423f .. 0x4246, the hand names
   "Paar" .. "Royal Flush") is built straight-line: row i at y = 0xa0 + 0x6d * i has five card pictures 0x20 apart (x = 5,
   0x25, 0x45, 0x65, 0x85; the pictures 0x775 + n: the ones that make the hand are highlighted, the others greyed) and the
   hand's name as a centred text label at x = 0xcb, 0x29 below the row (font 0x54, as wide as the window less 0xd2). The
   window's content height is raised by 8, then the pump (this function, 0x777c7) waits for the right button or the button
   and closes the view. Void. Reasoned: the rows are read off the pictures and texts (the hand rows follow the text order
   0x423f .. 0x4246); which picture number is which card is not decoded. */

void ai_PokerWindow2(void)
{
    int ids[1];
    int a, b, c;
    int form, win, o;
    int w;

    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\poker2");
    if (form == -1)
        return;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 1);
    io_PrintText((char *)0x423e);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 1, txt_Ptrs[0x4249]);
    d2_SetForm(form, 2);
    win = d2_LastWindow;
    w = d2_Wins[d2_LastWindow].w - 0xd2;
    d2_SetForm(form, 4);
    sv_CreateFormSlider(0, 0, d2_LastWindow, win, 0x35, &c, &b, &a);
    d2_SetForm(form, 2);
    io_PrintText((char *)0x4247);
    io_PrintText((char *)0x4248);
    o = d2_WinOpenPic(0x5, 0xa0, 0x779, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0xa0, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0xa0, 0x77f, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x65, 0xa0, 0x784, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x85, 0xa0, 0x78a, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0xc9, txt_Ptrs[0x423f], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x10d, 0x779, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x10d, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x10d, 0x785, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x10d, 0x786, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x10d, 0x78a, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x136, txt_Ptrs[0x4240], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x17a, 0x781, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x17a, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x17a, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x17a, 0x777, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x85, 0x17a, 0x77b, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x1a3, txt_Ptrs[0x4241], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x1e7, 0x775, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x1e7, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x1e7, 0x77f, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x1e7, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x1e7, 0x786, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x210, txt_Ptrs[0x4242], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x254, 0x781, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x254, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x254, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x254, 0x789, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x254, 0x78a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x27d, txt_Ptrs[0x4243], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x2c1, 0x777, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x2c1, 0x77b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x2c1, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x2c1, 0x787, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x2c1, 0x78b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x2ea, txt_Ptrs[0x4244], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x32e, 0x789, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x32e, 0x78a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x32e, 0x78b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x32e, 0x78c, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x32e, 0x77b, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x357, txt_Ptrs[0x4245], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x39b, 0x778, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x39b, 0x77c, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x39b, 0x780, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x39b, 0x784, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x39b, 0x788, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x3c4, txt_Ptrs[0x4246], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    d2_ClickedId = 1;
    d2_Wins[d2_LastWindow].contentHeight += 8;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_PointerObj == ids[0])
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PokerWindow2, 0x777c7) != 0);
    d2_CloseView(form);
}
/* 0x5dbc10, the default effect record (5 B): returns the address of ai_DefaultEffect (0x62de60: an
   empty name, the scale 1.0, the lifetime 25.0, then the entry's ids / callbacks). The twin
   0x5dbc20 returns its scale from the same record. */

int ai_GetDefaultEffect(void)
{
    return (int)&ai_DefaultEffect;
}
/* 0x5dbc20, the default effect scale (13 B): the scale field of the default effect record
   (ai_DefaultEffect.scale 0x62de6c) through the 0 .. 60 -> 0 .. 1 clamp 0x5dd090
   (ai_NormalizeEffectScale); the result is the entry's scale (passed to the 0 .. 100 slider as
   100 * value, unk_575860.c). The callers pass an ignored argument through a K&R declaration.
   @original the original TU called 0x5dd090 through an int K&R declaration: it pushes the float's
   bit pattern as an int (mov eax,ds:0x62de6c; push eax) and does not pop the float result, so the
   function returns the callee's st(0) (float) and the result is used as such by its callers. */

float ai_GetDefaultEffectScale(void)
{
    ((int (__cdecl *)())ai_NormalizeEffectScale)(ai_DefaultEffect.scaleBits); /* @original the float callee was called through an int K&R declaration (push the bits, no fstp) */
}

#include <math.h>

/* 0x5dbc30, builds the wind state of the weather (649 B; sv_SetupWeather calls it with the day phase, the game clock's first
   word and the two 24 entry wind rose tables sin / cos of the random wind directions): type (0 .. 3, else the default) is
   the day phase, id the clock word, from / to the sin and cos tables. The state record ai_EffectState gets the clock of
   its last update (year id, hour 6, minute 0; ai_UpdateWind advances from it), a copy of both tables, and the caller's two tables are negated in place (every
   entry * -1). Then for each of the 24 directions three random values are drawn by the phase's constants (base = random * 60 * k + 1, gust = random * 60 * k + c,
   phase = (random + n) * pi or 2 pi): type 0 k 0.25 / 0.3 + 0.2 / 5 * 2 pi, 1 k 0.05 / 0.4 + 0.1 / 3 * pi, 2 k 0.5 / 0.2 + 0.3 /
   5 * 2 pi, 3 k 0.25 / 0.5 / 3 * pi, else 0.5 / 0.5 / 2 * 2 pi. At the end a random angle (0 .. 2 pi) is stored and the
   default effect ai_DefaultEffect gets the vector (the sin table entry 6, 0, the cos table entry 6) and the scale
   |sin(angle) * gust[6]| + base[6]. Void. Reasoned: the wind meaning follows from the caller (the wind rose tables
   of sv_SetupWeather) and the sin / cos copies; the arrays base / gust / phase are named from their formulas. */
void ai_InitWind(int type, int id, float *from, float *to)
{
    int i;

    ai_EffectState.year = id;
    ai_EffectState.hour = 6;
    ai_EffectState.min = 0;
    memcpy(ai_EffectState.sinTab, from, sizeof(ai_EffectState.sinTab));
    memcpy(ai_EffectState.cosTab, to, sizeof(ai_EffectState.cosTab));
    for (i = 0; i < 24; i++) {
        from[i] = from[i] * -1.0f;
        to[i] = to[i] * -1.0f;
    }
    switch (type) {
    case 0:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.25f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.3f + 0.2f;
            ai_EffectState.phase[i] = (util_RandomF() + 5.0f) * 6.2831855f;
        }
        break;
    case 1:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.05f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.4f + 0.1f;
            ai_EffectState.phase[i] = (util_RandomF() + 3.0f) * 3.1415927f;
        }
        break;
    case 2:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.5f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.2f + 0.3f;
            ai_EffectState.phase[i] = (util_RandomF() + 5.0f) * 6.2831855f;
        }
        break;
    case 3:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.25f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.5f;
            ai_EffectState.phase[i] = (util_RandomF() + 3.0f) * 3.1415927f;
        }
        break;
    default:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.5f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.5f;
            ai_EffectState.phase[i] = (util_RandomF() + 2.0f) * 6.2831855f;
        }
        break;
    }
    ai_EffectState.angle = util_RandomF() * 6.2831855f;
    ai_DefaultEffect.pos[0] = ai_EffectState.sinTab[6];
    ai_DefaultEffect.pos[1] = 0;
    ai_DefaultEffect.pos[2] = ai_EffectState.cosTab[6];
    ai_DefaultEffect.scale = (float)(fabs(sin(ai_EffectState.angle) * ai_EffectState.gust[6]) + ai_EffectState.base[6]);
}
/* 0x5dbed0, advances the wind (235 B; sv_RunFrame calls it every frame): the hours since the state's last
   update (gm_MinutesBetween of the stored clock and ai_Time, / 60); when not negative, the angle of
   ai_EffectState grows by hours * phase[hour], the default effect ai_DefaultEffect gets the hour's wind
   vector (sin, 0, cos) and the scale (sin(angle) + 1) * gust[hour] / 2 + base[hour], and the clock is
   stored as the last update. */

void ai_UpdateWind(void)
{
    GAMECLOCK last;
    GAMECLOCK *now = (GAMECLOCK *)ai_Time;
    float hours;
    int hour;

    last.year = ai_EffectState.year;
    last.hour = (unsigned short)ai_EffectState.hour;
    last.min = ai_EffectState.min;
    last.xy = 0;
    hours = gm_MinutesBetween((struct AITIME *)&last, (struct AITIME *)ai_Time) * (1.0f / 60.0f);
    if (!(hours < 0.0f)) {
        hour = now->hour;
        ai_DefaultEffect.pos[1] = 0;
        ai_EffectState.angle += hours * ai_EffectState.phase[hour];
        ai_DefaultEffect.pos[0] = ai_EffectState.sinTab[hour];
        ai_DefaultEffect.pos[2] = ai_EffectState.cosTab[hour];
        ai_DefaultEffect.scale = ((float)sin(ai_EffectState.angle) + 1.0f) * ai_EffectState.gust[hour] * 0.5f
            + ai_EffectState.base[hour];
        ai_EffectState.year = now->year;
        ai_EffectState.hour = hour;
        ai_EffectState.min = now->min;
    }
}
/* 0x5dbfc0, writes the wind state to the save game (241 B; the save writer 0x51c870 calls it): the fields of
   ai_EffectState one by one with vfs_fwrite, the clock of the last update, the sin / cos / base / gust /
   phase tables and the angle. 0 as soon as a write fails, else 1. */

int ai_SaveWind(struct VFSFILE *f)
{
    if (vfs_fwrite(&ai_EffectState.year, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_EffectState.hour, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_EffectState.min, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.sinTab, sizeof(ai_EffectState.sinTab), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.cosTab, sizeof(ai_EffectState.cosTab), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.base, sizeof(ai_EffectState.base), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.gust, sizeof(ai_EffectState.gust), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.phase, sizeof(ai_EffectState.phase), 1, f) <= 0)
        return 0;
    return vfs_fwrite(&ai_EffectState.angle, 4, 1, f) > 0;
}
/* 0x5dc0c0, reads the wind state from the save game (223 B; the save reader 0x5205f0 calls it from version
   0x10050): the fields of ai_EffectState in ai_SaveWind's order with ls_Read. 0 as soon as a read fails,
   else 1. */

int ai_LoadWind(int f)
{
    if (!ls_Read(&ai_EffectState.year, 4, 1, f))
        return 0;
    if (!ls_Read(&ai_EffectState.hour, 4, 1, f))
        return 0;
    if (!ls_Read(&ai_EffectState.min, 4, 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.sinTab, sizeof(ai_EffectState.sinTab), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.cosTab, sizeof(ai_EffectState.cosTab), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.base, sizeof(ai_EffectState.base), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.gust, sizeof(ai_EffectState.gust), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.phase, sizeof(ai_EffectState.phase), 1, f))
        return 0;
    return ls_Read(&ai_EffectState.angle, 4, 1, f) != 0;
}
/* 0x5dc1a0, the sphere volume record (89 B): radius and size plus the metrics derived from the size
   into out (+0 = radius, +4 = size, +8 = size^2 * 2 pi, +0xc = size^3 * 4/3 pi, +0x10 = that *
   radius). Returns 0 when a negative value comes in, 1 otherwise. */

int ai_SphereVolume(float radius, float size, float *out)
{
    float volume;

    if (radius < 0.0f || size < 0.0f)
        return 0;
    out[0] = radius;
    out[1] = size;
    out[2] = size * size * 6.2831855f;
    volume = size * size * size * 4.1887903f;
    out[3] = volume;
    out[4] = volume * radius;
    return 1;
}
/* 0x5dc210, the two-axis position share (35 B): the two values a / b divided by the sphere volume
   record's reach (+0x10, volume * radius - ai_SphereVolume's record, am.c 0x4a67a0 passes it as
   int *in) and scaled by 0.1; the pair goes into out (+0, +4) as the ballistic range rng[0] / rng[1].
   Void: the caller ignores the result (am.h declares it void). The division is written value / field
   (no parentheses): the parenthesised form makes VC6 emit fld divisor; fdivr value instead. */

void ai_PositionShare(float a, float b, SPHEREVOLUME *rec, float *out)
{
    out[0] = a / rec->reach * 0.1f;
    out[1] = b / rec->reach * 0.1f;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* 0x5dc240, builds a cannon ball in flight (341 B; am.c's shot callback 0x4a6ae0 calls it per ball): the
   ball's sphere (ai_SphereVolume of density and size: mass = volume * density, am.c passes 7874, the density
   of iron) gives the speed charge / mass * 0.1; the start velocity is the barrel direction dir times it (also
   the current velocity), the start and the current position are pos, the wind is ai_DefaultEffect with
   its scale clamped to 0 .. 60 (ai_WindGain) and the vector scaled by it; the surface and the mass, state 1
   (in flight), the clock d3_Time (ms) and 0 steps complete the record. Returns 1. @deep body complete and read against 0x5dc240..0x5dc395 (all 113 ins accounted for: GetDefaultEffect, SphereVolume(density, size), dir copy, speed = charge / reach * 0.1, vel scale, vel copy, wind copy + clamp 0..60 (MAX(MIN)) + scale, posStart / pos, area / mass, state 1, d3_Time, steps 0, return 1). Left is VC6 placement: (1) our compiler sinks the call ai_GetDefaultEffect (same TU, known side-effect free) to the wind copy at +0x79 where the original calls it first and keeps the pointer in edx across ai_SphereVolume (interprocedural regs); this shifts the whole +0x09..+0xe8 block (76 ins); (2) the vel scale: the original reloads the speed (fld [ecx+0xc]) after each store, ours keeps it on the FPU (fld st(0)); (3) the last hunk loads surface / reach into swapped registers. Tried: the wind pointer initialised at its declaration, compound *=, D3VECTOR-typed velocity (the dir copy as a struct assign), none moved the call */
int ai_InitShot(SHOTREC *shot, D3VECTOR *pos, D3VECTOR *dir, float charge, float size, float density)
{
    EFFECTREC *wind = (EFFECTREC *)ai_GetDefaultEffect();
    SPHEREVOLUME ball;

    ai_SphereVolume(density, size, (float *)&ball);
    shot->velStart.v = *dir;
    shot->velStart.speed = charge / ball.reach * 0.1f;
    shot->velStart.v.x = shot->velStart.speed * shot->velStart.v.x;
    shot->velStart.v.y = shot->velStart.speed * shot->velStart.v.y;
    shot->velStart.v.z = shot->velStart.speed * shot->velStart.v.z;
    shot->vel = shot->velStart;
    shot->wind = *wind;
    shot->wind.scale = MAX(MIN(shot->wind.scale, ai_WindGain), 0.0f);
    shot->wind.pos[0] = shot->wind.scale * shot->wind.pos[0];
    shot->wind.pos[1] = shot->wind.scale * shot->wind.pos[1];
    shot->wind.pos[2] = shot->wind.scale * shot->wind.pos[2];
    shot->posStart = *pos;
    shot->pos = *pos;
    shot->area = ball.surface;
    shot->mass = ball.reach;
    shot->state = 1;
    shot->time = d3_Time;
    shot->steps = 0;
    return 1;
}
/* 0x5dc3a0, scatters a cannon ball's start velocity (254 B; am.c's shot callback 0x4a6ae0 calls it after
   ai_InitShot for each ball): 0 once the ball made a flight step. The start velocity and speed are scaled by
   (1.5 r - 1) * speedSpread + 1, then two random deviations are added to the velocity: (2 r - 1) * speed *
   sideSpread along side (am.c passes the vector the balls are set apart along) and ((2 r - 1) * speed *
   otherSpread + 1) along other (a second vector of the planned shot); the result is also the current
   velocity. Returns 1. r is util_RandomF each time. Reasoned: the spread names follow the formulas,
   am.c passes the same spread value for both deviations. @deep body complete, same 79 instructions as 0x5dc3a0..0x5dc49e and every operand accounted for (steps check, speed scale (1.5r-1)*spread+1 on v and speed, side deviation (2r-1)*speed*sideSpread, other deviation (2r-1)*speed*otherSpread+1, vel.speed copy, the three sums kept as x / y / z with z through the dead arg slot [esp+0x20] like the original, vel = sums). Left 18 ins, all operand order / register choice: (1) both deviations: the original does fmul [esi+0xc] (speed) then fmul [esp+0x24|0x2c] (spread), ours the other way round for either source order of the factors; (2) the vel.speed copy: original mov ecx,[esi+0xc] inside the second deviation and stores it after dother, ours mov eax at the start of it; (3) y / z sums: original fld dother; fadd v; fadd dside, ours fadd dside before v (x matches). Tried: speed*spread and spread*speed, the copy before / after the second deviation, chained assigns with ghidra's 0 / 2 / 1 store order (worse: z leaves the slot) */

int ai_ScatterShot(SHOTREC *shot, float sideSpread, D3VECTOR *side, float otherSpread, D3VECTOR *other,
                   float speedSpread)
{
    float k, x, y, z;
    D3VECTOR dside, dother;

    if (shot->steps != 0)
        return 0;
    k = (util_RandomF() * 1.5f - 1.0f) * speedSpread + 1.0f;
    shot->velStart.v.x = k * shot->velStart.v.x;
    shot->velStart.v.y = k * shot->velStart.v.y;
    shot->velStart.v.z = k * shot->velStart.v.z;
    shot->velStart.speed = k * shot->velStart.speed;
    k = (util_RandomF() * 2.0f - 1.0f) * shot->velStart.speed * sideSpread;
    dside.x = k * side->x;
    dside.y = k * side->y;
    dside.z = k * side->z;
    k = (util_RandomF() * 2.0f - 1.0f) * shot->velStart.speed * otherSpread + 1.0f;
    shot->vel.speed = shot->velStart.speed;
    dother.x = k * other->x;
    dother.y = k * other->y;
    dother.z = k * other->z;
    x = dother.x + shot->velStart.v.x + dside.x;
    shot->velStart.v.x = x;
    y = dother.y + shot->velStart.v.y + dside.y;
    shot->velStart.v.y = y;
    z = dother.z + shot->velStart.v.z + dside.z;
    shot->velStart.v.z = z;
    shot->vel.v.x = x;
    shot->vel.v.y = y;
    shot->vel.v.z = z;
    return 1;
}

#define SIGN(v) ((v) > 0.0f ? 1.0f : -1.0f)
#define DRAG(v) (SIGN(v) * shot->area * ai_ShotDrag * ai_AirDensity * (v) * (v) * 0.5f)

/* 0x5dc4a0, moves a cannon ball (1318 B; gv.c 0x4edf80 calls it every frame): 0 unless the ball is in flight
   (state 1). Off the height map (d3_GetHeightAt fails) the state becomes 3 and the energy 0. Otherwise one
   step of ai_ShotStep (1/74 s) is made per millisecond since the last one: the force on each axis is the
   drag 0.5 * ai_AirDensity * ai_ShotDrag * area * v^2 (signed with v) plus the same drag of the wind, and
   gravity pulls y; the velocity takes Heun's average of the change at the old and the predicted velocity,
   the position moves by velocity * step in inches. When ai_ShotHitsGround finds the move from the old to the
   new position hits the ground and the ball made more than 50 steps, it lands there: state 2, the energy
   0.5 * mass * v^2, returns 0. Returns 1 while it flies. @deep Read whole against the original 0x5dc4a0..0x5dc9c6: statement set and meaning match (state test, d3_GetHeightAt out-of-map exit, the three wind drags, the per-ms loop with Heun's average, next = pos copy, time/steps ++, ai_ShotHitsGround with px/pz, the landing energy). The velocity is now copied whole into a D3VECTOR v each step (the original copies shot->vel by dwords to [esp+0x10..0x18] and reloads v.x/v.y/v.z from memory for the drags; 311 -> 332 differing but 339 -> 348 of 354 instructions). Left: (1) VC6 reorders the drag product chain - the original multiplies sign*area*drag*density*v*v*0.5 (wind x/z: area*w*w*sign*...), ours sign*drag*density*v*v*area*0.5 for every association tried (6 macro forms give identical code or worse); (2) the temporaries: the original spills dx and the half sums into 0x84/0x68/0x6c and reuses the dead v slots [0x10]/[0x14] for (bx+ax)*0.5 / (by+ay)*0.5, frame 0x98 vs ours 0x5c - FPU stack scheduling and spill-slot allocation. */
int ai_StepShot(SHOTREC *shot)
{
    float fall = ai_ShotStep * ai_Gravity;
    float perMass = ai_ShotStep / shot->mass;
    D3SM *map = (D3SM *)init_SceneHeightMaps[0];
    int ticks, i, px, pz, npx, npz;
    float height;
    float windX, windY, windZ;
    float dx, dy, dz, ax, ay, az, bx, by, bz;
    D3VECTOR v, next;

    if (shot->state != 1)
        return 0;
    ticks = d3_Time - shot->time;
    if (!d3_GetHeightAt(map, &shot->pos, &px, &pz, &height)) {
        shot->state = 3;
        shot->energy = 0;
        return 0;
    }
    windX = DRAG(shot->wind.pos[0]);
    windY = DRAG(shot->wind.pos[1]);
    windZ = DRAG(shot->wind.pos[2]);
    for (i = 0; i < ticks; i++) {
        v = shot->vel.v;
        dx = DRAG(v.x);
        dy = DRAG(v.y);
        dz = DRAG(v.z);
        ax = (dx + windX) * perMass;
        ay = (dy + windY) * perMass - fall;
        az = (dz + windZ) * perMass;
        v.x = ax + v.x;
        v.y = ay + v.y;
        v.z = az + v.z;
        dx = DRAG(v.x);
        dy = DRAG(v.y);
        dz = DRAG(v.z);
        next.x = shot->pos.x;
        next.y = shot->pos.y;
        next.z = shot->pos.z;
        shot->time++;
        shot->steps++;
        bx = (dx + windX) * perMass;
        by = (dy + windY) * perMass - fall;
        bz = (dz + windZ) * perMass;
        shot->vel.v.x += (bx + ax) * 0.5f;
        shot->vel.v.y += (by + ay) * 0.5f;
        shot->vel.v.z += (bz + az) * 0.5f;
        next.x += shot->vel.v.x * ai_InchPerMeter * ai_ShotStep;
        next.y += shot->vel.v.y * ai_InchPerMeter * ai_ShotStep;
        next.z += shot->vel.v.z * ai_InchPerMeter * ai_ShotStep;
        if (ai_ShotHitsGround(map, &shot->pos, &next, px, pz, &npx, &npz) && shot->steps > 50) {
            shot->pos = next;
            shot->state = 2;
            shot->energy = (shot->vel.v.x * shot->vel.v.x + shot->vel.v.y * shot->vel.v.y
                + shot->vel.v.z * shot->vel.v.z) * shot->mass * 0.5f;
            return 0;
        }
        shot->pos = next;
        px = npx;
        pz = npz;
    }
    return 1;
}

#define SIGN(v) ((v) > 0.0f ? 1.0f : -1.0f)
#define DRAG(v) (SIGN(v) * shot->area * ai_ShotDrag * ai_AirDensity * (v) * (v) * 0.5f)

/* 0x5dc9d0, moves a cannon ball in slow motion (1318 B; gv.c 0x4edf80 calls it instead of ai_StepShot when the
   effect's +0x14 flag is set): ai_StepShot instruction for instruction with the step ai_ShotHalfStep (1/148 s)
   for ai_ShotStep, in the gravity, the per-mass factor and the position move. @deep Read whole against the original 0x5dc9d0..0x5dcef6: it is ai_StepShot instruction for instruction with ai_ShotHalfStep for ai_ShotStep (the disassemblies differ only in those four operands); same body and the same velocity copy v (348 of 354 instructions). Left, as in ai_StepShot: (1) VC6 reorders the drag product chain - the original multiplies sign*area*drag*density*v*v*0.5 (wind x/z: area*w*w*sign*...), ours sign*drag*density*v*v*area*0.5 for every association tried (6 macro forms, identical code or worse); (2) the temporaries: the original spills dx and the half sums into 0x84/0x68/0x6c and reuses the dead v slots [0x10]/[0x14], frame 0x98 vs ours 0x5c - FPU stack scheduling and spill-slot allocation. */
int ai_StepShotSlow(SHOTREC *shot)
{
    float fall = ai_ShotHalfStep * ai_Gravity;
    float perMass = ai_ShotHalfStep / shot->mass;
    D3SM *map = (D3SM *)init_SceneHeightMaps[0];
    int ticks, i, px, pz, npx, npz;
    float height;
    float windX, windY, windZ;
    float dx, dy, dz, ax, ay, az, bx, by, bz;
    D3VECTOR v, next;

    if (shot->state != 1)
        return 0;
    ticks = d3_Time - shot->time;
    if (!d3_GetHeightAt(map, &shot->pos, &px, &pz, &height)) {
        shot->state = 3;
        shot->energy = 0;
        return 0;
    }
    windX = DRAG(shot->wind.pos[0]);
    windY = DRAG(shot->wind.pos[1]);
    windZ = DRAG(shot->wind.pos[2]);
    for (i = 0; i < ticks; i++) {
        v = shot->vel.v;
        dx = DRAG(v.x);
        dy = DRAG(v.y);
        dz = DRAG(v.z);
        ax = (dx + windX) * perMass;
        ay = (dy + windY) * perMass - fall;
        az = (dz + windZ) * perMass;
        v.x = ax + v.x;
        v.y = ay + v.y;
        v.z = az + v.z;
        dx = DRAG(v.x);
        dy = DRAG(v.y);
        dz = DRAG(v.z);
        next.x = shot->pos.x;
        next.y = shot->pos.y;
        next.z = shot->pos.z;
        shot->time++;
        shot->steps++;
        bx = (dx + windX) * perMass;
        by = (dy + windY) * perMass - fall;
        bz = (dz + windZ) * perMass;
        shot->vel.v.x += (bx + ax) * 0.5f;
        shot->vel.v.y += (by + ay) * 0.5f;
        shot->vel.v.z += (bz + az) * 0.5f;
        next.x += shot->vel.v.x * ai_InchPerMeter * ai_ShotHalfStep;
        next.y += shot->vel.v.y * ai_InchPerMeter * ai_ShotHalfStep;
        next.z += shot->vel.v.z * ai_InchPerMeter * ai_ShotHalfStep;
        if (ai_ShotHitsGround(map, &shot->pos, &next, px, pz, &npx, &npz) && shot->steps > 50) {
            shot->pos = next;
            shot->state = 2;
            shot->energy = (shot->vel.v.x * shot->vel.v.x + shot->vel.v.y * shot->vel.v.y
                + shot->vel.v.z * shot->vel.v.z) * shot->mass * 0.5f;
            return 0;
        }
        shot->pos = next;
        px = npx;
        pz = npz;
    }
    return 1;
}
/* 0x5dcf00, the ground test of a cannon ball's flight step (390 B; ai_StepShot / ai_StepShotSlow): the cell
   of to (d3_GetHeightAt into npx / npz). When it is at most one cell from the cell px / pz of from, the
   step hits when to lies less than ai_ShotHitHeight above the ground there. A longer step is walked from
   from to to in steps of 1 / (the cell distance): the first point less than ai_ShotHitHeight above the
   ground is the hit, to is moved there. 1 on a hit, else 0. @deep body complete, size 390 = 390, 134 = 134 ins, every statement in the original's order (end cell, abs cell distance, <= 1 short test, step = 1 / sqrt, p = from field by field, d = to - from then *= step, the walk with d3_GetHeightAt into cx / cz, the hit copy, t += step while t < 1). D3VECTOR4 locals give the original's 0x24 frame. Left 44 ins: (1) the commutative fadds of the walk: the original does fld p.y; fadd d.y / fld p.z; fadd d.z / fld t; fadd step, ours loads the other operand first (same for +=, p = p + d); (2) the from->z copy is scheduled after the subtractions in the original, before them in ours; (3) the lea / push order of the walk's GetHeightAt args (ecx / edx / eax rotation) and the hit copy's register rotation (regs eax->edx ecx->eax edx->ecx); (4) slots t / step swapped ([esp+0x40] / [esp+0x50]) */

int ai_ShotHitsGround(D3SM *map, D3VECTOR *from, D3VECTOR *to, int px, int pz, int *npx, int *npz)
{
    float height, t, step;
    int dx, dz, cx, cz;
    D3VECTOR4 p, d;                     /* the engine's padded vectors (the original's frame holds 16 bytes each) */

    d3_GetHeightAt(map, to, npx, npz, &height);
    dx = abs(*npx - px);
    dz = abs(*npz - pz);
    if (dx <= 1 && dz <= 1) {
        if (ai_ShotHitHeight + height > to->y)
            return 1;
        return 0;
    }
    t = 0;
    step = 1.0f / (float)sqrt((float)(dz * dz) + (float)(dx * dx));
    p.x = from->x;
    p.y = from->y;
    p.z = from->z;
    d.x = to->x - from->x;
    d.y = to->y - from->y;
    d.z = to->z - from->z;
    d.x *= step;
    d.y *= step;
    d.z *= step;
    do {
        p.x += d.x;
        p.y += d.y;
        p.z += d.z;
        d3_GetHeightAt(map, (D3VECTOR *)&p, &cx, &cz, &height);
        if (ai_ShotHitHeight + height > p.y) {
            to->x = p.x;
            to->y = p.y;
            to->z = p.z;
            return 1;
        }
        t += step;
    } while (t < 1.0f);
    return 0;
}
/* 0x5dd090, the 0 .. 60 clamp divided by 60 (69 B): 0 when <= 0, 1 when >= 60, else value / 60.
   Every comparison is written negated (`!(value < 60.0f)`, `!(clamped > 0.0f)`): the positive forms
   make VC6 emit `and eax,0x100` where the original tests the C0/C2 bits (`test ah,0x5; jp`), and the
   divisions stay float (a double / 60.0 becomes an fmul of the reciprocal).
   @deep behaviour: 34 code hunks remain (56 at session start), first at +0x10; the meaning matches (0 .. 60 clamp, the select, the three returns) and the remaining hunks are VC6 float-compare idioms / block layout: the original keeps the selected value on the FPU stack across fcomp 0.0, tests C0|C3 (and eax,0x4100; jne) and puts the return-0 block out of line, ours folds the select and uses test ah,0x41; jp. Leads tried, one build each: positive vs negated comparisons (x >= 60 / !(x < 60) / ternary select), v <= 0 vs !(v > 0), float vs double division (double becomes fmul of the reciprocal; the float fdiv is the original). Kept: negated first test and float divisions (56 -> 34 hunks). Left: FPU/branch scheduling - deep */

float ai_NormalizeEffectScale(float value)
{
    float clamped;

    clamped = value;
    if (!(value < 60.0f))
        clamped = 60.0f;
    if (clamped <= 0.0f)
        return 0.0f / 60.0f;
    if (!(value < 60.0f))
        return 60.0f / 60.0f;
    return value / 60.0f;
}

#include <math.h>

/* 0x5dd100, solves the cannon aim (727 B; am.c's 0x4a67a0 calls it): the ball flies from `from` to `to`
   (world units, 0.0254 m each) under gravity; bisects the launch speed (range[0] .. range[1]) and the
   elevation angle (range[2] .. range[3], radians) in 16 steps until the landing point is within 2.5 m of the
   target. The error is the flight time (vertical speed / g plus the fall from the height difference) times the
   horizontal speed minus the horizontal distance; too short raises the angle (the speed once the angle is at
   its maximum), too far lowers the speed (the angle once the speed is at its minimum). Returns 0 with the
   speed in *outSpeed, the elevation in *outElev, the heading angle (d3_VecAngle2 against ai_AxisPair) in
   *outYaw and three vectors: outDir the horizontal direction tilted by the elevation, outUp d3_YAxis tilted
   the same way, outSide their cross product; -1 when no setting in the range hits (reasoned: the names follow
   the use in am.c). @deep body complete and read against the original (bisection of speed range[0..1] / elevation range[2..3], 16 steps, +-2.5 m window, outputs outElev/outSpeed/outDir/outUp/outSide; conditions in the original's > / < jump forms, D3VECTOR4 dir/hdir give the 0x34 frame). Left (208 ins, all allocation): the original keeps speed/ang/speedStep/angStep on the FPU stack from before the loop (fld [ebx+0xc]; fld [ebx]; fld [g]...) and holds the sign in ebp / the counter in ecx with g, 2g, 1/g, drop, horiz*0.0254 in the slots 0x14/0x20/0x48/0x18/0x10; ours loads speed through an int copy (mov edx,[ebx]; mov [esp+0x48],edx), keeps sign in ecx, counter in edx and orders the three FPU temps differently (asmdiff hunks at 0x5dd108, 0x5dd1f7, 0x5dd294 fpu stack order, 0x5dd2d0, 0x5dd383 the cross-product operand order); horiz*0.0254 compiles fld mem; fmul const where the original has fld const; fmul mem */
int ai_SolveCannonAim(float *from, float *to, float *range, float *outElev, float *outYaw, float *outSpeed,
                      float *outDir, float *outUp, float *outSide)
{
    float speedStep = (range[1] - range[0]) * 0.5f;
    float angStep = (range[3] - range[2]) * 0.5f;
    float g = ai_Gravity;
    D3VECTOR4 dir, hdir;
    float horiz, drop, inv, s, c, h, t, err;
    float speed, ang;
    int sign, i;

    dir.x = to[0] - from[0];
    dir.y = to[1] - from[1];
    dir.z = to[2] - from[2];
    hdir.x = dir.x;
    hdir.y = 0.0f;
    hdir.z = dir.z;
    horiz = (float)sqrt(dir.x * dir.x + dir.z * dir.z);
    d3_Normalize(&dir);
    d3_Normalize(&hdir);
    *outYaw = d3_VecAngle2((D3VECTOR *)ai_AxisPair, (D3VECTOR *)&dir);
    drop = (float)fabs(to[1] - from[1]) * 0.0254f;
    horiz = 0.0254f * horiz;
    sign = 0;
    if (from[1] > to[1])
        sign = 1;
    else if (from[1] < to[1])
        sign = -1;
    speed = range[0];
    ang = range[3];
    inv = 1.0f / g;
    for (i = 0; i < 16; i++) {
        s = (float)sin(ang);
        c = (float)cos(ang);
        h = s * s * speed * speed / (g * 2.0f);
        t = inv * s * speed;
        if (sign == 1) {
            h = h + drop;
            t = (float)sqrt((h * inv + h * inv)) + t;
        } else if (sign == -1) {
            h = h - drop;
            t = (float)sqrt((h * inv + h * inv)) + t;
        }
        err = t * c * speed - horiz;
        if (err > 2.5f) {
            if (speed > range[0]) {
                speed = speed - speedStep;
                speedStep = speedStep * 0.5f;
            } else if (ang > range[2]) {
                ang = ang - angStep;
                angStep = angStep * 0.5f;
            } else
                return -1;
        } else if (err < -2.5f) {
            if (ang < range[3]) {
                ang = ang + angStep;
                angStep = angStep * 0.5f;
            } else if (speed < range[1]) {
                speed = speed + speedStep;
                speedStep = speedStep * 0.5f;
            } else
                return -1;
        } else {
            *outSpeed = speed;
            *outElev = ang;
            outDir[0] = hdir.x;
            outDir[1] = c * hdir.y - s * hdir.z;
            outDir[2] = s * hdir.y + c * hdir.z;
            outUp[0] = d3_YAxis.x;
            outUp[1] = d3_YAxis.y * c - d3_YAxis.z * s;
            outUp[2] = d3_YAxis.z * c + d3_YAxis.y * s;
            outSide[0] = outUp[2] * outDir[1] - outDir[2] * outUp[1];
            outSide[1] = outDir[2] * outUp[0] - outUp[2] * outDir[0];
            outSide[2] = outDir[0] * outUp[1] - outDir[1] * outUp[0];
            return 0;
        }
    }
    return -1;
}


/* ai_TextLoadGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextSaveGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGameOptions: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGfxOptions: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextSfxOptions: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextLeaveGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextBack: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_QuickSaveNoPlayerText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_QuickSaveDisabledText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextBloodRevenge: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextCurse: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTheatre: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextHomage: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextChat: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TailorBlockedText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TailorAmountText: view of txt_Ptrs (build/mk_txt_views.py) */
POKERUI ai_PokerUi = { -1 };  /* 0x62dda0, the poker window state (window -1 when closed, the rest zero) */
float ai_ShotDrag = 1.5f;  /* 0x62de58, the drag coefficient of a cannon ball (ai_StepShot) */
float ai_ShotHitHeight = 98.4252014f;  /* 0x62de5c, 2.5 m in inches: a ball within this height above the ground hits it (ai_ShotHitsGround) */
EFFECTREC ai_DefaultEffect = { { 0.0f, 0.0f, 1.0f }, { 25.0f } };  /* 0x62de60, the default effect: the vector (0, 0, 1) and the scale 25.0 (0x62de6c) */
/* sv_MenuOpen: view of ai_GameState (tools/data_views.py) */
