/* main.h -- the startup / shutdown code of the game window (0x531670 ..), see src/game/main.c */
#ifndef MAIN_H
#define MAIN_H

/* the globals of the program level (the window is created in main_Unk5324f0) */
extern HWND main_Window;                /* 0x6f2588, the game window ("Die Gilde") */
extern void *main_Instance;             /* 0x75e140, its instance handle (HINSTANCE) */
extern char main_ScreenOpen;            /* 0x6f258c, the video mode is open */
extern int main_PlatformNT;             /* 0x75e62c, the OS is NT based (not Win9x) */
extern int main_ScreenSaverOff;         /* 0x6f25bc, the screen saver was switched off and is restored at exit */

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
int main_Unk531c50(int m);
int main_Unk532160(void);
int main_Unk5327e0(void);
void main_Unk532700(int name, int net);
int main_Unk5335b0(void);

#endif
