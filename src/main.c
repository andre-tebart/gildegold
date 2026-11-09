/* main.c -- the program level code (0x531670 .. 0x53d430), tag "main_" (0x624a8c ..): the window class
   "Die Gilde", the single instance mutex, the version string, engine start / stop, the main menu, the
   network and character creation dialogs, the round loop. Its strings (0x62493c ..) follow sv.c's last one
   ("MASTER(): .." 0x624534); he.c starts at 0x53d430.
   Open: data: 0x624ec4 main_ModelArbeiter / 0x624ed4 main_ModelWirt, owner main (the men's model names of
   main_ChooseModel's automatic tables: the only access is the initializer copy at 0x534bb7 / 0x534b5d, so the
   strings stay gap bytes until that table construction is rewritten)
   Open: data: 0x624ee0 main_ModelPatrizier, owner main (the same table, copy at 0x534b51; the "Sky_Mittel_02" at
   0x624ef0 after it has no reference at this address)
   Open: 0x534040, 0x53a520 and 0x53b2e0 are out of address order.
   unit of rea 8cef7d5b (src/game/main.c); boundaries in decl/text.dcl */
extern int gm_PlayerRecords[688];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "he.h"
#include "sfx.h"
#include <objbase.h>
#include <strmif.h>
#include <control.h>
#include "f3gm.h"
#define gm_TitleJobI ((int (*)(int, int))gm_TitleJob)  /* 0x5044b0 (f3gm.h: unsigned char): @original the byte-matched callers here read the result as an int (no `and eax,0xff` after the call): main_RundenBeginn 0x538140 */
#include "mov.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include <d3d.h>
#include "d3.h"
#include "d3_light.h"
#include "d3_engine.h"
#include "data_views.h"
#include "d2.h"
#include "lc.h"
#include "ecmd.h"
#include "ts_texture.h"
#include "menu.h"
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.h: int view): @original the callers push the placement flags too, which the original's tail jump leaves on the stack for d2_PlaceWindowScaled */
#include "ai.h"
#include "gfx.h"
#include "dd_vesa.h"
#include "d3_interface.h"
#include "d3_int_io.h"
#include "err.h"
#include "tr.h"
#include "vfs.h"
#include "tb.h"
#include "msx.h"
#include "evtc.h"
#include "sndc.h"
#include "net.h"
#include "ls.h"                      /* ls_SavePath (LSSAVE, its NETHDR), the save game functions */
#include "udp.h"                     /* udp_FindServers, UDPSERVER */
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int main_Inactive;       /* 0x6f2584, 1 while the app is deactivated */
extern unsigned char ai_GfxResolution;  /* 0x6f2f94 (menu.c: OPTIONBYTES): @original main_OpenScreen reads it as a byte object (0x531d43 mov al, byte ptr [0x6f2f94]); through the 4-byte union VC6 loads the dword and masks it (and eax, 0xff: 264 instead of 226 differing instructions) */
extern int ai_ScreenModeH;
extern int ai_ScreenModeW;
extern struct SCREENMODE { int w, h; } ai_ScreenModes[];
extern char d3_AnimDir[];
extern int di_MaxX;
extern char main_GamePath[];
extern int main_TextureDivider;
#include "cm_types.h"
#define cm_JoinChar (*(CMJOIN *)((char *)&cm_LobbyRecords + 0x2c0))  /* 0x9367a0, the joined character (CMJOIN) */
extern GAPDATA full_data_5f320c[];
extern int main_TextReopen;
extern int main_HmtMode;
extern int main_HmtBuild;
extern char main_AppDir[];
#include "f3init_types.h"
extern GAPDATA full_data_60da10[];
extern const char main_DefaultHost[12];       /* 0x624b48, the default host address "127.0.0.1" of the [Network] Host option */
extern GAPDATA full_data_5f1084[];
extern const char main_DefaultHost[12];       /* 0x624b48, the default host address "127.0.0.1" of the [Network] Host option */
extern char ai_OptionsFile[288];

extern char ai_OptionsFile[288];
extern char cm_LobbyRecords[752];



extern GAPDATA full_data_5f1084[];
extern int sv_PopRequest;




extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */





extern GAPDATA full_data_5f320c[];
extern INIT_PLAYER init_PlayerProfile;
extern int sv_PopRequest;

extern GAPDATA full_data_624cf8[];
extern INIT_PLAYER init_PlayerProfile;
extern int sv_PopRequest;
extern GAPDATA full_data_602568[];
extern char ai_OptionsFile[288];




extern INIT_PLAYER init_PlayerProfile;
extern int sv_PopRequest;
extern int ai_OptionHistorie;

extern unsigned char hst_TextsOn;
extern INIT_PLAYER init_PlayerProfile;
extern int sv_PopRequest;
extern OPTIONBYTES ai_GamePanel;
extern int ai_Opponents[7];






extern int sv_PopRequest;



extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_602568[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
#define main_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* @original this caller passes the id as a word ("mov ax,[id]; push eax") and pops the float result */
extern float gm_DayStartHours[], gm_DayEndHours[];  /* 0x617830 / 0x617840 (f3gm.c) */
#pragma pack(push, 1)
typedef struct MAINCARRY {          /* 0x21d bytes (+0x219 the entry count), what gm_CollectCarried collects for a character (f3gm.c: +0x31 the free capacity) */
    int units;                      /* 0x00, counts the 0xe-byte units */
    struct {
        unsigned char good;         /* a good (its name is the text 0x253 + good), 0 ends the list */
        int amount;
    } goods[9];                     /* 0x04, what the round change lists on the balance page (main.c 0x5384e0) */
    int capacity;                   /* 0x31, the free capacity (unaligned) */
    char rest[0x1e8];
} MAINCARRY;
#pragma pack(pop)
typedef struct MAINPLREC {          /* gm_PlayerRecord's record, the part gm_TitleJob and the round start read */
    char pad00[0x74];
    unsigned char job[0x11];        /* 0x74, the job byte of each title group */
} MAINPLREC;
typedef struct MAINCHARNEW {        /* the AICHAR fields the character replacement writes (ai.h AICHAR: name is shorter than its 0x58 union) */
    unsigned short id;              /* 0x00 */
    unsigned char kind;             /* 0x02 */
    char pad03;
    unsigned int netId;             /* 0x04 */
    unsigned char health;           /* 0x08 */
    unsigned char flag9;            /* 0x09, 1: the female name table is used */
    char pad0a[0x26];
    union {
        char name[0x20];            /* 0x30 */
        struct {
            char firstName[0x10];   /* 0x30 */
            char familyName[0x10];  /* 0x40, the start couple's father gets the profile's family name (main_CreateStartCouple) */
        };
    };
    short seq;                      /* 0x50, a running number (ai_NextCharNumber) */
    char pad52[2];
    int coat;                       /* 0x54, the coat of arms (AICHAR.ref54: the save adds the base 0x59a back, as INIT_PLAYER.coat's object ids); the start couple get the player's */
    char pad58[4];
    union {
        int clear[8];               /* 0x5c, set to -1 */
        struct {
            int spouse;             /* 0x5c, the netId of the husband / wife (the start couple point at each other) */
            int parent1;            /* 0x60, the netIds of the parents (the player gets the start couple) */
            int parent2;            /* 0x64 */
            int child;              /* 0x68, the netId of a child (the start couple get the player) */
        } rel;
    };
    char pad7c[4];
    unsigned char talents[5];       /* 0x80, the five talent values (from the profile, INIT_PLAYER.talents) */
    char pad85[0x107];
    int modelId;                    /* 0x18c, the model's id (INIT_PLAYER.modelId) */
    char pad190[0x64];
    char ownName[0x1c];             /* 0x1f4, the model name (AICHAR.ownName; from INIT_PLAYER.model) */
} MAINCHARNEW;
#define main_BookObject (*(int *)((char *)&cm_RequestedSaveName + 0x44))  /* 0x6f2614, the book object "Buch" loaded at the game start (reasoned: a pointer-sized field after the 64-byte name and the int at +0x40) */
#define main_LastLoadProgress (*(int *)((char *)&cm_RequestedSaveName + 0x40))  /* 0x6f2610, the last loading progress the callback drew (below the book object at +0x44) */
extern int main_SoundBanks[0x80];   /* 0x75df20 */
extern char cm_RequestedSaveName[72];
extern float gm_DayStartHours[], gm_DayEndHours[];  /* 0x617830 / 0x617840 (f3gm.c) */
extern int main_NoRoundFade;        /* 0x6f25c0, 1 skips the black fade before the round page */
extern unsigned char main_GameEnded;  /* 0x6f25ad, sv.c: nonzero when the player reached the game's goal */
extern int main_WindBank;           /* 0x75e630, the wind sound bank */
extern int sv_FrameMask;
extern OPTIONBYTES ai_GamePanel;
extern char ai_MissionMode;
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
extern void (*he_RunHandlers[5544])(unsigned char *);
extern int sv_PopRequest;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;


extern int ai_NextCharNumber;
extern GAPDATA full_data_92bca8[];
extern unsigned char he_Events[339968];
extern char * he_QuickList[10921];
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
extern GAPDATA full_data_6024e4[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_MissionMode;


extern GAPDATA full_data_75e401[];
extern INIT_PLAYER init_PlayerProfile;
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
extern int sv_FrameMask;
extern int sv_PopRequest;

extern OPTIONBYTES ai_GamePanel;
extern char gm_TagTmp[4];
extern int cm_NetFlags;


extern int init_StatusForm;
extern int sv_FrameMask;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern char main_MoviePath[0x104];   /* 0x624798, the movie directory */
extern int main_Movie;               /* 0x6f241c, the playing movie's id */
extern int di_Disabled;
extern unsigned char snd_AmbientOn;     /* 0x6f0b60, 1: ambient sounds on */
extern unsigned char ai_GfxResolution;  /* 0x6f2f94 (menu.c: OPTIONBYTES): @original main_OpenScreen reads it as a byte object (0x531d43 mov al, byte ptr [0x6f2f94]); through the 4-byte union VC6 loads the dword and masks it (and eax, 0xff: 264 instead of 226 differing instructions) */
extern char ai_OptionsFile[288];
extern int ai_ScreenModeH;
extern int ai_ScreenModeW;
extern struct SCREENMODE ai_ScreenModes[];





extern GAPDATA full_data_5f320c[];
extern GAPDATA full_data_602568[];
extern unsigned char main_GfxPath[];
extern char main_GamePath[];
extern GAPDATA full_data_624cf8[];
extern int main_TextureDivider;
extern int main_Inactive;       /* 0x6f2584, 1 while the app is deactivated */
extern char main_AppDir[];
extern INIT_PLAYER init_PlayerProfile;
extern int main_MoneyCheat;
extern int sv_FrameDepth;
extern int sv_PopRequest;

#define U_I(a) (*(int *)(a))

extern HWND main_Window;

void main_EndGame(void);
void main_CloseLoadingScreen(void);
void main_RundenBeginn(void);
void main_EndRound(void);
void main_RoundReport(void);

void main_SettleRound(unsigned short pl);
void main_PlayOutro(void);

extern unsigned int d3anim_CacheMax;   /* 0x5f927c, d3_anim.h */
void ai_ClearTurnTable(void);           /* 0x587de0, sv_t.c: not in sv_t.h (adding it there moves sv_t's drafts ai_TutorialInitChapter3 / ai_TutorialInitChapter5) */
int main_PreloadSound(char *f);         /* 0x537c70 */
#define d2_MoveObjS ((void (*)(short, short, int))d2_MoveObj)  /* 0x46cda0 as its definition reads it (d2.c f_46cda0.c: short x, short y): this unit's callers push the short sums untruncated (0x532212 `sub dx, [..]; sub edx, 0x30; push edx`); d2.h keeps int for d2_CreatePicObj (@original) */
#define util_RandomInt ((int (*)(unsigned short))util_Random)  /* 0x42ec00 as tr.c defines it: @original this unit reads the full int result (0x53b8c9 util_Random(4) + 0x20 pushed without the 16-bit mask ai.h's unsigned short prototype adds) */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* the 4-argument call (as menu.h) */
#define d2_SetObjValue2 ((void (*)(int, int))d2_SetObjValue)  /* 0x475cb0: @original the text-only calls push two (0x532c9d main_EnterHostAddress: push buf; push obj; 0x53649d / 0x5364ac main_ChoosePlayer: push 0x75e5aa / 0x75e5ca; push obj) */
int main_ChooseCity(int unused, int host);  /* 0x537230, the player setup / city choice of a new game ("Menu\CHOOSECITY", the start menu page "Startmenu/A_Stadtwahl.esc"; host = 1 for a network host (the list of ".NET" files, else ".CTY"): lists the city files
   "gamedata/cities/*" (0x5d33b0: up to 64 records of 0x210 bytes, the path at +0x109), reads each header (0x51cee0, an LSHEADER: +5 the name, from which the city point is created (ai_CreateCityPoint, its dummy "dummy_<NAME>"), +0x30 slotByte the player limit; diff[i] = limit - players (0x62489c)),
   creates the name object of the city (0x59a2c0 "stadt_<name>", the text plate 0x52e1c0 from the table 0x143a1c0 by the key "_STADTAUSWAHL_<NAME>_INFO+0" (0x42e770)), puts the city tower "sp_STADTTURM" (0x446e10,
   "sonstiges\wimpel_STADTTURM.baf") over the picked city (the pick object 0x448630 with a name starting with "stadt_" selects: sel; the default is the object of the 5th city) and shows its description
   ("_STADTAUSWAHL_<NAME>_BESCHR+0"); the buttons: ok (or enter / 0x1c) goes on to main_ChooseDifficulty and main_ChooseHistory (host: ends at once), the second button opens the opponent
   selection main_ChooseOpponents (diff of the picked city); returns 1 when a game was set up (the second parameter is the host flag, the first unused) */
struct D3OBJECT;
void main_FreeSoundBanks(void);         /* 0x537db0 */

#include "f3init.h"
#include "sv.h"
#include "mem.h"
#include "d3_d3d.h"
#include "io_text.h"
#include "d3_snd.h"
#include "d3_sky.h"
#include "d3_io_t.h"
#include "anm.h"
#include "d3_tools.h"
#include "ahm.h"
#include "gv.h"
#include "cm.h"
#include "amt.h"
#include "d3_anim.h"
#include "d3_io_a.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "f3snd.h"
#include "sblib.h"
#include "d3_pose.h"
#include "sv_t.h"
#include "lobby.h"
#include "am.h"
#include "ch.h"
#include "chc.h"
#include "hmt.h"
#include "hst.h"
#include "mission.h"
#include "rmsg.h"
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_ExecuteBuyRoom / ai_ExecuteBuyMarketObject) */
/* 0x531670, destroys the game window and unregisters its class "Die Gilde" */

void main_DestroyWindow(void)
{
    DestroyWindow(main_Window);
    UnregisterClassA("Die Gilde", (HINSTANCE)main_Instance);
}
/* 0x531690, shuts the screen / render side down: closes the video mode flag, marks the d2 layer active, frees the item
   list surfaces, saves the options, exits the 2D layer, deselects the scene, frees the snow mipmaps and shuts the 3D
   engine down, then releases the direct input devices */

void main_ShutdownScreen(void)
{
    main_ScreenOpen = 0;
    d2_CutsceneActive = 1;
    d2_FreeItemListSurfaces();
    ai_SaveOptions();
    d2_Exit();
    d3if_SelectScene(0, 1);
    d3t_FreeSnowMipmaps();
    d3_Shutdown();
    di_Exit();
}
/* 0x5316d0, shuts the game modules down in reverse order of their start (0x520f10 unregisters the tick callback of the sv screens) */

void main_ShutdownModules(void)
{
    ai_SaveOptions();
    gm_FreeIconNames();
    main_FreeSoundBanks();
    evt_Shutdown();
    sv_UnregisterTick();
    if (snd_SfxEnabled != 0) {
        sb_Exit();
        d3snd_Exit();
    }
    if (snd_MusicEnabled != 0)
        msx_Exit();
    if (snd_SfxEnabled != 0) {
        sfx_Shutdown();
        d3sndw_Exit();
    }
    he_SaveMessageFilter();
    txt_FreeFiles();
    gm_Free();
    ai_FreeSettings();
    ahm_FreePrototypes();
    hmt_Exit();
    gv_FreeEffectObjects();
    ch_Close();
    ai_ResetMessages();
}
/* 0x531750, the exit handler (registered with 0x4220b0 by 0x531ba0): module shutdown, window shutdown, engine shutdown, window class, and restores the screen saver setting (0x531b80 (1)) when it was switched off */

void main_Exit(void)
{
    main_ShutdownModules();
    main_ShutdownScreen();
    main_ShutdownEngine();
    main_DestroyWindow();
    if (main_PlatformNT == 0 && main_ScreenSaverOff != 0)
        main_SetScreenSaver(1);
}

#define SCREEN ((struct D2SURF *)d2_ScreenSurf)
#define BACK ((struct D2SURF *)d2_BackSurf)

/* 0x531780, the window procedure of "Die Gilde": WM_DESTROY / WM_CLOSE post the quit message, WM_PAINT / WM_MOUSEWHEEL validate the window, WM_SYSCOLORCHANGE revalidates and repaints the screen
   (d2_BlitPart copies the back surface 0x6ed92c onto the screen surface 0x6ed924) when both surfaces are set, WM_ACTIVATEAPP restores the screen and reacquires the sound on activation (after
   the flip waits) and releases everything on deactivation (window to the bottom of the z order), 0x218 with wparam 0 answers the magic 0x424d5144 ("DQMB"), 0x7e7 reacquires the sound;
   everything else goes to DefWindowProc. The surfaces are read from their globals at every use, like the original */
LRESULT CALLBACK main_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case 0x2:
    case 0x10:
        PostQuitMessage(0);
        return 0;
    case 0xf:
        ValidateRect(hwnd, 0);
        return 0;
    case 0x14:
        ValidateRect(hwnd, 0);
        if (SCREEN != 0 && BACK != 0)
            d2_BlitPart(0, 0, SCREEN->w, SCREEN->h, BACK, 0, 0, SCREEN);
        return 1;
    case 0x1c:
        if (wparam != 0) {
            if (main_Inactive != 0) {
                if (main_ScreenOpen != 0) {
                    while (dd_Restore() == 0)
                        ;
                    while (d3_Hardware != 0 && d3d_Restore() == 0)
                        ;
                    d2_FlushDirty();
                    if (d2_LockSurface(SCREEN) != 0) {
                        d2_BlitPart(0, 0, SCREEN->w, SCREEN->h, BACK, 0, 0, SCREEN);
                        d2_DrawObjects();
                        d2_UnlockSurface(SCREEN);
                        dd_Flip();
                        d2_BlitPart(0, 0, SCREEN->w, SCREEN->h, BACK, 0, 0, SCREEN);
                    }
                    tb_Paused = 0;
                    sfx_ReacquireAll(hwnd, 0x7e7);
                    di_AcquireMouse(1);
                    di_AcquireKeyboard(1);
                }
                main_Inactive = 0;
            }
        }
        else {
            if (main_Inactive != 1) {
                main_Inactive = 1;
                if (main_ScreenOpen != 0) {
                    di_AcquireMouse(0);
                    di_AcquireKeyboard(0);
                    sfx_ReleaseAll();
                    tb_Paused = 1;
                }
                SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, 3);
            }
        }
        return 0;
    case 0x20a:
        ValidateRect(hwnd, 0);
        return 1;
    case 0x218:
        if (wparam == 0)
            return 0x424d5144;
        break;
    case 0x7e7:
        sfx_Reacquire(wparam, hwnd, 0x7e7);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}
/* 0x531a00, the version string: the build date literal "Oct  1 2003" is split at the blanks (strtok) into up to three tokens and the month token is looked up in the month table (Jan .. Dec); the
   index is not used afterwards. Writes "Europa 1400 - Gold Edition - 2.06" to out. The original reserves 0x164 bytes of locals, the month index is dead (date is a 0x100 byte buffer, reasoned from the frame) @deep body complete, frame 0x164 matched (date is char[0x100] + inline strcpy of the literal, sep char[2], months[12][4], tok[3][16] by the frame/offsets), prologue copies and the strcmp/sprintf tail identical (asmdiff: 43 equal ins at the head, 47+ at the tail). Left (13 ins, one hunk at 0x531abb): the token copy loop. The original hoists 'lea esi,[tok]' after the je of the first strtok result and copies with mov edx,esi; mov ecx,eax; add esi,0x10; mov al,[ecx]; inc ecx; mov [edx],al; inc edx; test al,al (src kept in ecx, p advanced before the copy); ours puts lea esi before the test and uses the 'sub edx,eax; mov cl,[eax]; mov [edx+eax],cl; inc eax' dst-src idiom. Tried: tok[n]/n++, char (*p)[0x10] / p++, char *p with p += 0x10 after or before the strcpy (via a q copy, which moved the add before the loop as in the original but not the idiom), p = tok placed before / after the first strtok */

void main_BuildVersionString(char *out)
{
    char sep[2] = " ";
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char date[0x100];
    char tok[3][0x10];
    char *t;
    char *p, *q;
    int i;

    strcpy(date, "Oct  1 2003");
    t = strtok(date, sep);
    p = tok[0];
    while (t != 0) {
        q = p;
        p += 0x10;
        strcpy(q, t);
        t = strtok(0, sep);
    }
    for (i = 0; i < 12; i++) {
        if (strcmp(tok[0], months[i]) == 0)
            break;
    }
    sprintf(out, "Europa 1400 - Gold Edition - 2.06");
}
/* 0x531b50, creates the single instance mutex "Die Gilde" (the handle stored in *mutex); returns 1 when it already existed (ERROR_ALREADY_EXISTS), 0 when new, -1 on failure */

unsigned main_CreateMutex(void **mutex)
{
    HANDLE h;

    h = CreateMutexA(0, 1, "Die Gilde");
    if (h != 0) {
        *mutex = h;
        return GetLastError() == 0xb7;
    }
    return 0xffffffff;
}
/* 0x531b80, switches the screen saver (SPI_SETSCREENSAVEACTIVE 0x11) on / off */

void main_SetScreenSaver(unsigned on)
{
    SystemParametersInfoA(0x11, on, 0, 2);
}
/* 0x531ba0, the first init: registers the exit handler (0x4220b0 (0x531750, 0x44ee40, the window, 0x10)), sets 0x4226a0 (48000) / 0x42ea90 (0x80), creates "<path>gamedata\Screenshots" and starts the
   project gfx path (0x431490) */

int main_Init(void)
{
    char dir[260];

    err_Init((int)main_Exit, d3io_NoopCallback, main_Window, 0x10);
    mem_Init(48000);
    tr_startup(0x80);
    sprintf(dir, "%sgamedata\\Screenshots", (char *)(unsigned)&main_GfxPath);
    CreateDirectoryA(dir, 0);
    vfs_Init((char *)(unsigned)&main_GfxPath, 0);
    tb_Init(0xe, 0);
    return 1;
}
/* 0x531c20, the engine shutdown (the low level parts) */

void main_ShutdownEngine(void)
{
    tb_Exit();
    vfs_Exit(1);
    tr_shutdown();
    mem_Exit();
    err_Exit();
    mov_Exit();
}

int sv_PumpMessages();

/* 0x531c50, opens the 3D / 2D screen for the display mode m (3 and 4 = full screen windows, topmost): picks a supported resolution (0 = 800x600, 1 = 1024x768, 2 = 1152x864: the chosen one is
   kept when its mode exists, else the first of the others that does), sizes and centres the window on the work area, fills the video record (the DXT1 texture format 0x31545844, the screen size,
   the Gfx/TextureDivider key, the view scale 512 / (640 / width)) for d3_Init, starts the input (full screen mode 3: exclusive keyboard), sets the directories ("animations/", "objects/",
   "groups/", "scripts/", "forms\\") and opens the 2D layer with "gilde_add_on_<language>.gfx"; returns 1 on success. Reasoned: when AdjustWindowRectEx fails the original takes the window
   exstyle for both the width and the height @deep body reconstructed and readable (video record = d3_engine.h's D3INITCFG, its byte-read fields dwords, resolution fallback chain 800 -> 1024 -> 1152, window sizing with the original's failure path w = h = GetWindowLong(EXSTYLE), view scale 512 / (640 / (double)(unsigned)w), directories, d2_Open); 337 = 337 instructions. Left (asmdiff, 226 ins in 10 hunks, allocation / block layout): (1) the original keeps the exstyle in a stack slot [esp+0x10] (reloaded for the AdjustWindowRectEx argument and for both w and h on failure) and caches GetWindowLongA's import in ebp (call ebp twice), work.right-left / bottom-top in esi / edi, all four loaded right after SystemParametersInfoA; ours keeps ex in esi, loads the work fields in another order and calls through the thunk twice; (2) x / y = work - w / h computed late (first form) gives the original's shape but moves the video record 8 bytes (the 64-bit fild temp of the scale shares the ex slot in the original), computed early (current form) keeps the frame but not the schedule; (3) the resolution chain: the original lays out 'res==0 && 800 ok -> done; 1024 ? al=1 : 1152 ? al=2' inline right after the first test and a shared 'xor al,al' tail for the 800 fallback, ours (gotos pick1024 / try800) emits the blocks in another order (hunks at 0x531d4e / 0x531d70); (4) after d3_Init the three-register rotation of the input / path set-up (regs eax->edx ecx->eax edx->ecx, 20 ins). Data fixes made: ai_DataPath / d3_AnimDir / d2_FormsDir typed as char[], ai_ScreenModes as {w,h}[], full_data_6f242c -> main_TextureDivider, full_data_624690 -> main_GamePath, the 'groups/' literal views removed */
int main_OpenScreen(int m)
{
    char path[260];
    RECT work;
    D3INITCFG cfg;
    RECT rc;
    int w, h, x, y, ex;

    cfg.w = 0;
    cfg.h = 0;
    cfg.bpp = 0x10;
    cfg.mode = 1;
    cfg.d3d = 1;
    cfg.triple = 0;
    cfg.maxpolys = 0x10000;
    cfg.texcount = 0x1000;
    cfg.maxhicol = 600;
    cfg.texParam = 8;
    cfg.zbpp = 0x10;
    cfg.texbpp = 0;
    cfg.flFlag = 0;
    cfg.flShift = 0;
    cfg.flArg = 0;
    cfg.texShift = 0x20;
    cfg.shadowSize = 0x284;
    cfg.viewW = 456.0f;
    cfg.viewH = 78.0f;
    cfg.viewX = 50.0f;
    cfg.viewY = 4.0f;
    cfg.zn = 12000.0f;
    cfg.zf = 512.0f;
    cfg.scale = 0.0f;
    ai_LoadOptions();
    if (dd_CheckModes(m, 0x10, main_Window, (int)main_Instance) == 0)
        return 0;
    if (dd_Has800x600 == 0 && dd_Has1024x768 == 0 && dd_Has1152x864 == 0)
        return 0;
    if (ai_GfxResolution == 0) {
        if (dd_Has800x600 != 0)
            goto sized;
pick1024:
        if (dd_Has1024x768 == 0) {
            if (dd_Has1152x864 != 0)
                ai_GfxResolution = 2;
        } else
            ai_GfxResolution = 1;
    } else if (ai_GfxResolution == 1) {
        if (dd_Has1024x768 != 0)
            goto sized;
        goto try800;
    } else if (ai_GfxResolution == 2) {
        if (dd_Has1152x864 != 0)
            goto sized;
try800:
        if (dd_Has800x600 == 0)
            goto pick1024;
        ai_GfxResolution = 0;
    } else
        goto sized;
    ai_ScreenModeW = ai_ScreenModes[ai_GfxResolution].w;
    ai_ScreenModeH = ai_ScreenModes[ai_GfxResolution].h;
sized:
    sv_ViewRectLeft = 0;
    sv_ViewRectTop = 0;
    sv_ViewRectRight = ai_ScreenModeW - 0x6a;
    sv_ViewRectBottom = ai_ScreenModeH;
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
    x = work.right - work.left;
    y = work.bottom - work.top;
    rc.left = 0;
    rc.top = 0;
    rc.right = ai_ScreenModeW;
    rc.bottom = ai_ScreenModeH;
    ex = GetWindowLongA(main_Window, GWL_EXSTYLE);
    w = h = ex;
    if (AdjustWindowRectEx(&rc, GetWindowLongA(main_Window, GWL_STYLE), 0, ex)) {
        w = rc.right - rc.left;
        h = rc.bottom - rc.top;
    }
    x -= w;
    y -= h;
    if (m == 3 || m == 4)
        SetWindowPos(main_Window, HWND_TOPMOST, abs(abs(y) / 2), abs(abs(x) / 2), w, h, 0);
    else
        SetWindowPos(main_Window, 0, abs(y) / 2, abs(x) / 2, w, h, 0);
    sv_PumpMessages();
    cfg.w = ai_ScreenModeW;
    cfg.h = ai_ScreenModeH;
    cfg.viewW = (float)ai_ScreenModeW;
    cfg.viewH = (float)ai_ScreenModeH;
    cfg.zbpp = 0x20;
    cfg.shadowSize = 0x20;
    d3_DefaultD3DConfig[1] = (d3_DefaultD3DConfig[1] & 0xf9) | 1;
    cfg.texShift = main_TextureDivider;
    cfg.bpp = 0x10;
    cfg.mode = m;
    cfg.d3d = 1;
    cfg.triple = 0;
    cfg.viewX = 0.0f;
    cfg.viewY = 0.0f;
    cfg.flFlag = 1;
    cfg.flShift = 0;
    cfg.flArg = 0;
    cfg.texParam = 4;
    cfg.texbpp = 0x31545844;
    cfg.maxpolys = 64000;
    cfg.texcount = 0x1000;
    cfg.maxhicol = 0;
    cfg.zn = 4.0f;
    cfg.zf = 12000.0f;
    d3io_Lod.all = 2;
    cfg.scale = (float)(512.0 / (640.0 / (double)cfg.w));
    if (d3_Init(&cfg, main_Window, (int)main_Instance) != 0) {
        d3if_CreateCameras();
        while (ShowCursor(0) > 0)
            ;
        if (cfg.mode == 3) {
            di_Init(main_Window, main_Instance, 1);
            kb_SetBase(0x50);
        } else
            di_Init(main_Window, main_Instance, 6);
        ai_ApplyOptions();
        tex_SetPalette("textures/");
        strcpy(d3_AnimDir, "animations/");
        d3io_SetStockDir("objects/");
        strcpy(ai_DataPath, "groups/");
        strcpy(evt_Path, "scripts/");
        sprintf(d2_FormsDir, "forms\\");
        ai_ApplyGfxOptions();
        sprintf(path, "gilde_add_on_%s.gfx", txt_Language);
        if (d2_Open((int)main_Window, ai_ScreenModeW, ai_ScreenModeH, main_GamePath, path) != 0) {
            di_MaxY = d2_ScreenH;
            di_MaxX = d2_ScreenW - 8;
            d2_MainWindow = (int)main_Window;
            d2_CreateItemListSurfaces();
            sv_PumpMessages();
            return 1;
        }
    }
    return 0;
}

#define SB_MEMORY 10000000     /* sb_Init's buffer size in bytes (a constant of the original) */
#define ANIM_CACHE_BYTES 0x400000   /* 4 MB: d3anim_CacheMax is set again here */

int sv_PumpMessages();
int sv_RunFrame(int cb, unsigned mask);

/* 0x532160, main_OpenEngine: loads the text definitions ("<GamePath>die_gilde_add_on_text.def": txt_LoadIncludes, or txt_OpenText when main_TextReopen is set), shows the loading screen form
   "Misc\Loading_main" (window 1 above the bottom edge, window 0 at the top of the screen) while the history lists, the net states, the data tables, the effect slots, the icon names and the item
   prices are initialised, starts the sound (sound bank, 3D sound, "system.sbf", the "include_sfx.ini" of the program directory) and the music, then the game modules (sv's tick callback,
   scene universes, characters, snow mipmaps, the AI, the hmt tables), waits for the black fade ("BLACK") by running the frame loop 0x5309a0, clears the player profile and registers the
   event commands; returns 1. Reasoned: the item prices are called only for their (discarded) results' side effects, the order follows the original */
int main_OpenEngine(void)
{
    char tmp[260], tmp2[260];
    int f;
    struct D2FADE *fade;

    d2_CutsceneActive = 1;
    if (main_TextReopen != 0) {
        sprintf(tmp, "%s%s", main_GamePath, "die_gilde_add_on_text.def");
        txt_OpenText(tmp, 0);
    } else {
        sprintf(tmp, "%s%s", main_GamePath, "die_gilde_add_on_text.def");
        if (txt_LoadIncludes(tmp) == 0)
            err_Message("main_OpenEngine(): could not open txt-engine!");
    }
    hst_ResetLists();
    d2_ClearScreen();
    f = d2_OpenForm(0, 0, "Misc\\Loading_main");
    d2_PlaceWindow(d2_Views[f].wins[0], 3);
    d2_MoveObjS(0, d2_ScreenH - d2_Wins[d2_Views[f].wins[1]].h - 0x30, d2_Wins[d2_Views[f].wins[1]].parent);
    d2_PlaceWindow(d2_Views[f].wins[1], 1);
    io_PrintText("$ADie Gilde - Gold Edition - Version 2.05$X");
    net_Connect(0, 0);
    ai_InitStates();
    ai_ClearTurnTable();
    sprintf(tmp, "%sdata\\", main_GamePath);
    gm_LoadTables(tmp);
    unk_ResetEffectSlots();
    unk_LoadIconNames();
    ai_FreeSettings();
    gm_ItemPrice(0x1d5, 100);
    gm_ItemPrice(0x1d7, 100);
    gm_ItemPrice(0x1d4, 100);
    gm_ItemPrice(0x1da, 100);
    gm_ItemPrice(0x1d9, 100);
    gm_ItemPrice(0x1d6, 100);
    gm_ItemPrice(0x1d8, 100);
    if (snd_SfxEnabled != 0) {
        sfx_Startup();
        if (sb_Init(SB_MEMORY, 0x30, 2, 0x10, 0xac44) != 0) {
            snd_SfxEnabled = 0;
            snd_MusicEnabled = 0;
        }
        d3snd_Init(0x40);
        d3sndw_Init(0x30);
        sprintf(tmp, "%ssfx\\", main_GamePath);
        sb_SetPath(tmp);
        snd_SystemBank = sb_LoadSampleBank("system.sbf", 0);
        sprintf(tmp2, "%s\\include_sfx.ini", main_AppDir);
        main_PreloadSound(tmp2);
    }
    if (snd_MusicEnabled != 0) {
        msx_Init(2, 0x10, 0xac44);
        sprintf(tmp, "%smsx\\", main_GamePath);
        msx_SetPath(tmp);
    }
    ai_ApplyVolumes();
    sv_InitTick();
    init_ResetSceneUniverses();
    ch_Open();
    d3t_CreateSnowMipmaps();
    ai_Init();
    ahm_LoadPrototypes(main_HmtMode);
    if (main_HmtBuild != 0)
        hmt_CreateBinaryFile();
    main_HmtBuild = 0;
    hmt_Init();
    cm_InitOrderChances();
    d2_CloseView(f);
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
    while ((fade->flags & 4) == 0)
        sv_RunFrame(0, 0x24087);
    d2_ReleaseFade(fade);
    D3_ZERO_AT(offset init_PlayerProfile, 0x8c);  /* the original's inline-asm clear (it makes the ebp frame) */
    main_ScreenOpen = 1;
    evt_Init();
    evtc_Register();
    ocmd_Register();
    chc_Register();
    sndc_Register();
    d3e_Resolve = evt_GetStartScript;
    d3anim_CacheMax = ANIM_CACHE_BYTES;
    ai_ApplyOptions();
    sv_PumpMessages();
    return 1;
}
/* 0x5324f0, registers the window class "Die Gilde" (the window procedure main_WindowProc, the icon 0x65, the arrow cursor, a black brush) and creates and shows the game window (title " <version text>"
   built by main_BuildVersionString into the program's version buffer at ls_SavePath + 0x120; a full screen topmost popup for the mode 3, else a captioned window that is resized so its client area is
   ai_ScreenModeW x ai_ScreenModeH and centred on the work area); returns 1 on success @deep body complete and readable (class registration, the ' %s' title from main_BuildVersionString's buffer at ls_SavePath + 0x120, work area / mode 3 popup, client-area resize + centre with abs(..)/2); 178 vs 175 ins. Left (asmdiff 181 ins, all allocation): frame 0xcc vs 0xc8 and wc at [esp+0x20] vs [esp+0x1c]: ours spills exs (the extended style) to a 4-byte stack slot ([esp+0x10]) because it holds work.right in ebx and work.bottom in ebp and the zero in esi; the original keeps exs in ebx (xor ebx,ebx doubles as the zero constant for every push 0 / mov [wc.x],ebx), only work.right in ebp (mode 3 reloads work.bottom with mov edi,[esp+0x1c]) and w / h in esi / edi. Tried: all 8 declaration orders of wc / rc / work / title (no effect, VC6 sorts by size); the source reads exactly as ghidra/the asm do. Needs the variable-lifetime variant (e.g. which of w / h / exs are assigned before the branch) */

int main_CreateWindow(HINSTANCE inst, int unused, int mode)
{
    WNDCLASSA wc;
    RECT rc, work;
    char title[0x80];
    DWORD style, exs;
    int w, h, nw, nh;

    memset(&wc, 0, sizeof(wc));
    exs = 0;
    wc.style = 0;
    wc.lpfnWndProc = main_WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = inst;
    wc.hIcon = LoadIconA(inst, (LPCSTR)0x65);
    wc.hCursor = LoadCursorA(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "Die Gilde";
    if (RegisterClassA(&wc) == 0)
        return 0;
    main_BuildVersionString(ls_SavePath.version);
    sprintf(title, " %s", ls_SavePath.version);
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
    w = work.right - work.left;
    h = work.bottom - work.top;
    if (mode == 3) {
        style = WS_POPUP | WS_VISIBLE | 0x12000000;
        exs = WS_EX_TOPMOST;
        work.top = 0;
        work.left = 0;
        w = work.right;
        h = work.bottom;
    } else
        style = 0x6000000;
    main_Window = CreateWindowExA(exs, "Die Gilde", title, style, work.left, work.top, w, h, 0, 0, inst, 0);
    if (main_Window == 0)
        return 0;
    ShowWindow(main_Window, SW_SHOW);
    UpdateWindow(main_Window);
    SetFocus(main_Window);
    if (mode == 3)
        SetWindowPos(main_Window, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
    else {
        GetClientRect(main_Window, &rc);
        nh = ai_ScreenModeH - rc.bottom + h;
        nw = ai_ScreenModeW - rc.right + w;
        MoveWindow(main_Window, abs(w - nw) / 2, abs(h - nh) / 2, nw, nh, 1);
        SetWindowPos(main_Window, 0, 0, 0, 0, 0, 3);
    }
    return 1;
}
/* 0x532700, starts a city game: the scene / lists are reset, the town `name` is initialised, the city file "gamedata/cities/<name>.CTY" (".NET" for a network game: net != 0) is written through ls_SaveGame
   after the game clock is set to the start (year 0, hour 6, minute 0), the characters, buildings and objects are cleared and the scene "scenes/*ChooseCity.ed3" (language 0) or
   "scenes/*ChooseCity_Europa.ed3" is loaded */

void main_StartCityGame(char *name, int net)
{
    char path[260];

    d3if_SelectScene(0, 0);
    d3if_Reset();
    init_InitTown(name);
    if (net != 0)
        sprintf(path, "%s/%s.NET", "gamedata/cities", name);
    else
        sprintf(path, "%s/%s.CTY", "gamedata/cities", name);
    ai_Time.year = 0;
    ai_Time.hour = 6;
    ai_Time.min = 0;
    ls_SaveGame(path, "city", 2);
    gm_ResetCharacters();
    gm_ClearBuildings();
    gm_ClearObjects();
    init_FreeScenes();
    d3if_SelectScene(0, 0);
    d3if_Reset();
    if (ls_GetLanguage() == 0) {
        d3_LoadScene("scenes/*ChooseCity.ed3", 0, 0);
        return;
    }
    d3_LoadScene("scenes/*ChooseCity_Europa.ed3", 0, 0);
}

int sv_RunFrame(int cb, unsigned mask);

/* 0x5327e0, the player count dialog (the form "Menu\CHOOSE_PLAYERCOUNT"): a count slider (at least 2, started from ai_NumPlayers), the heading text 0x1ab5 and the "%s$A%s" texts 0x7d / 0x7e; runs the
   frame loop 0x5309a0 until a button is pressed (button 0 = ok: stores the slider value in ai_NumPlayers and returns 1, right mouse button / button 1 = cancel); returns 1 for ok, 0 for cancel @deep body complete, 87 = 87 instructions, readable (d2 / dm_L / sv_PopRequest typed, slider / form names, the 4-argument d2_SetObjValue4 call, the right mouse button dm_L.rup cancels). One hunk left (14 ins at 0x53280d): the original clamps the player count as 'mov al,[ai_NumPlayers]; cmp al,2; mov byte [ai_NumPlayers],2; jb L; mov [ai_NumPlayers],al; L: mov eax,[ai_NumPlayers]; and eax,0xff; push eax' (immediate pre-store, then a reload of the global as dword + mask for the d2_SetObjValue argument); ours keeps the value in al / cl ('jae; mov al,2; mov [x],al; xor ecx,ecx; mov cl,al'). Tried: if (n < 2) n = 2; the ternary assignment; cnt local copy with ai_NumPlayers = 2 stored first and re-stored when cnt >= 2 (89 ins, worse); a char bitfield view of ai_NumPlayers (the dword + and 0xff read does not appear); & 0xff on the argument */
int main_PlayerCountDialog(void)
{
    int ok, form, slider, run;

    ok = 0;
    form = d2_OpenForm(0, 0, "Menu\\CHOOSE_PLAYERCOUNT");
    d2_CentreViewWindows(form);
    slider = d2_GetWinObj(form, 0, 0);
    if (ai_NumPlayers < 2)
        ai_NumPlayers = 2;
    d2_SetObjValue4(slider, 2, 8, ai_NumPlayers);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 1);
    io_PrintText(0x1ab5);
    d2_SetForm(form, 2);
    io_PrintText("%s$A%s", 0x7d, 0x7e);
    run = sv_RunFrame((int)main_PlayerCountDialog, 0x24087);
    while (run != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 0) {
            ai_NumPlayers = d2_GetObjValue(slider);
            d2_SetViewEnabled(form, 0);
            ok = 1;
            sv_PopRequest = 1;
        }
        run = sv_RunFrame((int)main_PlayerCountDialog, 0x24087);
    }
    if (form != -1)
        d2_CloseView(form);
    return ok;
}

extern int d2_PointerObj, d2_LastWindow;

/* 0x5328f0, hosts a network game: the player count dialog (main_PlayerCountDialog), the player name / city choice (main_ChooseCity (1, 1)), then the network game record in ls_SavePath (106 bytes at
   +0xa0: the record size 0x6a, two words of 1, the city name from ai_DataPath + 0x220, the player count, the word at +0x84) is filled, the save name "<gamedata/cities>/<city>.net" and the host address
   127.0.0.1 are set, the network flag 0x10 of ai_GameFlags is raised and the record is copied to main_NetGameRecord; the view is set up (d3_SetViewport) and the mode mask 0x3f stored; returns 1,
   or 0 when cancelled (the screen is cleared and picture 0x797 shown) */
int main_HostNetworkGame(void)
{
    int i;
    char c;

    if (main_PlayerCountDialog() != 0) {
        ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
        if (main_ChooseCity(1, 1) != 0) {
            sprintf(main_NetSaveName, "%s/%s.net", "gamedata/cities", ai_DataPath + 0x220);
            strcpy(main_NetHost, "127.0.0.1");
            ai_GameFlags |= 0x10;
            ls_SavePath.net.players = ai_NumPlayers;
            ls_SavePath.net.size = 0x6a;
            ls_SavePath.net.a = 1;
            ls_SavePath.net.b = 1;
            ls_SavePath.net.x = ls_SavePath.cur;
            i = 0;
            do {
                c = ai_DataPath[0x220 + i];
                ls_SavePath.net.name[i] = c;
                i++;
            } while (c != 0);
            memcpy(&cm_NetGame.hdr, &ls_SavePath.net, sizeof(NETHDR));
            cm_NetGame.mode = 0x3f;
            d3_SetViewport((float)(sv_ViewRectRight - sv_ViewRectLeft), (float)(sv_ViewRectBottom - sv_ViewRectTop), (float)sv_ViewRectLeft, (float)sv_ViewRectTop, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
            return 1;
        }
    }
    d2_ClearSurface(d2_ScreenSurf);
    d2_ShowPicture(0x797);
    return 0;
}
/* 0x532a70, joins a saved network game: asks for the game (ai_JoinGameWindow), opens "gamedata/network/<name>.SAV" (vfs_Tfopen, "rb"), reads its header (ls_ReadSaveHeader) and fills
   the network game record ls_SavePath.net (its save name and party table) and cm_NetGame from it, the host 127.0.0.1, the network flags 0x50 and the joined character cm_JoinChar
   (version >= 0x10054: the second text and two more fields); returns 1 on success @deep body read against the original and typed (LSHDR 0xfc + path[260] give the 0x200 frame exactly; NETHDR.save/party, the cm_NetGame.hdr struct copy, the CMJOIN view at 0x9367a0); every call, store and constant matches. Left is scheduling: ours loads ls_SavePath.cur into ax first and stores net.x at once (0x532acc/0x532ade) where the original loads it late into dx and stores it after net.b (0x532b34), so ai_GameFlags is or-ed in eax (or al,0x50) not ecx (or ecx,0x50, 4 bytes longer in all); ours hoists the hdr.char164 load above the first rep movsd (mov edx,[esp+0x6c] at 0x532b6d) where the original loads it after the rep stosd; the char0c/char54 pair is a pure eax->ecx ecx->edx edx->eax rotation. Tried: the original's store order (players before the flag, mode between char164 and char09): byte-neutral. */

int main_JoinNetworkGame(void)
{
    LSHDR hdr;
    char path[260];
    VFSFILE *f;
    int i;
    char c;

    if (ai_JoinGameWindow() != 0) {
        sprintf(path, "gamedata/network/%s.SAV", main_NetSaveName);
        f = vfs_Tfopen(path, "rb", (int)vfs_Root);
        if (f != 0) {
            ls_ReadSaveHeader((int)f, (int *)&hdr);
            strcpy(main_NetHost, "127.0.0.1");
            ai_NumPlayers = hdr.players;
            ls_SavePath.net.players = hdr.players;
            ai_GameFlags |= 0x50;
            ls_SavePath.net.size = 0x6a;
            ls_SavePath.net.a = 1;
            ls_SavePath.net.b = 4;
            ls_SavePath.net.x = ls_SavePath.cur;
            i = 0;
            do {
                c = hdr.player[i];
                ls_SavePath.net.name[i] = c;
                i++;
            } while (c != 0);
            i = 0;
            do {
                c = main_NetSaveName[i];
                ls_SavePath.net.save[i] = c;
                i++;
            } while (c != 0);
            memcpy(ls_SavePath.net.party, hdr.party, sizeof(hdr.party));
            memcpy(&cm_NetGame.hdr, &ls_SavePath.net, sizeof(NETHDR));
            memset(&cm_JoinChar, 0, sizeof(CMJOIN));
            cm_JoinChar.char164 = hdr.char164;
            cm_NetGame.mode = 0x7f;
            cm_JoinChar.char09 = hdr.char09;
            i = 0;
            do {
                c = hdr.charText1[i];
                cm_JoinChar.text1[i] = c;
                i++;
            } while (c != 0);
            if (hdr.version >= 0x10054) {
                i = 0;
                do {
                    c = hdr.charText2[i];
                    cm_JoinChar.text2[i] = c;
                    i++;
                } while (c != 0);
                cm_JoinChar.char0c = hdr.char0c;
                cm_JoinChar.char54 = hdr.char54;
            }
            vfs_fclose(f);
            return 1;
        }
    }
    return 0;
}
/* 0x532c30, the "Menu\CHOOSENETWORK_IP" dialog: asks for the host address (the INI value [Network] Host, default 127.0.0.1), stores the entered address in 0x75e1c0 and in the INI file 0x75e480; returns 1 when
   confirmed */

int main_EnterHostAddress(void)
{
    char buf[260];
    int r, f, ed, ok, run, i;
    char *s;
    char c;

    r = 0;
    GetPrivateProfileStringA("Network", "Host", main_DefaultHost, buf, 0x104, ai_OptionsFile);
    f = d2_OpenForm(0, 0, "Menu\\CHOOSENETWORK_IP");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 2);
    io_PrintText(0x1cae);
    d2_SetForm(f, 0);
    ed = d2_GetWinObj(f, 0, 0);
    d2_SetObjValue2(ed, buf);
    d2_SetForm(f, 1);
    ok = d2_GetWinObj(f, 1, io_PrintText(0x1cb0));
    run = sv_RunFrame((int)main_EnterHostAddress, 0x24087);
    while (run != 0) {
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if ((d2_ClickedId != -1 || kb_Last == 0x1c) && (d2_PointerObj == ok || kb_Last == 0x1c)) {
            r = 1;
            s = (char *)d2_GetObjValue(ed);
            strcpy(buf, s);
            i = 0;
            do {
                c = buf[i];
                main_NetHost[i] = c;
                i++;
            } while (c != 0);
            WritePrivateProfileStringA("Network", "Host", buf, ai_OptionsFile);
            sv_PopRequest = 1;
        }
        run = sv_RunFrame((int)main_EnterHostAddress, 0x24087);
    }
    d2_CloseView(f);
    return r;
}

/* 0x532da0, the network game search window "Menu\SEARCH_NETWORK": every 1.5 s (the clock d3_Time) or on the
   refresh button (btn[1]) it scans the LAN (udp_FindServers, port 12345, 3 s, up to 8 hosts) under the message
   txt 0x1cb1 and lists each host's game: its city arms ("_STADTWAPPEN_<city>", "_STADTWAPPEN_BERLIN" without
   one), the host, the name and the players (txt 0x1cbc). saved 0 lists the new games (0x3f), saved 1 the saved
   ones (0x7f) whose "Gamedata\network\<save>.SAV" holds the same party table; a full new game is disabled. The
   join button (btn[0], enabled while one is selected) copies the chosen host into main_NetHost (the "Network"
   "Host" option) and its record into cm_NetGame and, for a saved game, the city, the save name and the joined
   character cm_JoinChar from the save's header (as main_JoinNetworkGame); btn[2] opens main_EnterHostAddress.
   Returns 1 when a game was chosen. */
void sv_ButtonRow(int f, int y, int *out, int n, ...);
int d2_RemoveWindowObjects();
void d2_RadioAdd(int g, int obj);
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
void d2_SetObjFont();
int d2_FindPic();
int sv_OpenMessageBox(int text, unsigned char flags);
int ls_ReadSaveHeader();

/* 0x532da0, the network game search window "Menu\SEARCH_NETWORK": every 1.5 s (the clock d3_Time) or on the
   refresh button (btn[1]) it scans the LAN (udp_FindServers, port 12345, 3 s, up to 8 hosts) under the message
   txt 0x1cb1 and lists each host's game: its city arms ("_STADTWAPPEN_<city>", "_STADTWAPPEN_BERLIN" without
   one), the host, the name and the players (txt 0x1cbc). saved 0 lists the new games (0x3f), saved 1 the saved
   ones (0x7f) whose "Gamedata\network\<save>.SAV" holds the same party table; a full new game is disabled. The
   join button (btn[0], enabled while one is selected) copies the chosen host into main_NetHost (the "Network"
   "Host" option) and its record into cm_NetGame and, for a saved game, the city, the save name and the joined
   character cm_JoinChar from the save's header (as main_JoinNetworkGame); btn[2] opens main_EnterHostAddress.
   Returns 1 when a game was chosen. @deep body read against the original and typed: UDPSERVER recs[8] (info.game is the NETHDR), LSHDR for both headers, one name[0x80] (the original's copy target stays fixed while the record walks), key[0x80], ent[8][3], two path[260], d2_Objs / d2_Radios / d2_Wins fields, txt_Ptrs[0x1cb1 / 0x1cbd..f]; the call sequence and every constant are identical to the original (asmdiff --all, calls and immediates diffed). Left is allocation: the original's frame is 0x990 (ours 0x988), it keeps list in [esp+0x14] (ours in ebp / [esp+0x24]), the ent row pointer in [esp+0x18] and the name copy as src + (name - &recs[i].name) in ebp stepped by -0x80 per host (ours: two pointers), and ok in [esp+0x24]. Tried: recs[i] indexed instead of a record pointer (383 -> 347 ins, kept). */
int main_SearchNetworkGame(int saved)
{
    LSHDR joined;                       /* the header of the chosen saved game */
    char name[0x80];
    char key[0x80];
    LSHDR hdr;                          /* the header of a listed saved game */
    int ent[8][3];                      /* per host: its picture object, its text window */
    char path[260];
    char path2[260];
    UDPSERVER recs[8];
    UDPSERVER *r;
    VFSFILE *fp;
    unsigned ts;
    int ret, redraw, ok, f, btn[3], h, n, i, k, cnt, list, obj, sel;
    char c;

    ret = 0;
    redraw = 1;
    ts = d3_Time;
    list = -1;
    f = d2_OpenForm(0, 0, "Menu\\SEARCH_NETWORK");
    d2_PlaceViewWindows(f, 1);
    d2_PlaceMainWindowF(f, 2);
    d2_SetForm(f, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(f, 1);
    io_PrintText(0x1ca5);
    d2_SetForm(f, 3);
    sv_ButtonRow(d2_LastWindow, 0, btn, 3, txt_Ptrs[0x1cbd], txt_Ptrs[0x1cbe], txt_Ptrs[0x1cbf]);
    d2_SetObjEnabled(btn[0], 0);
    if (sv_RunFrame((int)main_SearchNetworkGame, 0x24087) != 0) {
        do {
            if ((unsigned)d3_Time > ts + 0x5dc || redraw) {
                ok = 1;
                d2_CutsceneActive = 1;
                h = sv_OpenMessageBox((int)txt_Ptrs[0x1cb1], 0);
                n = udp_FindServers(0x3039, 0xbb8, recs, 8);
                d2_CloseView(h);
                d2_CutsceneActive = 0;
                d2_SetForm(f, 2);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                if (list != -1)
                    d2_RadioClear(list);
                list = d2_CreateRadioGroup(0);
                for (i = 0; i < n; i++) {
                    ent[i][0] = -1;
                    if ((recs[i].flags == 0x3f && saved != 0) || (recs[i].flags == 0x7f && saved == 0))
                        continue;
                    if (recs[i].flags == 0x3f) {
                        k = 0;
                        do {
                            c = recs[i].info.game.name[k];
                            name[k] = c;
                            k++;
                        } while (c != 0);
                    } else {
                        sprintf(path, "Gamedata\\network\\%s.SAV", recs[i].info.game.save);
                        fp = vfs_Tfopen(path, "rb", (int)vfs_Root);
                        if (fp == 0) {
                            ok = 0;
                        } else {
                            ok = 1;
                            ls_ReadSaveHeader((int)fp, (int *)&hdr);
                            cnt = hdr.players;
                            for (k = 0; k < cnt; k++) {
                                if (recs[i].info.game.party[k] != hdr.party[k]) {
                                    ok = 0;
                                    break;
                                }
                            }
                            vfs_fclose(fp);
                        }
                        k = 0;
                        do {
                            c = recs[i].info.game.name[k];
                            name[k] = c;
                            k++;
                        } while (c != 0);
                    }
                    if (ok == 0) {
                        ent[i][0] = -1;
                        continue;
                    }
                    _strupr(name);
                    sprintf(key, "_STADTWAPPEN_%s", name);
                    h = d2_FindPic(key);
                    if (h == -1)
                        h = d2_FindPic("_STADTWAPPEN_BERLIN");
                    d2_SetForm(f, 2);
                    obj = d2_WinOpenPic(8, i << 7, h, d2_LastWindow);
                    ent[i][0] = obj;
                    d2_Objs[obj].interactive = 1;
                    if (recs[i].flags == 0x3f && recs[i].info.game.joined >= recs[i].info.game.players)
                        d2_SetObjEnabled(obj, 0);
                    d2_Objs[obj].style = 3;
                    d2_RadioAdd(list, obj);
                    ent[i][1] = d2_CreateChildWindow(0x88, (i << 7) + 0xc, 0x190, 0x78, 0x10, d2_LastWindow);
                    d2_SetObjFont(d2_Wins[d2_LastWindow].parent, 0x53);
                    io_PrintText(0x1cbc, recs[i].ip, recs[i].info.game.name, recs[i].info.game.players);
                }
                redraw = 0;
                ts = d3_Time;
            }
            if (dm_L.rup != 0 || kb_Last == 1)
                sv_PopRequest = 1;
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == btn[0]) {
                    sel = d2_Radios[list].sel;
                    if (sel != -1) {
                        r = &recs[sel];
                        k = 0;
                        do {
                            c = r->ip[k];
                            main_NetHost[k] = c;
                            k++;
                        } while (c != 0);
                        WritePrivateProfileStringA("Network", "Host", main_NetHost, ai_OptionsFile);
                        sv_PopRequest = 1;
                        ret = 1;
                        memcpy(&cm_NetGame, r, sizeof(CMNETGAME));
                        if (saved != 0 && r->flags == 0x7f) {
                            k = 0;
                            do {
                                c = r->info.game.name[k];
                                ai_DataPath[0x220 + k] = c;
                                k++;
                            } while (c != 0);
                            k = 0;
                            do {
                                c = r->info.game.save[k];
                                main_NetSaveName[k] = c;
                                k++;
                            } while (c != 0);
                            k = 0;
                            do {
                                c = r->info.game.save[k];
                                ls_SavePath.net.save[k] = c;
                                k++;
                            } while (c != 0);
                            ai_GameFlags |= 0x40;
                            sprintf(path2, "Gamedata\\network\\%s.SAV", main_NetSaveName);
                            memset(&joined, 0, sizeof(LSHDR));
                            fp = vfs_Tfopen(path2, "rb", (int)vfs_Root);
                            if (fp != 0) {
                                ls_ReadSaveHeader((int)fp, (int *)&joined);
                                vfs_fclose(fp);
                            }
                            memset(&cm_JoinChar, 0, sizeof(CMJOIN));
                            cm_JoinChar.char164 = joined.char164;
                            cm_JoinChar.char09 = joined.char09;
                            k = 0;
                            do {
                                c = joined.charText1[k];
                                cm_JoinChar.text1[k] = c;
                                k++;
                            } while (c != 0);
                            if (joined.version >= 0x10054) {
                                k = 0;
                                do {
                                    c = joined.charText2[k];
                                    cm_JoinChar.text2[k] = c;
                                    k++;
                                } while (c != 0);
                                cm_JoinChar.char0c = joined.char0c;
                                cm_JoinChar.char54 = joined.char54;
                            }
                        }
                    }
                } else if (d2_PointerObj == btn[1]) {
                    redraw = 1;
                } else if (d2_PointerObj == btn[2]) {
                    d2_SetViewEnabled(f, 0);
                    if (main_EnterHostAddress() != 0) {
                        sv_PopRequest = 1;
                        ret = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                }
            }
            if (d2_Radios[list].sel != -1)
                d2_SetObjEnabled(btn[0], 1);
            else
                d2_SetObjEnabled(btn[0], 0);
        } while (sv_RunFrame((int)main_SearchNetworkGame, 0x24087) != 0);
        if (list != -1)
            d2_RadioClear(list);
    }
    d2_CloseView(f);
    return ret;
}
/* 0x533470, the client side network dialog "Menu\CHOOSENETWORK": two buttons in a radio group (keyboard
   navigation), b1 joins the saved game main_NetSaveName (main_JoinNetworkGame), b2 searches the LAN for a saved
   game (main_SearchNetworkGame (1)); the right mouse button / Escape (kb_Last 1) closes it. Returns 1 when a
   game was chosen. */

int main_ChooseNetworkGame(void)
{
    int r, f, b1, b2, g;

    r = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSENETWORK");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    io_PrintText(0x1ca5);
    b1 = d2_GetWinObj(f, 0, io_PrintText(0x1ca9));
    b2 = d2_GetWinObj(f, 0, io_PrintText(0x1caa));
    g = d2_CreateRadioGroup(2, b1, b2);
    if (sv_RunFrame((int)main_ChooseNetworkGame, 0x24087) != 0) {
        do {
            d2_MenuNavigate(g);
            if (dm_L.rup != 0 || kb_Last == 1) {
                sv_PopRequest = 1;
                r = 0;
            }
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == b1) {
                    d2_SetViewEnabled(f, 0);
                    if (main_JoinNetworkGame() != 0) {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                } else if (d2_PointerObj == b2) {
                    d2_SetViewEnabled(f, 0);
                    if (main_SearchNetworkGame(1) != 0) {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                }
            }
        } while (sv_RunFrame((int)main_ChooseNetworkGame, 0x24087) != 0);
    }
    d2_RadioClear(g);
    d2_CloseView(f);
    return r;
}
/* 0x5335b0, the network main menu "Menu\CHOOSENETWORK": three buttons in a radio group (keyboard navigation),
   b1 hosts a new game (ai_GameFlags 5, main_HostNetworkGame), b2 searches the LAN for a new game (ai_GameFlags 5,
   main_SearchNetworkGame (0)), b3 opens the client dialog (ai_GameFlags 4, main_ChooseNetworkGame); the mission
   mode is set to 0xfe first, the right mouse button / Escape (kb_Last 1) closes it. Returns 1 when a game was
   started. */

int main_NetworkMenu(void)
{
    int r, f, b1, b2, b3, g;

    r = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSENETWORK");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    io_PrintText(0x1ca5);
    b1 = d2_GetWinObj(f, 0, io_PrintText(0x1ca6));
    b2 = d2_GetWinObj(f, 0, io_PrintText(0x1ca7));
    b3 = d2_GetWinObj(f, 0, io_PrintText(0x1ca8));
    g = d2_CreateRadioGroup(3, b1, b2, b3);
    ai_MissionMode = (char)0xfe;
    if (sv_RunFrame((int)main_NetworkMenu, 0x24087) != 0) {
        do {
            d2_MenuNavigate(g);
            if (dm_L.rup != 0 || kb_Last == 1) {
                sv_PopRequest = 1;
                r = 0;
            }
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == b1) {
                    ai_GameFlags = 5;
                    d2_SetViewEnabled(f, 0);
                    if (main_HostNetworkGame() != 0) {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                } else if (d2_PointerObj == b2) {
                    ai_GameFlags = 5;
                    d2_SetViewEnabled(f, 0);
                    if (main_SearchNetworkGame(0) != 0) {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                } else if (d2_PointerObj == b3) {
                    ai_GameFlags = 4;
                    d2_SetViewEnabled(f, 0);
                    if (main_ChooseNetworkGame() != 0) {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                }
            }
        } while (sv_RunFrame((int)main_NetworkMenu, 0x24087) != 0);
    }
    d2_RadioClear(g);
    d2_CloseView(f);
    return r;
}
/* 0x533740, the money amount dialog (a number input 0x472390 (0x20, 0x60, 0x26c, 400, 0x15) with the text "$3ABetrag:%m", range 0 .. 10000): runs 0x5309a0 until closed */

void main_MoneyDialog(void)
{
    int dlg, ed, run;

    d2_DirtyRectRight = 0;
    d2_DirtyRectLeft[0] = 1;
    d2_DirtyRectTop = 1;
    d2_DirtyRectBottom = 2;
    dlg = d2_CreateWindow(0x20, 0x60, 0x26c, 400, 0x15);
    ed = d2_GetWinObj(-1, dlg, io_PrintText("$3ABetrag:%m"));
    d2_SetObjValue4(ed, 0, 10000, 0);
    run = sv_RunFrame((int)main_MoneyDialog, 0x24087);
    while (run != 0) {
        if (kb_Last == 1)
            sv_PopRequest = 1;
        run = sv_RunFrame((int)main_MoneyDialog, 0x24087);
    }
    d2_RemoveWindow(dlg);
}
/* 0x5337f0, creates a menu button (0x46bf10 (0x20, n * 0x2b + 10, 0xbf, text, form), text centred, width 300) for the row n of a dialog */

int main_MenuButton(int n, int text, int form)
{
    int b;

    b = d2_WinCreateSlider(0x20, n * 0x2b + 10, 0xbf, text, form);
    d2_Objs[b].centred = 1;
    d2_SetObjWidth(b, 300);
    return b;
}

float d3_VecAngle();
int sv_RegisterNamePlate();

/* 0x533850, "main_Menue": the main menu ("MENU\MAIN_MENU"): fades to black, loads the city scene
   ("scenes/*ChooseCity.ed3", "..._Europa.ed3" for language 1 and up) and the player selection scene
   "scenes/*spielerauswahl.ed3", sets the full screen viewport, clears the player profile, builds the 9 buttons
   (main_MenuButton: new game, load, tutorial, network, game options, gfx / sfx options or with main_DevMenu the data
   tools "Create Save with INI" / "Create Save for Network" / "Check Textfile", credits, quit), starts one of three
   menu songs and runs the screen loop until a choice pops it; then it clears both scenes and lets the song fade
   out. */
void main_Menue(void)
{
    unsigned char *fade;
    char again;
    int f, grp, win;
    int bnew, bload, btut, bnet, bopt3, bcred, bquit, bopt1, bopt2, bini, bnetsave, bcheck;
    char ini[260], path[260], ini2[260], path2[260];
    char city[0x60], city2[0x60];
#ifdef REA_START
    int rea_entry;
    int rea_delay;                         /* frames to let the menu run before the entry is taken */
    char *rea_want;
#endif

    bopt1 = -1;
    bopt2 = -1;
    bini = -1;
    bnetsave = -1;
    bcheck = -1;
    sv_PumpMessages();
    di_GetMouse();
    kb_Last = 0;
    evt_ResetNoGroup();
    fade = (unsigned char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0, 1);
    while ((*fade & 4) == 0)
        sv_RunFrame(0, 0x24087);
    d2_CutsceneActive = 1;
    d3if_SelectScene(0, 0);
    d3if_Reset();
    if (ls_GetLanguage() == 0)
        d3_LoadScene("scenes/*ChooseCity.ed3", 0, 0);
    else
        d3_LoadScene("scenes/*ChooseCity_Europa.ed3", 0, 0);
    sv_PumpMessages();
    d3if_SelectScene(1, 0);
    d3if_Reset();
    if (d3_LoadScene("scenes/*spielerauswahl.ed3", 0, 0) == 0)
        err_Message("main_Menue:Could not load 3D-scene...");
    d3_SetViewport((float)d2_ScreenW, (float)d2_ScreenH, 0.0f, 0.0f, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
    sv_FrameBase = sv_FrameDepth + 1;
    ai_GameFlags = 0;
    D3_ZERO_AT(offset init_PlayerProfile, 0x8c);  /* the profile and the bytes after it up to 0x75e62c */
    d2_DirtyRectLeft[0] = d2_ScreenW;
    d2_CutsceneActive = 0;
    d2_DirtyRectRight = 0;
    d2_DirtyRectTop = 0;
    d2_DirtyRectBottom = d2_ScreenH;
    d2_ClearScreen();
    d2_ShowPicture(0x797);
    again = 0;
    f = d2_OpenForm(0, 0, "MENU\\MAIN_MENU");
    d2_PlaceMainWindowF(f, 3);
    d2_SetForm(f, 0);
    win = d2_LastWindow;
    bnew = main_MenuButton(0, (int)txt_Ptrs[0x1c63], win);
    bload = main_MenuButton(1, (int)ai_TextLoadGame, win);
    btut = main_MenuButton(2, (int)txt_Ptrs[0x3f19], win);
    bnet = main_MenuButton(3, (int)txt_Ptrs[0x1c64], win);
    bopt3 = main_MenuButton(4, (int)ai_TextGameOptions, win);
    bcred = main_MenuButton(7, (int)txt_Ptrs[0x1c65], win);
    bquit = main_MenuButton(8, (int)txt_Ptrs[0x1c66], win);
    if (main_DevMenu == 0) {
        bopt1 = main_MenuButton(5, (int)ai_TextGfxOptions, win);
        bopt2 = main_MenuButton(6, (int)ai_TextSfxOptions, win);
        grp = d2_CreateRadioGroup(9, bnew, bload, btut, bnet, bopt3, bopt1, bopt2, bcred, bquit);
    } else {
        bini = main_MenuButton(4, (int)"Create Save with INI", win);
        bnetsave = main_MenuButton(5, (int)"Create Save for Network", win);
        bcheck = main_MenuButton(6, (int)"Check Textfile", win);
        grp = d2_CreateRadioGroup(9, bnew, bload, btut, bnet, bopt3, bini, bnetsave, bcred, bquit);
    }
    if (snd_MusicEnabled != 0) {
        switch (rand() % 3) {
        case 0:
            main_MenuSong = msx_Play("cd1\\Rittersleut.mp3", 1);
            break;
        case 1:
            main_MenuSong = msx_Play("cd1\\MauerUndTor.mp3", 1);
            break;
        case 2:
            main_MenuSong = msx_Play("cd2\\KraeuterUndPhiolen.mp3", 1);
            break;
        }
    }
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 10);
    kb_Last = 0;
    msx_SetFadeTime(2000);
#ifdef REA_START
    /* REA_START=1 (tools/build.py, `uv run play|dbg --start NAME`): take one main-menu entry without a mouse
       click, so a start-up path is reachable without the GUI automation (build/dbg_tutorial.ps1). The entry
       then runs the same code a click runs (the loop below); the names are the buttons' own. A click cannot
       happen before the menu has run once: on the zero-th pass the menu callback is not registered yet, so
       the pop request the entry sets (sv_PopRequest) would not be consumed and would leak into the game
       loop, which then exits after its first frame. Wait one pass, as a real click does. */
    rea_entry = -1;
    rea_delay = 1;
    rea_want = getenv("REA_START");
    if (rea_want != 0 && *rea_want != 0) {
        if (strcmp(rea_want, "new") == 0)
            rea_entry = bnew;
        else if (strcmp(rea_want, "load") == 0)
            rea_entry = bload;
        else if (strcmp(rea_want, "tutorial") == 0)
            rea_entry = btut;
        else if (strcmp(rea_want, "network") == 0)
            rea_entry = bnet;
        else if (strcmp(rea_want, "gameoptions") == 0)
            rea_entry = bopt3;
        else if (strcmp(rea_want, "gfxoptions") == 0)
            rea_entry = bopt1;
        else if (strcmp(rea_want, "sfxoptions") == 0)
            rea_entry = bopt2;
        else if (strcmp(rea_want, "credits") == 0)
            rea_entry = bcred;
        else if (strcmp(rea_want, "quit") == 0)
            rea_entry = bquit;
        else if (strcmp(rea_want, "ini") == 0)
            rea_entry = bini;
        else if (strcmp(rea_want, "netsave") == 0)
            rea_entry = bnetsave;
        else if (strcmp(rea_want, "checktext") == 0)
            rea_entry = bcheck;
    }
#endif
    do {
        d2_MenuNavigate(grp);
#ifdef REA_START
        if (rea_entry != -1 && rea_delay-- <= 0) {
            d2_PointerObj = rea_entry;     /* what a click on that button leaves behind */
            dm_L.lclick = 1;
            rea_entry = -1;                /* one shot: the options screens return to this loop */
        }
#endif
        if (again != 0) {
            d2_ShowPicture(0x797);
            again = 0;
        }
        if (dm_L.lclick != 0 && d2_PointerObj != -1) {
            if (d2_PointerObj == bnew) {
                d2_EnableObject(win, 0);
                ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
                hmt_Reset();
                if (main_NewSinglePlayerGame() != 0) {
                    sv_PopRequest = 1;
                    ai_GameFlags |= 1;
                } else {
                    d2_EnableObject(win, 1);
                }
            } else if (d2_PointerObj == btut) {
                d2_EnableObject(win, 0);
                if (ai_TutorialWindow() != 0) {
                    ai_SetupTutorialPlayer();
                    ai_NumPlayers = 1;
                    strcpy(ai_DataPath + 0x220, "Tutorial");
                    ai_GameFlags = 0x89;
                    sv_PopRequest = 1;
                } else {
                    d2_EnableObject(win, 1);
                }
            } else if (d2_PointerObj == bini) {
                d2_EnableObject(win, 0);
                if (ai_FilePickWindow(0x24087, 1, "$Z$[Load INI$]", "gamedata/cities", ".INI", ini) != 0) {
                    sprintf(path, "%s%s", main_GfxPath, ini);
                    GetPrivateProfileStringA("A - ALLGEMEIN", "Stadtname", "Unknown", city, 0x60, path);
                    main_StartCityGame(city, 0);
                }
                d2_EnableObject(win, 1);
            } else if (d2_PointerObj == bnetsave) {
                d2_EnableObject(win, 0);
                if (ai_FilePickWindow(0x24087, 1, "$Z$[Load INI$]", "gamedata/cities", ".INI", ini2) != 0) {
                    sprintf(path2, "%s%s", main_GfxPath, ini2);
                    GetPrivateProfileStringA("A - ALLGEMEIN", "Stadtname", "Unknown", city2, 0x60, path2);
                    main_StartCityGame(city2, 1);
                }
                d2_EnableObject(win, 1);
            } else if (d2_PointerObj == bload) {
                d2_EnableObject(win, 0);
                if (ai_LoadGameWindow() != 0)
                    sv_PopRequest = 1;
                d2_EnableObject(win, 1);
            } else if (d2_PointerObj == bnet) {
                d2_EnableObject(win, 0);
                if (main_NetworkMenu() != 0) {
                    sv_PopRequest = 1;
                } else {
                    d2_EnableObject(win, 1);
                    ai_GameFlags = 0;
                }
            } else if (d2_PointerObj == bopt1) {
                d2_EnableObject(win, 0);
                ai_OptionsGfxScreen();
                d2_EnableObject(win, 1);
                again = 1;
                if (main_ScreenRestart != 0)
                    sv_PopRequest = 1;
            } else if (d2_PointerObj == bopt2) {
                d2_EnableObject(win, 0);
                ai_OptionsSfxScreen();
                d2_EnableObject(win, 1);
            } else if (d2_PointerObj == bopt3) {
                d2_EnableObject(win, 0);
                ai_OptionsGameScreen();
                d2_EnableObject(win, 1);
            } else if (d2_PointerObj == bquit) {
                ai_GameOver = 1;
                main_QuitProgram = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == bcheck) {
                main_MoneyDialog();
            } else if (d2_PointerObj == bcred) {
                d2_EnableObject(win, 0);
                if (snd_MusicEnabled != 0) {
                    msx_SetFadeTime(1000);
                    msx_Play("cd2\\ZumGutenEnd.mp3", 1);
                }
                ai_CreditsScreen();
                d2_EnableObject(win, 1);
                if (snd_MusicEnabled != 0) {
                    msx_SetFadeTime(1000);
                    msx_Start(main_MenuSong, 1);
                }
            }
        }
        if (kb_Last == 1) {
            main_QuitProgram = 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)main_Menue, 0x24087) != 0);
    d2_RadioClear(grp);
    d2_CloseView(f);
    d2_ClearScreen();
    d3if_SelectScene(0, 0);
    d3if_Reset();
    d3if_SelectScene(1, 0);
    d3if_Reset();
    msx_SetFadeTime(1000);
    if (main_MenuSong != 0) {
        msx_Stop(main_MenuSong, 1);
        d2_CutsceneActive = 1;
        while (main_MenuSong->playing != 0)
            sv_RunFrame(0, sv_FrameMask);
        Sleep(1000);
    }
    msx_SetFadeTime(5000);
}
/* 0x5344f0, puts a person (the template model name n of the list main_FamilyModelNames, index k) at the dummy "a" of the scene: the dummy position (0x458a20), a character object (0x403390 (b, pos)) that
   looks along the dummy's heading (0x458810 / 0x4539f0, 0x43b860), walks ("bewegung/gehen", walkSpeed 0.7), is registered as a status text plate (0x52e1c0); returns the new object or 0 */

int main_PlaceFamilyModel(int a, int b, int k)
{
    D3OBJECT *o;
    CHARACTER *ch;
    float dir[3], pos[4], loc[3];

    o = d3_FindObject(0, 0x100, 0, (char *)a);
    if (o == 0)
        return 0;
    d3_PointToWorld(o, &o->pos, (D3VECTOR *)pos);
    ch = ch_Create((char *)b, (D3VECTOR *)pos);
    if (ch != 0) {
        dir[0] = 0;
        dir[1] = 0.0f;
        dir[2] = 0;
        d3_DirToWorld((struct XOBJ *)o, (float *)&d3_ZAxis, loc);
        dir[1] = d3_VecAngle((D3VECTOR *)&d3_ZAxis, (D3VECTOR *)loc);
        d3_SetObjectRot(ch->obj, (D3VECTOR *)dir);
        ch_PlaceOnGround(ch, 0);
        ch->obj->owner = ch;
        d3_UpdateObject(ch->obj, 1);
        ch_PreloadAnims(ch, 1, "bewegung/gehen");
        ch->obj->pickId = 0;
        ch->walkSpeed = 0.7f;
        ch->familySlot = -1;
        ch->group = 1;
        ch->pathType = 4;
        sv_RegisterNamePlate((char *)ch->obj, main_FamilyModelNames[k], 0);
    }
    return (int)ch;
}
/* 0x534040, gives the player p the start buildings: the home (kind 6, named "Home-Sweet-Home", its 0x13 objects
   shown by the character's slots, the start messages 0x15 / 0x17 / 0x18 / 0x14 to its id) and the buildings of
   the kinds 0x20 (master and owner p, three 3d messages), 0x2a, 0x1e, 0x1f, 0x12, 0x14, 0x32, 0x21, 0x19, 0x34,
   0x36 (gm_AssignBuilding); the stores (gm_Query 2, 6, 0, 0x2a) of 0x20, 0x2a, 0x32 and 0x34 get their start
   goods as one message group whose completion is awaited (cm_SendGroupState, pumping the net). */

void main_GiveStartBuildings(int p)
{
    AIBLD *b;
    AIOBJ *o;
    char *l;
    int i;

    b = (AIBLD *)gm_BuildQuery(1, 0, 6);
    if (b != 0) {
        memcpy(b->name, main_HomeSweetHome, sizeof main_HomeSweetHome);
        gm_AssignBuilding((unsigned char *)b, p, p);
        l = gm_Query((void *)b->objList, 1, 0, 0x13);
        if (l != 0)
            gm_ShowSlotObjects((int)b, (int)l, p);
        cm_SendExSellObjectI(b->id, -1, 0x15, 1, ai_OwnPlayer, 0);
        cm_SendExSellObjectI(b->id, -1, 0x17, 1, ai_OwnPlayer, 0);
        cm_SendExSellObjectI(b->id, -1, 0x18, 1, ai_OwnPlayer, 0);
        cm_SendExSellObjectI(b->id, -1, 0x14, 1, ai_OwnPlayer, 0);
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x20);
    if (b != 0) {
        b->masterId = (unsigned short)p;
        b->owner = (unsigned short)p;
        i = 3;
        do {
            cm_SendMessage3d((int)b, 0, 10, 0);
            i--;
        } while (i != 0);
        o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(o->objId, -1, 0x1d0, 0x19, ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x2a);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(o->objId, -1, 0x1d0, 0x14, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x1d7, 0xf, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x1d6, 0xf, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x158, 10, ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x1e);
    if (b != 0)
        gm_AssignBuilding((unsigned char *)b, p, p);
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x1f);
    if (b != 0)
        gm_AssignBuilding((unsigned char *)b, p, p);
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x12);
    if (b != 0)
        gm_AssignBuilding((unsigned char *)b, p, p);
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x14);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        cm_SendExSellObjectI(b->id, -1, 0xf7, 1, ai_OwnPlayer, 0);
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x32);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(o->objId, -1, 0x1cb, 10, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x1cc, 10, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x1d0, 10, ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x21);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x19);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x34);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(o->objId, -1, 0x1cb, 10, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x1cc, 10, ai_OwnPlayer, 0);
            cm_SendExSellObjectI(o->objId, -1, 0x1d0, 10, ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    b = (AIBLD *)gm_BuildQuery(1, 0, 0x36);
    if (b != 0) {
        gm_AssignBuilding((unsigned char *)b, p, p);
        gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
    }
}

#define SV_WI2(i, o) (*(int *)(U_I((unsigned)&d2_Objs) + (i) * 0x2ec + (o)))

/* 0x534610, the talent choice of the character creation ("menu\choosecharacter_talent"): the five talents of
   main_Family start at 0, the father's and the mother's profession group (group[4] / group[5]) add 63 points
   to their talent (a grandparent would add 21: the loop starts at the parents); each row shows a label (text
   0x16b7 + n), the value as a bar (sv_ValueBar, picture 0x4d9) and two arrows (0x519 down / 0x51a up) that move
   21 points between the talent and the pool of 0x7e (a talent goes no lower than its start, no higher than
   126). The ok button (enabled with the pool spent) stores the five values as bytes in the profile
   (init_PlayerProfile.talents), the start tools of the parents' groups (init_PlayerProfile tool1 / tool2) and the
   family record; returns 1 when confirmed. */
int main_ChooseTalents(void)
{
    MAINFAMILY fam;
    int btn[14];                        /* per talent the down and the up arrow (@original 14: the frame is 0x7c) */
    int pts, sel, f, again, r, i, w, row, id, v;

    r = 0;
    memset(main_Family.talent, 0, 6 * sizeof(int));  /* @original six ints: one past the five talents */
    fam = main_Family;
    again = 1;
    pts = 0x7e;
    sel = -1;
    for (i = 4; i < 6; i++) {
        if (i < 4)
            fam.talent[fam.group[i]] += 21.0f;
        else
            fam.talent[fam.group[i]] += 63.0;
    }
    main_Family = fam;
    f = d2_OpenForm(0, 0, "menu\\choosecharacter_talent");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(f, 2);
    io_PrintText(0x1ae2);
    if (sv_RunFrame((int)main_ChooseTalents, 0x12d6) != 0) {
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (again) {
                d2_SetForm(f, 1);
                io_PrintText("$C$A");
                i = 0;
                w = 0x1c;
                do {
                    io_PrintText("%s:$N", i + 0x16b7);
                    sv_ValueBar(0xd2, w, fam.talent[i], 0x4d9);
                    btn[i * 2] = d2_WinOpenPic(d2_LastWindowPtr->w - 0x2c, i * 0x25 + 0x1c, 0x519, d2_LastWindow);
                    d2_Objs[btn[i * 2]].interactive = 1;
                    d2_Objs[btn[i * 2]].style = 3;
                    btn[i * 2 + 1] = d2_WinOpenPic(d2_LastWindowPtr->w - 0x18, i * 0x25 + 0x1c, 0x51a, d2_LastWindow);
                    d2_Objs[btn[i * 2 + 1]].interactive = 1;
                    d2_Objs[btn[i * 2 + 1]].style = 3;
                    w += 0x25;
                    i++;
                } while (w < 0xd5);
                io_PrintText(0x1ae3);
                sv_ValueBar(0xd2, 0xec, pts, 0x4d9);
                sel = d2_GetWinObj(f, 1, io_PrintText(0x1ae4));
                for (i = 0; i < 5; i++) {
                    v = fam.talent[i];
                    if (v > main_Family.talent[i]) {
                        d2_Objs[btn[i * 2]].clickDisabled = 0;
                        d2_Objs[btn[i * 2]].greyed = 0;
                    } else {
                        d2_Objs[btn[i * 2]].clickDisabled = 1;
                        d2_Objs[btn[i * 2]].greyed = 1;
                    }
                    if (pts > 0 && (float)v < 126.0f) {
                        d2_Objs[btn[i * 2 + 1]].clickDisabled = 0;
                        d2_Objs[btn[i * 2 + 1]].greyed = 0;
                    } else {
                        d2_Objs[btn[i * 2 + 1]].clickDisabled = 1;
                        d2_Objs[btn[i * 2 + 1]].greyed = 1;
                    }
                }
                again = 0;
            }
            if (pts != 0)
                d2_SetObjEnabled(sel, 0);
            else
                d2_SetObjEnabled(sel, 1);
            id = d2_ClickedId;
            if (id != -1) {
                if (id == 0 || kb_Last == 0x1c) {
                    r = 1;
                    for (i = 0; i < 5; i++)
                        init_PlayerProfile.talents[i] = (unsigned char)fam.talent[i];
                    switch (fam.group[4]) {
                    case 0: init_PlayerProfile.tool1 = 0x1c; break;
                    case 1: init_PlayerProfile.tool1 = 0x31; break;
                    case 2: init_PlayerProfile.tool1 = 4; break;
                    case 3: init_PlayerProfile.tool1 = 0x37; break;
                    case 4: init_PlayerProfile.tool1 = 0x10; break;
                    }
                    switch (fam.group[5]) {
                    case 0: init_PlayerProfile.tool2 = 0x1c; break;
                    case 1: init_PlayerProfile.tool2 = 0x25; break;
                    case 2: init_PlayerProfile.tool2 = 4; break;
                    case 3: init_PlayerProfile.tool2 = 0; break;
                    case 4: init_PlayerProfile.tool2 = 0x49; break;
                    }
                    main_Family = fam;
                    sv_PopRequest = 1;
                } else if (id == 1) {
                    sv_PopRequest = 1;
                }
                if (id == 0x519 || id == 0x51a) {
                    again = 1;
                    for (i = 0; i < 12; i++) {
                        if (d2_PointerObj == btn[i]) {
                            row = i / 2;
                            if (i % 2 != 0) {
                                if (pts > 0) {
                                    fam.talent[row] += 21.0f;
                                    pts -= 21.0f;
                                }
                            } else if (fam.talent[row] > main_Family.talent[row]) {
                                fam.talent[row] -= 21.0f;
                                pts += 21.0f;
                            }
                        }
                    }
                }
            }
        } while (sv_RunFrame((int)main_ChooseTalents, 0x12d6) != 0);
    }
    d2_CloseView(f);
    return r;
}
/* 0x534b30, the character model choice of the character creation: the two tables of the 0x20-byte model names (the men:
   "patrizier_MANN2", "wirt_DICKER", .. ending with ""; the women: "patrizierin_FRAU", .. ending with ""), the three buttons
   (next 0x1a, previous 0x1b, ok 0x51b, placed in 800-wide screen units scaled by d2_ScreenScale) and the preview model of the
   current entry, put at the dummy "dummy_SPIELER" with the effect "Effekte/choose_character.esc"; the chosen name goes to
   init_PlayerProfile.model, its id to modelId; job is the profile's job, its title group is the start index; returns 1 when ok
   was pressed (Enter), 0 when cancelled (the original counts both tables through the men table) */

int main_ChooseModel(int job)
{
    char men[17][0x20] = {"patrizier_MANN2", "wirt_DICKER", "arbeiter_MANN", "wirt3_MANN", "wirt2_DICKER", "buerger_MANN", "buerger2_MANN", "domherr_KUTTE", "buerger3_DICKER",
                          "patrizier_MANN2", "offizier_SOLDAT", "DIEB2_KUTTE", "raeubermeister_SOLDAT", "Nekromant_MEISTER", "zigeunermeister_MANN3", "schneider3_DICKER", ""};
    char women[17][0x20] = {"patrizierin_FRAU", "handwerkerin3_FRAU", "handwerkerin2_FRAU", "handwerkerin_FRAU", "magd_FRAU", "buergerin3_FRAU", "Bauerin_FRAU", "minerin2_FRAU", "buergerin2_FRAU",
                            "buergerin2_FRAU", "buergerin_FRAU", "arbeiterin_FRAU", "minerin_FRAU", "feldarbeiterin_FRAU", "zigeunerin_FRAU", "schneiderin3_FRAU", ""};
    char cur[0x30];
    D3VECTOR4 pos;                  /* d3_PointToWorld writes the point and an angle after it */
    D3OBJECT *dummy;
    EVTSCRIPT *fx;
    int result, bnext, bprev, bok, idx, shown, obj, nmen, nwomen;
    char *s;

    result = 0;
    obj = 0;
    shown = -1;
    idx = gm_TitleGroup(job);
    dummy = d3_FindObject(0, 0x100, 0, "dummy_SPIELER");
    if (dummy != 0)
        d3_PointToWorld(dummy, &dummy->pos, (D3VECTOR *)&pos);
    bnext = d2_CreatePicObj((int)(d2_ScreenScale * 150.0f + d2_ScreenScale * 330.0f), (int)(d2_ScreenScale * 370.0f), 0x1a);
    d2_Objs[bnext].interactive = 1;
    d2_Objs[bnext].style = 3;
    bprev = d2_CreatePicObj((int)(d2_ScreenScale * 330.0f), (int)(d2_ScreenScale * 370.0f), 0x1b);
    d2_Objs[bprev].interactive = 1;
    d2_Objs[bprev].style = 3;
    bok = d2_CreatePicObj((int)(d2_ScreenScale * 70.0f + d2_ScreenScale * 330.0f), (int)(d2_ScreenScale * 45.0f + d2_ScreenScale * 370.0f), 0x51b);
    d2_Objs[bok].interactive = 1;
    d2_Objs[bok].style = 3;
    nmen = 0;
    if (men[0][0] != 0) {
        s = men[0];
        do {
            s += 0x20;
            nmen++;
        } while (*s != 0);
    }
    nwomen = 0;
    if (men[0][0] != 0) {
        s = men[0];
        do {
            s += 0x20;
            nwomen++;
        } while (*s != 0);
    }
    if (sv_RunFrame((int)main_ChooseModel, 0x12d6) != 0) {
        do {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                result = 0;
            }
            if (kb_Last == 'N' || (d2_ClickedId != -1 && d2_PointerObj == bnext))
                idx++;
            else if (kb_Last == 'J' || (d2_ClickedId != -1 && d2_PointerObj == bprev))
                idx--;
            else if (kb_Last == 0x1c || (d2_ClickedId != -1 && d2_PointerObj == bok)) {
                sv_PopRequest = 1;
                result = 1;
            }
            if (obj != 0)
                *(unsigned char *)(obj + 0x8c) |= 8;
            if (shown != idx) {
                fx = evt_Load("Effekte/choose_character.esc");
                if (fx != 0)
                    evt_RunScriptArgs(fx, 6, (int)pos.x, (int)pos.y + 0x14, (int)pos.z, 0x28, 0x5a, ch_EmptyName);
                if (obj != 0) {
                    ch_AddAction(obj, 0x38, 0);
                    while (sv_RunFrame(0, 0x12d6) != 0 && *(int *)(obj + 0x128) != 0)
                        ;
                    ch_Delete(obj);
                }
                if (init_PlayerProfile.sex == 0) {
                    idx %= nmen;
                    if (idx < 0)
                        idx = nmen - 1;
                    strcpy(cur, men[idx]);
                } else {
                    idx %= nwomen;
                    if (idx < 0)
                        idx = nwomen - 1;
                    strcpy(cur, women[idx]);
                }
                strcpy(init_PlayerProfile.model, cur);
                init_PlayerProfile.modelId = gm_HeadIconNumber(cur) + 0x639;
                obj = ch_Create(cur, &pos);
                ch_PlaceAtObject(obj, dummy);
                ch_SetVisible(obj, 0);
                (*(D3OBJECT **)(obj + 0x34))->objectFlags &= ~4;
                *(unsigned char *)(obj + 0x8c) |= 8;
                ch_AddAction(obj, 0x38, 1);
                shown = idx;
            }
        } while (sv_RunFrame((int)main_ChooseModel, 0x12d6) != 0);
        if (obj != 0)
            ch_Delete(obj);
    }
    d2_RemoveObj(bprev);
    d2_RemoveObj(bnext);
    d2_RemoveObj(bok);
    return result;
}
/* 0x5359f0, the family choice of the character creation ("Menu\CHOOSECITY_HEADER" over the sky "Sky_Mittel_02"): nine template persons (0x5344f0: thief / gypsy 0 / 8, craftsmen 1 / 7, fighter 4, traders 3 / 5, priest 2 / 6) stand
   in the scene; the player clicks one for each of the four slots 0x75e180 [4] (father) and [5] (mother) (the clicked person 0x6f22e4 + 0x200 gets the slot in +0x23c and the dummy name "dummy_VATER" / "dummy_MUTTER" ..), the
   value stored in the slot is the profession group 0 .. 4; with both slots set the talent choice main_ChooseTalents and the model choice main_ChooseModel follow; returns 1 when the character was created (the dummy name table
   dn[6] of 0x30 bytes, entries 4 / 5 are the only ones used) */

int main_ChooseFamily(int unused)
{
    char dn[6][0x30] = { "dummy_GROSSVATER_VAETERLICH", "dummy_GROSSMUTTER_VAETERLICH", "dummy_GROSSVATER_MUETTERLICH", "dummy_GROSSMUTTER_MUETTERLICH", "dummy_VATER", "dummy_MUTTER" };
    int o1, o2, o3, o4, o5, o6, o7, o8, o9, f, run, k, sel, v, h, ret, scene, redraw;
    int r1[3], r2[3];
    unsigned char *fade;
    int *p;
    int q;

    ret = 0;
    redraw = 1;
    fade = 0;
    for (k = 0; k < 11; k++)
        ((int *)(unsigned)&main_Family)[k] = 0;
    for (k = 0; k < 6; k++)
        ((int *)(unsigned)&main_Family)[k] = -1;
    sv_ClearNamePlates();
    sv_ReleaseNamePlates();
    d3if_SelectScene(1, 0);
    p = (int *)d3_DefaultScene;
    if (p != 0 && (q = p[0x3c8 / 4]) != 0 && *(int *)(q + 0x210) != 0)
        ((void (*)(int *, int, int, int))*(int *)(q + 0x210))(p, 4, q + 0x214, 0);
    scene = U_I((unsigned)&evt_LastId);
    if ((int)d3_Terrain != 0)
        *(unsigned char *)((int)d3_Terrain + 0x1c70) = (*(unsigned char *)((int)d3_Terrain + 0x1c70) & 0xeb) | 8;
    o1 = main_PlaceFamilyModel((int)"dummy_NACHT_UND_NEBEL_MANN", (int)"dieb_MANN2", 0);
    o2 = main_PlaceFamilyModel((int)"dummy_NACHT_UND_NEBEL_FRAU", (int)"zigeunerin_FRAU", 8);
    o3 = main_PlaceFamilyModel((int)"dummy_HANDWERKSKUNST_MANN", (int)"handwerker_MANN", 1);
    o4 = main_PlaceFamilyModel((int)"dummy_HANDWERKSKUNST_FRAU", (int)"handwerkerin2_FRAU", 7);
    o5 = main_PlaceFamilyModel((int)"dummy_KAMPF_MANN", (int)"offizier_SOLDAT", 4);
    o6 = main_PlaceFamilyModel((int)"dummy_VERHANDELN_MANN", (int)"buerger3_DICKER", 3);
    o7 = main_PlaceFamilyModel((int)"dummy_VERHANDELN_FRAU", (int)"buergerin2_FRAU", 5);
    o8 = main_PlaceFamilyModel((int)"dummy_RHETORIK_MANN", (int)"priester_KUTTE", 2);
    o9 = main_PlaceFamilyModel((int)"dummy_RHETORIK_FRAU", (int)"buergerin_FRAU", 6);
    f = d2_OpenForm(0, 0, "Menu\\CHOOSECITY_HEADER");
    d2_PlaceMainWindowF(f, 1);
    k = 4;
    cm_CreateSceneSky("Sky_Mittel_02");
    d3_SetLightSet(4, 0.0f, 1.0f, 1);
    if (main_MessageBox != -1) {
        d2_CloseView(main_MessageBox);
        main_MessageBox = -1;
    }
    U_I((unsigned)&d2_CutsceneActive) = 0;
    run = sv_RunFrame((int)main_ChooseFamily, 0x12d6);
    if (run != 0) {
        do {
            if (redraw) {
                k = 4;
                do {
                    if (((int *)(unsigned)&main_Family)[k] == -1) {
                        if (k < 6) {
                            d2_SetForm(f, 0);
                            io_PrintText((unsigned)"$C");
                            io_PrintText(0x1ae6, k + 0x1aea);
                            if (k % 2 != 0) {
                                r1[0] = *(int *)&d3_Camera->views[4].pos.x;
                                r1[1] = *(int *)&d3_Camera->views[4].pos.y;
                                r1[2] = *(int *)&d3_Camera->views[4].pos.z;
                                r2[0] = *(int *)&d3_Camera->views[4].dir.x;
                                r2[1] = *(int *)&d3_Camera->views[4].dir.y;
                                r2[2] = *(int *)&d3_Camera->views[4].dir.z;
                            } else {
                                r1[0] = *(int *)&d3_Camera->views[3].pos.x;
                                r1[1] = *(int *)&d3_Camera->views[3].pos.y;
                                r1[2] = *(int *)&d3_Camera->views[3].pos.z;
                                r2[0] = *(int *)&d3_Camera->views[3].dir.x;
                                r2[1] = *(int *)&d3_Camera->views[3].dir.y;
                                r2[2] = *(int *)&d3_Camera->views[3].dir.z;
                            }
                            d3anim_FreePath((int)d3_Camera);
                            U_I((unsigned)&d3t_CamMoving) = 0;
                            U_I((unsigned)&d3t_CamMove) = 0;
                            d3t_PlaceObjectV((int)d3_Camera, r1, r2, 0x50, 8);
                        }
                        break;
                    }
                    k++;
                } while (k < 6);
                redraw = 0;
            }
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                sv_PopRequest = 1;
            if (U_I((unsigned)&ai_ClickedObject) != 0 && (sel = *(int *)(U_I((unsigned)&ai_ClickedObject) + 0x200)) != 0) {
                if (*(int *)(sel + 0x23c) == -1) {
                    v = 0;
                    if (sel == o1 || sel == o2)
                        v = 2;
                    else if (sel == o6 || sel == o7)
                        v = 0;
                    else if (sel == o5)
                        v = 3;
                    else if (sel == o8 || sel == o9)
                        v = 4;
                    else if (sel == o3 || sel == o4)
                        v = 1;
                    if (k % 2 != 0) {
                        if (sel != o2 && sel != o7 && sel != o9 && sel != o4)
                            goto lskip;
                        k = 5;
                    } else {
                        if (sel != o1 && sel != o6 && sel != o8 && sel != o3 && sel != o5)
                            goto lskip;
                        k = 4;
                    }
                    ((int *)(unsigned)&main_Family)[k] = v;
                    *(int *)(sel + 0x23c) = k;
                    h = d3_FindObject(0, 0x100, 0, dn[k]);
                    if (h != 0)
                        ch_WalkToObjectTurn(sel, h);
                }
lskip:
                redraw = 1;
            }
            if (k >= 6) {
                d2_SetForm(f, 0);
                io_PrintText((unsigned)"$C");
                d2_SetViewEnabled(f, 0);
                d3anim_FreePath((int)d3_Camera);
                U_I((unsigned)&d3t_CamMoving) = 0;
                U_I((unsigned)&d3t_CamMove) = 0;
                d3t_PlaceObjectV((int)d3_Camera, &d3_Camera->views[5].pos, &d3_Camera->views[5].dir, 0x50, 8);
                if (main_ChooseTalents() != 0) {
                    if (lobby_ChooseProfession(0xc6) != 0 && main_ChooseModel(init_PlayerProfile.job) != 0) {
                        sv_PopRequest = 1;
                        ret = 1;
                    }
                } else {
                    sv_PopRequest = 1;
                    ret = 0;
                }
                if (ret == 0)
                    d2_SetViewEnabled(f, 1);
            }
            run = sv_RunFrame((int)main_ChooseFamily, 0x12d6);
        } while (run != 0);
        if (ret == 1) {
            fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x5a, 1);
            while ((*fade & 4) == 0)
                sv_RunFrame(0, 0xc6);
        }
    }
    if (o1 != 0)
        ch_Delete(o1);
    if (o2 != 0)
        ch_Delete(o2);
    if (o3 != 0)
        ch_Delete(o3);
    if (o4 != 0)
        ch_Delete(o4);
    if (o5 != 0)
        ch_Delete(o5);
    if (o6 != 0)
        ch_Delete(o6);
    if (o7 != 0)
        ch_Delete(o7);
    if (o9 != 0)
        ch_Delete(o9);
    if (o8 != 0)
        ch_Delete(o8);
    if (f != -1)
        d2_CloseView(f);
    if (scene != -1) {
        h = (int)evt_FindById(scene);
        if (h != 0)
            evt_Stop((EVTSCRIPT *)h);
        sv_RunFramesUntilStopped(scene);
    }
    if ((int)d3_DefaultScene->hmap != 0) {
        d3sm_Destroy((int)d3_DefaultScene->hmap);
        d3_DefaultScene->hmap = 0;
    }
    cm_DestroySceneSky();
    p = (int *)d3_DefaultScene;
    if (p != 0 && (q = p[0x3c8 / 4]) != 0 && *(int *)(q + 0x294) != 0)
        ((void (*)(int *, int, int, int))*(int *)(q + 0x294))(p, 5, q + 0x298, 0);
    d3anim_FreePath((int)d3_Camera);
    U_I((unsigned)&d3t_CamMoving) = 0;
    U_I((unsigned)&d3t_CamMove) = 0;
    if (fade != 0)
        d2_ReleaseFade(fade);
    return ret;
}
/* 0x5361d0, the player setup of a network game ("Menu\CHOOSEPLAYER"; mask for the frame loop 0x5309a0, flag = quit on the first escape / enter at stage 0): reads the saved values from game.INI ([Network] Name,
   Familienname, Wappen (+ 0x59a), Geschlecht, Glauben), then six stages (stage): 0 first name edit, 1 family name edit (enter = next), 2 sex (two buttons, byte 0x75e5a8), 3 faith (two buttons, byte 0x75e5a9), 4 the coat
   of arms (eight objects 0x59a .. 0x5a1 in a list; click = next), 5 done (0x75e5a4 = the chosen coat object id, 0x75e5a2 = 0x12); the character record 0x75e5a0 holds the names at +0xa (0x75e5aa) / +0x2a (0x75e5ca).
   Writes the values back to the INI file on leaving; returns 1 when finished. The two button rows are copied into four variables
   (male / female, faith0 / faith1); redraw is a char @deep read against the original and reconstructed (310 -> 184 ins): both button rows copied into four variables (two int[2] + male/female/faith0/faith1: the original's frame 0x190), redraw a char, the back / escape test with stage--; redraw = 1 (the original jumps straight into the redraw body), d2_EnableObject through a temp with if / else (push 1 / push 0), do .. while (sv_RunFrame) with the INI write-back after it, typed init_PlayerProfile / d2_Objs / d2_Radios / d2_Wins. Left: (1) allocation: the original keeps -1 in ebp (or ebp,-1 at the loop head, cmp [d2_ClickedId], ebp) and 1 in ecx, ours 1 in ebp; e1 / e2 / list / ret take other slots; (2) stage 5: the original loads flag (mov eax,[esp+0x1a8]) when ai_MissionMode != -1 and discards it before ret = 1; every spelling tried (if (..) ret = flag; if (..) flag; ret = .. ? flag : 1) is removed entirely by VC6. Q7: d2.h declares d2_SetObjValue() without parameters */

int main_ChoosePlayer(int mask, int flag)
{
    int stage, ret, f, e1, e2, ok, list, gsel, fsel, i, cols, e;
    int sexb[2], faithb[2], male, female, faith0, faith1;
    int ent[8];
    char buf[0x30], txt[0x100], redraw;
    char *s, *d;

    stage = 0;
    redraw = 1;
    ret = 0;
    GetPrivateProfileStringA("Network", "Name", "Vorname", init_PlayerProfile.firstName, 0x20, ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Familienname", "Nachname", init_PlayerProfile.familyName, 0x20, ai_OptionsFile);
    init_PlayerProfile.coat = GetPrivateProfileIntA("Network", "Wappen", 0, ai_OptionsFile) + 0x59a;
    init_PlayerProfile.sex = (unsigned char)GetPrivateProfileIntA("Network", "Geschlecht", 0, ai_OptionsFile);
    init_PlayerProfile.faith = (unsigned char)GetPrivateProfileIntA("Network", "Glauben", 0, ai_OptionsFile);
    f = d2_OpenForm(0, 0, "Menu\\CHOOSEPLAYER");
    d2_CentreViewWindows(f);
    io_TownDisplayName(((unsigned)&ai_DataPath + 0x220), txt);
    d2_SetForm(f, 1);
    io_PrintText(0x1acf, txt);
    e1 = d2_GetWinObj(f, 1, io_PrintText(0x1ad0, 0x1ad2));
    d2_SetForm(f, 2);
    e2 = d2_GetWinObj(f, 2, io_PrintText(0x1ad1, 0x1ad3));
    d2_SetForm(f, 3);
    io_PrintText(0x1ad4);
    sv_ButtonRow(d2_LastWindow, 0x1c, sexb, 2, txt_Ptrs[0x1ad5], txt_Ptrs[0x1ad6]);
    male = sexb[0];
    female = sexb[1];
    d2_SetForm(f, 4);
    io_PrintText(0x1ad7);
    sv_ButtonRow(d2_LastWindow, 0x1c, faithb, 2, txt_Ptrs[0x1ad8], txt_Ptrs[0x1ad9]);
    faith0 = faithb[0];
    faith1 = faithb[1];
    d2_SetForm(f, 5);
    io_PrintText(0x1adb);
    cols = (d2_Wins[d2_LastWindow].w - 0x20) / 0x30;
    list = d2_CreateRadioGroup(0);
    for (i = 0; i < 8; i++) {
        e = d2_WinOpenPic((i % cols) * 0x30 + 0x50, (i / cols) * 0x30 + 0x20, i + 0x59a, d2_LastWindow);
        ent[i] = e;
        d2_Objs[e].checkable = 1;
        d2_Objs[e].checked = 0;
        d2_Objs[e].highlighted = 0;
        d2_Objs[e].greyed = 1;
        d2_Objs[e].style = 3;
        d2_RadioAdd(list, e);
    }
    d2_SetForm(f, 6);
    ok = d2_GetWinObj(f, 6, io_PrintText(0x7e));
    d2_SetObjValue2(e1, init_PlayerProfile.firstName);
    d2_SetObjValue2(e2, init_PlayerProfile.familyName);
    for (i = 0; i < 8; i++) {
        if (d2_Objs[ent[i]].pic == init_PlayerProfile.coat) {
            d2_RadioSelect(list, i);
            break;
        }
    }
    d2_SetObjMax(e1, 0xc);
    d2_SetObjMax(e2, 0xc);
    gsel = d2_CreateRadioGroup(2, male, female);
    d2_RadioSelect(gsel, init_PlayerProfile.sex);
    fsel = d2_CreateRadioGroup(2, faith0, faith1);
    d2_RadioSelect(fsel, init_PlayerProfile.faith);
    d2_ClearInput();
    kb_Last = 0;
    do {
        if (dm_L.rup != 0 || (d2_ClickedId != -1 && d2_PointerObj == ok) || kb_Last == 1) {
            if (stage == 0 && flag == 0)
                sv_PopRequest = 1;
            else {
                stage--;
                redraw = 1;
            }
        }
        if (redraw) {
            for (i = 0; i < 5; i++) {
                e = d2_GetWinObj(f, 0, i);
                if (i <= stage)
                    d2_EnableObject(e, 1);
                else
                    d2_EnableObject(e, 0);
            }
            if (stage == 0)
                d2_FocusInput(e1);
            else if (stage == 1)
                d2_FocusInput(e2);
            redraw = 0;
        }
        if (stage == 0) {
            if (kb_Last == 0x1c) {
                s = (char *)d2_GetObjValue(e1);
                d = init_PlayerProfile.firstName;
                while ((*d++ = *s++) != 0)
                    ;
                stage = 1;
                kb_Last = 0;
                redraw = 1;
            }
        } else if (stage == 1) {
            if (kb_Last == 0x1c) {
                s = (char *)d2_GetObjValue(e2);
                d = init_PlayerProfile.familyName;
                while ((*d++ = *s++) != 0)
                    ;
                stage = 2;
                redraw = 1;
                kb_Last = 0;
            }
        } else if (stage == 2) {
            d2_MenuNavigate(gsel);
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == male) {
                    init_PlayerProfile.sex = 0;
                    stage = 3;
                    redraw = 1;
                } else if (d2_PointerObj == female) {
                    init_PlayerProfile.sex = 1;
                    stage = 3;
                    redraw = 1;
                }
            }
        } else if (stage == 3) {
            d2_MenuNavigate(fsel);
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == faith1) {
                    init_PlayerProfile.faith = 1;
                    stage = 4;
                    redraw = 1;
                } else if (d2_PointerObj == faith0) {
                    init_PlayerProfile.faith = 0;
                    stage = 4;
                    redraw = 1;
                }
            }
        } else if (stage == 4) {
            d2_MenuNavigate(list);
            if (kb_Last == 0x1c) {
                stage = 5;
                redraw = 1;
            }
            if (d2_ClickedId >= 0x59a && d2_ClickedId < 0x5a2) {
                for (i = 0; i < 8; i++) {
                    if (&d2_Objs[ent[i]] != d2_ClickedObj) {
                        d2_Objs[ent[i]].checked = 0;
                        d2_Objs[ent[i]].highlighted = 0;
                        d2_Objs[ent[i]].greyed = 1;
                    }
                }
                stage++;
            }
        } else if (stage == 5) {
            init_PlayerProfile.tool1 = 0;
            init_PlayerProfile.tool2 = 0;
            init_PlayerProfile.status = 0x12;
            init_PlayerProfile.coat = d2_Objs[d2_Radios[list].objs[d2_Radios[list].sel]].pic;
            ret = 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)main_ChoosePlayer, mask) != 0);
    d2_RadioClear(gsel);
    d2_RadioClear(fsel);
    d2_RadioClear(list);
    d2_CloseView(f);
    WritePrivateProfileStringA("Network", "Name", init_PlayerProfile.firstName, ai_OptionsFile);
    WritePrivateProfileStringA("Network", "Familienname", init_PlayerProfile.familyName, ai_OptionsFile);
    _itoa(init_PlayerProfile.coat - 0x59a, buf, 10);
    WritePrivateProfileStringA("Network", "Wappen", buf, ai_OptionsFile);
    _itoa(init_PlayerProfile.sex, buf, 10);
    WritePrivateProfileStringA("Network", "Geschlecht", buf, ai_OptionsFile);
    _itoa(init_PlayerProfile.faith, buf, 10);
    WritePrivateProfileStringA("Network", "Glauben", buf, ai_OptionsFile);
    return ret;
}

int main_CharacterIntro(int unused);

/* 0x536980, the history choice of the new game ("Menu\CHOOSEHISTORY": the texts 0x1ac0 ..; three or four buttons by 0x5190a0: the level 0x4b1f90 (1 / 2 / 0), the group value 0x6f365c from 0x6f0724 shown by 0x46c630), then the
   start menu pages: 0x4b4040 (the history choice, hist), 0x5361d0 (the player setup, cb mask 0xc6), the character creation 0x536d80 / 0x5359f0 (1 = continue with the family choice, 0 = quick start with the model
   "patrizier_MANN2" (0x75e628 = index + 0x639), -1 = back) and the scripted start menu pages "Startmenu/B_Persoenliches.esc" / "Startmenu/A_Stadtwahl.esc" (0x4e2bf0 / 0x4e2c70, 0x760638 = 0x12d6); returns
   non-zero when the character was created, sets the mode flag ai_GameFlags |= 8 then (else clears it). The quick start copies the
   talents of the job's record: gm_GetJobTalents returns a 6-byte record by value in the original (hidden pointer, eax), the call here
   passes the buffer by hand */
int main_ChooseHistory(void)
{
    int again, back, ret, f, b0, b1, b2, grp, hist, h, r;
    JOBTALENTS rec;

    again = 0;
    ret = 0;
    b1 = -1;
    hist = 0xff;
    back = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSEHISTORY");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    if (ls_GetLanguage() != 0) {
        h = io_PrintText(0x1ac0, 0x1ac2, 0x1ac4, 0x1ac2, 0x1ac4, 1);
        b0 = d2_GetWinObj(f, 0, h);
        b2 = d2_GetWinObj(f, 0, h + 1);
        grp = d2_CreateRadioGroup(3, b0, b2, d2_GetWinObj(f, 0, h + 2));
    } else {
        h = io_PrintText(0x1ac0, 0x1ac2, 0x1ac3, 0x1ac4, 0x1ac2, 0x1ac3, 0x1ac4, 1);
        b0 = d2_GetWinObj(f, 0, h);
        b1 = d2_GetWinObj(f, 0, h + 1);
        b2 = d2_GetWinObj(f, 0, h + 2);
        grp = d2_CreateRadioGroup(4, b0, b1, b2, d2_GetWinObj(f, 0, h + 3));
    }
    if (ai_OptionHistorie == 0)
        d2_RadioSelect(grp, 2);
    else if (ai_OptionHistorie == 1)
        d2_RadioSelect(grp, 0);
    else if (ai_OptionHistorie == 2)
        d2_RadioSelect(grp, 1);
    if (sv_RunFrame((int)main_ChooseHistory, 0xc6) != 0) {
        do {
            d2_MenuNavigate(grp);
            if (dm_L.rup != 0 || kb_Last == 1)
                sv_PopRequest = 1;
            if (d2_ClickedId != 0 && kb_Last != 0x1c && !again) {
                if (back)
                    goto lback;
                if (d2_ClickedId == 1)
                    sv_PopRequest = d2_ClickedId;
                continue;
            }
            if (!back) {
                if (d2_PointerObj == b0)
                    hst_SetTextsOn(1);
                else if (d2_PointerObj == b1)
                    hst_SetTextsOn(2);
                else if (d2_PointerObj == b2 || kb_Last == 0x1c)
                    hst_SetTextsOn(0);
                ai_OptionHistorie = hst_TextsOn;
                if (ai_OptionHistorie == 0)
                    d2_RadioSelect(grp, 2);
                else if (ai_OptionHistorie == 1)
                    d2_RadioSelect(grp, 0);
                else if (ai_OptionHistorie == 2)
                    d2_RadioSelect(grp, 1);
                d2_SetViewEnabled(f, 0);
                hist = ai_ChooseMission();
                d2_SetViewEnabled(f, 1);
            }
lback:
            d2_SetViewEnabled(f, 0);
            again = 0;
            if (hist != 0xff || back) {
                init_PlayerProfile.mission = hist;
                if (!back) {
                    sv_FrameMask = 0x12d6;
                    cm_WaitEvent(cm_StartScript("Startmenu/B_Persoenliches.esc"));
                }
                back = 0;
                ret = main_ChoosePlayer(0xc6, 0);
                if (ret == 0) {
                    sv_FrameMask = 0x12d6;
                    cm_WaitEvent(cm_StartScript("Startmenu/A_Stadtwahl.esc"));
                    again = 1;
                } else {
                    r = main_CharacterIntro(0);
                    if (r == 1) {
                        d2_CutsceneActive = r;
                        main_MessageBox = sv_OpenMessageBox((int)txt_Ptrs[0x1af9], 0);
                        if (main_ChooseFamily(0) == 0) {
                            d3if_SelectScene(0, 0);
                            goto lback2;
                        }
                        goto lok;
                    } else if (r == 0) {
                        if (lobby_ChooseProfession(0xc6) != 0) {
                            rec = gm_GetJobTalents(init_PlayerProfile.job);
                            *(int *)init_PlayerProfile.talents = *(int *)rec.skills;
                            init_PlayerProfile.talents[4] = rec.skills[4];
                            init_PlayerProfile.modelId = gm_HeadIconNumber("patrizier_MANN2") + 0x639;
lok:
                            ret = 1;
                            sv_PopRequest = 1;
                        } else {
                            d3if_SelectScene(0, 0);
                            ret = 0;
                            again = 1;
                        }
                    } else if (r == -1) {
                        ret = 0;
lback2:
                        back = 1;
                    }
                }
            }
            d2_SetViewEnabled(f, 1);
        } while (sv_RunFrame((int)main_ChooseHistory, 0xc6) != 0);
    }
    d2_RadioClear(grp);
    d2_CloseView(f);
    if (ret != 0) {
        ai_GameFlags |= 8;
        return ret;
    }
    ai_GameFlags = 0;
    return ret;
}
/* 0x536d80, the "menu\choosecharacter_intro" dialog with two buttons (texts 0x1adf .. 0x1ae1, 0x1ae0 only in a mission): returns 1 for the first button
   (create a character), -1 when cancelled, else 0 (quick start); unused: main_ChooseHistory passes 0 */

int main_CharacterIntro(int unused)
{
    int r, f, t, b1, b2, g;

    r = 0;
    f = d2_OpenForm(0, 0, "menu\\choosecharacter_intro");
    d2_CentreViewWindows(f);
    io_PrintText(0x1adf);
    if (ai_MissionMode != -1)
        io_PrintText(0x1ae0);
    t = io_PrintText(0x1ae1);
    b1 = d2_GetWinObj(f, 0, t);
    b2 = d2_GetWinObj(f, 0, t + 1);
    g = d2_CreateRadioGroup(2, b1, b2);
    if (sv_RunFrame((int)main_CharacterIntro, 0xc6) != 0) {
        do {
            d2_MenuNavigate(g);
            if (dm_L.rup != 0 || kb_Last == 1) {
                r = -1;
                sv_PopRequest = 1;
            }
            if (d2_ClickedId == 0 || kb_Last == 0x1c) {
                r = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)main_CharacterIntro, 0xc6) != 0);
    }
    d2_RadioClear(g);
    d2_CloseView(f);
    return r;
}
/* 0x536e80, the difficulty choice (the same form, five buttons from the text 0x1abf and a sixth one: the chosen level 0 .. 4 goes to
   ai_Difficulty and back to the option byte ai_GamePanel.byte[2]); returns 1 when confirmed */

int main_ChooseDifficulty(void)
{
    int r, f, t, b0, b1, b2, b3, b4, g;

    r = 0;
    f = d2_OpenForm(0, 0, "menu\\choosecharacter_intro");
    d2_CentreViewWindows(f);
    ai_Difficulty = ai_GamePanel.byte[2];
    t = io_PrintText(0x1abf);
    b0 = d2_GetWinObj(f, 0, t);
    b1 = d2_GetWinObj(f, 0, t + 1);
    b2 = d2_GetWinObj(f, 0, t + 2);
    b3 = d2_GetWinObj(f, 0, t + 3);
    b4 = d2_GetWinObj(f, 0, t + 4);
    g = d2_CreateRadioGroup(6, b0, b1, b2, b3, b4, d2_GetWinObj(f, 0, t + 5));
    d2_RadioSelect(g, ai_Difficulty);
    if (sv_RunFrame((int)main_ChooseDifficulty, 0xc6) != 0) {
        do {
            d2_MenuNavigate(g);
            if (dm_L.rup != 0 || kb_Last == 1) {
                r = 0;
                sv_PopRequest = 1;
            }
            if (d2_ClickedId == 0 || kb_Last == 0x1c) {
                if (d2_PointerObj == b0)
                    ai_Difficulty = 0;
                else if (d2_PointerObj == b1)
                    ai_Difficulty = 1;
                else if (d2_PointerObj == b2)
                    ai_Difficulty = 2;
                else if (d2_PointerObj == b3)
                    ai_Difficulty = 3;
                else if (d2_PointerObj == b4)
                    ai_Difficulty = 4;
                r = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)main_ChooseDifficulty, 0xc6) != 0);
    }
    ai_GamePanel.byte[2] = (unsigned char)ai_Difficulty;
    d2_RadioClear(g);
    d2_CloseView(f);
    return r;
}
/* 0x537040, the opponent selection ("menu\choose_opponents"): n rows (d2_WinOpenSlider list boxes "<name> <letter>", txt_Ptrs[0x42c6 ..]) with the entries of ai_Opponents, the ok button (text 0x7d); the chosen entries are stored back
   in 0x75e160; the row height is 10.0f / n * 24.0f @deep read against the original and rebuilt (63 -> 24 ins): the row height (int)(10.0f / n * 24.0f) (was a lost float), for loops with i++, y += h, if / do .. while frame loop, txt_Ptrs[0x42c6 .. 0x42cb] for the labels, ai_Opponents[i]. Left is allocation only: the original loads n into ebp before the first loop, computes y in eax and moves it into ebp after the n test (mov ebp,eax), reloads n from the stack inside the loop and moves it back into ebp after it (mov ebp,eax at +0x148); ours computes y in ebp directly and reloads n into ebx for the second loop (tried: i = 0 before y). Q7: d2.h declares d2_SetObjValue() without parameters */

void main_ChooseOpponents(int n)
{
    int f, h, y, i, ok;
    int ids[16];
    char buf[256];

    f = d2_OpenForm(0, 0, "menu\\choose_opponents");
    d2_CentreViewWindows(f);
    d2_SetForm(f, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(f, 1);
    io_PrintText(0x42c5);
    d2_SetForm(f, 3);
    h = (int)(10.0f / n * 24.0f);
    y = (d2_LastWindowPtr->h - h * n) / 2 + 0xe;
    for (i = 0; i < n; i++, y += h) {
        sprintf(buf, "%s %c", txt_Ptrs[0x42c6], i + 'A');
        ids[i] = d2_WinOpenSlider(0x80, y, 200, 0, 100, 0x7b, 0x802, d2_LastWindow);
        d2_SetObjValue4(ids[i], 0, 4, ai_Opponents[i]);
        d2_SetSliderFlags(ids[i], 0x82);
        d2_SetLabel(ids[i], buf);
        d2_SetList(ids[i], 5, txt_Ptrs[0x42c7], txt_Ptrs[0x42c8], txt_Ptrs[0x42c9], txt_Ptrs[0x42ca], txt_Ptrs[0x42cb]);
    }
    d2_SetForm(f, 2);
    ok = d2_GetWinObj(f, 2, io_PrintText(0x7d));
    if (sv_RunFrame((int)main_ChooseOpponents, 0xc6) != 0) {
        do {
            if (dm_L.rup != 0 || kb_Last == 1 || (d2_ClickedId == 0 && d2_PointerObj == ok))
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)main_ChooseOpponents, 0xc6) != 0);
    }
    for (i = 0; i < n; i++)
        ai_Opponents[i] = d2_GetObjValue(ids[i]);
    d2_CloseView(f);
}
/* 0x537230, the player setup / city choice of a new game ("Menu\CHOOSECITY", the start menu page "Startmenu/A_Stadtwahl.esc"; host = 1 for a network host (the list of ".NET" files, else ".CTY"): lists the city files
   "gamedata/cities/*" (0x5d33b0: up to 64 records of 0x210 bytes, the path at +0x109), reads each header (0x51cee0, an LSHEADER: +5 the name, from which the city point is created (ai_CreateCityPoint, its dummy "dummy_<NAME>"), +0x30 slotByte the player limit; diff[i] = limit - players (0x62489c)),
   creates the name object of the city (0x59a2c0 "stadt_<name>", the text plate 0x52e1c0 from the table 0x143a1c0 by the key "_STADTAUSWAHL_<NAME>_INFO+0" (0x42e770)), puts the city tower "sp_STADTTURM" (0x446e10,
   "sonstiges\wimpel_STADTTURM.baf") over the picked city (the pick object 0x448630 with a name starting with "stadt_" selects: sel; the default is the object of the 5th city) and shows its description
   ("_STADTAUSWAHL_<NAME>_BESCHR+0"); the buttons: ok (or enter / 0x1c) goes on to main_ChooseDifficulty and main_ChooseHistory (host: ends at once), the second button opens the opponent
   selection main_ChooseOpponents (diff of the picked city); returns 1 when a game was set up (the second parameter is the host flag, the first unused) */

int main_ChooseCity(int unused, int host)
{
    int ret, tower, last, cap, n, i, k, f, sel, f1, f2, h, ok, opp, out[2], w, ht;
    int cityobj[64], diff[64];
    char recs[64][0x210], key[0x100], name[0x40], buf4[0x100];
    LSHEADER hdr;
    float pos[4], rot[3];
    char *s, *d;
    char c;

    ret = 0;
    tower = 0;
    last = 0;
    cap = -1;
    sv_ClearNamePlates();
    sv_ReleaseNamePlates();
    d3if_SelectScene(0, 0);
    d3_SetObjectPos((int)d3_Camera, &d3_Camera->savedPos);
    d3_SetObjectRot((int)d3_Camera, &d3_Camera->savedRot);
    d3_SetLightSet(0, 0.0f, 1.0f, 0);
    if (host == 0)
        n = ai_ListFiles("gamedata/cities", ".CTY", (char *)recs);
    else
        n = ai_ListFiles("gamedata/cities", ".NET", (char *)recs);
    for (i = 0; i < n; i++) {
        f = vfs_Tfopen(recs[i] + 0x109, (char *)(unsigned)"rb", (int)vfs_Root);
        if (f == 0)
            return 0;
        if (ls_ReadSaveHeader(f, (int *)&hdr) == 0) {
            vfs_fclose(f);
            return 0;
        }
        vfs_fclose(f);
        diff[i] = (signed char)hdr.slotByte - (U_I((unsigned)&ai_NumPlayers) & 0xff);
        h = ai_CreateCityPoint(hdr.name);
        cityobj[i] = h;
        if (h != 0) {
            sprintf(key, "_STADTAUSWAHL_%s_INFO+0", hdr.name);
            _strupr(key);
            k = txt_FindKey(key);
            if (k != -1)
                strcpy(name, d2_Strings[k]);
            else
                strcpy(name, hdr.name);
            sprintf(buf4, "stadt_%s", hdr.name);
            strcpy((char *)h, buf4);
            sv_RegisterNamePlate(buf4, name, 0);
        }
    }
    sel = cityobj[4];
    if (sel != 0) {
        pos[0] = *(float *)(sel + 0x4c);
        pos[1] = *(float *)(sel + 0x50);
        pos[2] = *(float *)(sel + 0x54);
        pos[3] = 0;
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        tower = d3_CreateObject(0, pos, rot, "sp_STADTTURM");
        *(unsigned char *)(tower + 0x211) |= 4;
        *(unsigned char *)(tower + 0x212) = (*(unsigned char *)(tower + 0x212) & 0xf7) | 4;
        d3_RelightObject(tower);
        d3t_PlayAnim(tower, "sonstiges\\wimpel_STADTTURM.baf", 1);
    }
    d3if_Show(sel, 0);
    U_I((unsigned)&sv_FrameMask) = 0x12d6;
    U_I((unsigned)&cm_DialogFrameFlags) = 0x12d6;
    cm_WaitEvent(cm_StartScript("Startmenu/A_Stadtwahl.esc"));
    f1 = d2_OpenForm(0, 0, "Menu\\CHOOSECITY_HEADER");
    d2_PlaceMainWindowF(f1, 3);
    d2_PlaceViewWindows(f1, 1);
    d2_SetForm(f1, 0);
    io_PrintText(0x1abb);
    ht = d2_ScreenH;
    w = d2_ScreenW;
    d3_SetViewport((float)w, (float)ht, 0.0f, 0.0f, d3_ZNear, d3_ZFar, d3_ScaleX, w, ht);
    f2 = d2_OpenForm(0, 0, "Menu\\CHOOSECITY");
    d2_PlaceMainWindowF(f2, 3);
    d2_PlaceViewWindows(f2, 1);
    d2_SetForm(f2, 2);
    sv_ButtonRow(d2_LastWindow, 0, out, 2, txt_Ptrs[0x42cc], txt_Ptrs[0x42cd]);
    ok = out[0];
    opp = out[1];
    if (sv_RunFrame((int)main_ChooseCity, 0x10d6) != 0) {
        do {
            ai_ClickedObject = 0;
            h = (int)d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x60, 1);
            ai_PickedObject = (char *)h;
            if (dm_L.lup != 0) {
                ai_ClickedObject = h;
            } else if (dm_L.rup != 0 || kb_Last == 1) {
                sv_PopRequest = 1;
            }
            if (ai_ClickedObject != 0 && strncmp((char *)ai_ClickedObject, "stadt_", 6) == 0)
                sel = ai_ClickedObject;
            if (sel != last) {
                pos[0] = 0;
                pos[1] = 0;
                pos[2] = 0;
                pos[3] = 0;
                rot[0] = 0;
                rot[1] = 0;
                rot[2] = 0;
                d2_SetForm(f2, 1);
                io_PrintText((unsigned)"$C");
                pos[0] = pos[0] + *(float *)(sel + 0x4c);
                pos[1] = pos[1] + *(float *)(sel + 0x50);
                pos[2] = pos[2] + *(float *)(sel + 0x54);
                d3t_PlaceObjectV(tower, pos, rot, 0x28, 0x20);
                for (i = 0; i < n; i++) {
                    if (sel == cityobj[i]) {
                        if (cityobj[i] != 0) {
                            strcpy((char *)((unsigned)&ai_DataPath + 0x220), (char *)sel + 6);
                            sprintf(key, "_STADTAUSWAHL_%s_BESCHR+0", (char *)((unsigned)&ai_DataPath + 0x220));
                            _strupr(key);
                            k = txt_FindKey(key);
                            if (k != -1) {
                                d2_SetForm(f2, 1);
                                io_PrintText(k);
                                io_PrintText("$A");
                                io_PrintText(k + 1);
                            }
                            d3if_Show(sel, 0);
                            cap = diff[i];
                        }
                    } else if (cityobj[i] != 0) {
                        d3if_Show(cityobj[i], 1);
                    }
                }
                last = sel;
            }
            if (d2_ClickedId == 0 && d2_PointerObj == opp) {
                d2_SetViewEnabled(f2, 0);
                d2_SetViewEnabled(f1, 0);
                main_ChooseOpponents(cap);
                d2_SetViewEnabled(f2, 1);
                d2_SetViewEnabled(f1, 1);
            }
            if ((d2_ClickedId == 0 && d2_PointerObj == ok) || kb_Last == 0x1c) {
                d2_SetViewEnabled(f2, 0);
                d2_SetViewEnabled(f1, 0);
                if (host != 0) {
                    sv_PopRequest = 1;
                    ret = 1;
                } else {
                    if (main_ChooseDifficulty() != 0 && main_ChooseHistory() != 0) {
                        sv_PopRequest = 1;
                        ret = 1;
                        d2_SetViewEnabled(f2, 1);
                    }
                    d2_SetViewEnabled(f2, 1);
                    d2_SetViewEnabled(f1, 1);
                }
            }
        } while (sv_RunFrame((int)main_ChooseCity, 0x10d6) != 0);
    }
    if (f2 != -1)
        d2_CloseView(f2);
    if (f1 != -1)
        d2_CloseView(f1);
    if (tower != 0) {
        d3t_ReleaseObjectTexture(tower, "wimpel_STADTTURM");
        d3if_Delete((struct D3OBJECT *)tower);
    }
    for (i = 0; i < n; i++) {
        if (cityobj[i] != 0)
            d3if_Delete((struct D3OBJECT *)cityobj[i]);
    }
    return ret;
}
/* 0x537930, starts a new single player game: the player count 1 (0x62489c) and the player setup 0x537230 (1, 0); returns 1 when done, else the start screen is rebuilt */

int main_NewSinglePlayerGame(void)
{
    int r;

    ai_GameFlags = 0;
    d2_SetPointerSprite(0, 0);
    ai_NumPlayers = 1;
    r = main_ChooseCity(1, 0);
    if (r == 0) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_ShowPicture(0x797);
    }
    return r;
}
/* 0x537980, the loading screen: the form "Misc\Loading_game" (or "Misc\Loading_net" for a network game, 0x6f23f8 & 4), a progress bar (0x46a960), the music / sound hook 0x42e970 (0x44ee40, 7) */

void main_LoadingScreen(void)
{
    short s;
    int id;

    d2_CutsceneActive = 1;
    d2_ClearScreen();
    sv_SetScreenCenter(0x24087);
    s = (short)util_RandomInt(2);
    d2_ShowPicture(((s != 1) ? 3 : 0) + 0x78e);
    d2_DirtyRectLeft[0] = -1;
    d2_DirtyRectRight = -1;
    d2_DirtyRectTop = -1;
    d2_DirtyRectBottom = -1;
    d3_SetViewport((float)d2_ScreenW, (float)d2_ScreenH, 0.0f, 0.0f, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
    if (ai_GameFlags & 4)
        id = d2_OpenForm(0, 0, "Misc\\Loading_net");
    else
        id = d2_OpenForm(0, 0, "Misc\\Loading_game");
    init_StatusForm = id;
    d2_MoveObjS(0, d2_ScreenH - d2_Wins[d2_Views[id].wins[0]].h - 0x30, d2_Wins[d2_Views[id].wins[0]].parent);
    d2_PlaceWindow(d2_Views[init_StatusForm].wins[0], 1);
    d2_SetForm(init_StatusForm, 1);
    d2_WinOpenSlider(0, 0, 0x246, 0, 0x246, 0x83, 0x42, d2_LastWindow);
    d2_DrawAll();
    tb_OpenFunction(d3io_NoopCallback, 7);
}
/* 0x537af0, sets the loading progress bar to pct percent (the bar 0x246 wide) */

void main_SetLoadProgress(int unused, int pct)
{
    int b;

    if (init_StatusForm != -1) {
        d2_SetForm(init_StatusForm, 0);
        d2_SetForm(init_StatusForm, 1);
        b = d2_GetWinObj(init_StatusForm, 1, 0);
        d2_SetObjValue4(b, 0, 0x246, (pct * 0x246) / 100);
        d2_DrawAll();
    }
}
/* 0x537b60, the scene load's progress callback (kind, cur, total): kind 0 sets the bar to 10 + cur * 70 / (2 * total) percent, kind 1 to 45 + cur * 70 / (2 * total) (only
   when it differs from the last value main_LastLoadProgress) */

void main_LoadProgressCallback(int kind, int cur, int total)
{
    int v;

    if (kind == 0) {
        main_SetLoadProgress(0, cur * 70 / (total * 2) + 10);
    } else if (kind == 1) {
        v = cur * 70 / (total * 2) + 0x2d;
        if (v != main_LastLoadProgress) {
            main_LastLoadProgress = v;
            main_SetLoadProgress(0, v);
        }
    }
}
/* 0x537be0, closes the loading screen: fades to black (0x4767d0 "BLACK" 0x1e), runs the frame loop until the fade ends, frees the form 0x75df00 and clears the busy flag 0x6ed9d4 */

void main_CloseLoadingScreen(void)
{
    unsigned char *fade;
    unsigned mask;

    mask = 0x24087;
    if (net_Conn.s == -1)
        mask = 0x24086;
    tb_CloseFunction(d3io_NoopCallback);
    fade = (unsigned char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x1e, 1);
    while ((*fade & 4) == 0)
        sv_RunFrame(0, mask);
    d2_CloseView(init_StatusForm);
    ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
    d2_ClearSurface(d2_ScreenSurf);
    d2_CutsceneActive = 0;
    d2_ReleaseFade(fade);
}

void sv_UpdateClock(void);

/* 0x537c70, "main_PreloadSound": reads the sound definition file f (lines "#include "<bank>"", comments "//") and loads each named sound bank (0x426950, or the language bank 0x4c2830 when the path starts with
   "sprache\") into the table 0x75df20 (0x80 slots); returns 1 when done, 0 on errors (a message box "main_PreloadSound(): Could not open sound-definition!") */
int main_PreloadSound(char *f)
{
    int *slot;
    int *line;
    FILE *fp;
    char *q, *inc;
    char buf[0x100];
    int i;

    memset(main_SoundBanks, 0, sizeof(main_SoundBanks));
    fp = fopen(f, "rt");
    if (fp == 0) {
        MessageBoxA(0, "main_PreloadSound(): Could not open sound-definition!", "Error", 0x30);
        return 0;
    }
    slot = main_SoundBanks;
    do {
        memset(buf, 0, 0x100);
        fgets(buf, 0x100, fp);
        if (strstr(buf, "//") == 0 && (inc = strstr(buf, "#include")) != 0) {
            q = strchr(inc, 0x22);
            if (q == 0)
                return 0;
            q++;
            inc = strchr(q, 0x22);
            if (inc == 0)
                return 0;
            *inc = 0;
            if (_strnicmp(q, "sprache\\", 8) == 0) {
                q = strchr(q, 0x5c);
                *slot = snd_LoadVoiceBank(q + 1, 0);
            } else
                *slot = sb_LoadSampleBank(q, 0);
            slot++;
            if ((int)slot > (int)&main_SoundBanks[0x80])
                return 0;
        }
        if ((fp->_flag & 0x10) != 0) {
            fclose(fp);
            return 1;
        }
    } while (1);
}
/* 0x537db0, frees the preloaded sound banks (0x426ba0) of the table 0x75df20 */

void main_FreeSoundBanks(void)
{
    int *p;

    p = main_SoundBanks;
    do {
        if (*p != 0)
            sb_UnloadSampleBank(*p);
        p++;
    } while ((int)p < (int)&main_SoundBanks[0x80]);
}
/* 0x537de0, "start of a game": resets the global state (0x6f2358 = 1, 0x6f2598 = 1 the round number, the day 0x770ae0 = 2, the quit flags 0x6f259c / 0x6f25a0 / 0x6f25ad), registers the tick callback 0x5315c0
   (0x42e970 (cb, 0x47) / 0x42ea10), starts the sound banks (natur\regen, wind, blitzunddonner, objekte\buecher, TuerenUndRaeume, Transporter, Lebewesen\schritte) when the sound is on, and
   clears the selection (0x6f2308 .. 0x75ec60) before starting the modules (0x4bb810 anm, 0x50d460, 0x4e5770 ..) */

void main_StartGame(void)
{
    he_InitEvents();
    sv_ScreenDirty = 1;
    main_Round = 1;
    ai_Time.year = 2;
    ai_GameOver = 0;
    ai_LeaveGame = 0;
    main_GameEnded = 0;
    cm_SpeedLevel = 2;
    tb_OpenFunction(sv_UpdateClock, 0x47);
    tb_SetFunctionOff(sv_UpdateClock, 1);
    he_AgeCarryRecs();
    gv_ClearCrimes();
    ord_ResetStatistics();
    main_BookObject = d3io_LoadObjectRef("Buch", "Buch");
    if (snd_SfxEnabled != 0) {
        snd_Bank = sb_LoadSampleBank("natur\\regen.sbf", 0);
        main_WindBank = sb_LoadSampleBank("natur\\wind.sbf", 0);
        snd_ThunderBank = sb_LoadSampleBank("natur\\blitzunddonner.sbf", 0);
        snd_BookBank = sb_LoadSampleBank("objekte\\buecher.sbf", 0);
        snd_RoomBank = sb_LoadSampleBank("objekte\\TuerenUndRaeume.sbf", 0);
        snd_TransportBank = sb_LoadSampleBank("objekte\\Transporter.sbf", 0);
        ch_SndLib = sb_LoadSampleBank("Lebewesen\\schritte.sbf", 1);
    }
    msx_SetFadeTime(3000);
    he_CreateEventBar();
    sv_ClearNamePlates();
    sv_ReleaseNamePlates();
    main_NoRoundFade = 0;
    sv_SelectedObject = 0;
    sv_PanelShown = 0;
    ai_PlayerBuilding = 0;
    ai_FallbackBuilding = 0;
    ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
    anm_Alloc();
    ch_Reset();
    ai_InitBookmarks();
    init_ResetSceneUniverses();
    cm_InitScenes();
}
/* 0x537f40, "end of a game": the reverse of 0x537de0: plates / hints cleared, the tick callback removed, the sound banks and the weather emitters (0x76061c / 0x760618 / the sky layers 0x760620 ..) freed, the
   modules shut down (0x4b1e40, 0x4b39f0, 0x53d590 / 0x53d9b0 / 0x53d5d0, sv's 0x5223c0, 0x503e80, 0x4fe7f0 / 0x4fe830, the objects 0x1468ae0 ..), and the selection state cleared */

void main_EndGame(void)
{
    int i;

    sv_ClearNamePlates();
    d2_CopyCaption(ch_EmptyName);
    sv_SetStatusText(ch_EmptyName);
    d2_CutsceneActive = 1;
    tb_CloseFunction(sv_UpdateClock);
    cm_SkipEvents = 0;
    evt_UnloadAll();
    if (main_BookObject != 0)
        d3io_ReleaseObject(main_BookObject);
    if (snd_SfxEnabled != 0) {
        snd_ClearQueue();
        d3snd_DetachAll();
        if (snd_Bank != 0) {
            sb_UnloadSampleBank(snd_Bank);
            snd_Bank = 0;
            sb_UnloadSampleBank(snd_ThunderBank);
            snd_ThunderBank = 0;
            sb_UnloadSampleBank(snd_BookBank);
            snd_BookBank = 0;
            sb_UnloadSampleBank(snd_RoomBank);
            snd_RoomBank = 0;
            sb_UnloadSampleBank(snd_TransportBank);
            snd_TransportBank = 0;
            sb_UnloadSampleBank(ch_SndLib);
            ch_SndLib = 0;
        }
    }
    hst_ResetPlayerTexts();
    ai_ClearMissions();
    he_FreeEvents();
    he_CloseEventSlots();
    he_ReleaseEvents();
    sv_DestroyScreen();
    gm_ResetCharacters();
    gm_ClearBuildings();
    gm_ClearObjects();
    for (i = 0; i < 0x300; i++) {
        if (ch_List[i] != 0)
            ch_Delete(ch_List[i]);
    }
    if (d3t_Snow != 0) {
        d3t_FreeSnow(d3t_Snow);
        d3t_Snow = 0;
    }
    if (d3t_Rain != 0) {
        d3t_DeleteRain(d3t_Rain);
        d3t_Rain = 0;
    }
    if (d3_Sky != 0) {
        for (i = 0; i < 6; i++) {
            if (d3_SkyLayers[i] != 0) {
                d3sky_DeleteLayer(d3_Sky, d3_SkyLayers[i]);
                d3_SkyLayers[i] = 0;
            }
        }
        d3sky_Destroy(d3_Sky);
        d3_Sky = 0;
    }
    d3_FogByDistance = 0;
    if (d3t_SunRays != 0) {
        d3t_RemoveSunRays(d3t_SunRays);
        d3t_SunRays = 0;
    }
    d3t_CamMove = 0;
    init_FreeScenes();
    ai_FreePicture();
    anm_Free();
    ai_ResetMessages();
    udp_Exit();
    net_Close();
    main_UdpOpen = 0;
    d2_SetPointerSprite(0, 0);
    sv_SelectedObject = 0;
    sv_PanelShown = 0;
    ai_PlayerBuilding = 0;
    ai_FallbackBuilding = 0;
}
/* 0x538140, "main_RundenBeginn": the start of a game round (main_Round): for every active character (kind 1 .. 8; in a network game only those ai_CanAct allows, not kind 7) the one-shot message texts are queued as AP events (he_AddApEvent decrements the counter byte of an existing entry): the outstanding bills of the title groups of the kinds 6 / 5, the items 0x2c1 / 0x15b / 0x2c4, the flags 0x100000 / 0x200000 of flags1cc (sent back cleared), the guild house (bld2) and the buildings of the character; finally the money delta (the free capacity of what it carries + 4 + the sum of the AP events of the character) */

void main_RundenBeginn(void)
{
    MAINCARRY carry;
    char b5[0x200], b6[0x200], b4[0x200];
    char b1[0x100], b2[0x100], b3[0x100], b0[0x100];
    char hdr[0x100] = "";
    AICHAR *ch;
    MAINPLREC *rec;
    unsigned char *r;
    char *e;
    int i, k, m, t;

    net_PrintStats();
    net_ClearStats();
    sprintf(hdr, "main_RundenBeginn(): init round no %i", main_Round);
    for (i = 0; i < 768; i++) {
        ch = &ai_CharsT[i];
        if (ch->id != 0xffff && ch->kind >= 1 && ch->kind <= 8 && (net_Conn.s == -1 || (ai_CanAct(ch) != 0 && (net_Conn.s == -1 || ch->kind != 7)))) {
            if (ch->kind == 6 || ch->kind == 5) {
                rec = (MAINPLREC *)gm_PlayerRecord((char *)ch);
                k = 0;
                do {
                    t = gm_TitleJobI(ch->id, k);
                    if (gm_TitlesLeft(t) == 6) {
                        txt_Format(b4, 0x15c5, rec->job[k] + 0x130);
                        e = (char *)he_AddApEvent(ch->id, b4, 1, 0);
                        if (e != 0)
                            *e = *e - 1;
                    }
                    k++;
                } while (k < 0x11);
            }
            m = ai_InventoryHas(ch, 0x2c1);
            if (m != 0) {
                txt_Format(b1, 0x2052, 0xf2a);
                e = (char *)he_AddApEvent(ch->id, b1, 6, 0);
                if (e != 0)
                    *e = *e - 1;
            }
            m = ai_InventoryHas(ch, 0x15b);
            if (m != 0) {
                txt_Format(b2, 0x2052, 0xc5e);
                e = (char *)he_AddApEvent(ch->id, b2, 1, 0);
                if (e != 0)
                    *e = *e - 1;
            }
            m = ai_InventoryHas(ch, 0x2c4) != 0;
            if ((ch->flags1cc & 0x100000) != 0) {
                if (m == 0) {
                    txt_Format(b3, 0x2054, 0xf18);
                    e = (char *)he_AddApEvent(ch->id, b3, -3, 0);
                    if (e != 0)
                        *e = *e - 1;
                }
                cm_SendSetField(ch->netId, 0x1cc, 4, 0, 0x100000);
            }
            if ((ch->flags1cc & 0x200000) != 0) {
                if (m == 0) {
                    txt_Format(b0, 0x2054, 0xf2c);
                    e = (char *)he_AddApEvent(ch->id, b0, -8, 0);
                    if (e != 0)
                        *e = *e - 1;
                }
                cm_SendSetField(ch->netId, 0x1cc, 4, 0, 0x200000);
            }
            gm_CollectCarried(ch->id, (int *)&carry);
            if (ch->bld2 != 0 && (k = gm_BuildingSkill((unsigned char *)ch->bld2, 3, 1)) != 0) {
                txt_Format(b5, 0x2053, (int)ch->bld2);
                e = (char *)he_AddApEvent(ch->id, b5, k, 0);
                if (e != 0)
                    *e = *e - 1;
            }
            r = gm_BuildQuery(2, 4, ch->id, 5, 5);
            while (r != 0) {
                k = gm_BuildingSkill(r, 3, 1);
                if (k != 0) {
                    txt_Format(b6, 0x3fd7, (int)r);
                    e = (char *)he_AddApEvent(ch->id, b6, k, 0);
                    if (e != 0)
                        *e = *e - 1;
                }
                r = gm_BuildNext();
            }
            t = carry.capacity + 4;
            t += he_SumApEvents(ch->id);
            cm_SendMoneyDelta(ch->netId, t);
        }
    }
}

/* message record (0xf8 bytes) of 0x4c4b70 / 0x4e4060: kind byte +4, owner id +8, target +0xc, time stamp (the 0x770ae0 clock, 14 bytes) +0x28 .. +0x36, flag +0x36 = 1 */
#define MAIN_PID ((int)(U_I((unsigned)&ai_PlayerId) & 0xffff))  /* the player character */
#define MSG_B(m, o) (*(unsigned char *)((m) + (o)))
#define MSG_I(m, o) (*(int *)((m) + (o)))
#define main_PlayerIdInt (*(int *)&ai_PlayerId)  /* 0x6f25b8, ai_PlayerId (unsigned short) as the int the round change reads and masks */
#define MAIN_PLAYER (main_PlayerIdInt & 0xffff)  /* the player character's index */
extern unsigned char main_GameEnded;  /* 0x6f25ad, sv.c */
/* the announcement message of the kind `kind` to the player's own character (fromId -1), stamped with the game clock */
#define MAIN_NOTE(m, kind) do { memset(&(m), 0, sizeof(m)); (m).type = (kind); (m).fromId = -1; (m).flags = 1; (m).to = ai_CharsT[MAIN_PLAYER].netId; (m).time = ai_Time; } while (0)

/* 0x5384e0, the round / day change of the player: fades out (BLACK), advances the clock copy (+1 when networked (ai_GameFlags bit 2) after the first round), queues the announcement messages of the kinds 0x66 / 0x70 / 0x68 / 0x85 / 0x7c / 0x93 / 0x84 /
   0x4c when none is pending (he_FindEvent), shows the balance page of the player (texts 0x1ef9 .. 0x1eff, 0x2051: the AP events of the player with their incomes / costs and the goods of gm_CollectCarried), takes the AP event costs, renovates the run-down
   buildings of the masters this machine runs (kind 0x1c message "renovieren") and spreads each character's goods value over its buildings as their work load (AIBLD.budget), then beams the figures of the characters home ("Beamed home")
   @deep Read the whole function against dcp disasm / asmdiff and reconstructed it: the AP event balance
   lines (HEAPEVENT table 0x758ca0, the income / cost branch structure), the carried goods list at
   MAINCARRY+4 (was misread at +0x2e), the time-stamped AIMSG notes (to before time), the 0x4c note, the
   cart visibility pair of calls, the renovation (32.0 > cost, budget * 0.75 > cost), the building budget
   split with >= caps through one store, unsigned short id compares, the Beamed home strcpy,
   snd_FadeMusic(1.0f); block-scoped value/cost/budget and x/y/place give the original's 0x3b8 frame. Same
   instruction count (1226 / 1226), 74 differing. Left is allocation / scheduling only: 0x538ed0 the 0x4c
   note's time copy stores scheduled around the push of &msg (ours stores hour / unk8 / unkc before the
   push, the original hour / year before and unk8 after it); 0x538f4c gm_Query argument and the cart bldId
   in ecx / edx where the original reuses eax / ecx; 0x538f85 the &ai_Chars[owner] address in eax vs edx;
   0x539101 the util_Random(0x5a) limit in edi (mov edi, eax; and edi, 0xffff) where the original keeps it
   in bx (mov bx, ax; and ebx, 0xffff) because its weight sum sits in ebp and ours in ebx; plus 56 regs / 8
   order hunks of the same ebx / ebp / edi rotation in the building loops. Tried: five MAIN_NOTE store
   orders, four 0x4c orders (time first gives 4402 bytes but rotates every register of the building loops),
   the query inline, three renovation store orders. */
void main_EndRound(void)
{
    MAINCARRY carry;   /* what gm_CollectCarried collects (the original's buffer sits at +0x1a8 of a 0x3b8 frame) */
    struct {
        int clock0;
        int tick;
        char pad[0x74];
    } sysmsg;
    AIMSG msg;
    short rid;
    AITIME clk;
    D2FADE *fade;
    AIFIN *fin;
    AIBLD *b;
    AIOBJ *obj;
    CHARACTER *fig;
    CHACTION *act;
    int season;
    int win, r, k, q;
    CHARACTER *scene;
    int weight;

    fin = (AIFIN *)gm_PlayerRecord(ai_Chars[MAIN_PLAYER]);
    fade = 0;
    gm_ClockSeason((unsigned)&ai_Time);
    clk = ai_Time;
    if ((ai_GameFlags & 4) != 0 && 1 < main_Round)
        clk.year++;
    season = sv_ClockSeason(&clk);
    ahm_FillSteps();
    fin->roundFactors[0] = 0.5f;
    fin->roundFactors[1] = 1.0f;
    fin->roundFactors[2] = 1.0f;
    if (main_GfxOn != 0) {
        d2_DirtyRectRight = 0;
        d2_DirtyRectLeft[0] = d2_ScreenW;
        d2_DirtyRectTop = 0;
        d2_DirtyRectBottom = d2_ScreenH;
        if (main_NoRoundFade == 0) {
            fade = d2_StartFade(0, 0, d2_DirtyRectLeft[0], d2_DirtyRectBottom, "BLACK", 0, 1);
            while ((fade->flags & 4) == 0)
                sv_RunFrame(0, 0x24087);
            d2_ShowPicture(0x79d);
            d2_ReleaseFade(fade);
            d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x32, 10);
        }
    }
    if ((ai_GameFlags & 4) != 0)
        sv_NetworkSyncWait(0x24087);
    gm_CollectCarried(MAIN_PLAYER, (int *)&carry);
    sv_FillDayPhases((int)&clk);
    sv_Season = sv_ClockSeason(&clk);
    gm_FindRichest(&rid, &k);
    k = 0;
    sysmsg.clock0 = clk.year;
    sysmsg.tick = timeGetTime();
    q = cm_SendExSysMessage(0, &sysmsg);
    while (cm_GetSlotState(q) == 0)
        sv_PumpNet();
    tb_SetFunctionOff((int)sv_UpdateClock, 1);
    if ((ai_GameFlags & 0x80) == 0) {
        if (he_FindEvent(1, 0, 0x66) == 0) {
            MAIN_NOTE(msg, 0x66);
            cm_SendLongMessage((char *)&msg);
        }
        if (he_FindEvent(1, 0, 0x70) == 0) {
            MAIN_NOTE(msg, 0x70);
            cm_SendLongMessage((char *)&msg);
        }
        if (he_FindEvent(1, 0, 0x68) == 0) {
            MAIN_NOTE(msg, 0x68);
            cm_SendLongMessage((char *)&msg);
        }
    }
    if (cm_TutorialActive != 0 && he_FindEvent(1, 0, 0x85) == 0) {
        MAIN_NOTE(msg, 0x85);
        cm_SendLongMessage((char *)&msg);
    }
    if (he_FindEvent(1, 0, 0x7c) == 0) {
        MAIN_NOTE(msg, 0x7c);
        cm_SendLongMessage((char *)&msg);
    }
    if (he_FindEvent(1, 0, 0x93) == 0) {
        MAIN_NOTE(msg, 0x93);
        cm_SendLongMessage((char *)&msg);
    }
    if (he_FindEvent(1, 0, 0x84) == 0 && ai_GamePanel.byte[3] != 0) {
        MAIN_NOTE(msg, 0x84);
        cm_SendLongMessage((char *)&msg);
    }
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 0);
    he_ShowEventBar(0xffff);
    if (main_GfxOn != 0) {
        win = sv_CreateScrollForm();
        d2_SetForm(win, 2);
        io_PrintText(0x7d);
        d2_CreateScrollButtons(-1, -1, d2_LastWindow, d2_GetViewWin(win, 1), 0x767);
        d2_Wins[d2_GetViewWin(win, 1)].visible = 0x18;
        d2_SetForm(win, 1);
        io_PrintText(0x2051, &clk, (int)gm_DayStartHours[season], (int)gm_DayEndHours[season] % 0xc);
        io_PrintText(0x1ef9);
        io_PrintText(0x1eff, ai_CharsT[MAIN_PLAYER].v190);
        k = 0;
        do {
            if ((he_ApEvents[k].income != 0 || he_ApEvents[k].cost != 0 || he_ApEvents[k].name[0] != 0) && he_ApEvents[k].key == MAIN_PLAYER) {
                if (he_ApEvents[k].income != 0) {
                    if (he_ApEvents[k].income >= 0)
                        io_PrintText(0x1efc, he_ApEvents[k].name, he_ApEvents[k].income);
                    else
                        io_PrintText(0x1efb, he_ApEvents[k].name, he_ApEvents[k].income);
                } else if (he_ApEvents[k].cost > 0)
                    io_PrintText(0x1efe, he_ApEvents[k].name, he_ApEvents[k].cost);
                else if (he_ApEvents[k].cost < 0)
                    io_PrintText(0x1efd, he_ApEvents[k].name, he_ApEvents[k].cost);
                else
                    io_PrintText(0x1efb, he_ApEvents[k].name);
            }
            k++;
        } while (k < 0x100);
        k = 0;
        while (carry.goods[k].good != 0 && k < 9) {
            io_PrintText(0x1efd, carry.goods[k].good + 0x253, carry.goods[k].amount);
            k++;
        }
        io_PrintText(0x1efa, ai_CharsT[MAIN_PLAYER].money);
        r = sv_RunFrame((int)main_EndRound, 0x24087);
        while (r != 0) {
#ifdef REA_START
            /* REA_START=tutorial (`uv run play|dbg --start tutorial`): the first screen the started tutorial
               stops at is this round-change page with its "Weiter" button. The hook advances it the way a
               click does (sv_PopRequest, the only thing the button's handler leaves behind), so a run without
               GUI automation gets past the page. The frame above has drawn and registered it once. */
            {
                char *rea = getenv("REA_START");
                if (rea != 0 && strcmp(rea, "tutorial") == 0)
                    sv_PopRequest = 1;
            }
#endif
            if (d2_ClickedId == 0 || kb_Last == 0x1c)
                sv_PopRequest = 1;
            r = sv_RunFrame((int)main_EndRound, 0x24087);
        }
        fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x1e, 1);
        while ((fade->flags & 4) == 0)
            sv_RunFrame(0, sv_FrameMask | 0x80);
        sv_DestroyScrollForm();
        ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
    }
    cm_SendMoveGoodsUnchecked(-1, ai_CharsT[MAIN_PLAYER].netId, ai_OwnPlayer, -he_SumApEventCosts(MAIN_PLAYER));
    ai_PostRandomMessages(3, ai_CharsT[MAIN_PLAYER].netId);
    cm_SendExSysMessage(1, 0);
    if ((ai_GameFlags & 4) != 0)
        sv_NetworkSyncWait(0x24087);
    d2_DirtyRectRight = sv_ViewRectRight;
    d2_DirtyRectLeft[0] = sv_ViewRectLeft;
    d2_DirtyRectTop = sv_ViewRectTop;
    d2_DirtyRectBottom = sv_ViewRectBottom;
    init_AfterTownLoad();
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    if (main_GfxOn != 0) {
        if (fade != 0)
            d2_ReleaseFade(fade);
        d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x32, 10);
    }
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 1);
    if (ai_MissionMode != 5 && ai_MissionMode > -1 && he_FindEvent(2, 0, 0x4c, 2, MAIN_PLAYER) == 0) {
        MAIN_NOTE(msg, 0x4c);
        msg.fromId = msg.to;
        msg.nIds = 0;
        msg.reserved14 = 0;
        cm_SendLongMessage((char *)&msg);
    }
    if ((ai_GameFlags & 8) != 0) {
        b = (AIBLD *)gm_BuildQuery(1, 0, 0x44);
        obj = (AIOBJ *)gm_Query((void *)b->objList, 1, 4, 0x1d);
        while (obj != 0) {
            scene = (CHARACTER *)obj->sceneObj;
            b = (AIBLD *)gm_BuildQuery(1, 1, obj->bldId);
            if (b != 0 && &ai_CharsT[b->owner] != 0 && ai_CanAct(&ai_CharsT[b->owner]) != 0)
                ch_SetVisible(scene, 1);
            else
                ch_SetVisible(scene, 0);
            obj = (AIOBJ *)gm_QueryNext();
        }
    }
    k = 0;
    do {
        if (ai_CharsT[k].id != 0xffff && ai_CharsT[k].kind != 6 && ai_CharsT[k].kind != 7) {
            int value;
            float cost, budget;

            value = gm_GetGoodsValue(ai_Chars[k]) * 0x4b;
            weight = 0;
            b = (AIBLD *)gm_BuildQuery(1, 4, k);
            while (b != 0) {
                if (AI_KIND(b->type) == 1 || AI_KIND(b->type) == 4 || AI_KIND(b->type) == 2 || ai_BTypesT[b->type].kind == 0x13 || ai_BTypesT[b->type].kind == 4 || ai_BTypesT[b->type].kind == 0x10 || ai_BTypesT[b->type].kind == 5 || ai_BTypesT[b->type].kind == 9) {
                    if (ai_BTypesT[b->type].level == 1)
                        weight += 2;
                    else if (ai_BTypesT[b->type].level == 2)
                        weight += 4;
                    else if (ai_BTypesT[b->type].level == 3)
                        weight += 8;
                }
                if (ai_IsLocalMaster(&ai_CharsT[k]) != 0 || ai_CharsT[k].kind == 1) {
                    unsigned short lim = (unsigned short)util_Random(0x5a);
                    if (gm_BuildingLevel((char *)b) < lim) {
                        cost = (float)gm_EquipmentValue((unsigned char *)b, 100) * 0.005f;
                        if (32.0f > cost)
                            cost = 32.0f;
                        budget = (float)value;
                        if (budget * 0.75f > cost) {
                            memset(&msg, 0, sizeof(msg));
                            msg.type = 0x1c;
                            msg.fromId = -1;
                            msg.flags = 1;
                            msg.arg[1] = 10;
                            msg.to = ai_CharsT[k].netId;
                            msg.arg[0] = b->id;
                            cm_BeginGroup("renovieren");
                            cm_SendMoveGoods(-1, ai_CharsT[k].netId, ai_OwnPlayer, (int)cost);
                            cm_SendLongMessage((char *)&msg);
                            cm_EndGroup("renovieren");
                            fin = (AIFIN *)gm_PlayerRecord(ai_Chars[k]);
                            if (fin != 0)
                                fin->renovationSpent = (int)(fin->renovationSpent + cost);
                            value = (int)(budget - cost);
                        }
                    }
                }
                b = (AIBLD *)gm_BuildNext();
            }
            b = (AIBLD *)gm_BuildQuery(1, 4, k);
            while (b != 0) {
                if (AI_KIND(b->type) == 1 || AI_KIND(b->type) == 4 || AI_KIND(b->type) == 2 || ai_BTypesT[b->type].kind == 0x13 || ai_BTypesT[b->type].kind == 4 || ai_BTypesT[b->type].kind == 0x10 || ai_BTypesT[b->type].kind == 5 || ai_BTypesT[b->type].kind == 9) {
                    if (ai_BTypesT[b->type].level == 1) {
                        b->budget = value * 2 / weight;
                        b->budget = b->budget < 0 ? 0 : b->budget;
                        if (ai_BTypesT[b->type].kind == 5)
                            b->budget = b->budget < 192000 ? b->budget : 192000;
                        else
                            b->budget = b->budget < 64000 ? b->budget : 64000;
                    } else if (ai_BTypesT[b->type].level == 2) {
                        b->budget = value * 4 / weight;
                        b->budget = b->budget < 0 ? 0 : b->budget;
                        if (ai_BTypesT[b->type].kind == 5)
                            b->budget = b->budget < 320000 ? b->budget : 320000;
                        else
                            b->budget = b->budget < 128000 ? b->budget : 128000;
                    } else if (ai_BTypesT[b->type].level == 3) {
                        b->budget = value * 8 / weight;
                        b->budget = b->budget < 0 ? 0 : b->budget;
                        if (ai_BTypesT[b->type].kind == 5)
                            b->budget = b->budget < 640000 ? b->budget : 640000;
                        else
                            b->budget = b->budget < 256000 ? b->budget : 256000;
                    }
                }
                b = (AIBLD *)gm_BuildNext();
            }
        }
        k++;
    } while (k < 0x300);
    for (k = 0; k < 0x300; k++) {
        if (ai_CharsT[k].id != 0xffff) {
            ai_CharsT[k].flags1c8 &= 0xffffe7ff;
            if (ai_IsLocalJob(&ai_CharsT[k]) != 0)
                ai_CharsT[k].budget = ai_CharsT[k].bld->budget;
        }
    }
    for (k = 0; k < 0x300; k++) {
        int x, y, place;

        x = -1;
        y = -1;
        if (ai_CharsT[k].model != 0 && he_FindFigurePlace(ai_Chars[k], &x, &y, &place) != 0) {
            fig = (CHARACTER *)ai_CharsT[k].model;
            if (x != fig->group || y != fig->scriptGroup) {
                if (fig->scriptId != -1)
                    evt_Stop(evt_FindById(fig->scriptId));
                ch_StopActions((CHARACTER *)ai_CharsT[k].model);
                if (y != -1 && (act = ch_AddAction((CHARACTER *)ai_CharsT[k].model, 0x33, place, x, y)) != 0)
                    memcpy(CHACT_NAME70(act), main_BeamedHome, sizeof main_BeamedHome);
            }
        }
        if ((ai_GameFlags & 8) != 0 && ai_CharsT[k].model != 0 && ai_CharsT[k].kind == 0xd && ai_CharsT[k].job == 0)
            cm_SendKillCharacter(ai_CharsT[k].netId, 1);
    }
    hst_SeedPending = 1;
    amt_TellOpenOffices(ai_Chars[MAIN_PLAYER]);
    snd_FadeMusic(1.0f, 2000);
}
/* 0x539620, replaces a dead / imprisoned player (character slot p, kind 6 and health 0) by a new figure: with no
   slot of kind 6 and health left it sets ai_GameOver; the character's events (he_Events as AIJOB records) whose
   kind is in la are closed (he_FreeEvent), and a second walk (terminated by lb, comparing la) sets their stage
   to -1; the kind becomes 5, the name is taken from txt_MaleFirstNames (flag9 clear, random 0xbf) or
   txt_FemaleFirstNames (random 0x70), seq takes the old ai_NextCharNumber (then raised) and the eight ids are
   reset; finally every party record (cm_Parties, G28REC) the character is a member of, with a kind 3 .. 9, is
   cleared (cm_ClearParty). */

void main_ReplaceFigure(unsigned p)
{
    int la[15] = { 0x5d, 0x11, 0x30, 0x35, 0x42, 0x60, 0x4b, 0x5b, 0x5c, 0x5a, 0x46, 0x2e, 0x2d, 0x41, 0 };
    int lb[3] = { 0x45, 0x47, 0 };
    MAINCHARNEW *ch;
    AIJOB *e;
    G28REC *pt;
    int *pi;
    int i, j;

    i = 0;
    while (ai_CharsT[i].kind != 6 || ai_CharsT[i].health == 0) {
        i++;
        if (i >= 768) {
            ai_GameOver = 1;
            return;
        }
    }
    if (i == 0xffff) {
        ai_GameOver = 1;
        return;
    } else if (ai_CharsT[p].kind == 6 && ai_CharsT[p].health == 0) {
        ch = (MAINCHARNEW *)&ai_CharsT[p];
        e = (AIJOB *)he_Events;
        do {
            if (e->kind != 0 && e->chr == p) {
                for (pi = la; *pi != 0; pi++) {
                    if (*pi == (unsigned char)e->kind) {
                        he_FreeEvent((int *)e);
                        break;
                    }
                }
                for (j = 0; lb[j] != 0; j++) {
                    if (la[j] == (unsigned char)e->kind) {
                        e->stage = -1;
                        break;
                    }
                }
            }
            e++;
        } while ((int)e < (int)((AIJOB *)he_Events + 0x400));
        ch->kind = 5;
        if (ch->flag9 == 0) {
            strcpy(ch->name, txt_MaleFirstNames[(unsigned short)util_RandomInt(0xbf)]);
        } else {
            strcpy(ch->name, txt_FemaleFirstNames[(unsigned short)util_RandomInt(0x70)]);
        }
        ch->seq = (short)ai_NextCharNumber;
        for (i = 0; i < 8; i++)
            ch->clear[i] = -1;
        ai_NextCharNumber = ai_NextCharNumber + 1;
        /* the original walks the records by their kind byte, 8 bytes into each */
        for (pt = cm_Parties; (int)&pt->kind < (int)((unsigned)cm_Parties + 0x6788); pt++) {
            if (pt->used != 0) {
                if (cm_IsPartyMember(ch->netId, (char *)pt) != 0 && (pt->kind == 3 || pt->kind == 5 || pt->kind == 4 || pt->kind == 6 || pt->kind == 9 || pt->kind == 7))
                    cm_ClearParty(pt->id);
            }
        }
    }
}
/* 0x539870, the end of the round report of the player (0x6f25b8): the bookkeeping of the character record (0x506430: income / expense fields +0x14 .. +0x54, texts 0x2005 .. 0x204e of 0x515960)
   shown on a page (BLACK fade, the window 0x52fa10, the loop 0x5309a0 until a key), then the fields are rolled over (+0x14 .. +0x24 added to +0x2c .. +0x38 and cleared); when the balance goes
   negative (0x50a6c0) a message of kind 0x60 / 9 is sent and a replacement figure is created (0x539620) if 0x6f25b0 is set; the two row sums are float sums (price 0xc0 * 0.01 * count 0xb4, truncated per step) */

void main_RoundReport(void)
{
    char msg[0x114];
    int lines[0x2b];
    int a[3];
    int sum[6], cnt[6];
    unsigned char kind[0x38];  /* the GMINCOME record gm_CharacterIncomes clears (0xe ints) */
    unsigned char *fade;
    unsigned char *b;
    unsigned char *o;
    int bk, iv, v, t, tot, win, nrow, i, j, k;
    int w200, w1fc, w1f8, w1ec, w1e8;

    iv = 0;
    w200 = 0;
    w1ec = 0;
    w1e8 = 0;
    fade = 0;
    U_I((unsigned)&sv_PanelFlag) = 0;
    if (main_GfxOn != 0) {
        fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x1e, 1);
        while ((*fade & 4) == 0)
            sv_RunFrame(0, U_I((unsigned)&sv_FrameMask));
    }
    bk = gm_PlayerRecord((unsigned)&ai_CharsT + MAIN_PID * 0x21c);
    *(int *)(bk + 0x3c) = *(int *)(bk + 0x3c) + (U_I(((unsigned)&ai_Selected + 0x1c) + MAIN_PID * 0x21c) + U_I(((unsigned)&ai_Selected + 0x18) + MAIN_PID * 0x21c));
    d3_SetObjectPos((int)d3_Camera, bk + 0x8c);
    d3_SetObjectRot((int)d3_Camera, bk + 0x9c);
    if (main_GfxOn != 0) {
        w1f8 = 0;
        d2_SetPointerSprite(0, 0);
        sv_ClosePanelWidgets();
        sv_SetScreenCenter(0x24087);
        win = sv_CreateScrollForm();
        d2_CentreViewWindows(win);
        if (fade != 0) {
            d2_ReleaseFade(fade);
            fade = 0;
        }
        d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x1e, 10);
        d2_SetForm(win, 2);
        io_PrintText(0x7d);
        gm_ClockToDate((unsigned)&ai_Time, (char *)lines);
        d2_SetForm(win, 1);
        io_PrintText(0x2007);
        iv = U_I(((unsigned)&ai_Selected + 0x14) + MAIN_PID * 0x21c);
        w1fc = iv - U_I(((unsigned)&ai_Selected + 0x10) + MAIN_PID * 0x21c);
        if (iv != 0)
            io_PrintText(0x2008, iv);
        if (U_I(((unsigned)&ai_Selected + 0x10) + MAIN_PID * 0x21c) != 0)
            io_PrintText(0x2009, U_I(((unsigned)&ai_Selected + 0x10) + MAIN_PID * 0x21c));
        iv = w1fc;
        if (*(int *)(bk + 0x40) != 0) {
            io_PrintText(0x2014, *(int *)(bk + 0x40));
            w200 = *(int *)(bk + 0x40);
            iv = w1fc - w200;
        }
        if (*(int *)(bk + 0x44) != 0) {
            io_PrintText(0x2016, *(int *)(bk + 0x44));
            iv -= *(int *)(bk + 0x44);
            w200 += *(int *)(bk + 0x44);
        }
        if (*(int *)(bk + 0x48) != 0) {
            io_PrintText(0x201b, *(int *)(bk + 0x48));
            iv -= *(int *)(bk + 0x48);
            w200 += *(int *)(bk + 0x48);
        }
        if (*(int *)(bk + 0x4c) != 0) {
            io_PrintText(0x201a, *(int *)(bk + 0x4c));
            iv -= *(int *)(bk + 0x4c);
            w200 += *(int *)(bk + 0x4c);
        }
        if (*(int *)(bk + 0x54) != 0) {
            io_PrintText(0x2015, *(int *)(bk + 0x54));
            iv -= *(int *)(bk + 0x54);
            w200 += *(int *)(bk + 0x54);
        }
        if (*(int *)(bk + 0x28) != 0) {
            io_PrintText(0x201c, *(int *)(bk + 0x28));
            iv -= *(int *)(bk + 0x28);
            w200 += *(int *)(bk + 0x28);
        }
        if (*(int *)(bk + 0x14) != 0) {
            io_PrintText(0x200d, *(int *)(bk + 0x14));
            iv += *(int *)(bk + 0x14);
            w1fc += *(int *)(bk + 0x14);
        }
        if (*(int *)(bk + 0x18) != 0) {
            io_PrintText(0x200c, *(int *)(bk + 0x18));
            iv += *(int *)(bk + 0x18);
            w1fc += *(int *)(bk + 0x18);
        }
        if (*(int *)(bk + 0x1c) != 0) {
            io_PrintText(0x200b, *(int *)(bk + 0x1c));
            iv += *(int *)(bk + 0x1c);
            w1fc += *(int *)(bk + 0x1c);
        }
        if (*(int *)(bk + 0x20) != 0) {
            io_PrintText(0x2010, *(int *)(bk + 0x20));
            iv += *(int *)(bk + 0x20);
            w1fc += *(int *)(bk + 0x20);
        }
        if (*(int *)(bk + 0x50) != 0) {
            io_PrintText(0x201d, *(int *)(bk + 0x50));
            iv -= *(int *)(bk + 0x50);
            w200 += *(int *)(bk + 0x50);
        }
        if (iv == 0)
            io_PrintText(0x201e);
        else
            io_PrintText(0x204c, iv);
        d2_SetForm(win, 3);
        *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 0x27c) = 0x55;
        io_PrintText(0x2005);
        d2_SetForm(win, 1);
        io_PrintText((unsigned)"$N");
        b = (unsigned char *)gm_BuildQuery(1, 6);
        while (b != 0) {
            if (*(short *)(b + 0x27) == (short)U_I((unsigned)&ai_PlayerId) || *(short *)(b + 0x25) == (short)U_I((unsigned)&ai_PlayerId)) {
                t = gm_BuildingClass(*b);
                v = gm_BuildingBalance(b, U_I((unsigned)&ai_PlayerId) & 0xffff, a);
                if (v != 0 || (t != 5 && t != 6)) {
                    w1fc += a[0];
                    w1ec += a[1];
                    w200 += a[2];
                    w1f8 += a[2];
                }
            }
            b = (unsigned char *)gm_BuildNext();
        }
        nrow = gm_BuildingEvents(U_I((unsigned)&ai_PlayerId) & 0xffff, 0, 0xffff, lines);
        j = 0;
        v = 0;
        if (nrow > 0) {
            do {
                o = (unsigned char *)lines[j];
                w1fc = (int)((double)w1fc + (double)*(int *)(o + 0xc0) * 0.01 * *(int *)(o + 0xb4));
                v = (int)((double)v + (double)*(int *)(o + 0xc0) * 0.01 * *(int *)(o + 0xb4));
                w1f8 = (int)((double)w1f8 + (double)*(int *)(o + 0xc0) * 0.01 * *(int *)(o + 0xb4));
                j++;
            } while (j < nrow);
            if (v != 0)
                io_PrintText(0x2048, v);
        }
        k = MAIN_PID * 0x21c;
        if (*(char *)(((unsigned)&ai_CharInts + 0x162) + k) != 0 || *(char *)(((unsigned)&ai_CharInts + 0x165) + k) != 0) {
            gm_CharacterWages(U_I((unsigned)&ai_PlayerId), 0, (unsigned char *)a);
            k = (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c;
            t = (-(*(char *)((unsigned)&ai_CharSex + k) != 0) & 0x28) + 0x253;
            if (*(char *)(((unsigned)&ai_CharInts + 0x162) + k) != 0)
                io_PrintText(0x2049, *(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + k) + t, a[1]);
            if (*(char *)(((unsigned)&ai_CharInts + 0x165) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) != 0)
                io_PrintText(0x2049, *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) + t, a[2]);
            w1f8 = w1f8 + a[1] + a[2];
            w1fc = w1fc + a[1] + a[2];
        }
        if (gm_CharacterIncomes(U_I((unsigned)&ai_PlayerId), (GMINCOME *)kind, 0) != 0) {
            i = 0;
            do {
                if (kind[i] == 0)
                    break;
                io_PrintText(main_RoundTotalFmt, kind[i] + 0x1f64, cnt[i], sum[i]);
                t = sum[i];
                w1f8 -= t;
                w200 += t;
                w1e8 += t;
                i++;
            } while (i < 6);
        }
        if (w1ec != 0)
            io_PrintText(0x204a, w1ec);
        j = gm_CreditEvents(U_I((unsigned)&ai_PlayerId) & 0xffff, 0, lines);
        iv = 0;
        k = 0;
        if (0 < j) {
            do {
                o = (unsigned char *)lines[k];
                w200 = (int)((double)w200 + (double)*(int *)(o + 0xc0) * 0.01 * *(int *)(o + 0xb4));
                iv = (int)((double)iv + (double)*(int *)(o + 0xc0) * 0.01 * *(int *)(o + 0xb4));
                w1f8 = (int)((double)w1f8 - (double)*(int *)(o + 0xc0) * 0.01 * *(int *)(o + 0xb4));
                k++;
            } while (k < j);
        }
        if (w200 != 0 && iv != 0)
            io_PrintText(0x204b, iv);
        io_PrintText(0x204c, w1f8);
        w1fc -= w200;
        if (w1fc > 0)
            io_PrintText(0x204d, w1fc);
        else if (w1fc < 0)
            io_PrintText(0x204e, w1fc);
        t = gm_GetGoodsValue((unsigned)&ai_CharsT + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
        io_PrintText(0x2029, ((t - w1e8) - w1ec) - iv);
        he_ShowEventBar(0xffff);
        d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
        d2_ShowPicture(0x79d);
        U_I((unsigned)&d2_DirtyRectLeft) = (int)*(short *)(unsigned)&d2_ScreenW;
        U_I((unsigned)&d2_DirtyRectBottom) = (int)*(short *)(unsigned)&d2_ScreenH;
        U_I((unsigned)&main_NoRoundFade) = 1;
        U_I((unsigned)&d2_DirtyRectRight) = 0;
        U_I((unsigned)&d2_DirtyRectTop) = 0;
        d2_SetForm(win, 1);
        t = U_I((unsigned)&d2_LastWindow);
        d2_SetForm(win, 2);
        io_PrintText(0x7d);
        d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), t, 0x767);
        U_I(((unsigned)&ai_Unkf8f13c + 0x248) + t * 0x3c4) = 0x18;
        sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        t = sv_RunFrame((int)main_RoundReport, 0x24087);
        while (t != 0) {
            sv_ScrollFormAnimate();
            if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c)
                sv_PopRequest = 1;
            t = sv_RunFrame((int)main_RoundReport, 0x24087);
        }
        sv_DestroyScrollForm();
    }
    *(int *)(bk + 0x2c) = *(int *)(bk + 0x2c) + *(int *)(bk + 0x18) + *(int *)(bk + 0x14);
    *(int *)(bk + 0x14) = 0;
    *(int *)(bk + 0x30) = *(int *)(bk + 0x30) + *(int *)(bk + 0x1c);
    *(int *)(bk + 0x18) = 0;
    *(int *)(bk + 0x34) = *(int *)(bk + 0x34) + *(int *)(bk + 0x20);
    *(int *)(bk + 0x1c) = 0;
    *(int *)(bk + 0x38) = *(int *)(bk + 0x38) + *(int *)(bk + 0x24);
    *(int *)(bk + 0x20) = 0;
    *(int *)(bk + 0x24) = 0;
    *(int *)(bk + 0x28) = 0;
    *(int *)(bk + 0x40) = 0;
    *(int *)(bk + 0x44) = 0;
    *(int *)(bk + 0x48) = 0;
    *(int *)(bk + 0x4c) = 0;
    *(int *)(bk + 0x50) = 0;
    *(int *)(bk + 0x54) = 0;
    if (fade != 0)
        d2_ReleaseFade(fade);
    if (gm_IsBankrupt((unsigned)&ai_CharsT + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c, -(w1e8 + w1ec + iv)) != 0) {
        memset(msg, 0, 0x114);
        MSG_I(msg, 0xc) = U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
        MSG_B(msg, 0x30) = 1;
        MSG_B(msg, 0x26) = 0x60;
        MSG_I(msg, 0x14) = -1;
        MSG_B(msg, 8) = 9;
        MSG_I(msg, 0x34) = MSG_I(msg, 0xc);
        cm_ExecCutscene(msg);
        if (main_ReplaceFigureOn != 0)
            main_ReplaceFigure(U_I((unsigned)&ai_PlayerId) & 0xffff);
        main_ReplaceFigureOn = 0;
    }
}

#define PL(i) ((unsigned)&ai_CharsT + (i) * 0x21c)
float gm_TitlePeriodServed(int n, int *out);

/* 0x53a250, per-player update of figure `i` by the kind byte at +2: kinds 6 / 7 get the field delta 0x174 = 1 sent, kinds 0 .. 9 otherwise: the same delta for kinds 1 / 2 / 4 / 5 or a character with a building (ai_Unk770cec), and the master-exam start (message kind 0x24 when gm_TitlePeriodServed reaches 1.0 and the title group is below 6) for a character with a job */
void main_UpdateFigure(int i)
{
    int base = i * 0x21c;
    unsigned char kind = *(unsigned char *)(((unsigned)&ai_CharsT + 0x2) + base);
    int one;
    int pr, iv;
    float f;
    int ev[0x42];
    char txt[256];

    if (kind != 6) {
        if (kind == 7 || kind > 9) {
            if (kind == 6 || kind == 7)
                goto L404;
        } else {
            one = 1;
            if (U_I((unsigned)&ai_Unk770cec + base) != 0 || kind == 5 || kind == 1 || kind == 2 || kind == 4) {
                cm_BeginFieldBlock(PL(i), U_I((unsigned)&ai_CharInts + base));
                cm_AddFieldValues(4, 1, 0x174, &one);
                cm_SendFieldDeltas();
            }
            if (*(char *)(((unsigned)&ai_CharInts + 0x160) + base) != 0) {
                pr = gm_TitlesLeft(*(char *)(((unsigned)&ai_CharInts + 0x160) + base));
                iv = he_FindEvent(2, 2, U_I((unsigned)&ai_CharsT + base), 0, 0x24);
                U_I(((unsigned)&ai_Unk770cec + 0x8) + base) += 1;
                f = gm_TitlePeriodServed(i, ev);
                U_I(((unsigned)&ai_Unk770cec + 0x8) + base) -= 1;
                if (iv == 0 && pr != 0 && pr < 6 && 1.0 <= f && U_I(((unsigned)&ai_Selected + 0xc) + base) > -1) {
                    memset(ev, 0, 0xf8);
                    ev[2] = U_I((unsigned)&ai_CharInts + base);
                    ev[0x1f] = 0;
                    ev[0x1d] = 0;
                    ((char *)ev)[0x00 + 4] = 0x24;
                    ev[3] = -1;
                    ((char *)ev)[0x36] = 0x11;
                    ev[0x1e] = 1;
                    ev[0x1c] = ev[2];
                    cm_SendLongMessage(ev);
                    sprintf(txt, main_MasterExamLog, U_I((unsigned)&ai_CharInts + base), *(char *)(((unsigned)&ai_CharInts + 0x160) + base), pr + 1);
                    cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + base), -4);
                }
            }
        }
        return;
    }
L404:
    one = 1;
    cm_BeginFieldBlock(PL(i), U_I((unsigned)&ai_CharInts + base));
    cm_AddFieldValues(4, 1, 0x174, &one);
    cm_SendFieldDeltas();
}
/* 0x53a450, per-frame update of every character (768, ai_Chars): a figure with a job or a title goes through main_UpdateFigure; a bankrupt kind 2 / 4 / 5 loses health (the float at +0x1c plus 5); a set +0x1b1 byte is sent as a field delta (0xff) */

void main_UpdateFigures(void)
{
    AICHAR *ch;
    unsigned char on;
    int i, k;

    for (i = 0; i < 768; i++) {
        ch = &ai_CharsT[i];
        if (ch->id != 0xffff) {
            if (ch->jobId != 0 || ch->title != 0)
                main_UpdateFigure(i);
            if (ch->kind == 2 || ch->kind == 5 || ch->kind == 4) {
                if (gm_IsBankrupt((unsigned short *)ch, 0) != 0) {
                    k = (int)-(ch->f01c + 5.0f);
                    main_AddHealthS(ch->id, k);
                }
            }
            if (ch->heldRounds > 0) {
                on = 0xff;
                cm_BeginFieldBlock((char *)ch, ch->netId);
                cm_AddFieldValues(1, 1, (int)((char *)&ch->heldRounds - cm_FieldObject), &on);
                cm_SendFieldDeltas();
            }
        }
    }
}
/* 0x53b830, the CD check: returns 1 (the retry loops of main_BeginPlayerRound and main_GameLoop wait for 1; this build always passes) */

int main_CheckCD(void)
{
    return 1;
}
/* 0x53b840, creates the player character from the profile (cm_SendMessage0c) and its parents, the start couple (cm_SendCreateCharacter, mother with the female flag): random first names (txt_FemaleFirstNames / txt_MaleFirstNames), the father the profile's family name, the player's number and coat, the family links (spouse, parents, child), the model, love 0x7f between all three, 16000 .. 32352 money for each parent, the profile's mission and talents
   @deep Rewrote 0x53b840 typed against the original (154 -> 10 differing, 245 / 245 instructions, size 725
   / 725): the profile's int coat argument, the two cm_SendCreateCharacter (mother with the female flag),
   the start couple's fields through MAINCHARNEW (seq, coat, spouse / parent1 / parent2 / child, talents,
   modelId, ownName), the random index taken before the destination and the table pointer, the name copy as
   do { c = *t++; *d++ = c; } while (c) (the first copy now equal), the inline strcpy of the model,
   ai_MissionMode > -1 as a char. Left is one register choice in the second name copy: 0x53b9b9 .. 0x53b9c7
   ours mov eax, [tbl + idx*4] / mov cl, [eax] / inc eax / test cl, cl, the original mov ecx, [tbl + idx*4]
   / mov al, [ecx] / inc ecx / test al, al (the original pops ebp, the first copy's destination, right
   before this loop; ours keeps eax for the pointer). Tried: a second char / pointer variable for the
   second copy (no change), the while ((*d++ = *t++)) form (22 differing). */

void main_CreateStartCouple(void)
{
    MAINCHARNEW *a, *mother, *father;
    int h1, h2;
    char *d, *t;
    int k;
    char c;

    ai_ClearMissions();
    if (init_PlayerProfile.status != 0) {
        cm_BeginSendGroup();
        h1 = cm_SendMessage0c(-1, -1, 0x10, init_PlayerProfile.job, init_PlayerProfile.coat, init_PlayerProfile.sex, init_PlayerProfile.faith, init_PlayerProfile.firstName, init_PlayerProfile.familyName);
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0)
            sv_PumpNet();
        a = (MAINCHARNEW *)cm_GetSlotValue(h1);
        cm_BeginSendGroup();
        h1 = cm_SendCreateCharacter(9, -1, -1, util_Random(4) + 0x20, -1, init_PlayerProfile.tool2, 0, 1);
        h2 = cm_SendCreateCharacter(9, -1, -1, util_Random(4) + 0x20, -1, init_PlayerProfile.tool1, 0, 0);
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0)
            sv_PumpNet();
        mother = (MAINCHARNEW *)cm_GetSlotValue(h1);
        father = (MAINCHARNEW *)cm_GetSlotValue(h2);
        strncpy(father->familyName, init_PlayerProfile.familyName, 0x10);
        mother->seq = a->seq;
        mother->coat = a->coat;
        mother->rel.child = a->netId;
        k = (unsigned short)util_Random(0x70);
        d = mother->firstName;
        t = txt_FemaleFirstNames[k];
        do {
            c = *t++;
            *d++ = c;
        } while (c != 0);
        gm_CharNameRecord(mother->id);
        father->seq = a->seq;
        father->coat = a->coat;
        father->rel.child = a->netId;
        k = (unsigned short)util_Random(0xbf);
        d = father->firstName;
        t = txt_MaleFirstNames[k];
        do {
            c = *t++;
            *d++ = c;
        } while (c != 0);
        gm_CharNameRecord(father->id);
        mother->rel.spouse = father->netId;
        father->rel.spouse = mother->netId;
        a->rel.parent1 = father->netId;
        a->rel.parent2 = mother->netId;
        mother->rel.child = a->netId;
        father->rel.child = a->netId;
        strcpy(a->ownName, init_PlayerProfile.model);
        a->modelId = init_PlayerProfile.modelId;
        cm_SendRelations(a->netId, father->netId, 0x7f, 0);
        cm_SendRelations(father->netId, a->netId, 0x7f, 0);
        cm_SendRelations(a->netId, mother->netId, 0x7f, 0);
        cm_SendRelations(mother->netId, a->netId, 0x7f, 0);
        cm_SendRelations(father->netId, mother->netId, 0x7f, 0);
        cm_SendRelations(mother->netId, father->netId, 0x7f, 0);
        cm_SendMoveGoods(father->netId, -1, ai_OwnPlayer, ((unsigned short)util_Random(0x200) + 500) * 0x20);
        cm_SendMoveGoods(mother->netId, -1, ai_OwnPlayer, ((unsigned short)util_Random(0x200) + 500) * 0x20);
        if (ai_MissionMode > -1)
            ai_NewMission(a->netId, init_PlayerProfile.mission, 0);
        for (k = 0; k < 5; k++)
            a->talents[k] = init_PlayerProfile.talents[k];
    }
}

void ord_InitMessageRecord(int a, float b, float c, float d);
int sv_MessageBox();

/* 0x53b2e0, starts the player's round: dumps every character and clears its round flags, takes the kind 6 one as the player, a random CD check loop (main_Unk53b830, text 0x79), the parties, the round intro page, the alchemists' plants and the kind 7 buildings, then the daily steps of the world (each followed by sv_PumpNet / Sleep(100), main_SettleRound for every living character) or only the income histories when this machine is not the master; the goal fanfare and the outro, the yearly history */
void main_BeginPlayerRound(void)
{
    int sv = snd_AmbientOn;
    int cur = -1;
    int i, w;
    AIBLD *b;
    char date[12];
    int rec[9];
    char buf[256];

    ord_CountGridObjects();
    for (i = 0; i < 0x300; i++) {
        ai_DebugDumpPerson(ai_Chars[i]);
        ai_CharsT[i].flags1c8 &= 0xe0874703;
        ai_CharsT[i].flags1cc &= 0x3d31ffc7;
        if (ai_CharsT[i].kind == 6)
            cur = i;
    }
    ai_PlayerId = (unsigned short)cur;
    he_RunEveryFrameEvents();
    if ((ai_GameFlags & 4) == 0 && (unsigned short)util_Random(10) > 6 && main_CheckCD() != 1) {
        do {
            d2_CutsceneActive = 0;
            sv_FrameMask = 0x24087;
            txt_Format(buf, 0x79);
            if (sv_MessageBox((int)buf, 1) == 0) {
                ai_GameOver = 1;
                sv_PopTo((int)main_GameLoop);
                return;
            }
            d2_DrawAll();
            Sleep(1000);
        } while (main_CheckCD() != 1);
    }
    sv_FrameMask = 0x24087;
    if ((ai_GameFlags & 4) != 0)
        sv_NetworkSyncWait(0x24087);
    cm_ResetCutInfos(0);
    if (ai_RunPendingParties(cur) != 0 && main_GfxOn != 0) {
        d2_DirtyRectRight = 0;
        d2_DirtyRectLeft[0] = d2_ScreenW;
        d2_DirtyRectTop = 0;
        d2_DirtyRectBottom = d2_ScreenH;
        d2_ClearSurface(d2_ScreenSurf);
        d2_ShowPicture(0x79d);
    }
    he_AgeCarryRecs();
    he_CountDownApEvents();
    if (main_GfxOn != 0) {
        w = d2_OpenForm(0, 0, "Runden\\Spielerrunde_Beginn_NEU");
        d2_PlaceMainWindowF(w, 3);
        d2_SetForm(w, 1);
        io_PrintText(0x7d);
        gm_ClockToDate((struct AITIME *)&ai_Time, date);
        d2_SetForm(w, 0);
        d2_CloseView(w);
    }
    b = (AIBLD *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (b->type == 0x1e) {
            int scene;

            i = init_BuildingSceneUniverse((unsigned char *)b);
            scene = d3_CurrentScene;
            if (i != -1) {
                d3if_SelectScene(init_BuildingUniverse[ai_BTypesT[b->type].kind], 1);
                d3_InflateScene(3);
                veg_GrowPlants((G15PLANT *)b->plants);
                d3_DeflateScene(3);
                d3if_SelectScene(scene, 1);
            }
        } else if (ai_BTypesT[b->type].kind == 7)
            b->unk69 = 0;
        b = (AIBLD *)gm_BuildNext();
    }
    gm_UpdatePlayers();
    ord_AppendStatisticsRound();
    if ((cm_NetFlags & 8) == 0 && net_Conn.s != -1)
        gm_EndDayIncomes(2);
    else {
        gv_SendCrimes();
        sv_PumpNet();
        Sleep(100);
        for (i = 0; i < 0x300; i++) {
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].health != 0)
                main_SettleRound((unsigned short)i);
        }
        sv_PumpNet();
        Sleep(100);
        gm_DailyCharacters();
        sv_PumpNet();
        Sleep(100);
        gm_UpdateWorthMood();
        sv_PumpNet();
        Sleep(100);
        gm_EndDayIncomes(3);
        gm_CharacterUpkeep();
        sv_PumpNet();
        Sleep(100);
        gm_PayAllCharacterWages();
        amt_CheckOffices();
        sv_PumpNet();
        Sleep(100);
        he_DailyEventChecks();
        ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
        sv_PumpNet();
        Sleep(100);
        gv_CheckRankUps();
        sv_PumpNet();
        Sleep(100);
        ai_RollCharFlags();
        sv_PumpNet();
        Sleep(100);
        ord_UpdateStatistics();
        ord_GetMessageRecord(rec);
        if (rec[0] == -1)
            ord_InitMessageRecord(1, 1.0f, 0.5f, (float)(10.0 - (4 - ai_Difficulty) * 2.5));
        else
            ord_UpdateMessageEvent();
        he_SpawnFounders();
        sv_PumpNet();
        Sleep(100);
        for (i = 0; i < 0x300; i++)
            cm_SendRelations(-1, ai_CharsT[i].netId, 0, 3);
        cm_SendRelations(-1, -1, 0, 4);
        hmt_Unk4ad7a0();
    }
    for (i = 0; i < 0x300; i++)
        ai_CharsT[i].unk1ec = 0;
    if (main_GfxOn != 0) {
        if (main_GameEnded != 0 && (ai_GameFlags & 4) == 0) {
            w = sv_CreateScrollForm();
            d2_SetForm(w, 2);
            io_PrintText(0x7d);
            d2_SetForm(w, 1);
            io_PrintText(0x1ba0, MAIN_PLAYER, MAIN_PLAYER);
            snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftLang", -1, 0);
            i = sv_RunFrame((int)main_BeginPlayerRound, 0x66087);
            while (i != 0) {
                if (d2_ClickedId == 0)
                    sv_PopRequest = 1;
                i = sv_RunFrame((int)main_BeginPlayerRound, 0x66087);
            }
            d2_CloseView(w);
            main_PlayOutro();
            ai_GameOver = 1;
        }
        if (main_GfxOn != 0 && ai_GameOver == 0) {
            if ((cm_NetFlags & 8) == 0 && net_Conn.s != -1)
                hst_YearlyHistory(0);
            else
                hst_YearlyHistory(1);
        }
    }
    if ((cm_NetFlags & 8) != 0 || net_Conn.s == -1)
        main_UpdateFigures();
    snd_AmbientOn = (unsigned char)sv;
}

#define MAIN_ME ((AICHAR *)ai_PlayerChar)  /* the player's character (ai_PlayerChar: unsigned short *) */

/* 0x53a520, settles the round for the player pl: every building of pl loses its round sums and the 0x3800 flags; the kinds 4 / 0x10 / 0x13 (stores) age their objects (+0x37 by the
   rank of the target building, message 0x2b / 0xe), the kind 5 clears its three records and pays the owner level * 16000 below a worth of 160000, the kinds 0xb .. 0xd (the player's) check the
   master (cm_SendMessage3d / flags1b4 bit 0), the kind 0x1d runs the graveyard; a building whose master is the player gets the 0x1b equipment message; buildings of a player that is gone or
   kind 0 / 0xd go to the player (cm_SendOwnership, 0x1b, a 0x31 for every character living there); most kinds wear down (gm_ChangeBuildingState -2 .. -4 with 69 percent). Then every building of pl
   pays its workers (gm_ListWorkers's lists: wage * wageRate / 100, AICHAR.wage), places the ones without a figure on the stage, moves their opinion of pl by wageRate / workRate, lets the
   unhappy ones quit (message 0x33 / 0x34 in 4 .. 9 hours) and lose 10 percent of the skills +0x82 / +0x83 above 42, pays the others of the list, and moves the goods of gm_BuildingBalance's summary.
   sv_PumpNet and Sleep(20) every 0x20 messages */
void main_SettleRound(unsigned short pl)
{
    AIMSG msg2;     /* 0x1b to a new owner; gm_ListWorkers's worker lists; 0x31 */
    AIMSG msg1;     /* 0x1b to the master; 0x34 */
    AIMSG msg4;     /* 0x33 */
    int sum[0x11];  /* gm_BuildingBalance's summary */
    int ev[4];
    int cnt;
    AIBLD *c;
    AIBLD *p;
    AIOBJ *o;
    AICHAR *ps;

    cnt = 0;
    c = ai_BldsT;
    {
        int left = 0x100;

        do {
            if (c->type != 0 && c->owner == pl) {
                AIBTYPE *bt = &ai_BTypesT[c->type];
                char kind;

                c->roundSums[1] = 0;
                c->roundSums[2] = 0;
                c->roundSums[0] = 0;
                if ((short)(c->flags5aw & 0x3800) != 0) {
                    cm_SendSetField(c->id, 0x5a, 2, 0, 0x3800);
                    cnt++;
                }
                kind = bt->kind;
                if (kind == 4 || kind == 0x10 || kind == 0x13) {
                    o = (AIOBJ *)gm_Query((void *)c->objList, 1, 0, 0xca);
                    while (o != 0) {
                        cnt++;
                        if (o->reportKnow <= 5)
                            cm_SendMoveObject(c->id, -1, o->objId);
                        else {
                            unsigned char *t = (unsigned char *)gm_FindBuildingById(o->reportBld);

                            if (t != 0) {
                                char age = o->reportKnow - (char)gm_BuildingRank(t) * 2;

                                age = age < 0 ? 0 : age;
                                cm_BeginFieldBlock((char *)o, o->objId);
                                cm_AddFieldDelta(1, 1, (char *)&o->reportKnow - cm_FieldObject, &age);
                                cm_SendFieldDeltas();
                            } else
                                cm_SendDeleteObject(o->objId);
                        }
                        o = (AIOBJ *)gm_QueryNext();
                    }
                } else if (kind == 5) {
                    int zero = 0;
                    int i;

                    cm_BeginFieldBlock((char *)c, c->id);
                    for (i = 0; i < 3; i++)
                        cm_AddFieldValue(4, 1, (char *)&c->plants + i * 12 - cm_FieldObject, &zero);
                    cm_SendFieldValues();
                    cnt++;
                    if (ai_CharsT[pl].kind != 6 && ai_CharsT[pl].kind != 7 && gm_GetGoodsValue(ai_Chars[pl]) < 160000) {
                        cnt++;
                        cm_SendMoveGoods(ai_CharsT[pl].netId, -1, ai_OwnPlayer, (unsigned char)bt->level * 16000);
                    }
                } else if (kind == 0xd || kind == 0xc || kind == 0xb) {
                    if (c->owner == MAIN_ME->id) {
                        AICHAR *m = (AICHAR *)gm_FindCharacterByMaster((int)c);

                        if (m == 0) {
                            cnt++;
                            cm_SendMessage3d((char *)c, 1, 0, ai_BTypesT[c->type].jobBytes);
                        } else if ((m->flags1b4 & 1) == 0) {
                            ev[0] = m->flags1b4 | 1;
                            ev[1] = m->budget;
                            ev[2] = m->unk1bc;
                            ev[3] = m->target;
                            cm_BeginFieldBlock((char *)m, m->netId);
                            cm_AddFieldDelta(4, 1, (char *)&m->flags1b4 - cm_FieldObject, ev);
                            cm_SendFieldDeltas();
                        }
                    }
                } else if (kind == 0x1d)
                    gv_GraveyardTurn((char *)c);
                if (c->masterId == MAIN_ME->id && (c->flags5a & 3) == 0) {
                    kind = bt->kind;
                    if (kind != 1 && kind != 3 && kind != 6 && kind != 10 && kind != 11 && kind != 12 && kind != 13 && kind != 15 && kind != 0x1c && kind != 0x1b && kind != 0x1a
                        && kind != 0x19 && kind != 0x18 && kind != 0x17) {
                    msg1.type = 0x1b;
                    msg1.to = MAIN_ME->netId;
                    msg1.fromId = -1;
                    msg1.flags = 2;
                    msg1.arg[0] = c->id;
                    msg1.arg[1] = gm_EquipmentValue((unsigned char *)c, c->level);
                    cm_SendLongMessage((char *)&msg1);
                    }
                }
                if (ai_CharsT[pl].kind == 0xd || ai_CharsT[pl].kind == 0 || ai_CharsT[pl].netId == -1) {
                    cm_SendOwnership(c->id, MAIN_ME->netId, MAIN_ME->netId);
                    cnt++;
                    kind = bt->kind;
                    if (kind != 0xd && kind != 0xc && kind != 0xb) {
                        int i;

                        msg2.type = 0x1b;
                        msg2.fromId = -1;
                        msg2.to = MAIN_ME->netId;
                        msg2.flags = 2;
                        msg2.arg[0] = c->id;
                        msg2.arg[1] = gm_EquipmentValue((unsigned char *)c, c->level);
                        cm_SendLongMessage((char *)&msg2);
                        cnt++;
                        for (i = 0; i < 0x300; i++) {
                            if (ai_CharsT[i].bld == c) {
                                memset(&msg2, 0, sizeof(msg2));
                                msg2.type = 0x31;
                                msg2.fromId = c->id;
                                msg2.to = MAIN_ME->netId;
                                msg2.time = ai_Time;
                                msg2.flags = 1;
                                msg2.arg[0] = ai_CharsT[i].netId;
                                cm_SendLongMessage((char *)&msg2);
                                cnt++;
                            }
                        }
                    }
                }
                if ((unsigned short)util_Random(100) > 0x1e) {
                    kind = bt->kind;
                    if (kind != 0xd && kind != 0xc && kind != 0xb && kind != 0x10 && (c->flags5a & 1) == 0 && kind != 10) {
                        gm_ChangeBuildingState((unsigned char *)c, -(int)((util_Random(3) & 0xffff) + 2));
                        cnt++;
                    }
                }
            }
            c++;
            left--;
        } while (left != 0);
    }
    {
        int t = gm_TitleNumberGroup(ai_CharsT[pl].jobId);

        if (t >= 2 && t <= 5)
            gm_HasBuilding(t);
    }
    p = (AIBLD *)gm_BuildQuery(1, 6);
    while (p != 0) {
        if (p->owner == pl) {
            ps = &ai_CharsT[p->owner];
            if (ai_BTypesT[p->type].kind == 0x1e) {
                AICHAR *r = (AICHAR *)ahm_OwnerChar((unsigned char *)p);

                if (r != 0) {
                    cm_SendMoveGoodsUnchecked(r->netId, ps->netId, ai_OwnPlayer, ahm_ObjectValue((unsigned char *)p));
                    cnt++;
                }
            } else if (gm_ListWorkers((unsigned char *)p, (int *)&msg2) != 0) {
                int *pn;
                int pass;
                int k;

                pass = 0;
                pn = (int *)&msg2 + 1;
                do {
                    int sub = 0xc;

                    do {
                        int wage = (int)(gm_JobWage((char *)ps, p->type, -1, pass) * (p->wageRate * 0.01));

                        if (*pn != 0xffff) {
                            AICHAR *q = &ai_CharsT[*pn];
                            char rel = 0;
                            int m;

                            if (q->model == 0) {
                                if (q->bld2 != 0) {
                                    o = (AIOBJ *)gm_HomeObject((unsigned char *)q->bld2);
                                    if (o != 0 && o->objId != -1) {
                                        cnt++;
                                        gm_PlaceOnStage(q->netId, q->bld2->id, o->objId, 0, 0);
                                    }
                                } else {
                                    o = (AIOBJ *)gm_BuildingMainObject((unsigned char *)p);
                                    if (o != 0 && o->objId != -1) {
                                        cnt++;
                                        gm_PlaceOnStage(q->netId, p->id, o->objId, 0, 0);
                                    }
                                }
                            } else
                                ((CHARACTER *)q->model)->flags.bytes.lo &= 0xfb;
                            if (q->kind != 0x14)
                                cm_SendMoveGoodsUnchecked(ai_CharsT[*pn].netId, ai_CharsT[pl].netId, ai_OwnPlayer, wage);
                            cnt++;
                            if (p->wageRate < 100 && gm_Opinion(pl, q->id) > -0x78) {
                                m = (100 - p->wageRate) / 3;
                                if (m < 1)
                                    m = 1;
                                rel = -(char)m;
                            } else if (p->wageRate > 100 && gm_Opinion(pl, q->id) < 100) {
                                rel = 1;
                                m = (p->wageRate - 100) / 3;
                                if (m >= 1)
                                    rel = (char)m;
                            }
                            if (p->workRate < 100 && gm_Opinion(pl, q->id) > -0x78) {
                                m = (p->workRate - 100) / 5;
                                if (m < 1)
                                    m = 1;
                                rel += (char)m;
                            } else if (p->workRate > 100 && gm_Opinion(pl, q->id) < 100) {
                                m = (100 - p->workRate) / 5;
                                if (m < 1)
                                    m = 1;
                                rel -= (char)m;
                            }
                            if (rel != 0) {
                                cnt++;
                                cm_SendRelations(ai_CharsT[pl].netId, q->netId, rel, 0);
                            }
                            cm_BeginFieldBlock((char *)q, q->netId);
                            cm_AddFieldDelta(4, 1, 0x19c, &wage);
                            cm_SendFieldDeltas();
                            cnt++;
                            m = gm_Opinion(pl, q->id) + rel;
                            if (m < -0x1a) {
                                if (-(int)(util_Random(0x4a) & 0xffff) < m) {
                                    if ((unsigned short)util_Random(100) > 0x32) {
                                        memset(&msg4, 0, sizeof(msg4));
                                        msg4.type = 0x33;
                                        msg4.to = ai_CharsT[pl].netId;
                                        msg4.fromId = p->id;
                                        msg4.time = ai_Time;
                                        msg4.flags = 1;
                                        msg4.arg[0] = q->netId;
                                        gm_AddTime((struct AITIME *)&msg4.time, (util_Random(6) & 0xffff) + 4, 0, 0);
                                        cm_SendLongMessage((char *)&msg4);
                                    } else {
                                        memset(&msg1, 0, sizeof(msg1));
                                        msg1.type = 0x34;
                                        msg1.to = ai_CharsT[pl].netId;
                                        msg1.fromId = p->id;
                                        msg1.time = ai_Time;
                                        msg1.flags = 1;
                                        msg1.arg[0] = q->netId;
                                        gm_AddTime((struct AITIME *)&msg1.time, (util_Random(6) & 0xffff) + 4, 0, 0);
                                        cm_SendLongMessage((char *)&msg1);
                                    }
                                    cnt++;
                                }
                                if (q->title == 2 || q->title == 1 || q->title == 0x13 || q->title == 0x14 || q->title == 0x10 || q->title == 0xf) {
                                    int skill;
                                    char lower;

                                    cm_BeginFieldBlock((char *)q, q->netId);
                                    skill = q->sk82;
                                    lower = (char)(skill * 0.9 + 0.5);
                                    if (skill > 42.0f)
                                        cm_AddFieldDelta(1, 1, (char *)&q->sk82 - cm_FieldObject, &lower);
                                    skill = q->sk83;
                                    lower = (char)(skill * 0.9 + 0.5);
                                    if (skill > 42.0f)
                                        cm_AddFieldDelta(1, 1, (char *)&q->sk83 - cm_FieldObject, &lower);
                                    cm_SendFieldDeltas();
                                    cnt++;
                                }
                            }
                        }
                        pn++;
                        sub--;
                    } while (sub != 0);
                    pass++;
                } while (pass < 2);
                for (k = 0; k < 0xc; k++) {
                    int wage2 = (int)(gm_JobWage(ai_Chars[p->owner], p->type, -1, 0) * (p->wageRate * 0.01));
                    int w = ((int *)&msg2)[0x19 + k];

                    if (w != 0xffff) {
                        cm_SendMoveGoodsUnchecked(ai_CharsT[w].netId, ai_CharsT[pl].netId, ai_OwnPlayer, wage2);
                        cm_BeginFieldBlock(ai_Chars[w], ai_CharsT[w].netId);
                        cm_AddFieldDelta(4, 1, 0x19c, &wage2);
                        cm_SendFieldDeltas();
                        cnt += 2;
                    }
                }
            }
        }
        p = (AIBLD *)gm_BuildNext();
    }
    if (cnt > 0x20) {
        sv_PumpNet();
        Sleep(0x14);
        cnt = 0;
    }
    p = (AIBLD *)gm_BuildQuery(1, 4, pl);
    while (p != 0) {
        if (gm_BuildingBalance(p, pl, sum) != 0) {
            int v;

            if (sum[0xc] != 0 && (o = (AIOBJ *)gm_Query((void *)p->objList, 2, 6, 0, 0xe8)) != 0 && (v = gm_SumListAmounts((void *)o->list, ai_OwnPlayer)) > 0) {
                cnt++;
                cm_SendMoveGoods(ai_CharsT[p->owner].netId, o->objId, ai_OwnPlayer, v);
            }
            if (sum[0xf] != 0 && (o = (AIOBJ *)gm_Query((void *)p->objList, 1, 0, 0xf0)) != 0 && (v = gm_SumListAmounts((void *)o->list, ai_OwnPlayer)) > 0) {
                cnt++;
                cm_SendMoveGoods(ai_CharsT[p->owner].netId, o->objId, ai_OwnPlayer, v);
            }
        }
        p = (AIBLD *)gm_BuildNext();
        if (cnt > 0x20) {
            sv_PumpNet();
            Sleep(0x14);
            cnt = 0;
        }
    }
}

void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b);

/* 0x53bb20, the main game loop of the process (the dialog callback of the process level, `ctl` = 0 on the first call): the start-up / load of the game by the mode flags 0x6f23f8 (1 new game, 2 load, 4 network, 8, 0x10, 0x40 .., 0x80), the "Init" export of the optional
   plug-in DLL (0x75dda0), the autosave, then the round loop: one player at a time (0x6f25b8) the day change 0x5384e0, the start of the round 0x53b2e0 ... until the game is left (0x6f259c); ends with the network error / player left messages (ROUGH) */
void main_GameLoop(int ctl)
{
    int iv, i6, i5;
    int w;
    FARPROC pf;
    char *pc;
    int *pi;
    unsigned short us;
    unsigned u;
    int cl[0x3e];
    int sys[0x1f];
    AITIME ck;
    int clk[4];
    char ap[0x104], ab[0x104];
    int pl[0x45];
    int idx;
    int *pp;
    int fx, fy, fh;
    int dst;
    unsigned char *p3;
    unsigned short *p4;

    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0 && ctl == 0)
        amt_InitTable();
    U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0) {
        w = -1;
        hst_SetTextsOn(0);
        if ((U_I((unsigned)&ai_GameFlags) & 0x10) != 0) {
            iv = main_CheckCD();
            if (iv != 1) {
                while (1) {
                    U_I((unsigned)&sv_FrameMask) = 0x24087;
                    txt_Format(cl, 0x79);
                    iv = sv_MessageBox(cl, 1);
                    if (iv == 0)
                        break;
                    d2_DrawAll();
                    Sleep(1000);
                }
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            U_I(((unsigned)&ls_SavePath + 0x80)) = (int)LoadLibraryA((char *)((unsigned)&ai_DataPath + 0x100));
            if (U_I(((unsigned)&ls_SavePath + 0x80)) == 0) {
                U_I((unsigned)&sv_FrameMask) = 0x66087;
                sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x7c)), 0x100);
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            w = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
            d2_CentreViewWindows(w);
            io_PrintText(0x3ff6);
            sv_RunFrame(0, 0x24087);
            pf = GetProcAddress((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)), "Init");
            if (pf == 0) {
                FreeLibrary((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)));
                U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
                U_I((unsigned)&sv_FrameMask) = 0x66087;
                d2_CloseView(w);
                sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x7c)), 0x100);
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            iv = ((int (*)(int))pf)(((unsigned)&ls_SavePath + 0xa0));
            if (iv == 0) {
                FreeLibrary((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)));
                U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
                U_I((unsigned)&sv_FrameMask) = 0x66087;
                d2_CloseView(w);
                sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x7c)), 0x100);
                U_I((unsigned)&ai_GameOver) = 1;
                U_I((unsigned)&ai_LeaveGame) = 0;
                return;
            }
            Sleep(0x9c4);
            udp_Init(0x3039);
            main_UdpOpen = 1;
        }
        net_Connect(main_NetHost, U_I(((unsigned)&ls_SavePath + 0x84)));
        d2_CloseView(w);
    } else {
        net_Connect(0, 0);
        iv = main_CheckCD();
        if (iv != 1) {
            while (1) {
                U_I((unsigned)&d2_CutsceneActive) = 0;
                U_I((unsigned)&sv_FrameMask) = 0x24087;
                txt_Format(cl, 0x79);
                iv = sv_MessageBox(cl, 1);
                if (iv == 0)
                    break;
                U_I((unsigned)&d2_CutsceneActive) = 1;
                d2_DrawAll();
                Sleep(1000);
            }
            U_I((unsigned)&ai_GameOver) = 1;
            U_I((unsigned)&ai_LeaveGame) = 0;
            return;
        }
    }
    ai_InitStates();
    if ((U_I((unsigned)&ai_GameFlags) & 0x44) != 0) {
        U_I((unsigned)&d2_CutsceneActive) = 0;
        iv = lobby_Screen();
        if (iv != 0) {
            if (U_I(((unsigned)&ls_SavePath + 0x80)) != 0) {
                Sleep(1000);
                FreeLibrary((HMODULE)U_I(((unsigned)&ls_SavePath + 0x80)));
                U_I(((unsigned)&ls_SavePath + 0x80)) = 0;
            }
            U_I((unsigned)&ai_LeaveGame) = 0;
            U_I((unsigned)&ai_GameOver) = 1;
            return;
        }
    }
    U_I((unsigned)&d2_CutsceneActive) = 1;
    main_LoadingScreen();
    d2_SetForm(U_I((unsigned)&init_StatusForm), 1);
    if ((ai_GameFlags & 4) != 0)
        main_SetLoadProgress(U_I(((unsigned)&ai_DifficultyTexts + 0x84)), 5);
    else
        main_SetLoadProgress(U_I(((unsigned)&d2_Strings + 0x1b4)), 5);
    sv_PumpMessages();
    main_SetLoadProgress(U_I(((unsigned)&d2_Strings + 0x1b0)), 5);
    main_StartGame();
    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0 && main_StartCoupleOn != 0)
        main_CreateStartCouple();
    main_SetLoadProgress(0, 10);
    if ((ai_GameFlags & 0x41) != 0) {
        if ((ai_GameFlags & 4) == 0) {
            sprintf(ap, "%s/%s.cty", "gamedata/cities", (char *)((unsigned)&ai_DataPath + 0x220));
            if (ls_LoadGame(ap, 2) == 0) {
                main_Exit();
                return;
            }
            main_CreateStartCouple();
            init_SetupNewGame(ai_OwnPlayer);
            init_LocalData();
            init_FillMarkets(ai_OwnPlayer);
            main_SetLoadProgress(0, 100);
            main_SetLoadProgress(0, 100);
            if (ai_Unk6f2440 != 0)
                init_GiveFirstPlayerBuildings();
        } else if (net_Conn.s != -1) {
            if ((ai_GameFlags & 0x40) != 0) {
                init_JoinNetworkGame(((unsigned)&ls_SavePath + 0xca));
                ai_Difficulty = 2;
            } else {
                init_StartNetworkGame(main_NetSaveName);
                ai_Difficulty = 2;
            }
        } else {
            sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x80)), 0);
            main_EndGame();
            main_CloseLoadingScreen();
            return;
        }
    } else if ((ai_GameFlags & 0x40) != 0) {
        ai_Difficulty = 2;
        if (net_Conn.s != -1)
            init_JoinNetworkGame(main_NetSaveName);
        else {
            sv_MessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x80)), 0);
            main_EndGame();
            main_CloseLoadingScreen();
            return;
        }
    } else if ((ai_GameFlags & 2) != 0) {
        ai_Difficulty = ai_GamePanel.byte[2];
        if (ls_LoadGame(main_NetSaveName, 1) == 0) {
            sv_FrameMask = 0x46086;
            sv_MessageBox(U_I((unsigned)&full_data_14414d0), 0);
            ai_GameOver = 1;
            main_EndGame();
            main_CloseLoadingScreen();
            return;
        }
        sv_FillDayPhases((int)&ai_Time);
        sv_Season = sv_ClockSeason(&ai_Time);
    }
    init_LoadTypeBScene();
    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0) {
        if ((U_I((unsigned)&ai_GameFlags) & 4) == 0) {
            clk[0] = 0;
            gm_SetTime(clk, 6, 0, 0);
            w = cm_SendExSysMessage(3, clk);
            iv = cm_GetSlotState(w);
            while (iv == 0) {
                sv_PumpNet();
                iv = cm_GetSlotState(w);
            }
            if (main_MoneyCheat != 0)
                iv = 750000;
            else
                iv = (5 - ai_Difficulty) * 0xfa;
            for (idx = 0; idx < 0x300; idx++) {
                if (ai_CharsT[idx].health != 0 && (ai_CharsT[idx].kind == 6 || ai_CharsT[idx].kind == 7))
                    cm_SendMoveGoods(ai_CharsT[idx].netId, -1, ai_OwnPlayer, gm_PlayerFactor(iv, ai_OwnPlayer));
            }
        }
        if ((U_I((unsigned)&ai_GameFlags) & 1) != 0)
            hst_LoadPlayerTexts();
    }
    sv_PumpMessages();
    ai_LoadTownMap();
    sv_PumpMessages();
    main_CloseLoadingScreen();
    U_I((unsigned)&d2_CutsceneActive) = 0;
    w = sv_PanelImage();
    sv_BuildScreenLayout(w);
    d3if_SelectScene(0, 0);
    if ((U_I((unsigned)&ai_GameFlags) & 1) != 0 && (U_I((unsigned)&ai_GameFlags) & 0x40) == 0) {
        init_PrepareTownScene();
        init_RecreateQuadTree();
        ai_BookmarksPreset();
    }
    ai_ApplyOptions();
    d3_SetViewport((float)(U_I((unsigned)&sv_ViewRectRight) - U_I((unsigned)&sv_ViewRectLeft)), (float)(U_I((unsigned)&sv_ViewRectBottom) - U_I((unsigned)&sv_ViewRectTop)), (float)U_I((unsigned)&sv_ViewRectLeft), (float)U_I((unsigned)&sv_ViewRectTop),
                 *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH);
    if ((U_I((unsigned)&ai_GameFlags) & 2) != 0 && (sv_SetupWeather((unsigned)&ai_Time, 0), U_I((unsigned)&full_data_76f990) != -1)) {
        p3 = (unsigned char *)gm_FindBuildingById(U_I((unsigned)&full_data_76f990));
        p4 = (unsigned short *)gm_FindObjectInst(U_I((unsigned)&full_data_76f994));
        U_I((unsigned)&ai_FrameWait) = 2;
        if (p3 != 0 && p4 != 0) {
            init_LoadObjectScene(*p4);
            sv_SelectPanelObject(p3, *(int *)(p4 + 1), (unsigned)&ch_EmptyName);
        } else {
            init_LoadBuildingScene(*p3);
            sv_SelectPanelObject(p3, -1, (unsigned)&ch_EmptyName);
        }
    }
    for (us = ai_PlayerId; us < 0x300 && ai_GameOver == 0 && ai_CharsT[MAIN_PLAYER].kind != 6; ai_PlayerId = us)
        us++;
    if ((ai_GameFlags & 0x80) != 0) {
        ai_TutorialStart();
        memset(cl, 0, 0xf8);
        /* `cl` is an int array, so the byte offsets must not pick up the int scaling of MSG_B / MSG_I. */
        MSG_B((char *)cl, 4) = 0x87;
        MSG_I((char *)cl, 0xc) = -1;
        MSG_B((char *)cl, 0x36) = 9;
        MSG_I((char *)cl, 8) = U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
        cm_SendLongMessage(cl);
    }
    while (U_I((unsigned)&ai_GameOver) == 0 && (iv = sv_RunFrame((int)main_GameLoop, 0x20007)) != 0 && main_Round < 800) {
        iv = 0;
        U_I((unsigned)&ai_CurrentPlayer) = 0;
        if ((U_I((unsigned)&ai_GameFlags) & 0x42) == 0) {
            *(unsigned short *)(unsigned)&ai_PlayerId = 0;
            U_I((unsigned)&ai_CurrentPlayer) = 0;
            main_RundenBeginn();
        } else {
            for (idx = 0; idx < MAIN_PLAYER; idx++) {
                if (ai_CharsT[idx].kind == 6)
                    iv++;
            }
            ai_CurrentPlayer = iv;
        }
        for (us = ai_PlayerId; us < 0x300 && ai_GameOver == 0 && ai_CharsT[MAIN_PLAYER].kind != 6; ai_PlayerId = us)
            us++;
        if (*(char *)(((unsigned)&ai_CharsT + 0x2) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) == 6) {
            if ((U_I((unsigned)&ai_GameFlags) & 0x46) == 0) {
                clk[1] = U_I((unsigned)&ai_Time.hour);
                clk[0] = U_I((unsigned)&ai_Time);
                clk[2] = U_I(((unsigned)&ai_Time + 0x8));
                *(short *)(clk + 3) = *(short *)((unsigned)&ai_Time + 0xc);
                gm_SetTime(clk, 6, 0, 0);
                w = cm_SendExSysMessage(3, clk);
                iv = cm_GetSlotState(w);
                while (iv == 0) {
                    sv_PumpNet();
                    iv = cm_GetSlotState(w);
                }
            }
            if ((U_I((unsigned)&ai_GameFlags) & 0x42) == 0) {
                main_EndRound();
            } else {
                init_AfterTownLoad();
                sv_RedrawScreen();
                if ((U_I((unsigned)&ai_GameFlags) & 4) != 0) {
                    cm_SendExSysMessage(1, 0);
                    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
                        sv_NetworkSyncWait(0x67fff);
                }
            }
            u = U_I((unsigned)&ai_GameFlags) & 0xff;
            if ((u & 2) == 0) {
                cm_BeginSendGroup();
                if ((U_I((unsigned)&ai_GameFlags) & 8) != 0 || (net_Conn.s != -1 && (U_I((unsigned)&cm_NetFlags) & 8) != 0)) {
                    he_PostEvent3a();
                    he_PostEvent3b();
                    he_PostEventA6();
                }
                he_PostEvent73();
                cm_EndSendGroup();
                iv = cm_SendGroupState();
                while (iv == 0) {
                    sv_PumpNet();
                    iv = cm_SendGroupState();
                }
                u = U_I((unsigned)&ai_GameFlags) & 0xff;
            }
            if ((u & 0x42) == 0 && main_AutoSaveOn != 0 && U_I((unsigned)&ai_GameOver) == 0 && (char)u >= 0) {
                ls_SaveSlot = 0;
                if ((u & 4) == 0) {
                    sprintf(ap, "%s\\gamedata\\saves\\autosave.sav", main_GfxPath);
                    i5 = fopen(ap, "rb");
                    if (i5 != 0) {
                        sprintf(ab, "%s\\gamedata\\saves\\autosave.bak", main_GfxPath);
                        fseek(i5, 0, 2);
                        w = ftell(i5);
                        fseek(i5, 0, 0);
                        i6 = mem_Alloc(w, (unsigned)gm_TagTmp);
                        iv = fopen(ab, "wb");
                        if (i6 != 0) {
                            if (iv != 0) {
                                fread(i6, w, 1, i5);
                                fwrite(i6, w, 1, iv);
                                fclose(iv);
                            }
                            mem_Free(i6);
                        }
                        fclose(i5);
                    }
                    sprintf(ap, "%s/standard.scr", "gamedata/cities");
                    ai_LoadScreenImage(ap);
                    ls_SaveGame("Gamedata\\Saves\\Autosave.SAV", "Autosave", 1);
                } else if ((u & 0x10) != 0) {
                    memset(sys, 0, 0x7c);
                    strcpy((char *)sys + 4, "AUTOSAVE");
                    sys[0] = 0;
                    w = cm_SendExSysMessage(0xf, sys);
                    iv = cm_GetSlotState(w);
                    while (iv == 0) {
                        sv_PumpNet();
                        iv = cm_GetSlotState(w);
                    }
                }
            }
            he_ShowEventBar(U_I((unsigned)&ai_PlayerId) & 0xffff);
            U_I((unsigned)&cm_DayEndRequest) = 0;
            main_DayEnding = 0;
            ai_GameFrame();
            if (U_I((unsigned)&ai_GameOver) == 0) {
                tb_SetFunctionOff((unsigned)&sv_UpdateClock, 1);
                if ((U_I((unsigned)&ai_GameFlags) & 8) == 0) {
                    ck = ai_Time;
                    while (ck.hour < 0x17) {
                        gm_AddTime((struct AITIME *)&ck, 0, 0x1e, 0);
                        w = cm_SendTime(&ck);
                        while (cm_GetSlotState(w) == 0)
                            sv_PumpNet();
                    }
                }
                main_RoundReport();
                if (U_I((unsigned)&ai_GameOver) == 0 && U_I((unsigned)&ai_LeaveGame) == 0) {
                    U_I((unsigned)&ai_GameFlags) &= 0xffffffbd;
                    U_I((unsigned)&ai_CurrentPlayer)++;
                    goto L_next;
                }
            }
            U_I((unsigned)&ai_CurrentPlayer)++;
        }
L_next:
        if (U_I((unsigned)&ai_GameOver) == 0) {
            if (net_Conn.s == -1) {
                tb_SetFunctionOff((unsigned)&sv_UpdateClock, 1);
                ck = ai_Time;
                while (ck.hour < 0x17) {
                    gm_AddTime((struct AITIME *)&ck, 0, 0x1e, 0);
                    w = cm_SendTime(&ck);
                    while (cm_GetSlotState(w) == 0)
                        sv_PumpNet();
                }
                gm_AddTime((struct AITIME *)&ck, 0x18, 0, 0);
                gm_SetTime((struct AITIME *)&ck, 6, 0, 0);
                cm_SendTime(&ck);
                tb_SetFunctionOff((unsigned)&sv_UpdateClock, 0);
            }
            if ((U_I((unsigned)&cm_NetFlags) & 8) != 0 || net_Conn.s == -1) {
                iv = amt_CheckOfficeColumns();
                if (iv == 0) {
                    amt_UpdateOffices();
                } else {
                    memset(pl, 0, 0x114);
                    idx = 0;
                    pp = (int *)(unsigned)&ai_CharInts;
                    do {
                        if (*((char *)pp - 2) == 6) {
                            w = *pp;
                            pl[3] = w;
                            pl[0x0d + idx] = w;
                            idx++;
                        } else if (*((char *)pp - 2) == 7) {
                            pl[0x0d + idx] = *pp;
                            idx++;
                        }
                        pp += 0x87;
                    } while ((int)pp < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
                    *((char *)pl + 0x30) = (char)idx;
                    pl[7] = U_I((unsigned)&ai_Time.hour);
                    pl[6] = U_I((unsigned)&ai_Time);
                    U_I((unsigned)&sv_FrameMask) = 0x24087;
                    *((char *)pl + 8) = 1;
                    *((char *)pl + 0x26) = 0x61;
                    pl[5] = -1;
                    pl[8] = U_I(((unsigned)&ai_Time + 0x8));
                    *(short *)(pl + 9) = *(short *)((unsigned)&ai_Time + 0xc);
                    w = cm_SendNewParty(pl);
                    iv = cm_GetSlotState(w);
                    while (iv == 0) {
                        sv_PumpNet();
                        iv = cm_GetSlotState(w);
                    }
                }
            }
            if ((U_I((unsigned)&ai_GameFlags) & 4) != 0)
                sv_NetworkSyncWait(0x24087);
            main_BeginPlayerRound();
        }
        main_Round++;
    }
    if ((U_I((unsigned)&ai_GameFlags) & 4) != 0) {
        if (U_I((unsigned)&cm_LeftPlayerId) == -1) {
            pc = "$3AEs trat ein Netzwerkfehler auf! Das Spiel wurde beendet...$2A%ia[Okay]";
        } else {
            if (U_I((unsigned)&cm_LeftPlayerId) == U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c))
                goto L_end;
            pc = "$3AEin Spieler hat die Partie verlassen. Das Spiel wurde beendet...$2A%ia[Okay]";
        }
        sv_MessageBox(pc, 0);
    }
L_end:
    if ((ai_GameFlags & 0x80) != 0)
        ai_TutorialStop();
    main_EndGame();
}
/* 0x53c9b0, plays the logo movies jowood / 4head / intro / intro_addon (main_MoviePath + "*.mpg", in the window 10,10,0xdc,0xdc) */

void main_PlayIntroMovies(void)
{
    char buf[0x104];

    Sleep(400);
    mov_Init();
    sprintf(buf, "%sjowood.mpg", main_MoviePath);
    main_Movie = mov_Open(buf, 0, (HINSTANCE)main_Instance, 10, 10, 0xdc, 0xdc, 0x8a);
    Sleep(100);
    mov_Play(main_Movie);
    Sleep(400);
    sprintf(buf, "%s4head.mpg", main_MoviePath);
    main_Movie = mov_Open(buf, 0, (HINSTANCE)main_Instance, 10, 10, 0xdc, 0xdc, 0x8a);
    sv_PumpMessages();
    Sleep(100);
    mov_Play(main_Movie);
    Sleep(400);
    sprintf(buf, "%sintro.mpg", main_MoviePath);
    main_Movie = mov_Open(buf, 0, (HINSTANCE)main_Instance, 10, 10, 0xdc, 0xdc, 0x8a);
    Sleep(100);
    mov_Play(main_Movie);
    sprintf(buf, "%sintro_addon.mpg", main_MoviePath);
    main_Movie = mov_Open(buf, 0, (HINSTANCE)main_Instance, 10, 10, 0xdc, 0xdc, 0x8a);
    Sleep(100);
    mov_Play(main_Movie);
    sv_PumpMessages();
}
/* 0x53cb40, the outro: fades the music out and waits for the black fade, switches the ambient sounds off, plays outro.mpg (input released, own window), restores the focus, the input and the music volume (msxVolume / 127) and the ambient flag */
void main_PlayOutro(void)
{
    int sv;
    D2FADE *f;
    char buf[0x104];

    if (main_MoviesOpen == 0)
        mov_Init();
    snd_FadeMusic(0.0f, 1000);
    f = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
    while ((f->flags & 4) == 0 || msx_FadeTo >= 0.0f)
        sv_RunFrame(0, sv_FrameMask);
    sv = snd_AmbientOn;
    snd_AmbientOn = 0;
    sprintf(buf, "%soutro.mpg", main_MoviePath);
    di_Disabled = 1;
    di_PollKeyboard();
    di_Disabled = 0;
    di_AcquireMouse(0);
    di_AcquireKeyboard(0);
    sv_PumpMessages();
    Sleep(500);
    main_Movie = mov_Open(buf, 0, (HINSTANCE)main_Instance, 0, 0, 0, 0, 0x8a);
    Sleep(200);
    mov_Play(main_Movie);
    d2_ReleaseFade(f);
    sv_PumpMessages();
    SetFocus(main_Window);
    di_AcquireMouse(1);
    di_AcquireKeyboard(1);
    di_Disabled = 1;
    di_PollKeyboard();
    di_Disabled = 0;
    kb_Last = 0;
    snd_FadeMusic((float)ai_SoundOptions.msxVolume * 0.007874016f, 1000);
    snd_AmbientOn = (unsigned char)sv;
    if (main_MoviesOpen == 0)
        mov_Exit();
}
/* 0x53ccf0, the program entry (WinMain: instance, previous, command line, show): reads game.INI (Network / Gfx / Sound / General), parses the command line (STADT=" BERUF=" IP=" PORT=" MO_MONEY FULLSCREEN DIRECTWINDOW), starts the engine
   (0x5324f0 / 0x531ba0 / 0x531c50 / 0x532160), then the game loop 0x53bb20 until the game ends (ROUGH) */

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prev, char *cmd, int show)
{
    int mode;
    char c;
    int net = 0;
    int sv = 0;
    int r, i, k;
    char *p;
    char *s, *e;
    char *d;
    unsigned res;
    char c224[4];
    JOBTALENTS c21c;
    char num[0x80];
    OSVERSIONINFOA vi;
    char bm[0x100];

    U_I((unsigned)&main_Instance) = (int)hInst;
    vi.dwOSVersionInfoSize = 0x94;
    GetVersionExA(&vi);
    U_I((unsigned)&main_PlatformNT) = (vi.dwPlatformId != 1);
    GetModuleFileNameA((HMODULE)U_I((unsigned)&main_Instance), (char *)(unsigned)&ai_OptionsFile, 0x104);
    i = 0;
    do {
        c = ((char *)&ai_OptionsFile)[i];
        ((char *)&main_AppDir)[i] = c;
        i++;
    } while (c != 0);
    p = strrchr((char *)(unsigned)&main_AppDir, 0x5c);
    *p = 0;
    p = strrchr((char *)(unsigned)&ai_OptionsFile, 0x5c);
    strcpy(p + 1, "game.INI");
    he_LoadMessageFilter((char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Server", "Server\\Server.dll", (char *)((unsigned)&ai_DataPath + 0x100), 0x104, (char *)(unsigned)&ai_OptionsFile);
    i = 0;
    do {
        c = ((char *)&ai_DataPath + 0x100)[i];
        ((char *)&ls_SavePath + 0x160)[i] = c;
        i++;
    } while (c != 0);
    p = strrchr((char *)((unsigned)&ls_SavePath + 0x160), 0x5c);
    *p = 0;
    _strupr(cmd);
    U_I((unsigned)&main_TextureDivider) = GetPrivateProfileIntA("Gfx", "TextureDivider", 0, (char *)(unsigned)&ai_OptionsFile);
    U_I((unsigned)&snd_MusicEnabled) = GetPrivateProfileIntA("Sound", "msx", 0, (char *)(unsigned)&ai_OptionsFile);
    main_AmbientSoundOn = GetPrivateProfileIntA("Sound", "ambient", 0, (char *)(unsigned)&ai_OptionsFile);
    snd_SfxEnabled = GetPrivateProfileIntA("Sound", "sfx", 0, (char *)(unsigned)&ai_OptionsFile);
    main_WeatherSound = GetPrivateProfileIntA("Sound", "weather", 1, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "Bildmodus", "FULLSCREEN", bm, 0x100, (char *)(unsigned)&ai_OptionsFile);
    _strupr(bm);
    mode = (strcmp(bm, "DIRECTWINDOW") != 0) * 2 + 1;
    if (strstr(cmd, "DIRECTWINDOW") != 0)
        mode = 1;
    if (strstr(cmd, "FULLSCREEN") != 0)
        mode = 3;
    GetPrivateProfileStringA("General", "GfxPath", "\\project\\gfx\\", (char *)(unsigned)&main_GfxPath, 0x104, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "GamePath", "\\project\\game\\", (char *)(unsigned)&main_GamePath, 0x104, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "MoviePath", "\\project\\movie\\", (char *)(unsigned)&main_MoviePath, 0x104, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("General", "Language", "german", txt_Language, 0x40, (char *)(unsigned)&ai_OptionsFile);
    ls_LanguageOf(txt_Language);
    main_MoviesOpen = GetPrivateProfileIntA("General", "show_intro", 0, (char *)(unsigned)&ai_OptionsFile);
    res = GetPrivateProfileIntA("Gfx", "cur_res", 0, (char *)(unsigned)&ai_OptionsFile);
    ai_GfxResolution = (unsigned char)res;
    ai_ScreenModeW = ai_ScreenModes[(unsigned char)res].w;
    ai_ScreenModeH = ai_ScreenModes[(unsigned char)res].h;
    if (strstr(cmd, "MO_MONEY") != 0)
        U_I((unsigned)&main_MoneyCheat) = 1;
    GetPrivateProfileStringA("General", "Stadt", "Augsburg", (char *)((unsigned)&ai_DataPath + 0x220), 0x40, (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Host", "128.0.0.1", main_NetHost, 0x80, (char *)(unsigned)&ai_OptionsFile);
    U_I(((unsigned)&ls_SavePath + 0x84)) = GetPrivateProfileIntA("Network", "Port", 0x1d6b, (char *)(unsigned)&ai_OptionsFile);
    r = strstr(cmd, "STADT=\"");
    if (r != 0) {
        s = (char *)(r + 7);
        d = (char *)((unsigned)&ai_DataPath + 0x220);
        e = (char *)strchr(s, 0x22);
        if (e != 0)
            while (s < e)
                *d++ = *s++;
        *d = 0;
    }
    r = strstr(cmd, "BERUF=\"");
    if (r != 0) {
        s = (char *)(r + 7);
        d = main_JobName;
        e = (char *)strchr(s, 0x22);
        if (e != 0)
            while (s < e)
                *d++ = *s++;
        *d = 0;
    }
    r = strstr(cmd, "IP=\"");
    if (r != 0) {
        s = (char *)(r + 4);
        d = main_NetHost;
        e = (char *)strchr(s, 0x22);
        if (e != 0)
            while (s < e)
                *d++ = *s++;
        *d = 0;
        net = 1;
    }
    i = strstr(cmd, "PORT=\"");
    if (i != 0) {
        s = (char *)(i + 6);
        d = num;
        e = (char *)strchr(s, 0x22);
        if (e != 0 && s < e) {
            k = e - s;
            memcpy(d, s, k);
            d += k;
        }
        *d = 0;
        U_I(((unsigned)&ls_SavePath + 0x84)) = atoi(num);
    }
    i = main_CreateMutex(c224);
    if (i != 0)
        exit(1);
    i = main_CreateWindow(hInst, show, mode);
    if (i == 0) {
        main_Exit();
        return 0;
    }
    i = main_Init();
    if (i == 0) {
        main_Exit();
        return 0;
    }
    if (main_MoviesOpen != 0)
        main_PlayIntroMovies();
    i = main_OpenScreen(mode);
    if (i == 0) {
        main_Exit();
        return 0;
    }
    i = main_OpenEngine();
    if (i == 0) {
        main_Exit();
        return 0;
    }
    do {
        if (main_DirectStart == 0) {
            if (net == 0) {
                main_Menue();
                if (main_ScreenRestart != 0) {
                    main_ShutdownScreen();
                    sv_PumpMessages();
                    i = main_OpenScreen(mode);
                    if (i == 0) {
                        main_Exit();
                        return 0;
                    }
                    main_ScreenRestart = net;
                    sv_PumpMessages();
                }
            } else {
                U_I((unsigned)&ai_GameFlags) = 5;
            }
        } else {
            k = 0x38;
            if (main_JobName[0] != 0) {
                for (i = 0; i < 0x5f; i++) {
                    if (_strcmpi(main_JobName, ((char **)(unsigned)&txt_JobNames)[i]) == 0) {
                        k = i;
                        break;
                    }
                }
            }
            U_I((unsigned)&ai_GameFlags) = 9;
            init_PlayerProfile.tool1 = (unsigned char)util_RandomInt(0x5f) + 1;
            init_PlayerProfile.tool2 = (unsigned char)util_RandomInt(0x5f) + 1;
            init_PlayerProfile.job = (unsigned char)k;
            init_PlayerProfile.status = 0x12;
            init_PlayerProfile.coat = 0x59a;
            init_PlayerProfile.sex = 0;
            init_PlayerProfile.faith = 1;
            i = gm_HeadIconNumber("patrizier_MANN2");
            init_PlayerProfile.modelId = i + 0x639;
            strcpy(init_PlayerProfile.firstName, "Tobias");
            strcpy(init_PlayerProfile.familyName, "Severin");
            c21c = gm_GetJobTalents(init_PlayerProfile.job);
            *(int *)init_PlayerProfile.talents = *(int *)c21c.skills;
            init_PlayerProfile.talents[4] = c21c.skills[4];
            init_PlayerProfile.talents[3] = (char)0xa8;
            init_PlayerProfile.talents[0] = 0;
            init_PlayerProfile.talents[1] = 0;
            hst_SetTextsOn(1);
        }
        U_I((unsigned)&ai_LeaveGame) = 1;
        if (U_I((unsigned)&ai_GameFlags) != 0 && U_I((unsigned)&main_QuitProgram) == 0) {
            do {
                main_GameLoop(0);
            } while (U_I((unsigned)&ai_LeaveGame) != 0);
        }
        if (main_DirectStart != 0 || net != 0)
            U_I((unsigned)&main_QuitProgram) = 1;
        U_I((unsigned)&sv_PopRequest) = 0;
        U_I((unsigned)&sv_FrameDepth) = 0;
        if (U_I((unsigned)&main_QuitProgram) != 0) {
            main_Exit();
            if (U_I((unsigned)&main_ScreenSaverOff) != 0)
                main_SetScreenSaver(1);
            return 1;
        }
    } while (1);
}

extern void * main_Instance;

/* ---- full-link gap data (tools/full_addrs.py) ---- */
#define GAPDATA __declspec(align(1)) unsigned char
GAPDATA full_rdata_5ebb40[4] = { 0x00, 0x00, 0xc8, 0x42 };  /* 0x5ebb40 */
GAPDATA full_rdata_5ebb44[4] = { 0x00, 0x00, 0x80, 0x43 };  /* 0x5ebb44 */
GAPDATA full_rdata_5ed0fc[4] = { 0x00, 0x00, 0x04, 0x42 };  /* 0x5ed0fc */
GAPDATA full_rdata_5ee01c[4] = { 0x89, 0x88, 0x88, 0x3c };  /* 0x5ee01c */
GAPDATA full_rdata_5ee020[4] = { 0x9a, 0x99, 0x19, 0x3f };  /* 0x5ee020 */
GAPDATA full_rdata_5eed8c[4] = { 0x52, 0xb8, 0xde, 0x3e };  /* 0x5eed8c */
GAPDATA full_rdata_5eed90[4] = { 0x8f, 0xc2, 0xf5, 0x3e };  /* 0x5eed90 */
GAPDATA gm_SpecialItemGroups[63] = { 0x1f, 0xcd, 0x01, 0xce, 0x01, 0xcf, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xd0, 0x01, 0xd1, 0x01, 0xd2, 0x01, 0xd3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xca, 0x01, 0xcb, 0x01, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x5eed94, the item groups of the good kinds ' ', 0x1e and 0x1f (3 x 0x15 bytes, a G7GRP each) */
GAPDATA full_rdata_5eedd3[5] = { 0x00, 0x03, 0x00, 0x00, 0x00 };  /* 0x5eedd3 */
int d3t_BoxCornerIdx = -1;  /* 0x5f3418, set to -1 by ai_GameFrame at the game start, no reader found; a reasoned guess: the index into the four corner vectors (+-200, 0, +-200) that follow (owner di_input or d3_tools, see lc.c) */
GAPDATA full_data_5f341c[64] = { 0x00, 0x00, 0x48, 0xc3, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x48, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xc3, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x48, 0xc3, 0x00, 0x00, 0x00, 0x00 };  /* 0x5f341c */
GAPDATA full_data_600c00[1] = { 0 };  /* 0x600c00 */
GAPDATA full_data_600c01[399] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x6f, 0x79, 0x61, 0x6c, 0x20, 0x46, 0x6c, 0x75, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x600c01 */
GAPDATA full_data_60d38c[36] = { 0x68, 0x61, 0x6e, 0x64, 0x77, 0x65, 0x72, 0x6b, 0x65, 0x72, 0x33, 0x5f, 0x4d, 0x41, 0x4e, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x62, 0x75, 0x65, 0x72, 0x67, 0x65, 0x72, 0x32, 0x5f, 0x4d, 0x41, 0x4e, 0x4e, 0x00, 0x00, 0x00 };  /* 0x60d38c */
GAPDATA full_data_60d5a4[16] = { 0x41, 0x4d, 0x54, 0x53, 0x57, 0x41, 0x48, 0x4c, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x00, 0x00 };  /* 0x60d5a4 */
GAPDATA full_data_60d5b4[16] = { 0x41, 0x42, 0x53, 0x45, 0x54, 0x5a, 0x55, 0x4e, 0x47, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x00 };  /* 0x60d5b4 */
GAPDATA full_data_60da10[432] = { 0x20, 0x25, 0x73, 0x00, 0x5f, 0x50, 0x52, 0x4f, 0x5a, 0x45, 0x53, 0x53, 0x5f, 0x35, 0x5f, 0x46, 0x4f, 0x4c, 0x54, 0x45, 0x52, 0x32, 0x5f, 0x25, 0x73, 0x00, 0x00, 0x00, 0x50, 0x52, 0x4f, 0x5a, 0x45, 0x53, 0x53, 0x5f, 0x35, 0x5f, 0x46, 0x4f, 0x4c, 0x54, 0x45, 0x52, 0x32, 0x5f, 0x55, 0x4e, 0x53, 0x43, 0x48, 0x55, 0x4c, 0x44, 0x53, 0x42, 0x45, 0x54, 0x45, 0x55, 0x45, 0x52, 0x55, 0x4e, 0x47, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x00, 0x50, 0x52, 0x4f, 0x5a, 0x45, 0x53, 0x53, 0x5f, 0x35, 0x5f, 0x46, 0x4f, 0x4c, 0x54, 0x45, 0x52, 0x32, 0x5f, 0x57, 0x45, 0x49, 0x54, 0x45, 0x52, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x00, 0x00, 0x50, 0x52, 0x4f, 0x5a, 0x45, 0x53, 0x53, 0x5f, 0x35, 0x5f, 0x46, 0x4f, 0x4c, 0x54, 0x45, 0x52, 0x32, 0x5f, 0x47, 0x45, 0x53, 0x54, 0x45, 0x48, 0x45, 0x4e, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x50, 0x52, 0x4f, 0x5a, 0x45, 0x53, 0x53, 0x5f, 0x35, 0x5f, 0x46, 0x4f, 0x4c, 0x54, 0x45, 0x52, 0x32, 0x5f, 0x25, 0x73, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x00, 0x00, 0x63, 0x75, 0x74, 0x73, 0x63, 0x65, 0x6e, 0x65, 0x73, 0x5c, 0x70, 0x72, 0x6f, 0x7a, 0x65, 0x73, 0x73, 0x5c, 0x66, 0x6f, 0x6c, 0x74, 0x65, 0x72, 0x5c, 0x00, 0x00, 0x00, 0x53, 0x54, 0x52, 0x45, 0x43, 0x4b, 0x42, 0x41, 0x4e, 0x4b, 0x00, 0x00, 0x48, 0x41, 0x45, 0x4e, 0x47, 0x45, 0x4e, 0x44, 0x45, 0x52, 0x5f, 0x4b, 0x41, 0x45, 0x46, 0x49, 0x47, 0x00, 0x00, 0x00, 0x45, 0x49, 0x53, 0x57, 0x41, 0x53, 0x53, 0x45, 0x52, 0x54, 0x52, 0x4f, 0x50, 0x46, 0x45, 0x52, 0x00, 0x00, 0x00, 0x00, 0x45, 0x53, 0x53, 0x45, 0x5f, 0x55, 0x4e, 0x44, 0x5f, 0x42, 0x52, 0x41, 0x4e, 0x44, 0x45, 0x49, 0x53, 0x45, 0x4e, 0x00, 0x4e, 0x45, 0x55, 0x4e, 0x53, 0x43, 0x48, 0x57, 0x41, 0x45, 0x4e, 0x5a, 0x49, 0x47, 0x45, 0x5f, 0x50, 0x45, 0x49, 0x54, 0x53, 0x43, 0x48, 0x45, 0x00, 0x00, 0x00, 0x00, 0x53, 0x50, 0x41, 0x4e, 0x49, 0x53, 0x43, 0x48, 0x45, 0x5f, 0x53, 0x54, 0x49, 0x45, 0x46, 0x45, 0x4c, 0x00, 0x00, 0x00, 0x44, 0x41, 0x55, 0x4d, 0x45, 0x4e, 0x53, 0x43, 0x48, 0x52, 0x41, 0x55, 0x42, 0x45, 0x4e, 0x00, 0x73, 0x74, 0x72, 0x65, 0x63, 0x6b, 0x62, 0x61, 0x6e, 0x6b, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x6b, 0x61, 0x65, 0x66, 0x69, 0x67, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x65, 0x69, 0x73, 0x74, 0x72, 0x6f, 0x70, 0x66, 0x65, 0x72, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x62, 0x72, 0x61, 0x6e, 0x64, 0x65, 0x69, 0x73, 0x65, 0x6e, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x70, 0x65, 0x69, 0x74, 0x73, 0x63, 0x68, 0x65, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00, 0x00, 0x73, 0x74, 0x69, 0x65, 0x66, 0x65, 0x6c, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x64, 0x73, 0x63, 0x68, 0x72, 0x61, 0x75, 0x62, 0x65, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00 };  /* 0x60da10 */
GAPDATA full_data_623abc[4] = { 0x00, 0x00, 0x00, 0x00 };  /* 0x623abc */
GAPDATA full_data_623eb8[4] = { 0x01, 0x00, 0x00, 0x00 };  /* 0x623eb8 */
int main_GfxOn = 1;  /* 0x624560, nonzero when the round change shows its pages and fades (main_EndRound, main_RoundReport and main_BeginPlayerRound test it) */
int main_AutoSaveOn = 1;  /* 0x62456c, nonzero: the end of a round writes the autosave (main_GameLoop, a single player game not in the menu); never written (reasoned: a switch) */
int main_ObjectsOn = 1;  /* 0x62457c, nonzero: sv_RunFrame draws the gui objects (d2_DrawObjects) each frame */
int main_AmbientSoundOn = 1;  /* 0x624584, the [Sound] ambient key of game.INI (WinMain stores it; no reader in the image) */
unsigned char main_GfxPath[264] = { 0x5c, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x5c, 0x67, 0x66, 0x78, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x624588, the GfxPath option (the vfs root and the base of the screenshot folder) */
char main_GamePath[264] = { 0x5c, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x5c, 0x67, 0x61, 0x6d, 0x65, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x624690 */
char main_MoviePath[0x104] = { 0x5c, 0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x5c, 0x6d, 0x6f, 0x76, 0x69, 0x65, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x624798 */
int main_ClockScale = 100;  /* 0x6248b4, the scale sv_UpdateClock multiplies the speed setting by to get the game seconds of one clock update ((speed * 0.00625 + 0.5) * this) */
int main_MessageBox = -1;  /* 0x624938, the message box open over the character creation (sv_OpenMessageBox), -1 = none */
const char main_DefaultHost[12] = "127.0.0.1";  /* 0x624b48, the default host address of the [Network] Host option (a string literal of the original) */
char main_HomeSweetHome[16] = "Home-Sweet-Home";  /* 0x624ce0, the name main_GiveStartBuildings gives the player's start building (AIBLD.name; the original copies the 16 bytes inline at 0x53405e) */
GAPDATA full_data_624cf8[36] = { 0x24, 0x43, 0x24, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x65, 0x6e, 0x75, 0x5c, 0x63, 0x68, 0x6f, 0x6f, 0x73, 0x65, 0x63, 0x68, 0x61, 0x72, 0x61, 0x63, 0x74, 0x65, 0x72, 0x5f, 0x74, 0x61, 0x6c, 0x65, 0x6e, 0x74, 0x00 };  /* 0x624cf8 */
GAPDATA full_data_624ec4[28] = { 0x61, 0x72, 0x62, 0x65, 0x69, 0x74, 0x65, 0x72, 0x5f, 0x4d, 0x41, 0x4e, 0x4e, 0x00, 0x00, 0x00, 0x77, 0x69, 0x72, 0x74, 0x5f, 0x44, 0x49, 0x43, 0x4b, 0x45, 0x52, 0x00 };  /* 0x624ec4 */
GAPDATA full_data_624ee0[32] = { 0x70, 0x61, 0x74, 0x72, 0x69, 0x7a, 0x69, 0x65, 0x72, 0x5f, 0x4d, 0x41, 0x4e, 0x4e, 0x32, 0x00, 0x53, 0x6b, 0x79, 0x5f, 0x4d, 0x69, 0x74, 0x74, 0x65, 0x6c, 0x5f, 0x30, 0x32, 0x00, 0x00, 0x00 };  /* 0x624ee0 */
GAPDATA full_data_624fcc[16] = { 0x68, 0x61, 0x6e, 0x64, 0x77, 0x65, 0x72, 0x6b, 0x65, 0x72, 0x5f, 0x4d, 0x41, 0x4e, 0x4e, 0x00 };  /* 0x624fcc */
char lobby_ChooseProfessionForm[24] = "Menu\\CHOOSEPROFESSION";  /* 0x6250b8, the choose-profession form lobby_ChooseProfession opens (21 bytes and 3 of padding) */
GAPDATA full_data_625150[4] = { 0x25, 0x69, 0x61, 0x5b };  /* 0x625150 */
GAPDATA full_data_625154[4] = { 0x25, 0x73, 0x5d, 0x00 };  /* 0x625154 */
char main_BeamedHome[12] = "Beamed home";  /* 0x6253b0, the activity name main_EndRound gives a character it beams home (CHACT.name70, the original copies the 12 bytes inline) */
char main_RoundTotalFmt[20] = "$L%s %i%%:$R-%T$A";  /* 0x6253bc, the line of the player's round totals main_EndRound prints per income kind (io_PrintText; 18 bytes and 2 of padding) */
char main_MasterExamLog[56] = "MeisterPr\xfc" "fung started: player_id:%i beruf:%i grad:%i";  /* 0x6253d0, the debug line main_EndRound sends to the log when a master exam starts (sprintf; 54 bytes and 2 of padding) */
unsigned char he_FindKind = 0xff;  /* 0x625708, he.h */
GAPDATA full_data_625709[3] = { 0x00, 0x00, 0x00 };  /* 0x625709 */
int he_FindId = -1;  /* 0x62570c, he.h */
unsigned short he_FindChr = 0xffff;  /* 0x625710, he.h */
GAPDATA full_data_625712[2] = { 0x00, 0x00 };  /* 0x625712 */
int he_FindFrom = -1;  /* 0x625714, he.h */
int he_LastSlot = -1;  /* 0x625718, he.h */
int he_BarShown = -1;  /* 0x62571c, he.h */
GAPDATA full_data_6279f0[32] = { 0x45, 0x6e, 0x74, 0x64, 0x65, 0x63, 0x6b, 0x74, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x48, 0x53, 0x5f, 0x37, 0x37, 0x00, 0x00 };  /* 0x6279f0 */
GAPDATA full_data_627a10[20] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x48, 0x53, 0x5f, 0x37, 0x36, 0x00, 0x00 };  /* 0x627a10 */
char he_VoiceHealing[20] = "_NACHRICHTEN_HS_80";  /* 0x627b0c, the voice of the healing event 0x6c's news 0x1a18 (he_RunHealing) */
char he_InitBetrieb[12] = "InitBetrieb";  /* 0x627b20, the scene name he.c's cm_SendChrGotoBuilding plays for the new worker */
GAPDATA full_data_627d74[48] = { 0x52, 0x61, 0x75, 0x62, 0x7a, 0x75, 0x67, 0x00, 0x73, 0x70, 0x65, 0x7a, 0x69, 0x61, 0x6c, 0x2f, 0x6d, 0x65, 0x63, 0x6b, 0x65, 0x72, 0x6e, 0x00, 0x67, 0x65, 0x73, 0x74, 0x69, 0x6b, 0x67, 0x2f, 0x68, 0x65, 0x72, 0x72, 0x61, 0x6e, 0x77, 0x69, 0x6e, 0x6b, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x00 };  /* 0x627d74 */
/* 0x627e98, the construction models of the building types (he.c's he_CreateConstruction): the types of each record, then
   its scaffolding model; the record with type 0 ends the table and names the model for the types not listed */
HEBLDMODEL he_BuildingModels[37] = {
    { { 8, 9, 10, 0 }, "*bau_BRUNNEN_a.ogr" },
    { { 5, 0, 0, 0 }, "*bau_BUERGERHAUS_a.ogr" },
    { { 17, 0, 0, 0 }, "*bau_GEFAENGNIS_a.ogr" },
    { { 18, 19, 0, 0 }, "*bau_GEFAENGNIS_b.ogr" },
    { { 50, 51, 52, 0 }, "*bau_HANDWERK_b.ogr" },
    { { 33, 34, 35, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 14, 15, 16, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 41, 42, 43, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 26, 27, 28, 0 }, "*bau_HANDWERK_d.ogr" },
    { { 4, 2, 0, 0 }, "*bau_HAUS_a.ogr" },
    { { 3, 1, 0, 0 }, "*bau_HAUS_a.ogr" },
    { { 22, 0, 0, 0 }, "*bau_KIRCHE_GROSS_a.ogr" },
    { { 20, 0, 0, 0 }, "*bau_KIRCHE_KLEIN_a.ogr" },
    { { 21, 0, 0, 0 }, "*bau_KIRCHE_MITTEL_a.ogr" },
    { { 44, 0, 0, 0 }, "*bau_STADTWACHE_a.ogr" },
    { { 45, 0, 0, 0 }, "*bau_STADTWACHE_b.ogr" },
    { { 46, 0, 0, 0 }, "*bau_STADTWACHE_b.ogr" },
    { { 11, 12, 13, 0 }, "*bau_STADTWACHE_b.ogr" },
    { { 23, 24, 25, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 47, 48, 49, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 53, 54, 55, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 6, 7, 0, 0 }, "*bau_WOHNSITZ_GROSS_a.ogr" },
    { { 57, 58, 56, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 60, 61, 59, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 63, 64, 62, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 66, 67, 65, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 75, 0, 0, 0 }, "*bau_DOMIZIL_LANDSITZ.ogr" },
    { { 76, 0, 0, 0 }, "*bau_DOMIZIL_BURG.ogr" },
    { { 77, 0, 0, 0 }, "*bau_DOMIZIL_SCHLOSS.ogr" },
    { { 84, 85, 86, 0 }, "*bau_BRUNNEN_a.ogr" },
    { { 81, 0, 0, 0 }, "*bau_HANDWERK_SCHNEIDER_a.ogr" },
    { { 82, 83, 0, 0 }, "*bau_HANDWERK_SCHNEIDER_b.ogr" },
    { { 79, 78, 80, 0 }, "*bau_ZIGEUNER_ZIGEUNER_a.ogr" },
    { { 72, 0, 0, 0 }, "*bau_FRIEDHOF_TOTENFELD.ogr" },
    { { 73, 0, 0, 0 }, "*bau_FRIEDHOF_GOTTESACKER.ogr" },
    { { 74, 0, 0, 0 }, "*bau_FRIEDHOF_FRIEDHOF.ogr" },
    { { 0, 0, 0, 0 }, "*bau_HAUS_a.ogr" }
};
GAPDATA full_data_628a28[20] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x48, 0x53, 0x5f, 0x30, 0x37, 0x00, 0x00 };  /* 0x628a28 */
char he_AssassinName[12] = "ASSASSIN";  /* 0x628dc8, the name of the agent the secret room event 0x99 spawns (he_AllocSecretRoom's cm_SendSpawnCharacter) */
GAPDATA full_data_628dd4[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x38, 0x00 };  /* 0x628dd4 */
GAPDATA full_data_628dec[12] = { 0x53, 0x70, 0x79, 0x20, 0x4b, 0x69, 0x6c, 0x6c, 0x65, 0x72, 0x00, 0x00 };  /* 0x628dec */
GAPDATA full_data_628df8[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x37, 0x00 };  /* 0x628df8 */
GAPDATA full_data_628e10[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x39, 0x00 };  /* 0x628e10 */
GAPDATA full_data_628e28[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x36, 0x00 };  /* 0x628e28 */
char he_NewsDomizil11[24] = "_NACHRICHTEN_DOMIZIL_11";  /* 0x628e40, the second text (the sound key) of the news 0x5fb the domicile event 0x9b's run sends */
GAPDATA full_data_628e58[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x31, 0x30, 0x00 };  /* 0x628e58 */
char he_ActivityUpgrade[12] = "Upgraden";  /* 0x628e70, the activity name (sv_RegisterActivity) of the grave training event 0x8f */
char he_ActivityRawMaterial[16] = "Rohstoff suchen";  /* 0x628e7c, the activity name of the master work event 0x8c */
char he_ActivityGraveCare[12] = "grabpflege";  /* 0x628e8c, the activity name of the grave care event 0x8d */
GAPDATA full_data_628e98[12] = { 0x48, 0x45, 0x49, 0x4d, 0x53, 0x55, 0x43, 0x48, 0x55, 0x4e, 0x47, 0x00 };  /* 0x628e98 */
GAPDATA full_data_628ea4[20] = { 0x48, 0x41, 0x55, 0x43, 0x48, 0x20, 0x44, 0x45, 0x53, 0x20, 0x56, 0x45, 0x52, 0x46, 0x41, 0x4c, 0x4c, 0x53, 0x00, 0x00 };  /* 0x628ea4 */
GAPDATA full_data_628eb8[20] = { 0x50, 0x41, 0x4b, 0x54, 0x20, 0x4d, 0x49, 0x54, 0x20, 0x44, 0x45, 0x4d, 0x20, 0x54, 0x4f, 0x44, 0x00, 0x00, 0x00, 0x00 };  /* 0x628eb8 */
GAPDATA full_data_628ecc[112] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x30, 0x39, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x5a, 0x49, 0x47, 0x45, 0x55, 0x4e, 0x45, 0x52, 0x5f, 0x30, 0x33, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x30, 0x32, 0x00, 0x00, 0x00, 0x00 };  /* 0x628ecc */
char he_MeetingSceneSmall[56] = "LOCATIONS\\NEKROMANTENTURM\\NEBEL_DES_GRAUENS_KLEIN.esc";  /* 0x628f3c, the meeting scene script the event 0x91 loads for the kind 0 / other (0x5726b0) */
char he_MeetingSceneMiddle[56] = "LOCATIONS\\NEKROMANTENTURM\\NEBEL_DES_GRAUENS_MITTEL.esc";  /* 0x628f74, the meeting scene script the event 0x91 loads for the kind 1 (0x5726b0) */
char he_MeetingSceneBig[56] = "LOCATIONS\\NEKROMANTENTURM\\NEBEL_DES_GRAUENS_GROSS.esc";  /* 0x628fac, the meeting scene script the event 0x91 loads for the kind 2 (0x5726b0) */
GAPDATA full_data_628fe4[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x30, 0x37, 0x00, 0x00, 0x00, 0x00 };  /* 0x628fe4 */
GAPDATA full_data_629000[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x30, 0x36, 0x00, 0x00, 0x00, 0x00 };  /* 0x629000 */
GAPDATA full_data_62901c[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x5a, 0x49, 0x47, 0x45, 0x55, 0x4e, 0x45, 0x52, 0x5f, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00 };  /* 0x62901c */
GAPDATA full_data_629038[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x5a, 0x49, 0x47, 0x45, 0x55, 0x4e, 0x45, 0x52, 0x5f, 0x30, 0x32, 0x00, 0x00, 0x00, 0x00 };  /* 0x629038 */
GAPDATA full_data_629054[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x5a, 0x49, 0x47, 0x45, 0x55, 0x4e, 0x45, 0x52, 0x5f, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 };  /* 0x629054 */
GAPDATA he_TheaterScripts[252] = { 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2f, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x7a, 0x65, 0x6c, 0x74, 0x2f, 0x74, 0x68, 0x65, 0x61, 0x74, 0x65, 0x72, 0x61, 0x75, 0x66, 0x66, 0x75, 0x65, 0x68, 0x72, 0x75, 0x6e, 0x67, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x74, 0x69, 0x65, 0x66, 0x5f, 0x76, 0x65, 0x72, 0x62, 0x65, 0x75, 0x67, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x73, 0x74, 0x65, 0x68, 0x65, 0x6e, 0x64, 0x5f, 0x62, 0x65, 0x73, 0x63, 0x68, 0x77, 0x6f, 0x65, 0x72, 0x65, 0x6e, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x72, 0x75, 0x65, 0x63, 0x6b, 0x73, 0x61, 0x6c, 0x74, 0x6f, 0x32, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x72, 0x75, 0x65, 0x63, 0x6b, 0x73, 0x61, 0x6c, 0x74, 0x6f, 0x00, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x70, 0x6f, 0x73, 0x69, 0x65, 0x72, 0x65, 0x6e, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x5f, 0x77, 0x69, 0x6e, 0x6b, 0x65, 0x6e, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x6c, 0x61, 0x63, 0x68, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x68, 0x61, 0x6e, 0x64, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x32, 0x00, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x68, 0x61, 0x6e, 0x64, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x00, 0x7a, 0x69, 0x72, 0x6b, 0x75, 0x73, 0x2f, 0x61, 0x6e, 0x66, 0x65, 0x75, 0x65, 0x72, 0x6e, 0x00, 0x42, 0x65, 0x74, 0x74, 0x6c, 0x65, 0x72, 0x00 };  /* 0x629070 */
GAPDATA full_data_6291b4[336] = { 0xf8, 0x92, 0x62, 0x00, 0xf0, 0x92, 0x62, 0x00, 0xe4, 0x92, 0x62, 0x00, 0xd8, 0x92, 0x62, 0x00, 0xd0, 0x92, 0x62, 0x00, 0xc0, 0x92, 0x62, 0x00, 0xb0, 0x92, 0x62, 0x00, 0xa4, 0x92, 0x62, 0x00, 0x98, 0x92, 0x62, 0x00, 0x84, 0x92, 0x62, 0x00, 0x74, 0x92, 0x62, 0x00, 0x68, 0x92, 0x62, 0x00, 0x58, 0x92, 0x62, 0x00, 0x48, 0x92, 0x62, 0x00, 0x38, 0x92, 0x62, 0x00, 0x24, 0x92, 0x62, 0x00, 0x10, 0x92, 0x62, 0x00, 0xfc, 0x91, 0x62, 0x00, 0x46, 0x54, 0x42, 0x65, 0x7a, 0x69, 0x65, 0x72, 0x45, 0x6e, 0x64, 0x44, 0x65, 0x6c, 0x61, 0x79, 0x00, 0x00, 0x00, 0x00, 0x46, 0x54, 0x42, 0x65, 0x7a, 0x69, 0x65, 0x72, 0x41, 0x6e, 0x69, 0x6d, 0x61, 0x74, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x46, 0x54, 0x42, 0x65, 0x7a, 0x69, 0x65, 0x72, 0x53, 0x74, 0x61, 0x72, 0x74, 0x44, 0x65, 0x6c, 0x61, 0x79, 0x00, 0x00, 0x46, 0x54, 0x42, 0x65, 0x7a, 0x69, 0x65, 0x72, 0x41, 0x6e, 0x69, 0x6d, 0x4f, 0x66, 0x66, 0x00, 0x46, 0x54, 0x4e, 0x6f, 0x42, 0x65, 0x7a, 0x69, 0x65, 0x72, 0x41, 0x6e, 0x69, 0x6d, 0x00, 0x00, 0x46, 0x54, 0x45, 0x6e, 0x64, 0x53, 0x75, 0x63, 0x63, 0x73, 0x65, 0x73, 0x73, 0x00, 0x00, 0x00, 0x46, 0x54, 0x53, 0x75, 0x63, 0x63, 0x73, 0x65, 0x73, 0x73, 0x00, 0x00, 0x46, 0x54, 0x42, 0x65, 0x67, 0x69, 0x6e, 0x53, 0x75, 0x63, 0x63, 0x73, 0x65, 0x73, 0x73, 0x00, 0x46, 0x54, 0x50, 0x61, 0x72, 0x74, 0x69, 0x61, 0x6c, 0x53, 0x75, 0x63, 0x63, 0x73, 0x65, 0x73, 0x73, 0x00, 0x00, 0x00, 0x46, 0x54, 0x52, 0x65, 0x6d, 0x69, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x00, 0x46, 0x54, 0x45, 0x6e, 0x64, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x00, 0x46, 0x54, 0x49, 0x64, 0x6c, 0x65, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x00, 0x00, 0x00, 0x00, 0x46, 0x54, 0x42, 0x65, 0x67, 0x69, 0x6e, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x00, 0x00, 0x00, 0x46, 0x54, 0x49, 0x64, 0x6c, 0x65, 0x00, 0x00, 0x46, 0x54, 0x52, 0x65, 0x76, 0x69, 0x65, 0x77, 0x69, 0x6e, 0x67, 0x00, 0x46, 0x54, 0x56, 0x69, 0x65, 0x77, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x46, 0x54, 0x53, 0x74, 0x61, 0x72, 0x74, 0x00, 0x46, 0x54, 0x44, 0x69, 0x73, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x00, 0x00 };  /* 0x6291b4, the state-name pointer table and its strings */
GAPDATA full_data_629304[16] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00 };  /* 0x629304 */
GAPDATA full_data_629314[20] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x65, 0x73, 0x73, 0x00, 0x00, 0x00 };  /* 0x629314 */
GAPDATA full_data_629328[24] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x62, 0x6f, 0x74, 0x74, 0x6f, 0x6d, 0x5f, 0x66, 0x6f, 0x72, 0x6d, 0x00, 0x00, 0x00, 0x00 };  /* 0x629328 */
GAPDATA full_data_629340[20] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x74, 0x6f, 0x70, 0x5f, 0x66, 0x6f, 0x72, 0x6d, 0x00, 0x00, 0x00 };  /* 0x629340 */
GAPDATA full_data_629354[20] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x72, 0x69, 0x67, 0x68, 0x74, 0x5f, 0x66, 0x6f, 0x72, 0x6d, 0x00 };  /* 0x629354 */
GAPDATA full_data_629368[20] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x6c, 0x65, 0x66, 0x74, 0x5f, 0x66, 0x6f, 0x72, 0x6d, 0x00, 0x00 };  /* 0x629368 */
GAPDATA full_data_62937c[20] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x61, 0x75, 0x78, 0x5f, 0x66, 0x6f, 0x72, 0x6d, 0x00, 0x00, 0x00 };  /* 0x62937c */
GAPDATA full_data_629390[24] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x69, 0x6e, 0x6c, 0x69, 0x6e, 0x65, 0x5f, 0x66, 0x6f, 0x72, 0x6d, 0x00, 0x00, 0x00, 0x00 };  /* 0x629390 */
GAPDATA full_data_6293a8[20] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x00 };  /* 0x6293a8 */
GAPDATA full_data_6293bc[28] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x5f, 0x4f, 0x55, 0x54, 0x52, 0x4f, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x00, 0x00 };  /* 0x6293bc */
GAPDATA full_data_6293d8[16] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x4d, 0x61, 0x69, 0x6e, 0x49, 0x6e, 0x74, 0x72, 0x6f, 0x00 };  /* 0x6293d8 */
GAPDATA full_data_6293e8[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x4f, 0x55, 0x54, 0x52, 0x4f, 0x00 };  /* 0x6293e8 */
GAPDATA full_data_6293f8[44] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x4a, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x6293f8 */
GAPDATA full_data_629424[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x4a, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629424 */
GAPDATA full_data_629438[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x4a, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629438 */
GAPDATA full_data_62944c[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x4f, 0x75, 0x74, 0x72, 0x6f, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x49, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x62944c */
GAPDATA full_data_629474[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x49, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629474 */
GAPDATA full_data_629488[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x49, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629488 */
GAPDATA full_data_62949c[44] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x31, 0x30, 0x00, 0x00, 0x00, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x48, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x62949c */
GAPDATA full_data_6294c8[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x48, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x6294c8 */
GAPDATA full_data_6294dc[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x48, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x6294dc */
GAPDATA full_data_6294f0[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x39, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x47, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x6294f0 */
GAPDATA full_data_629518[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x47, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629518 */
GAPDATA full_data_62952c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x47, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62952c */
GAPDATA full_data_629540[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x38, 0x00 };  /* 0x629540 */
GAPDATA full_data_629554[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x46, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629554 */
GAPDATA full_data_629568[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x37, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x45, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629568 */
GAPDATA full_data_629590[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x31, 0x5f, 0x45, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x629590 */
GAPDATA full_data_6295a0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x45, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x6295a0 */
GAPDATA full_data_6295b4[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x45, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x6295b4 */
GAPDATA full_data_6295c8[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x36, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x44, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x6295c8 */
GAPDATA full_data_6295f0[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x31, 0x5f, 0x44, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x6295f0 */
GAPDATA full_data_629600[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x44, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629600 */
GAPDATA full_data_629614[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x44, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629614 */
GAPDATA full_data_629628[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x35, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x43, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629628 */
GAPDATA full_data_629650[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x31, 0x5f, 0x43, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x629650 */
GAPDATA full_data_629660[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x43, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629660 */
GAPDATA full_data_629674[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x43, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629674 */
GAPDATA full_data_629688[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x34, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x41, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629688 */
GAPDATA full_data_6296b0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x41, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x6296b0 */
GAPDATA full_data_6296c4[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x41, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x6296c4 */
GAPDATA full_data_6296d8[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x33, 0x00 };  /* 0x6296d8 */
GAPDATA full_data_6296ec[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x31, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x00 };  /* 0x6296ec */
GAPDATA full_data_6296fc[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x53, 0x74, 0x65, 0x70, 0x31, 0x00 };  /* 0x6296fc */
GAPDATA full_data_629710[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x31, 0x49, 0x6e, 0x74, 0x72, 0x6f, 0x00 };  /* 0x629710 */
GAPDATA full_data_629724[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x4f, 0x55, 0x54, 0x52, 0x4f, 0x00 };  /* 0x629724 */
GAPDATA full_data_629734[24] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00 };  /* 0x629734 */
GAPDATA full_data_62974c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x46, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62974c */
GAPDATA full_data_629760[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x46, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629760 */
GAPDATA full_data_629774[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x4f, 0x75, 0x74, 0x72, 0x6f, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x45, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629774 */
GAPDATA full_data_62979c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x45, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62979c */
GAPDATA full_data_6297b0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x45, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x6297b0 */
GAPDATA full_data_6297c4[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x37, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x44, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x6297c4 */
GAPDATA full_data_6297ec[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x44, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x6297ec */
GAPDATA full_data_629800[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x44, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629800 */
GAPDATA full_data_629814[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x36, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x43, 0x31, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00 };  /* 0x629814 */
GAPDATA full_data_62983c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x43, 0x31, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00 };  /* 0x62983c */
GAPDATA full_data_629850[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x43, 0x31, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00 };  /* 0x629850 */
GAPDATA full_data_629864[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x35, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x43, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629864 */
GAPDATA full_data_62988c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x43, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62988c */
GAPDATA full_data_6298a0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x43, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x6298a0 */
GAPDATA full_data_6298b4[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x34, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x42, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x6298b4 */
GAPDATA full_data_6298dc[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x42, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x6298dc */
GAPDATA full_data_6298f0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x42, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x6298f0 */
GAPDATA full_data_629904[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x33, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x41, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629904 */
GAPDATA full_data_62992c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x41, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62992c */
GAPDATA full_data_629940[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x41, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629940 */
GAPDATA full_data_629954[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00 };  /* 0x629954 */
GAPDATA full_data_629968[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x32, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x00 };  /* 0x629968 */
GAPDATA full_data_629978[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x53, 0x74, 0x65, 0x70, 0x31, 0x00 };  /* 0x629978 */
GAPDATA full_data_62998c[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x32, 0x49, 0x6e, 0x74, 0x72, 0x6f, 0x00 };  /* 0x62998c */
GAPDATA full_data_6299a0[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x4f, 0x55, 0x54, 0x52, 0x4f, 0x00 };  /* 0x6299a0 */
GAPDATA full_data_6299b0[24] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00 };  /* 0x6299b0 */
GAPDATA full_data_6299c8[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x4f, 0x75, 0x74, 0x72, 0x6f, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x49, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x6299c8 */
GAPDATA full_data_6299f0[24] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x33, 0x5f, 0x49, 0x5f, 0x53, 0x74, 0x65, 0x70, 0x5f, 0x41, 0x75, 0x78, 0x00, 0x00, 0x00, 0x00 };  /* 0x6299f0 */
GAPDATA full_data_629a08[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x49, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629a08 */
GAPDATA full_data_629a1c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x48, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629a1c */
GAPDATA full_data_629a30[24] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x31, 0x30, 0x00, 0x00, 0x00, 0x00 };  /* 0x629a30 */
GAPDATA full_data_629a48[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x39, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x47, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629a48 */
GAPDATA full_data_629a70[24] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x33, 0x5f, 0x47, 0x5f, 0x53, 0x74, 0x65, 0x70, 0x5f, 0x41, 0x75, 0x78, 0x00, 0x00, 0x00, 0x00 };  /* 0x629a70 */
GAPDATA full_data_629a88[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x47, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629a88 */
GAPDATA full_data_629a9c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x47, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629a9c */
GAPDATA full_data_629ab0[24] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x41, 0x6e, 0x69, 0x6d, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x38, 0x00 };  /* 0x629ab0 */
GAPDATA full_data_629ac8[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x46, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629ac8 */
GAPDATA full_data_629adc[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x38, 0x00 };  /* 0x629adc */
GAPDATA full_data_629af0[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x37, 0x00 };  /* 0x629af0 */
GAPDATA full_data_629b04[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x54, 0x4f, 0x50, 0x00, 0x00, 0x00, 0x00 };  /* 0x629b04 */
GAPDATA full_data_629b1c[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x4c, 0x45, 0x46, 0x54, 0x00, 0x00, 0x00 };  /* 0x629b1c */
GAPDATA full_data_629b34[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x52, 0x49, 0x47, 0x48, 0x54, 0x00, 0x00 };  /* 0x629b34 */
GAPDATA full_data_629b4c[44] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x41, 0x6e, 0x69, 0x6d, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x36, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629b4c */
GAPDATA full_data_629b78[24] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x53, 0x74, 0x65, 0x70, 0x5f, 0x41, 0x75, 0x78, 0x00, 0x00, 0x00, 0x00 };  /* 0x629b78 */
GAPDATA full_data_629b90[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629b90 */
GAPDATA full_data_629ba4[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x45, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629ba4 */
GAPDATA full_data_629bb8[28] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x36, 0x00, 0x00, 0x00 };  /* 0x629bb8 */
GAPDATA full_data_629bd4[28] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x4e, 0x45, 0x58, 0x54, 0x5f, 0x4c, 0x45, 0x46, 0x54, 0x00, 0x00 };  /* 0x629bd4 */
GAPDATA full_data_629bf0[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x4c, 0x45, 0x46, 0x54, 0x00, 0x00, 0x00 };  /* 0x629bf0 */
GAPDATA full_data_629c08[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x52, 0x49, 0x47, 0x48, 0x54, 0x00, 0x00 };  /* 0x629c08 */
GAPDATA full_data_629c20[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x36, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629c20 */
GAPDATA full_data_629c48[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x629c48 */
GAPDATA full_data_629c58[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629c58 */
GAPDATA full_data_629c6c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x44, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629c6c */
GAPDATA full_data_629c80[28] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x34, 0x00, 0x00, 0x00 };  /* 0x629c80 */
GAPDATA full_data_629c9c[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x34, 0x00 };  /* 0x629c9c */
GAPDATA full_data_629cb0[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x42, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x4c, 0x45, 0x46, 0x54, 0x00, 0x00, 0x00 };  /* 0x629cb0 */
GAPDATA full_data_629cc8[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x42, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x52, 0x49, 0x47, 0x48, 0x54, 0x00, 0x00 };  /* 0x629cc8 */
GAPDATA full_data_629ce0[44] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x41, 0x6e, 0x69, 0x6d, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x42, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629ce0 */
GAPDATA full_data_629d0c[24] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x33, 0x5f, 0x42, 0x5f, 0x53, 0x74, 0x65, 0x70, 0x5f, 0x41, 0x75, 0x78, 0x00, 0x00, 0x00, 0x00 };  /* 0x629d0c */
GAPDATA full_data_629d24[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x42, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629d24 */
GAPDATA full_data_629d38[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x42, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629d38 */
GAPDATA full_data_629d4c[28] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00, 0x00, 0x00 };  /* 0x629d4c */
GAPDATA full_data_629d68[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x41, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x54, 0x4f, 0x50, 0x00, 0x00, 0x00, 0x00 };  /* 0x629d68 */
GAPDATA full_data_629d80[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x41, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x4c, 0x45, 0x46, 0x54, 0x00, 0x00, 0x00 };  /* 0x629d80 */
GAPDATA full_data_629d98[24] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x41, 0x5f, 0x53, 0x54, 0x45, 0x50, 0x5f, 0x52, 0x49, 0x47, 0x48, 0x54, 0x00, 0x00 };  /* 0x629d98 */
GAPDATA full_data_629db0[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x41, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629db0 */
GAPDATA full_data_629dd8[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x41, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629dd8 */
GAPDATA full_data_629dec[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x41, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629dec */
GAPDATA full_data_629e00[28] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x53, 0x63, 0x72, 0x69, 0x70, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x31, 0x00, 0x00, 0x00 };  /* 0x629e00 */
GAPDATA full_data_629e1c[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x33, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x00 };  /* 0x629e1c */
GAPDATA full_data_629e2c[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x33, 0x53, 0x74, 0x65, 0x70, 0x31, 0x00 };  /* 0x629e2c */
GAPDATA full_data_629e40[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x4f, 0x55, 0x54, 0x52, 0x4f, 0x00 };  /* 0x629e40 */
GAPDATA full_data_629e50[44] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x45, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629e50 */
GAPDATA full_data_629e7c[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x34, 0x5f, 0x45, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x629e7c */
GAPDATA full_data_629e8c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x45, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629e8c */
GAPDATA full_data_629ea0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x45, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629ea0 */
GAPDATA full_data_629eb4[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x34, 0x4f, 0x75, 0x74, 0x72, 0x6f, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x42, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629eb4 */
GAPDATA full_data_629edc[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x34, 0x5f, 0x42, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x629edc */
GAPDATA full_data_629eec[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x42, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629eec */
GAPDATA full_data_629f00[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x42, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629f00 */
GAPDATA full_data_629f14[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x34, 0x53, 0x74, 0x65, 0x70, 0x35, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x44, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629f14 */
GAPDATA full_data_629f3c[16] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x34, 0x5f, 0x44, 0x5f, 0x41, 0x75, 0x78, 0x00 };  /* 0x629f3c */
GAPDATA full_data_629f4c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x44, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x629f4c */
GAPDATA full_data_629f60[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x44, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629f60 */
GAPDATA full_data_629f74[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x34, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00 };  /* 0x629f74 */
GAPDATA full_data_629f88[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x34, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x00 };  /* 0x629f88 */
GAPDATA full_data_629f98[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x34, 0x53, 0x74, 0x65, 0x70, 0x34, 0x00 };  /* 0x629f98 */
GAPDATA full_data_629fac[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x34, 0x49, 0x6e, 0x74, 0x72, 0x6f, 0x00 };  /* 0x629fac */
GAPDATA full_data_629fc0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x47, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x629fc0 */
GAPDATA full_data_629fd4[44] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x2e, 0x73, 0x62, 0x66, 0x00, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x46, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x629fd4 */
GAPDATA full_data_62a000[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x46, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62a000 */
GAPDATA full_data_62a014[40] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x46, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x45, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a014 */
GAPDATA full_data_62a03c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x45, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62a03c */
GAPDATA full_data_62a050[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x45, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a050 */
GAPDATA full_data_62a064[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x53, 0x74, 0x65, 0x70, 0x35, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x43, 0x31, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00 };  /* 0x62a064 */
GAPDATA full_data_62a08c[24] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x35, 0x5f, 0x43, 0x31, 0x5f, 0x53, 0x74, 0x65, 0x70, 0x5f, 0x41, 0x75, 0x78, 0x00, 0x00, 0x00 };  /* 0x62a08c */
GAPDATA full_data_62a0a4[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x43, 0x31, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00 };  /* 0x62a0a4 */
GAPDATA full_data_62a0b8[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x43, 0x31, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00 };  /* 0x62a0b8 */
GAPDATA full_data_62a0cc[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x53, 0x74, 0x65, 0x70, 0x34, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x43, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a0cc */
GAPDATA full_data_62a0f4[24] = { 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x5f, 0x35, 0x5f, 0x43, 0x5f, 0x53, 0x74, 0x65, 0x70, 0x5f, 0x41, 0x75, 0x78, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a0f4 */
GAPDATA full_data_62a10c[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x43, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62a10c */
GAPDATA full_data_62a120[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x43, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a120 */
GAPDATA full_data_62a134[44] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x41, 0x6e, 0x69, 0x6d, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x44, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a134 */
GAPDATA full_data_62a160[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x44, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62a160 */
GAPDATA full_data_62a174[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x44, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a174 */
GAPDATA full_data_62a188[40] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x53, 0x74, 0x65, 0x70, 0x32, 0x00, 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x42, 0x5f, 0x44, 0x4f, 0x4e, 0x45, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a188 */
GAPDATA full_data_62a1b0[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x42, 0x5f, 0x52, 0x45, 0x4d, 0x49, 0x4e, 0x44, 0x00, 0x00 };  /* 0x62a1b0 */
GAPDATA full_data_62a1c4[20] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x42, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a1c4 */
GAPDATA full_data_62a1d8[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x53, 0x74, 0x65, 0x70, 0x33, 0x00 };  /* 0x62a1d8 */
GAPDATA full_data_62a1ec[16] = { 0x43, 0x48, 0x41, 0x50, 0x54, 0x45, 0x52, 0x5f, 0x35, 0x5f, 0x49, 0x4e, 0x54, 0x52, 0x4f, 0x00 };  /* 0x62a1ec */
GAPDATA full_data_62a1fc[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x53, 0x74, 0x65, 0x70, 0x31, 0x00 };  /* 0x62a1fc */
GAPDATA full_data_62a210[20] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x43, 0x68, 0x61, 0x70, 0x74, 0x65, 0x72, 0x35, 0x49, 0x6e, 0x74, 0x72, 0x6f, 0x00 };  /* 0x62a210 */
GAPDATA full_data_62a224[20] = { 0x54, 0x55, 0x54, 0x4f, 0x52, 0x49, 0x41, 0x4c, 0x5f, 0x4d, 0x41, 0x49, 0x4e, 0x5f, 0x4f, 0x55, 0x54, 0x52, 0x4f, 0x00 };  /* 0x62a224 */
GAPDATA full_data_62a238[16] = { 0x46, 0x54, 0x49, 0x6e, 0x69, 0x74, 0x4d, 0x61, 0x69, 0x6e, 0x4f, 0x75, 0x74, 0x72, 0x6f, 0x00 };  /* 0x62a238 */
GAPDATA full_data_62a248[60] = { 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x5c, 0x43, 0x48, 0x4f, 0x4f, 0x53, 0x45, 0x5f, 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x44, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x41, 0x6c, 0x63, 0x68, 0x65, 0x6d, 0x69, 0x73, 0x74, 0x72, 0x79, 0x31, 0x00, 0x00 };  /* 0x62a248 */
GAPDATA full_data_62a284[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x44, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x41, 0x6c, 0x63, 0x68, 0x65, 0x6d, 0x69, 0x73, 0x74, 0x72, 0x79, 0x32, 0x00, 0x00 };  /* 0x62a284 */
GAPDATA full_data_62a2a4[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x44, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x41, 0x6c, 0x63, 0x68, 0x65, 0x6d, 0x69, 0x73, 0x74, 0x72, 0x79, 0x33, 0x00, 0x00 };  /* 0x62a2a4 */
GAPDATA full_data_62a2c4[24] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x44, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x73, 0x6b, 0x69, 0x6c, 0x6c, 0x00 };  /* 0x62a2c4 */
GAPDATA full_data_62a2dc[32] = { 0x5f, 0x41, 0x44, 0x44, 0x4f, 0x4e, 0x5c, 0x41, 0x4f, 0x5f, 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x5f, 0x57, 0x49, 0x4d, 0x50, 0x45, 0x4c, 0x5f, 0x41, 0x61, 0x2e, 0x62, 0x61, 0x66, 0x00, 0x00 };  /* 0x62a2dc */
GAPDATA full_data_62a2fc[20] = { 0x41, 0x4f, 0x5f, 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x5f, 0x57, 0x49, 0x4d, 0x50, 0x45, 0x4c, 0x5f, 0x41, 0x61, 0x00 };  /* 0x62a2fc */
GAPDATA full_data_62a310[24] = { 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x5f, 0x44, 0x52, 0x45, 0x49, 0x46, 0x41, 0x43, 0x48, 0x5f, 0x52, 0x4f, 0x48, 0x52, 0x5f, 0x41, 0x61, 0x00 };  /* 0x62a310 */
GAPDATA full_data_62a328[24] = { 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x5f, 0x44, 0x4f, 0x50, 0x50, 0x45, 0x4c, 0x5f, 0x52, 0x4f, 0x48, 0x52, 0x5f, 0x41, 0x61, 0x00, 0x00, 0x00 };  /* 0x62a328 */
GAPDATA full_data_62a340[16] = { 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x5f, 0x52, 0x4f, 0x48, 0x52, 0x5f, 0x41, 0x61, 0x00, 0x00 };  /* 0x62a340 */
GAPDATA full_data_62a350[20] = { 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x5f, 0x55, 0x4e, 0x54, 0x45, 0x52, 0x42, 0x41, 0x55, 0x5f, 0x41, 0x61, 0x00, 0x00 };  /* 0x62a350 */
GAPDATA full_data_62a364[8] = { 0x25, 0x54, 0x20, 0x25, 0x61, 0x00, 0x00, 0x00 };  /* 0x62a364 */
GAPDATA full_data_62a36c[32] = { 0x4c, 0x4f, 0x43, 0x41, 0x54, 0x49, 0x4f, 0x4e, 0x53, 0x5c, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5c, 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x31, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00 };  /* 0x62a36c */
GAPDATA full_data_62a38c[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x64, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x63, 0x61, 0x6e, 0x6e, 0x6f, 0x6e, 0x5f, 0x72, 0x65, 0x73, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a38c */
GAPDATA full_data_62a3ac[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x64, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x63, 0x61, 0x6e, 0x6e, 0x6f, 0x6e, 0x5f, 0x74, 0x6f, 0x70, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a3ac */
GAPDATA full_data_62a3cc[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x64, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x63, 0x61, 0x6e, 0x6e, 0x6f, 0x6e, 0x5f, 0x62, 0x6f, 0x74, 0x74, 0x6f, 0x6d, 0x00 };  /* 0x62a3cc */
GAPDATA full_data_62a3ec[12] = { 0x4b, 0x4f, 0x4d, 0x4d, 0x45, 0x4e, 0x54, 0x41, 0x52, 0x45, 0x00, 0x00 };  /* 0x62a3ec */
GAPDATA full_data_62a3f8[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x44, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x6b, 0x61, 0x62, 0x69, 0x6e, 0x65, 0x74, 0x74, 0x5f, 0x73, 0x70, 0x79, 0x00, 0x00 };  /* 0x62a3f8 */
GAPDATA full_data_62a418[28] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x44, 0x6f, 0x6d, 0x69, 0x7a, 0x69, 0x6c, 0x5c, 0x6b, 0x61, 0x62, 0x69, 0x6e, 0x65, 0x74, 0x74, 0x00, 0x00 };  /* 0x62a418 */
GAPDATA full_data_62a434[28] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x41, 0x4c, 0x43, 0x48, 0x49, 0x4d, 0x49, 0x53, 0x54, 0x45, 0x4e, 0x56, 0x45, 0x52, 0x54, 0x52, 0x41, 0x47, 0x00, 0x00 };  /* 0x62a434 */
GAPDATA full_data_62a474[20] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x43, 0x4f, 0x4e, 0x54, 0x52, 0x41, 0x43, 0x54, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a474 */
GAPDATA full_data_62a488[32] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x50, 0x52, 0x4f, 0x44, 0x55, 0x4b, 0x54, 0x49, 0x4f, 0x4e, 0x5f, 0x41, 0x4c, 0x43, 0x48, 0x49, 0x4d, 0x49, 0x53, 0x54, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a488 */
GAPDATA full_data_62a4a8[20] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x52, 0x45, 0x5a, 0x45, 0x50, 0x54, 0x42, 0x55, 0x43, 0x48, 0x00, 0x00 };  /* 0x62a4a8 */
GAPDATA full_data_62a4bc[16] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x4b, 0x41, 0x4e, 0x4f, 0x4e, 0x45, 0x00, 0x00 };  /* 0x62a4bc */
GAPDATA full_data_62a4cc[28] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x42, 0x45, 0x57, 0x45, 0x49, 0x53, 0x45, 0x5f, 0x46, 0x41, 0x45, 0x4c, 0x53, 0x43, 0x48, 0x45, 0x4e, 0x00, 0x00, 0x00 };  /* 0x62a4cc */
GAPDATA full_data_62a4e8[32] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x53, 0x43, 0x48, 0x4c, 0x45, 0x49, 0x43, 0x48, 0x45, 0x4e, 0x44, 0x45, 0x5f, 0x4c, 0x41, 0x45, 0x48, 0x4d, 0x55, 0x4e, 0x47, 0x00, 0x00, 0x00 };  /* 0x62a4e8 */
GAPDATA full_data_62a508[28] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x53, 0x50, 0x49, 0x4f, 0x4e, 0x45, 0x5f, 0x41, 0x55, 0x53, 0x53, 0x43, 0x48, 0x41, 0x4c, 0x54, 0x45, 0x4e, 0x00, 0x00 };  /* 0x62a508 */
GAPDATA full_data_62a524[20] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x56, 0x45, 0x52, 0x47, 0x49, 0x46, 0x54, 0x45, 0x4e, 0x00, 0x00, 0x00 };  /* 0x62a524 */
GAPDATA full_data_62a53c[8] = { 0x25, 0x69, 0x20, 0x25, 0x25, 0x00, 0x00, 0x00 };  /* 0x62a53c */
GAPDATA full_data_62a544[8] = { 0x41, 0x44, 0x20, 0x25, 0x69, 0x00, 0x00, 0x00 };  /* 0x62a544 */
GAPDATA full_data_62a54c[24] = { 0x24, 0x37, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x24, 0x30, 0x3c, 0x24, 0x34, 0x3e, 0x24, 0x4c, 0x25, 0x73, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00 };  /* 0x62a54c */
char ai_CharNameFmt[8] = "%1N3";  /* 0x62a564, the text format of a character name by its id (txt_Format); used by lc only, owner open (before lobby's data) */
char ai_FmtLabel[4] = "%s:";  /* 0x62a56c, the "%s:" label format of ai_TechEffectsWindow's skill captions (txt_Format) */
GAPDATA full_data_62a570[8] = { 0x25, 0x73, 0x2c, 0x20, 0x25, 0x73, 0x00, 0x00 };  /* 0x62a570 */
GAPDATA full_data_62a578[8] = { 0x24, 0x43, 0x24, 0x5a, 0x25, 0x73, 0x00, 0x00 };  /* 0x62a578 */
GAPDATA full_data_62a580[28] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x47, 0x72, 0x61, 0x76, 0x65, 0x73, 0x00, 0x00, 0x00 };  /* 0x62a580 */
GAPDATA full_data_62a59c[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x52, 0x65, 0x6e, 0x74, 0x41, 0x47, 0x72, 0x61, 0x76, 0x65, 0x32, 0x00, 0x00 };  /* 0x62a59c */
GAPDATA full_data_62a5bc[28] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x00 };  /* 0x62a5bc */
GAPDATA full_data_62a5d8[32] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x52, 0x65, 0x6e, 0x74, 0x41, 0x47, 0x72, 0x61, 0x76, 0x65, 0x00, 0x00, 0x00 };  /* 0x62a5d8 */
GAPDATA full_data_62a5f8[52] = { 0x4c, 0x4f, 0x43, 0x41, 0x54, 0x49, 0x4f, 0x4e, 0x53, 0x5c, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5c, 0x47, 0x65, 0x68, 0x69, 0x6c, 0x66, 0x65, 0x6e, 0x5f, 0x65, 0x72, 0x73, 0x63, 0x68, 0x61, 0x66, 0x66, 0x65, 0x6e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x2e, 0x65, 0x73, 0x63, 0x00 };  /* 0x62a5f8 */
GAPDATA full_data_62a62c[8] = { 0x73, 0x70, 0x5f, 0x25, 0x75, 0x00, 0x00, 0x00 };  /* 0x62a62c */
GAPDATA full_data_62a64c[24] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x46, 0x6f, 0x67, 0x00, 0x00 };  /* 0x62a64c */
GAPDATA full_data_62a664[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x31, 0x34, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a664 */
GAPDATA full_data_62a680[28] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x53, 0x65, 0x61, 0x6e, 0x63, 0x65, 0x00, 0x00, 0x00 };  /* 0x62a680 */
GAPDATA full_data_62a69c[28] = { 0x5f, 0x53, 0x45, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x46, 0x4f, 0x52, 0x4d, 0x45, 0x4c, 0x5f, 0x50, 0x41, 0x52, 0x54, 0x5f, 0x25, 0x69, 0x5f, 0x30, 0x25, 0x69, 0x00, 0x00 };  /* 0x62a69c */
GAPDATA full_data_62a6b8[36] = { 0x4c, 0x4f, 0x43, 0x41, 0x54, 0x49, 0x4f, 0x4e, 0x53, 0x5c, 0x4b, 0x52, 0x59, 0x50, 0x54, 0x41, 0x5c, 0x53, 0x45, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x4b, 0x52, 0x59, 0x50, 0x54, 0x41, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00 };  /* 0x62a6b8 */
GAPDATA full_data_62a6dc[28] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x53, 0x65, 0x61, 0x6e, 0x63, 0x65, 0x32, 0x00, 0x00 };  /* 0x62a6dc */
GAPDATA full_data_62a6f8[20] = { 0x47, 0x45, 0x49, 0x53, 0x54, 0x5f, 0x4b, 0x4f, 0x4d, 0x4d, 0x54, 0x5f, 0x4e, 0x49, 0x43, 0x48, 0x54, 0x00, 0x00, 0x00 };  /* 0x62a6f8 */
GAPDATA full_data_62a70c[12] = { 0x47, 0x45, 0x49, 0x53, 0x54, 0x5f, 0x4b, 0x4f, 0x4d, 0x4d, 0x54, 0x00 };  /* 0x62a70c */
GAPDATA full_data_62a718[40] = { 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2f, 0x66, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x2f, 0x73, 0x63, 0x68, 0x6c, 0x61, 0x67, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00 };  /* 0x62a718 */
GAPDATA full_data_62a740[20] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x47, 0x52, 0x41, 0x42, 0x5f, 0x4d, 0x49, 0x45, 0x54, 0x45, 0x4e, 0x00 };  /* 0x62a740 */
GAPDATA full_data_62a754[20] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x4c, 0x49, 0x45, 0x47, 0x45, 0x50, 0x4c, 0x41, 0x4e, 0x00, 0x00, 0x00 };  /* 0x62a754 */
GAPDATA full_data_62a768[24] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x56, 0x45, 0x52, 0x53, 0x49, 0x43, 0x48, 0x45, 0x52, 0x55, 0x4e, 0x47, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a768 */
GAPDATA full_data_62a780[28] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x47, 0x45, 0x48, 0x49, 0x4c, 0x46, 0x45, 0x4e, 0x5f, 0x45, 0x52, 0x53, 0x43, 0x48, 0x41, 0x46, 0x46, 0x45, 0x4e, 0x00 };  /* 0x62a780 */
GAPDATA full_data_62a79c[24] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x47, 0x52, 0x41, 0x45, 0x42, 0x45, 0x52, 0x5f, 0x50, 0x46, 0x4c, 0x45, 0x47, 0x45, 0x4e, 0x00 };  /* 0x62a79c */
GAPDATA full_data_62a7b4[24] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x5a, 0x55, 0x54, 0x41, 0x54, 0x45, 0x4e, 0x5f, 0x53, 0x55, 0x43, 0x48, 0x45, 0x4e, 0x00, 0x00 };  /* 0x62a7b4 */
GAPDATA full_data_62a7cc[20] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x48, 0x45, 0x49, 0x4d, 0x53, 0x55, 0x43, 0x48, 0x55, 0x4e, 0x47, 0x00 };  /* 0x62a7cc */
GAPDATA full_data_62a7e0[24] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x41, 0x4c, 0x54, 0x45, 0x52, 0x4e, 0x5f, 0x4c, 0x41, 0x53, 0x53, 0x45, 0x4e, 0x00, 0x00, 0x00 };  /* 0x62a7e0 */
GAPDATA full_data_62a7f8[16] = { 0x6f, 0x62, 0x5f, 0x47, 0x45, 0x48, 0x49, 0x4c, 0x46, 0x45, 0x4e, 0x42, 0x55, 0x43, 0x48, 0x00 };  /* 0x62a7f8 */
GAPDATA full_data_62a808[16] = { 0x6f, 0x62, 0x5f, 0x4d, 0x45, 0x49, 0x53, 0x54, 0x45, 0x52, 0x42, 0x52, 0x49, 0x45, 0x46, 0x00 };  /* 0x62a808 */
GAPDATA full_data_62a818[36] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x50, 0x52, 0x4f, 0x44, 0x55, 0x4b, 0x54, 0x49, 0x4f, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x53, 0x4b, 0x41, 0x50, 0x45, 0x4c, 0x4c, 0x45, 0x00 };  /* 0x62a818 */
GAPDATA full_data_62a83c[16] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x53, 0x45, 0x41, 0x4e, 0x43, 0x45, 0x00, 0x00 };  /* 0x62a83c */
GAPDATA full_data_62a84c[36] = { 0x4c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x46, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x55, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x57, 0x6f, 0x72, 0x6b, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a84c */
GAPDATA full_data_62a870[28] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x4e, 0x45, 0x42, 0x45, 0x4c, 0x5f, 0x44, 0x45, 0x53, 0x5f, 0x47, 0x52, 0x41, 0x55, 0x45, 0x4e, 0x53, 0x00, 0x00, 0x00 };  /* 0x62a870 */
GAPDATA full_data_62a88c[28] = { 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x5f, 0x47, 0x45, 0x48, 0x49, 0x4c, 0x46, 0x45, 0x4e, 0x5f, 0x56, 0x45, 0x52, 0x42, 0x45, 0x53, 0x53, 0x45, 0x52, 0x4e, 0x00 };  /* 0x62a88c */
GAPDATA full_data_62a8a8[36] = { 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x66, 0x72, 0x69, 0x65, 0x64, 0x68, 0x6f, 0x66, 0x5c, 0x67, 0x65, 0x73, 0x70, 0x72, 0x61, 0x65, 0x63, 0x68, 0x65, 0x2e, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00 };  /* 0x62a8a8 */
GAPDATA full_data_62a8d8[28] = { 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5c, 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5f, 0x47, 0x65, 0x62, 0x61, 0x65, 0x75, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a8d8 */
GAPDATA full_data_62a8f4[4] = { 0x56, 0x00, 0x00, 0x00 };  /* 0x62a8f4 */
GAPDATA full_data_62a8f8[4] = { 0x49, 0x49, 0x49, 0x00 };  /* 0x62a8f8 */
GAPDATA full_data_62a8fc[2] = { 0x56, 0x49 };  /* 0x62a8fc */
GAPDATA full_data_62a8fe[2] = { 0 };  /* 0x62a8fe */
GAPDATA full_data_62a900[2] = { 0x49, 0x56 };  /* 0x62a900 */
GAPDATA full_data_62a902[2] = { 0 };  /* 0x62a902 */
GAPDATA full_data_62a904[2] = { 0x49, 0x49 };  /* 0x62a904 */
GAPDATA full_data_62a906[2] = { 0 };  /* 0x62a906 */
GAPDATA full_data_62a908[20] = { 0x49, 0x00, 0x00, 0x00, 0x25, 0x73, 0x24, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x25, 0x69, 0x20, 0x25, 0x73, 0x24, 0x4e, 0x00 };  /* 0x62a908 */
GAPDATA full_data_62a91c[36] = { 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5c, 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5f, 0x4f, 0x62, 0x6a, 0x65, 0x6b, 0x74, 0x65, 0x5f, 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x73, 0x67, 0x75, 0x74, 0x00, 0x00 };  /* 0x62a91c */
GAPDATA full_data_62a940[32] = { 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5c, 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5f, 0x4f, 0x62, 0x6a, 0x65, 0x6b, 0x74, 0x65, 0x5f, 0x57, 0x61, 0x66, 0x66, 0x65, 0x6e, 0x00, 0x00 };  /* 0x62a940 */
GAPDATA full_data_62a960[12] = { 0x25, 0x69, 0x25, 0x25, 0x20, 0x25, 0x73, 0x24, 0x4e, 0x00, 0x00, 0x00 };  /* 0x62a960 */
GAPDATA full_data_62a96c[12] = { 0x2b, 0x25, 0x69, 0x20, 0x25, 0x73, 0x24, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a96c */
GAPDATA full_data_62a978[12] = { 0x2b, 0x25, 0x61, 0x20, 0x25, 0x73, 0x24, 0x4e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a978 */
GAPDATA full_data_62a984[36] = { 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5c, 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5f, 0x4f, 0x62, 0x6a, 0x65, 0x6b, 0x74, 0x65, 0x5f, 0x55, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x73, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a984 */
GAPDATA full_data_62a9a8[12] = { 0x24, 0x5a, 0x24, 0x5b, 0x25, 0x73, 0x24, 0x5d, 0x00, 0x00, 0x00, 0x00 };  /* 0x62a9a8 */
GAPDATA full_data_62a9b4[24] = { 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5c, 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5f, 0x43, 0x6f, 0x6e, 0x74, 0x61, 0x63, 0x74, 0x00 };  /* 0x62a9b4 */
GAPDATA full_data_62a9cc[24] = { 0x5f, 0x48, 0x49, 0x4c, 0x46, 0x45, 0x5f, 0x25, 0x73, 0x2b, 0x30, 0x00, 0x24, 0x33, 0x3e, 0x25, 0x31, 0x4e, 0x31, 0x24, 0x41, 0x00, 0x00, 0x00 };  /* 0x62a9cc */
GAPDATA full_data_62a9e4[8] = { 0x24, 0x33, 0x3e, 0x25, 0x73, 0x24, 0x41, 0x00 };  /* 0x62a9e4 */
GAPDATA full_data_62a9ec[12] = { 0x24, 0x5a, 0x24, 0x5b, 0x25, 0x31, 0x4e, 0x33, 0x24, 0x5d, 0x00, 0x00 };  /* 0x62a9ec */
GAPDATA full_data_62a9f8[16] = { 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5c, 0x54, 0x6f, 0x6f, 0x6c, 0x54, 0x69, 0x70, 0x5f };  /* 0x62a9f8 */
GAPDATA full_data_62aa14[12] = { 0x25, 0x73, 0x20, 0x3d, 0x20, 0x25, 0x47, 0x2c, 0x20, 0x25, 0x73, 0x00 };  /* 0x62aa14 */
GAPDATA full_data_62aa20[24] = { 0x25, 0x73, 0x20, 0x3d, 0x20, 0x25, 0x47, 0x00, 0x25, 0x31, 0x73, 0x00, 0x25, 0x32, 0x4e, 0x38, 0x7e, 0x20, 0x25, 0x31, 0x47, 0x00, 0x00, 0x00 };  /* 0x62aa20 */
GAPDATA full_data_62aa38[20] = { 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x67, 0x65, 0x62, 0x5f, 0x68, 0x6f, 0x74, 0x6b, 0x65, 0x79, 0x73, 0x00 };  /* 0x62aa38 */
GAPDATA full_data_62aa4c[12] = { 0x66, 0x33, 0x5f, 0x73, 0x76, 0x5f, 0x74, 0x3a, 0x64, 0x00, 0x00, 0x00 };  /* 0x62aa4c */
GAPDATA full_data_62aa58[36] = { 0x67, 0x61, 0x6d, 0x65, 0x64, 0x61, 0x74, 0x61, 0x2f, 0x73, 0x63, 0x72, 0x65, 0x65, 0x6e, 0x73, 0x68, 0x6f, 0x74, 0x73, 0x2f, 0x67, 0x69, 0x6c, 0x64, 0x65, 0x25, 0x30, 0x34, 0x69, 0x2e, 0x62, 0x6d, 0x70, 0x00, 0x00 };  /* 0x62aa58 */
GAPDATA full_data_62aae4[4] = { 0 };  /* 0x62aae4, the alignment after lobby's chat format texts, before ai_BuildNames */
GAPDATA full_data_62af9c[4] = { 0 };  /* 0x62af9c */
GAPDATA full_data_62b098[12] = { 0x25, 0x73, 0x7e, 0x20, 0x25, 0x6c, 0x69, 0x20, 0x25, 0x73, 0x00, 0x00 };  /* 0x62b098 */
GAPDATA full_data_62b3d8[12] = { 0x54, 0x52, 0x41, 0x4e, 0x53, 0x50, 0x4f, 0x52, 0x54, 0x00, 0x00, 0x00 };  /* 0x62b3d8 */
GAPDATA full_data_62c274[16] = { 0x4d, 0x69, 0x73, 0x63, 0x5c, 0x4c, 0x61, 0x6e, 0x64, 0x6b, 0x61, 0x72, 0x74, 0x65, 0x32, 0x00 };  /* 0x62c274 */
GAPDATA full_data_62c284[20] = { 0x68, 0x70, 0x5f, 0x41, 0x64, 0x64, 0x50, 0x72, 0x6f, 0x64, 0x53, 0x65, 0x74, 0x74, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00 };  /* 0x62c284 */
GAPDATA full_data_62c298[20] = { 0x68, 0x70, 0x5f, 0x4c, 0x6f, 0x61, 0x64, 0x50, 0x72, 0x6f, 0x64, 0x53, 0x65, 0x74, 0x74, 0x69, 0x6e, 0x67, 0x00, 0x00 };  /* 0x62c298 */
GAPDATA full_data_62c2c0[28] = { 0x5f, 0x4b, 0x41, 0x54, 0x41, 0x53, 0x54, 0x52, 0x4f, 0x50, 0x48, 0x45, 0x5f, 0x50, 0x45, 0x53, 0x54, 0x5f, 0x48, 0x53, 0x5f, 0x30, 0x25, 0x69, 0x00, 0x00, 0x00, 0x00 };  /* 0x62c2c0 */
GAPDATA full_data_62c2dc[24] = { 0x5f, 0x4b, 0x41, 0x54, 0x41, 0x53, 0x54, 0x52, 0x4f, 0x50, 0x48, 0x45, 0x5f, 0x50, 0x45, 0x53, 0x54, 0x5f, 0x48, 0x53, 0x5f, 0x25, 0x69, 0x00 };  /* 0x62c2dc */
GAPDATA full_data_62c2f4[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x5a, 0x49, 0x47, 0x45, 0x55, 0x4e, 0x45, 0x52, 0x5f, 0x30, 0x35, 0x00, 0x00, 0x00, 0x00 };  /* 0x62c2f4 */
GAPDATA full_data_62c310[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x5a, 0x49, 0x47, 0x45, 0x55, 0x4e, 0x45, 0x52, 0x5f, 0x30, 0x36, 0x00, 0x00, 0x00, 0x00 };  /* 0x62c310 */
GAPDATA full_data_62c32c[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00 };  /* 0x62c32c */
GAPDATA full_data_62c348[28] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x46, 0x52, 0x49, 0x45, 0x44, 0x48, 0x4f, 0x46, 0x5f, 0x31, 0x31, 0x00, 0x00, 0x00, 0x00 };  /* 0x62c348 */
GAPDATA full_data_62c3b8[48] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x33, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x31, 0x00 };  /* 0x62c3b8 */
GAPDATA full_data_62c3e8[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x30, 0x34, 0x00 };  /* 0x62c3e8 */
GAPDATA full_data_62c400[24] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x44, 0x4f, 0x4d, 0x49, 0x5a, 0x49, 0x4c, 0x5f, 0x31, 0x32, 0x00 };  /* 0x62c400 */
GAPDATA full_data_62c41c[4] = { 0x00, 0x00, 0x00, 0x00 };  /* 0x62c41c */
int ai_DomizilCount = 0x42;  /* 0x62cb58, the number of 0x1c-byte entries of the Domizil table ai_DomizilTable (0x62c420) */
GAPDATA full_data_62cb5c[88] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x01, 0x01, 0xff, 0x01, 0xff, 0xff, 0x00, 0x00, 0x01, 0x02, 0x00, 0xff, 0x01, 0xff, 0xff, 0x01, 0xff, 0x00, 0x03, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0xff, 0x01, 0x04, 0x00, 0xff, 0x00, 0x00, 0xff, 0x01, 0x00, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 };  /* 0x62cb5c, the rest of the 92-byte block at 0x62cb58 (no reader) */
GAPDATA full_data_62cbb4[184] = { 0x68, 0x61, 0x6e, 0x64, 0x77, 0x65, 0x72, 0x6b, 0x65, 0x72, 0x32, 0x5f, 0x4d, 0x41, 0x4e, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x70, 0x61, 0x79, 0x5f, 0x6d, 0x65, 0x69, 0x73, 0x74, 0x65, 0x72, 0x00, 0x70, 0x61, 0x79, 0x5f, 0x77, 0x6f, 0x72, 0x6b, 0x65, 0x72, 0x00, 0x00, 0x24, 0x38, 0x41, 0x00, 0x42, 0x75, 0x63, 0x68, 0x5c, 0x50, 0x65, 0x72, 0x73, 0x6f, 0x6e, 0x61, 0x6c, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x31, 0x5f, 0x44, 0x69, 0x65, 0x62, 0x65, 0x73, 0x67, 0x69, 0x6c, 0x64, 0x65, 0x00, 0x42, 0x75, 0x63, 0x68, 0x5c, 0x50, 0x65, 0x72, 0x73, 0x6f, 0x6e, 0x61, 0x6c, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x31, 0x00, 0x42, 0x75, 0x63, 0x68, 0x5c, 0x50, 0x65, 0x72, 0x73, 0x6f, 0x6e, 0x61, 0x6c, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x32, 0x00, 0x42, 0x75, 0x63, 0x68, 0x5c, 0x50, 0x65, 0x72, 0x73, 0x6f, 0x6e, 0x61, 0x6c, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x33, 0x00, 0x42, 0x75, 0x63, 0x68, 0x5c, 0x50, 0x65, 0x72, 0x73, 0x6f, 0x6e, 0x61, 0x6c, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x34, 0x00, 0x24, 0x36, 0x41, 0x00, 0x42, 0x75, 0x63, 0x68, 0x5c, 0x53, 0x6f, 0x65, 0x6c, 0x64, 0x6e, 0x65, 0x72, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x31, 0x00 };  /* 0x62cbb4 */
GAPDATA full_data_62cc6c[40] = { 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x5c, 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x5f, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x6e, 0x65, 0x75, 0x65, 0x72, 0x5f, 0x4b, 0x61, 0x72, 0x72, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62cc6c */
GAPDATA full_data_62cc94[84] = { 0x6f, 0x62, 0x5f, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x70, 0x6f, 0x72, 0x74, 0x46, 0x72, 0x6f, 0x6d, 0x4c, 0x61, 0x67, 0x65, 0x72, 0x28, 0x29, 0x3a, 0x20, 0x43, 0x6f, 0x75, 0x6c, 0x64, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x66, 0x69, 0x6e, 0x64, 0x20, 0x6c, 0x61, 0x67, 0x65, 0x72, 0x20, 0x62, 0x79, 0x20, 0x69, 0x64, 0x2e, 0x2e, 0x2e, 0x00, 0x00, 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x69, 0x6e, 0x66, 0x6f, 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5f, 0x6d, 0x61, 0x72, 0x6b, 0x74, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x00, 0x00 };  /* 0x62cc94 */
GAPDATA full_data_62cce8[20] = { 0x25, 0x73, 0x62, 0x6d, 0x70, 0x5c, 0x6d, 0x61, 0x70, 0x73, 0x5c, 0x25, 0x73, 0x2e, 0x62, 0x6d, 0x70, 0x00, 0x00, 0x00 };  /* 0x62cce8 */
GAPDATA full_data_62ccfc[76] = { 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x5c, 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x5f, 0x4c, 0x61, 0x67, 0x65, 0x72, 0x5f, 0x4e, 0x65, 0x75, 0x65, 0x72, 0x5f, 0x53, 0x6c, 0x6f, 0x74, 0x00, 0x00, 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x5c, 0x48, 0x61, 0x6e, 0x64, 0x65, 0x6c, 0x5f, 0x4c, 0x61, 0x67, 0x65, 0x72, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x65, 0x6e, 0x00, 0x53, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x54, 0x61, 0x6c, 0x65, 0x6e, 0x74, 0x65, 0x00 };  /* 0x62ccfc */
GAPDATA full_data_62cd48[52] = { 0x6d, 0x69, 0x73, 0x63, 0x5c, 0x70, 0x65, 0x72, 0x67, 0x61, 0x5f, 0x72, 0x6f, 0x6c, 0x6c, 0x65, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x77, 0x6f, 0x68, 0x6e, 0x73, 0x69, 0x74, 0x7a, 0x5c, 0x73, 0x70, 0x79, 0x00, 0x00, 0x62, 0x65, 0x61, 0x74, 0x75, 0x70, 0x00, 0x00 };  /* 0x62cd48 */
GAPDATA full_data_62cd7c[32] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x42, 0x65, 0x77, 0x65, 0x69, 0x73, 0x65, 0x5f, 0x44, 0x65, 0x74, 0x61, 0x69, 0x6c, 0x73, 0x00, 0x00, 0x00, 0x00 };  /* 0x62cd7c */
GAPDATA full_data_62cd9c[12] = { 0x25, 0x69, 0x20, 0x28, 0x25, 0x69, 0x29, 0x20, 0x25, 0x73, 0x00, 0x00 };  /* 0x62cd9c */
GAPDATA full_data_62cda8[616] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x42, 0x65, 0x77, 0x65, 0x69, 0x73, 0x65, 0x5f, 0x53, 0x69, 0x63, 0x68, 0x74, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x69, 0x73, 0x63, 0x5c, 0x70, 0x61, 0x6d, 0x70, 0x68, 0x6c, 0x65, 0x74, 0x5f, 0x62, 0x72, 0x65, 0x74, 0x74, 0x00, 0x67, 0x65, 0x64, 0x69, 0x63, 0x68, 0x74, 0x65, 0x5c, 0x25, 0x73, 0x2e, 0x6d, 0x70, 0x33, 0x00, 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x62, 0x61, 0x72, 0x64, 0x65, 0x32, 0x00, 0x00, 0x47, 0x65, 0x64, 0x69, 0x63, 0x68, 0x74, 0x65, 0x5f, 0x41, 0x6e, 0x6e, 0x6f, 0x75, 0x6e, 0x63, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x5c, 0x25, 0x73, 0x2e, 0x6d, 0x70, 0x33, 0x00, 0x00, 0x00, 0x00, 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x62, 0x61, 0x72, 0x64, 0x65, 0x00, 0x00, 0x00, 0x24, 0x5a, 0x5a, 0x75, 0x6d, 0x20, 0x46, 0x65, 0x73, 0x74, 0x20, 0x65, 0x69, 0x6e, 0x6c, 0x61, 0x64, 0x65, 0x6e, 0x00, 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x77, 0x6f, 0x68, 0x6e, 0x73, 0x69, 0x74, 0x7a, 0x5c, 0x66, 0x65, 0x73, 0x74, 0x00, 0x24, 0x4c, 0x25, 0x69, 0x6e, 0x5b, 0x25, 0x73, 0x5d, 0x00, 0x00, 0x00, 0x24, 0x4c, 0x25, 0x69, 0x61, 0x5b, 0x25, 0x73, 0x5d, 0x24, 0x41, 0x00, 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x77, 0x6f, 0x68, 0x6e, 0x73, 0x69, 0x74, 0x7a, 0x5c, 0x66, 0x65, 0x73, 0x74, 0x32, 0x00, 0x00, 0x00, 0x00, 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x47, 0x65, 0x62, 0x61, 0x65, 0x75, 0x64, 0x65, 0x5f, 0x56, 0x65, 0x72, 0x6b, 0x61, 0x75, 0x66, 0x65, 0x6e, 0x5f, 0x70, 0x65, 0x72, 0x67, 0x61, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00, 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x47, 0x65, 0x62, 0x61, 0x65, 0x75, 0x64, 0x65, 0x5f, 0x52, 0x65, 0x6e, 0x6f, 0x76, 0x69, 0x65, 0x72, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x72, 0x6f, 0x6f, 0x6d, 0x5f, 0x75, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00, 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x47, 0x65, 0x62, 0x61, 0x65, 0x75, 0x64, 0x65, 0x5f, 0x52, 0x61, 0x75, 0x6d, 0x5f, 0x45, 0x72, 0x77, 0x65, 0x69, 0x74, 0x65, 0x72, 0x6e, 0x00, 0x00, 0x00, 0x24, 0x5a, 0x24, 0x5b, 0x25, 0x73, 0x24, 0x5d, 0x24, 0x4e, 0x25, 0x73, 0x00, 0x00, 0x00, 0x00, 0x74, 0x65, 0x63, 0x68, 0x74, 0x72, 0x65, 0x65, 0x5c, 0x61, 0x75, 0x73, 0x77, 0x69, 0x72, 0x6b, 0x75, 0x6e, 0x67, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x47, 0x65, 0x62, 0x61, 0x65, 0x75, 0x64, 0x65, 0x5f, 0x42, 0x72, 0x61, 0x6e, 0x64, 0x5f, 0x6c, 0x6f, 0x65, 0x73, 0x63, 0x68, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x67, 0x65, 0x62, 0x61, 0x65, 0x75, 0x64, 0x65, 0x5f, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x65, 0x6e, 0x5f, 0x70, 0x65, 0x72, 0x67, 0x61, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00, 0x00, 0x6f, 0x62, 0x5f, 0x48, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x42, 0x65, 0x70, 0x66, 0x6c, 0x61, 0x6e, 0x7a, 0x75, 0x6e, 0x67, 0x28, 0x29, 0x3a, 0x20, 0x43, 0x6f, 0x75, 0x6c, 0x64, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x6c, 0x6f, 0x61, 0x64, 0x20, 0x33, 0x44, 0x2d, 0x4f, 0x62, 0x6a, 0x65, 0x6b, 0x74, 0x2d, 0x72, 0x6f, 0x75, 0x70, 0x20, 0x27, 0x25, 0x73, 0x27, 0x2e, 0x2e, 0x2e, 0x00, 0x00, 0x00, 0x25, 0x73, 0x76, 0x65, 0x67, 0x65, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x2a, 0x70, 0x66, 0x6c, 0x5f, 0x25, 0x73, 0x5f, 0x30, 0x34, 0x2e, 0x6f, 0x67, 0x72, 0x00, 0x4d, 0x69, 0x73, 0x63, 0x5c, 0x50, 0x6c, 0x61, 0x6e, 0x74, 0x42, 0x61, 0x72, 0x00, 0x00, 0x00, 0x70, 0x61, 0x72, 0x74, 0x79, 0x00, 0x00, 0x00, 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x67, 0x65, 0x6c, 0x61, 0x67, 0x65, 0x00, 0x00, 0x25, 0x73, 0x7e, 0x20, 0x25, 0x6c, 0x69, 0x20, 0x53, 0x74, 0xfc, 0x63, 0x6b, 0x00, 0x00, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x48, 0x53, 0x5f, 0x33, 0x37, 0x00, 0x00 };  /* 0x62cda8 */
GAPDATA full_data_62d01d[279] = { 0x00, 0x00, 0x00, 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x69, 0x6e, 0x76, 0x65, 0x6e, 0x74, 0x6f, 0x72, 0x79, 0x5f, 0x32, 0x00, 0x00, 0x00, 0x24, 0x43, 0x25, 0x31, 0x4e, 0x32, 0x20, 0x25, 0x69, 0x25, 0x25, 0x00, 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x74, 0x72, 0x61, 0x69, 0x6e, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5c, 0x64, 0x69, 0x65, 0x62, 0x65, 0x73, 0x67, 0x69, 0x6c, 0x64, 0x65, 0x5c, 0x64, 0x69, 0x65, 0x62, 0x65, 0x73, 0x67, 0x69, 0x6c, 0x64, 0x65, 0x5f, 0x65, 0x71, 0x75, 0x69, 0x70, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x24, 0x43, 0x20, 0x25, 0x69, 0x20, 0x25, 0x69, 0x69, 0x20, 0x25, 0x31, 0x73, 0x00, 0x00, 0x00, 0x54, 0x72, 0x61, 0x69, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x44, 0x49, 0x45, 0x42, 0x00, 0x00, 0x00, 0x54, 0x72, 0x61, 0x69, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x53, 0x4f, 0x4c, 0x44, 0x41, 0x54, 0x00, 0x6d, 0x69, 0x73, 0x63, 0x5c, 0x63, 0x68, 0x6f, 0x6f, 0x73, 0x65, 0x77, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x67, 0x65, 0x6c, 0x64, 0x5f, 0x69, 0x6e, 0x66, 0x6f, 0x5f, 0x32, 0x00, 0x00, 0x00, 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x67, 0x65, 0x62, 0x5f, 0x6c, 0x69, 0x73, 0x74, 0x65, 0x5f, 0x32, 0x00, 0x00, 0x00, 0x24, 0x5b, 0x25, 0x31, 0x47, 0x24, 0x5d, 0x00, 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x70, 0x6c, 0x61, 0x79, 0x65, 0x72, 0x5f, 0x73, 0x74, 0x61, 0x74, 0x73, 0x5f, 0x67, 0x65, 0x62, 0x5f, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6c, 0x00, 0x00, 0x00, 0x25, 0x73, 0x7e, 0x20, 0x25, 0x73, 0x00, 0x00, 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x6d, 0x65, 0x69, 0x73, 0x74, 0x65, 0x72, 0x5f, 0x6c, 0x69, 0x73, 0x74, 0x65, 0x5f, 0x32, 0x00, 0x00, 0x00 };  /* 0x62d01d */
GAPDATA full_data_62d134[0x14] = { 0x50, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x70, 0x6c, 0x61, 0x79, 0x65, 0x72, 0x5f, 0x73, 0x74, 0x61, 0x74, 0x73, 0x00, 0x00 };  /* 0x62d134, the string "Panel\player_stats" (its NUL and pad) */
GAPDATA full_data_62d1e8[4] = { 0x25, 0x47, 0x00, 0x00 };  /* 0x62d1e8 */
GAPDATA full_data_62d1ec[0x8] = { 0x25, 0x31, 0x4e, 0x35, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d1ec, the string "%1N5" (its NUL and pad: the table's loop bound +0xa8 points into it) */
GAPDATA full_data_62d1f4[8] = { 0x25, 0x73, 0x20, 0x25, 0x31, 0x4e, 0x37, 0x00 };  /* 0x62d1f4 */
GAPDATA full_data_62d1fc[8] = { 0x25, 0x31, 0x4e, 0x37, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d1fc */
GAPDATA full_data_62d204[16] = { 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x61, 0x6d, 0x74, 0x32, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d204 */
GAPDATA full_data_62d214[16] = { 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x61, 0x6d, 0x74, 0x33, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d214 */
GAPDATA full_data_62d224[16] = { 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x61, 0x6d, 0x74, 0x31, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d224 */
GAPDATA full_data_62d254[48] = { 0x24, 0x32, 0x41, 0x24, 0x5a, 0x25, 0x31, 0x4e, 0x32, 0x00, 0x00, 0x00, 0x24, 0x41, 0x24, 0x5a, 0x25, 0x31, 0x4e, 0x32, 0x00, 0x00, 0x00, 0x00, 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x74, 0x61, 0x67, 0x65, 0x73, 0x6f, 0x72, 0x64, 0x6e, 0x75, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d254, the strings "$2A$Z%1N2", "$A$Z%1N2" and "special\tagesordnung" (ai_AgendaOfficeList writes them as literals; the amount info box strings 0x62d234 / 0x62d244 are ai_AmountInfoFmt / ai_AmountInfoForm in lc.c) */
GAPDATA full_data_62d284[16] = { 0x25, 0x69, 0x74, 0x24, 0x31, 0x3e, 0x25, 0x73, 0x24, 0x4e, 0x24, 0x30, 0x3e, 0x00, 0x00, 0x00 };  /* 0x62d284 */
GAPDATA full_data_62d294[16] = { 0x42, 0x75, 0x63, 0x68, 0x5c, 0x47, 0x65, 0x73, 0x65, 0x74, 0x7a, 0x62, 0x75, 0x63, 0x68, 0x00 };  /* 0x62d294 */
GAPDATA full_data_62d2a4[24] = { 0x47, 0x65, 0x73, 0x65, 0x74, 0x7a, 0x65, 0x5c, 0x47, 0x65, 0x73, 0x65, 0x74, 0x7a, 0x62, 0x75, 0x63, 0x68, 0x5f, 0x49, 0x6e, 0x66, 0x6f, 0x00 };  /* 0x62d2a4 */
GAPDATA full_data_62d2bc[24] = { 0x62, 0x65, 0x72, 0x75, 0x66, 0x65, 0x5c, 0x5f, 0x6d, 0x65, 0x69, 0x73, 0x74, 0x65, 0x72, 0x62, 0x72, 0x69, 0x65, 0x66, 0x5f, 0x31, 0x00, 0x00 };  /* 0x62d2bc */
GAPDATA full_data_62d2d4[24] = { 0x62, 0x65, 0x72, 0x75, 0x66, 0x65, 0x5c, 0x5f, 0x6d, 0x65, 0x69, 0x73, 0x74, 0x65, 0x72, 0x62, 0x72, 0x69, 0x65, 0x66, 0x5f, 0x32, 0x00, 0x00 };  /* 0x62d2d4 */
GAPDATA full_data_62d2ec[20] = { 0x47, 0x65, 0x73, 0x65, 0x74, 0x7a, 0x65, 0x5c, 0x41, 0x75, 0x73, 0x77, 0x61, 0x68, 0x6c, 0x32, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d2ec */
GAPDATA full_data_62d300[160] = { 0x53, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x53, 0x74, 0x61, 0x6d, 0x6d, 0x62, 0x61, 0x75, 0x6d, 0x00, 0x00, 0x00, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x2d, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x31, 0x4e, 0x31, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00, 0x24, 0x35, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x24, 0x30, 0x3c, 0x00, 0x00, 0x24, 0x35, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x20, 0x24, 0x30, 0x3c, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x73, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x24, 0x35, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x24, 0x30, 0x3c, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x54, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00, 0x24, 0x35, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x24, 0x30, 0x3c, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x73, 0x24, 0x41, 0x25, 0x73, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00, 0x24, 0x35, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x24, 0x30, 0x3c, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x73, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00 };  /* 0x62d300 */
GAPDATA full_data_62d3a0[24] = { 0x24, 0x35, 0x3c, 0x24, 0x52, 0x25, 0x73, 0x24, 0x30, 0x3c, 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x69, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00 };  /* 0x62d3a0 */
GAPDATA full_data_62d3b8[12] = { 0x2b, 0x25, 0x69, 0x2e, 0x25, 0x69, 0x2e, 0x25, 0x69, 0x00, 0x00, 0x00 };  /* 0x62d3b8 */
GAPDATA full_data_62d3c4[16] = { 0x24, 0x33, 0x3e, 0x24, 0x4c, 0x25, 0x73, 0x24, 0x41, 0x24, 0x30, 0x3e, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d3c4 */
GAPDATA full_data_62d3d4[12] = { 0x2a, 0x25, 0x69, 0x2e, 0x25, 0x69, 0x2e, 0x25, 0x69, 0x00, 0x00, 0x00 };  /* 0x62d3d4 */
GAPDATA full_data_62d3e0[20] = { 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x69, 0x6e, 0x66, 0x6f, 0x5f, 0x73, 0x70, 0x69, 0x65, 0x6c, 0x65, 0x72, 0x00, 0x00 };  /* 0x62d3e0 */
GAPDATA full_data_62d3f4[8] = { 0x67, 0x69, 0x66, 0x74, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d3f4 */
GAPDATA full_data_62d3fc[16] = { 0x6d, 0x69, 0x73, 0x63, 0x5c, 0x67, 0x65, 0x73, 0x63, 0x68, 0x65, 0x6e, 0x63, 0x6b, 0x00, 0x00 };  /* 0x62d3fc */
GAPDATA full_data_62d424[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x77, 0x65, 0x72, 0x62, 0x75, 0x6e, 0x67, 0x31, 0x00, 0x00, 0x00 };  /* 0x62d424 */
GAPDATA full_data_62d43c[4] = { 0x25, 0x69, 0x25, 0x00 };  /* 0x62d43c */
GAPDATA full_data_62d440[24] = { 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x73, 0x74, 0x61, 0x74, 0x69, 0x73, 0x74, 0x69, 0x6b, 0x5f, 0x73, 0x74, 0x61, 0x64, 0x74, 0x00, 0x00, 0x00 };  /* 0x62d440 */
GAPDATA full_data_62d458[16] = { 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x70, 0x63, 0x6f, 0x6d, 0x70, 0x61, 0x72, 0x65, 0x38, 0x00 };  /* 0x62d458 */
GAPDATA full_data_62d468[24] = { 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x70, 0x63, 0x6f, 0x6d, 0x70, 0x61, 0x72, 0x65, 0x34, 0x70, 0x6c, 0x75, 0x73, 0x36, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d468 */
GAPDATA full_data_62d480[20] = { 0x70, 0x61, 0x6e, 0x65, 0x6c, 0x5c, 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x00, 0x00, 0x00 };  /* 0x62d480 */
GAPDATA full_data_62d494[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x6d, 0x65, 0x64, 0x69, 0x63, 0x75, 0x73, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d494 */
GAPDATA full_data_62d4ac[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x73, 0x63, 0x68, 0x65, 0x69, 0x64, 0x75, 0x6e, 0x67, 0x00, 0x00 };  /* 0x62d4ac */
GAPDATA full_data_62d4c4[28] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x65, 0x72, 0x70, 0x72, 0x65, 0x73, 0x73, 0x75, 0x6e, 0x67, 0x32, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d4c4 */
GAPDATA full_data_62d4e0[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x65, 0x72, 0x70, 0x72, 0x65, 0x73, 0x73, 0x75, 0x6e, 0x67, 0x00 };  /* 0x62d4e0 */
GAPDATA full_data_62d4f8[32] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x66, 0x75, 0x72, 0x63, 0x68, 0x74, 0x5f, 0x65, 0x69, 0x6e, 0x66, 0x6c, 0x6f, 0x65, 0x73, 0x73, 0x65, 0x6e, 0x00 };  /* 0x62d4f8 */
GAPDATA full_data_62d518[20] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x67, 0x65, 0x73, 0x65, 0x74, 0x7a, 0x00 };  /* 0x62d518 */
GAPDATA full_data_62d538[28] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x75, 0x6e, 0x74, 0x65, 0x72, 0x73, 0x63, 0x68, 0x6c, 0x61, 0x67, 0x75, 0x6e, 0x67, 0x00 };  /* 0x62d538 */
GAPDATA full_data_62d554[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x62, 0x65, 0x63, 0x69, 0x72, 0x63, 0x65, 0x6e, 0x00, 0x00, 0x00 };  /* 0x62d554 */
GAPDATA full_data_62d56c[28] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x67, 0x65, 0x67, 0x65, 0x6e, 0x73, 0x70, 0x69, 0x6f, 0x6e, 0x61, 0x67, 0x65, 0x00, 0x00 };  /* 0x62d56c */
GAPDATA full_data_62d588[60] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x73, 0x65, 0x73, 0x73, 0x65, 0x6c, 0x5f, 0x76, 0x65, 0x72, 0x74, 0x61, 0x75, 0x73, 0x63, 0x68, 0x65, 0x6e, 0x00, 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x68, 0x61, 0x73, 0x73, 0x5f, 0x65, 0x72, 0x7a, 0x65, 0x75, 0x67, 0x65, 0x6e, 0x00, 0x00 };  /* 0x62d588 */
GAPDATA full_data_62d5c4[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x76, 0x65, 0x72, 0x68, 0x6f, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d5c4 */
GAPDATA full_data_62d5fc[56] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x66, 0x72, 0x69, 0x65, 0x64, 0x65, 0x6e, 0x5f, 0x73, 0x74, 0x69, 0x66, 0x74, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x62, 0x65, 0x6b, 0x65, 0x68, 0x72, 0x65, 0x6e, 0x00, 0x00, 0x00 };  /* 0x62d5fc, the strings "privillegien\frieden_stiften" and "privillegien\bekehren" (no user yet; the dismissal form 0x62d5dc is ai_WorkerDismissalForm in lc.c) */
GAPDATA full_data_62d634[24] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x61, 0x62, 0x62, 0x69, 0x74, 0x74, 0x65, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d634 */
GAPDATA full_data_62d660[52] = { 0x70, 0x72, 0x69, 0x76, 0x69, 0x6c, 0x6c, 0x65, 0x67, 0x69, 0x65, 0x6e, 0x5c, 0x43, 0x48, 0x4f, 0x4f, 0x53, 0x45, 0x50, 0x52, 0x4f, 0x46, 0x45, 0x53, 0x53, 0x49, 0x4f, 0x4e, 0x32, 0x00, 0x00, 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x4e, 0x45, 0x55, 0x5f, 0x30, 0x36, 0x00 };  /* 0x62d660 */
GAPDATA full_data_62d694[20] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x4e, 0x45, 0x55, 0x5f, 0x30, 0x35, 0x00 };  /* 0x62d694 */
GAPDATA full_data_62d6a8[20] = { 0x5f, 0x4e, 0x41, 0x43, 0x48, 0x52, 0x49, 0x43, 0x48, 0x54, 0x45, 0x4e, 0x5f, 0x4e, 0x45, 0x55, 0x5f, 0x30, 0x37, 0x00 };  /* 0x62d6a8 */
GAPDATA full_data_62d6bc[252] = { 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x5c, 0x75, 0x6e, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  /* 0x62d6bc */
GAPDATA full_data_6f1c30[4] = { 0 };  /* 0x6f1c30 */
GAPDATA full_data_6f22b0[4] = { 0 };  /* 0x6f22b0 */
int sv_PendingMoney = 0;  /* 0x6f22dc, the money the money panel adds to the player's (the credit window: the limit minus the amount chosen; the transport window: minus the goods' total); sv's by use */
GAPDATA full_data_6f22e0[4] = { 0 };  /* 0x6f22e0 */
struct AIOBJ *sv_LastRoom = 0;  /* 0x6f2314, the room whose handler ai_EnterBuilding ran last (cleared on entering a building); sv's by place (between sv_PanelShown and f3gm's ai_PermsDirty), only lc reads it */
GAPDATA full_data_6f2330[4] = { 0 };  /* 0x6f2330 */
unsigned char sv_InMissionComplete = 0;  /* 0x6f23f1, 1 while sv_RunFrame plays ai_MissionCompleteSequence (no re-entry); sv's by use, see sv.c's lead comment */
GAPDATA full_data_6f23f2[6] = { 0 };  /* 0x6f23f2 */
int snd_SystemBank = 0;  /* 0x6f2400, the handle of the system sample bank system.sbf (main_ 0x532160 loads it) */
int snd_ThunderBank = 0;  /* 0x6f2408 */
int snd_BookBank = 0;  /* 0x6f240c */
int snd_RoomBank = 0;  /* 0x6f2410 */
int snd_TransportBank = 0;  /* 0x6f2414 */
struct MSXTRACK *main_MenuSong = 0;  /* 0x6f2418, the main menu's song (main_Menue) */
int main_Movie = 0;  /* 0x6f241c, the id mov_Open gave the movie that plays (intro movies, outro.mpg) */
GAPDATA full_data_6f2420[8] = { 0 };  /* 0x6f2420 */
int main_StartCoupleOn = 0;  /* 0x6f2428, nonzero: main_GameLoop creates the starting couple (main_CreateStartCouple) after main_StartGame in a single player game; no writer in the image */
int main_TextureDivider = 0;  /* 0x6f242c, the Gfx/TextureDivider key of game.INI */
int main_DirectStart = 0;  /* 0x6f2434, nonzero: WinMain skips the main menu, starts the single player game at once (main_JobName picks the job) and quits after it; no writer in the image */
GAPDATA full_data_6f2450[12] = { 0 };  /* 0x6f2450 */
int main_DevMenu = 0;  /* 0x6f245c, nonzero: the main menu shows the data-tool buttons */
int main_TextReopen = 0;  /* 0x6f2460 */
int main_HmtMode = 0;  /* 0x6f2468 */
int main_HmtBuild = 0;  /* 0x6f246c */
char main_JobName[0x104] = { 0 };  /* 0x6f2470, the job name of the BERUF=" command line option (WinMain: the starting job, matched against txt_JobNames, else 0x38) */
int main_MoviesOpen = 0;  /* 0x6f2574, nonzero while the movie player is set up (mov_Init); the outro opens and closes it itself otherwise */
int main_WeatherSound = 0;  /* 0x6f2580, the "weather" key of [Sound] (WinMain): the rain / weather sounds play */
int main_Inactive = 0;  /* 0x6f2584, 1 while the app is deactivated (WM_ACTIVATEAPP) */
HWND main_Window = 0;  /* 0x6f2588, the game window ("Die Gilde") */
char main_ScreenOpen = 0;  /* 0x6f258c, the video mode is open ( */
int main_Round = 0;  /* 0x6f2598, the round number (1 at the game start) */
int main_ScreenRestart = 0;  /* 0x6f25a4, nonzero: the screen must be re-initialised (set by the gfx options screen 0x5d5fd0 when the resolution changed) */
int main_DayEnding = 0;  /* 0x6f25a8, set by sv_RunFrame when the day ends (hour 23 or the end request) before it pops to main_GameLoop, cleared by main_GameLoop before the next day */
int main_ReplaceFigureOn = 0;  /* 0x6f25b0, nonzero: a bankrupt player's figure is replaced (main_RoundReport calls main_ReplaceFigure then clears it); no writer in the image */
int main_QuitProgram = 0;  /* 0x6f25b4, set by quit / Escape / WM_QUIT: WinMain exits */
int main_ScreenSaverOff = 0;  /* 0x6f25bc, the screen saver was switched off and is restored at exit */
int main_NoRoundFade = 0;  /* 0x6f25c0, 1 skips the black fade before the round page (set by main_RoundReport, cleared at the game start) */
int main_UdpOpen = 0;  /* 0x6f25c4, the udp socket of the network game is open (udp_Init .. udp_Exit) */
char he_FindFirst = 0;  /* 0x6f2624, he.h */
GAPDATA full_data_6f2625[3] = { 0 };  /* 0x6f2625 */
int he_FindUnused = 0;  /* 0x6f2628, he.h */
HEEVSLOT *he_ActiveSlot = 0;  /* 0x6f262c, he.h; owner he or main (an open question of he.c) */
unsigned char he_SlotConfirmed = 0;  /* 0x6f2630, he.h */
GAPDATA full_data_6f2631[3] = { 0 };  /* 0x6f2631 */
int he_ReaperMusic = 0;  /* 0x6f2634, he.h; owner he or main (an open question of he.c) */
AIBLD *he_PatrolBuilding = 0;  /* 0x6f2638, he.h; owner he or main (an open question of he.c) */
int he_MessageFilterMask = 0;  /* 0x6f263c, he.h; owner he or main (an open question of he.c) */
GAPDATA full_data_6f2678[4] = { 0 };  /* 0x6f2678 */
GAPDATA full_data_6f26c4[4] = { 0 };  /* 0x6f26c4, the alignment after ai_Settings, before ai_NewsText */
GAPDATA full_data_6f3874[12] = { 0 };  /* 0x6f3874, the rest of the 16-byte bss at 0x6f3870 (no reader) */
GAPDATA full_data_6f4084[28] = { 0 };  /* 0x6f4084, the rest of the 32-byte bss at 0x6f4080 (no reader) */
GAPDATA full_data_6f40c8[24] = { 0 };  /* 0x6f40c8 */
int ai_MapHeight = 0;  /* 0x6f4ee4, the height of the town map bitmap (ai_MapSize its width), set by lc's map loader 0x5b2090; f3init's by place (next to ai_MapSize), only lc reads it */
GAPDATA full_data_6f4f50[16] = { 0 };  /* 0x6f4f50 */
GAPDATA full_data_6f5310[16] = { 0 };  /* 0x6f5310 */
GAPDATA full_data_6f5a64[28] = { 0 };  /* 0x6f5a64 */
GAPDATA full_data_6f5e04[28] = { 0 };  /* 0x6f5e04 */
GAPDATA full_data_6f5e7c[4] = { 0 };  /* 0x6f5e7c */
GAPDATA full_data_6f5ea8[24] = { 0 };  /* 0x6f5ea8 */
GAPDATA full_data_6f5f44[4] = { 0 };  /* 0x6f5f44 */
GAPDATA full_data_6f5f48[4] = { 0 };  /* 0x6f5f48 */
GAPDATA full_data_6f5f4c[20] = { 0 };  /* 0x6f5f4c */
GAPDATA full_data_6fa008[24] = { 0 };  /* 0x6fa008 */
GAPDATA full_data_6fa0d0[16] = { 0 };  /* 0x6fa0d0, the rest of the 128-byte bss at 0x6fa060 (no reader) */
char he_OptionsPath[256] = { 0 };  /* 0x6fa0e0, the options INI path he_LoadMessageFilter copies from ai_OptionsFile for its
                                      GetPrivateProfileIntA / WritePrivateProfileStringA calls (he_SaveMessageFilter) */
int he_TipOrder[0x20] = { 0 };  /* 0x6fa1e0, he.h: the tip order (the 27 tip text slots, gm_Shuffled fills it) */
HEHINT he_Hints[128] = { 0 };  /* 0x6fa260, he.h: the tutorial hint table (he_OpenHelpEvents fills it, he_RunHint walks it) */
int he_BarView = 0;  /* 0x6faa60, he.h */
GAPDATA full_data_6faa64[28] = { 0 };  /* 0x6faa64 */
HEEVSLOT he_EventSlots[16] = { 0 };  /* 0x6faa80, he.h: the event bar's 16 slots */
/* full_data_6faa84 / 6faa88 / 6faa90: views of he_EventSlots */
GAPDATA full_data_7585f4[12] = { 0 };  /* 0x7585f4 */
int main_SoundBanks[0x80] = { 0 };  /* 0x75df20, the loaded sample-bank handles (main_PreloadSound fills, main_FreeSoundBanks frees; the next object is ai_MissionSeed 0x75e120) */
void * main_Instance = 0;  /* 0x75e140, its instance handle (HINSTANCE) */
MAINFAMILY main_Family = { 0 };  /* 0x75e180, the character creation's family and talents */
GAPDATA full_data_75e1ac[20] = { 0 };  /* 0x75e1ac */
char main_NetHost[0x80] = { 0 };  /* 0x75e1c0, the network host address ([Network] Host of game.INI, read with size 0x80 at 0x53ccf0; up to main_AppDir 0x75e240) */
char main_AppDir[288] = { 0 };  /* 0x75e240 */
char main_NetSaveName[161] = { 0 };  /* 0x75e360, the network game's city / save name (sprintf "%s/%s.net", read for "gamedata/network/%s.SAV"); size: up to the address 0x75e401, which is only 0x53b840's copy-loop base constant, so the name may run to ai_OptionsFile 0x75e480 */
GAPDATA full_data_75e401[127] = { 0 };  /* 0x75e401 */
int main_PlatformNT = 0;  /* 0x75e62c, the OS is NT based (dwPlatformId != VER_PLATFORM_WIN32_WINDOWS) */
int main_WindBank = 0;  /* 0x75e630, the wind sample bank (sb_LoadSampleBank "natur\wind.sbf" at the game start) */
GAPDATA full_data_75e634[12] = { 0 };  /* 0x75e634 */
/* full_data_76f990 / full_data_76f994: views of ai_Unk76f920 (data_views.h), the save header's +0x70 / +0x74 */
GAPDATA full_data_91f6e0[3] = { 0 };  /* 0x91f6e0 */
GAPDATA full_data_91f6e3[1] = { 0 };  /* 0x91f6e3 */
GAPDATA full_data_91f6e4[28] = { 0 };  /* 0x91f6e4 */
GAPDATA full_data_92bca8[11] = { 0 };  /* 0x92bca8 */
GAPDATA full_data_92bcb3[5] = { 0 };  /* 0x92bcb3 */
GAPDATA full_data_92bcb8[8] = { 0 };  /* 0x92bcb8 */
GAPDATA full_data_930370[16] = { 0 };  /* 0x930370 */
/* full_data_143a3c4: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_GateItemSuffix: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_GatePlaceSuffix: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_SpeedHostOnly: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143a3e4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextMercenaryBook: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_Tribune: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143a3f0: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextParchment: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FemaleHonourTitle: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143b3f8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextInformation: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143ccf0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143ccf8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143cd00: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143cd08: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143cd10: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143cd18: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143cd20: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143df38: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143df40: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143ec88: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143f030: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143f18c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143f190: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143f194: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143fac4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildPriceRaise: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildPriceLower: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildPriceClose: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143fb34: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildPowerPick: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildLevel2Pick: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildLevel3PowerPick: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildLevel3Pick: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextGuildPowerUsed: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143fc78: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143fda0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143fdb8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143fdd8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143feb8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143feec: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143ff1c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143ff38: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_143ff58: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14401c4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextDiceGame: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextStammtisch: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextDarkCorner: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14404e0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14404f8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14404fc: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440510: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440534: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440590: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144059c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14405b0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14405b8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14405d8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14405f0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440794: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14407b8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14407d0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14407d4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14407f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextBribe: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440894: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextKidnap: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextKidnapMessage: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextRansom: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14408e0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14408e8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextBurglary: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144094c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440954: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_MapPickPrompt: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextSpyBuilding: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeNoneOpen: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeNoRank: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeAllFull: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeApply: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeColumn: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeInfo: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14409ec: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextOfficeApplied: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14409f8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextMapTable: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440a24: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440a50: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440a94: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440ab4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTarget: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTraining: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440ae4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440b30: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440b34: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440b38: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextPatrol: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_PatrolPickPrompt: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_RaidPickPrompt: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextWatchPlan: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_WatchPlanPickPrompt: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextCustomsBox: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ArrestPickPrompt: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440b90: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440ba4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextRaid: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_BribePickText: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_BreakInPickText: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_Male: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_Female: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_Faith0: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_Faith1: view of txt_Ptrs (build/mk_txt_views.py) */
/* main_FamilyModelNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1440da4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144134c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1441350: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1441354: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1441358: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14414b4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14414b8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14414bc: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_ChatConsoleSlider: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14414d0: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DefaultFileName: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextGelage: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProduction: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextAttack: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextAmbush: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextEquip: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextSpy: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextRazzia: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextRegeneration: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextExtort: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextPickpocket: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextFarBuy: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextSell: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442050: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442078: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionMixing: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextExtract: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextSearchOre: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextSearchStone: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14420e0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14420e8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14420f0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442100: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionSmithy: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionStonecutter: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionCarpenter: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionWood: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionBrewing: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442160: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442180: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442190: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_14421b8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextBuildingClosed: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442318: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442334: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442430: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1442500: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1449e24: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1449e48: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_1449e4c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a118: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a130: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a134: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a138: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a13c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextBreakOut: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a19c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1a4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1b0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1bc: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1c0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1c4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1ec: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a1f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a200: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a204: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a208: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a20c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a22c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a240: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a244: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a248: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a24c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a268: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a270: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a29c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a2a0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a2e0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a2f8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a334: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a338: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a33c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a340: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a344: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a348: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a34c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a350: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a354: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a358: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a35c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a360: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a364: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a40c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a410: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a414: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a428: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a440: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a444: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a490: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a498: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4bc: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4c0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4c4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4cc: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4d0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4d4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4d8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4f0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a4f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a514: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a590: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a5d0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a5d4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a620: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a624: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a634: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a638: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a63c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a65c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a680: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a684: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a69c: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a6c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a714: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a764: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a8a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a8b4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144a8b8: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_NetPlayers: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_JobLabel: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FaithLabel: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144abe8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144abec: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144abf0: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_ReadyMark: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_ChatPrivate: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144abfc: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144acd8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144acdc: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144ace0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144ace4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144ace8: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144acec: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144acf0: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144acf4: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144ad10: view of txt_Ptrs (build/mk_txt_views.py) */
/* full_data_144adb4: view of txt_Ptrs (build/mk_txt_views.py) */
