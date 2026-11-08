/* main.h -- the startup / shutdown code of the game window (0x531670 ..), see src/game/main.c */
#ifndef MAIN_H
#define MAIN_H
#include <windows.h>                  /* Win32 types */

typedef struct MAINFAMILY {             /* 0x2c bytes at 0x75e180, the character creation's choices */
    int group[6];                       /* 0x00, the profession group 0 .. 4 of each family member (-1: none): the
                                           paternal / maternal grandparents, [4] the father, [5] the mother
                                           (main_ChooseFamily) */
    int talent[5];                      /* 0x18, the five talent values in points, 21 a step (main_ChooseTalents) */
} MAINFAMILY;

/* the globals of the program level (the window is created in main_CreateWindow) */
extern HWND main_Window;                /* 0x6f2588, the game window ("Die Gilde") */
extern void *main_Instance;             /* 0x75e140, its instance handle (HINSTANCE) */
extern char main_ScreenOpen;            /* 0x6f258c, the video mode is open */
extern int main_PlatformNT;             /* 0x75e62c, the OS is NT based (not Win9x) */
extern int main_ScreenSaverOff;         /* 0x6f25bc, the screen saver was switched off and is restored at exit */
extern int main_MessageBox;             /* 0x624938, the message box over the character creation, -1 = none */
extern int snd_ThunderBank;     /* 0x6f2408, sample bank handle (natur/blitzunddonner.sbf) */
extern int snd_BookBank;        /* 0x6f240c, sample bank handle (objekte/buecher.sbf) */
extern int snd_RoomBank;        /* 0x6f2410, sample bank handle (objekte/TuerenUndRaeume.sbf) */
extern int snd_TransportBank;   /* 0x6f2414, sample bank handle (objekte/Transporter.sbf) */
extern int main_UdpOpen;               /* 0x6f25c4, the udp socket of the network game is open */
extern int main_Round;                   /* 0x6f2598, the round number (1 at the game start) */
extern struct MSXTRACK *main_MenuSong;  /* 0x6f2418, the main menu's song (msx_Play) */
extern int main_DevMenu;                /* 0x6f245c, nonzero: the main menu shows the data-tool buttons */
extern MAINFAMILY main_Family;          /* 0x75e180 */
extern int main_QuitProgram;            /* 0x6f25b4, set by quit / Escape / WM_QUIT: WinMain exits */
extern int main_ClockScale;              /* 0x6248b4, 100: the game seconds of a clock update per speed step (sv_UpdateClock) */
extern unsigned char main_GfxPath[264];  /* 0x624588, the GfxPath option of game.INI (default \project\gfx\): the vfs root and the base of the city ini / save paths */
extern char main_NetHost[0x80];        /* 0x75e1c0, the network host address (game.INI [Network] Host) */
extern char main_NetSaveName[161];      /* 0x75e360, the network game's name */

void main_DestroyWindow(void);          /* 0x531670 */
void main_ShutdownScreen(void);         /* 0x531690 */
void main_ShutdownModules(void);        /* 0x5316d0 */
void main_Exit(void);                   /* 0x531750, the exit handler */
LRESULT CALLBACK main_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void main_BuildVersionString(char *out);
unsigned main_CreateMutex(void **mutex);   /* 0x531b50 */
void main_SetScreenSaver(unsigned on);     /* 0x531b80 */
int main_Init(void);                    /* 0x531ba0 */
void main_ShutdownEngine(void);            /* 0x531c20 */
int main_OpenScreen(int m);
int main_OpenEngine(void);
int main_PlayerCountDialog(void);
void main_StartCityGame(char *name, int net);
int main_NewSinglePlayerGame(void);                /* 0x537930, the new game (main_Menue), 0: cancelled */
int main_NetworkMenu(void);

/* declared in other units' headers before (moved to the owner) */
void main_GameLoop(int ctl);  /* 0x53bb20 */
extern int main_ScreenRestart;  /* 0x6f25a4 */
extern int main_ObjectsOn;  /* 0x62457c, nonzero: sv_RunFrame draws the gui objects */
extern int main_GfxOn;  /* 0x624560, nonzero when the round change shows its pages and fades (main_EndRound, main_RoundReport, main_BeginPlayerRound) */
extern int main_MoviesOpen;  /* 0x6f2574, nonzero while the movie player is set up (mov_Init); the outro opens and closes it itself otherwise */
extern char main_JobName[0x104];  /* 0x6f2470, the job name of the BERUF=" command line option (WinMain: the starting job) */
extern int main_ReplaceFigureOn;  /* 0x6f25b0, nonzero: a bankrupt player's figure is replaced (main_RoundReport) */
extern int main_DirectStart;  /* 0x6f2434, nonzero: WinMain skips the main menu and starts the single player game at once */
extern int main_StartCoupleOn;  /* 0x6f2428, nonzero: main_GameLoop creates the starting couple after main_StartGame */
extern int main_WeatherSound;  /* 0x6f2580, the [Sound] weather key */
extern int main_DayEnding;  /* 0x6f25a8, the day is ending (sv_RunFrame sets, main_GameLoop clears) */
extern int snd_SystemBank;  /* 0x6f2400 */

#endif
