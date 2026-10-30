/* main.h -- the startup / shutdown code of the game window (0x531670 ..), see src/game/main.c */
#ifndef MAIN_H
#define MAIN_H

/* the globals of the program level (the window is created in main_CreateWindow) */
extern HWND main_Window;                /* 0x6f2588, the game window ("Die Gilde") */
extern void *main_Instance;             /* 0x75e140, its instance handle (HINSTANCE) */
extern char main_ScreenOpen;            /* 0x6f258c, the video mode is open */
extern int main_PlatformNT;             /* 0x75e62c, the OS is NT based (not Win9x) */
extern int main_ScreenSaverOff;         /* 0x6f25bc, the screen saver was switched off and is restored at exit */
extern struct MSXTRACK *main_MenuSong;  /* 0x6f2418, the main menu's song (msx_Play) */
extern int main_DevMenu;                /* 0x6f245c, nonzero: the main menu shows the data-tool buttons */
extern int main_QuitProgram;            /* 0x6f25b4, set by quit / Escape / WM_QUIT: WinMain exits */
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
int main_NetworkMenu(void);

#endif
