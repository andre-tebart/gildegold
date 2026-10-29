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

/* views of d2.h's D2OBJ / D2INPUT (d2.h cannot be included here: its vfs_Root / vfs_Tfopen
   declarations clash with vfs.h; the type debt is in notes/units/unk_5d33b0.md) */
typedef struct D2INPUTVIEW { char pad0[0x10]; int w; char pad1[0x148]; int maxw; } D2INPUTVIEW;

extern D2INPUTVIEW d2_Inputs[];         /* 0xfa6020, d2.h: D2INPUT [0x80] */
extern int d2_FontHeight;               /* 0xf8f084, d2.h */
extern void *d2_FocusedInput;           /* 0x6ed9dc, d2.h: D2INPUT * */
extern unsigned char kb_Last;           /* 0x1468410, d2.h */
int d2_WinCreateSlider(int x, int y, int pic, int unk, int win);     /* 0x46bf10, d2.h: D2OBJ *d2_Objs */
void d2_CloseView(int view);                                         /* 0x4758f0, d2.h */
void d2_SetObjWidth(int obj, short w);                               /* 0x46c260, d2.h */
int d2_OpenInput(int x, int y, int w, int h, unsigned char flags);   /* 0x46a110 */
void d2_RemoveObj(int obj);                                          /* 0x46e710 */
int d2_GetObjValue(int obj);                                         /* 0x475980 */
void d2_SetObjFont(int obj, int font);                               /* 0x4760b0 */
void d2_SetObjValue();                                               /* 0x475cb0, K&R: d2.c's definition takes 5 parameters, the call sites push 2, 4 or 5 */
void d2_SetObjMax(int obj, int v);                                   /* 0x475f40 */

/* d2.h's D2FADE (the fade record of 0x4767d0): only the flags byte is read here (bit 2 = done, the
   original tests the byte, not the dword); the type debt is in notes/units/unk_5d33b0.md */
typedef struct D2FADEVIEW { unsigned char flags; char pad1[0x63]; } D2FADEVIEW;
/* d2.h's D2WIN (0x3c4 bytes, the window records d2_Wins[0x61]): only the fields this unit reads.
   The struct keeps the full size, or d2_Wins[bar] would scale by the short size and read another
   record (the type debt is the same as D2OBJVIEW's). */
extern D2WINVIEW d2_Wins[];             /* 0xf8f120, d2.h: D2WIN d2_Wins[0x61] */
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
int ls_ReadSaveHeader(int f, void *h);      /* 0x51cee0, ls.c: reads a save header into h (no header there) */
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
extern char ls_SavePath[];                  /* 0x75e640, ls.c: the path stored in the save header (0x60 B) */
int sv_CreateFormSlider(int x, int y, int f, int g, int p5, int *b, int *a, int *c); /* 0x52e860, sv.c:
                                           the slider of the form f at (x, y) */
void d2_EnableObject(int obj, int on);      /* 0x475aa0, d2.h: enables / disables an object and its items */
int sv_OpenMessageBox(char *text, int flags); /* 0x521900, sv.c: opens the waiting box with text */
int d2_GetViewWin(int view, int i);         /* 0x475ff0, d2.h: window i of a view, -1 if out of range;
                                               the six-argument calls below go through a cast, tagged
                                               @original (the original pushes four stray arguments) */

int ai_LeaveGame = 0;  /* 0x6f25a0, set with ai_GameOver when the menu leaves the game (the
                                       load window / the game loop end); main.c clears it when a game
                                       starts ( */
OPTIONBYTES ai_GfxResolution = {0};  /* 0x6f2f94: byte 0 cur_res */
/* ai_ContactTextProductionTailor: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_PokerMinBet = 0;  /* 0x6f35a4 */
int ai_PokerMaxBet = 0;  /* 0x6f35a8 */
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
