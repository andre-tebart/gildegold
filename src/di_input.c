/* di_input.c -- original file name unknown: see include/di_input.h. Names are ours.
   unit of rea 8cef7d5b (src/game/di_input.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <dinput.h>
#include "di_input.h"

int kb_Base = 0x200;                    /* 0x5f3234 */
int di_MaxX = 790;                      /* 0x5f3238 mouse limits (as in the exe image) */
int di_MaxY = 599;                      /* 0x5f323c */
int di_Absolute = 1;                    /* 0x5f3240 mouse position from GetCursorPos */
extern unsigned short kb_Map[256];
extern unsigned char kb_Down[256];
extern unsigned char kb_Pressed[256];
extern DIMOUSEST dm_L;
extern DIMOUSEST dm_S;
extern DIMOUSEST dm_P;
extern DIMOUSEST dm_Q[32];
extern LPDIRECTINPUT di_Main;
extern LPDIRECTINPUTDEVICE di_Mouse;
extern LPDIRECTINPUTDEVICE di_Keyboard;
extern HANDLE di_Event;
extern HWND di_Hwnd;
extern HINSTANCE di_Hinst;
unsigned int kb_RepeatTime = 0;         /* 0x631098 */
int di_MinX = 0;                        /* 0x6310b0 */
int di_MinY = 0;                        /* 0x6310b4 */
unsigned int di_Flags = 0;              /* 0x6310b8 */
int di_GuiClick = 0;                    /* 0x6310bc, set by the GUI (gfx.c) when a left click hit a control: the button release is then swallowed */
int di_Disabled = 0;                    /* 0x6310c0 */
int di_SwapLR = 0;                      /* 0x6310c4 */
int di_LDownTime = 0;                   /* 0x6310c8 */
int di_RDownTime = 0;                   /* 0x6310cc */
int di_MDownTime = 0;                   /* 0x6310d0 */
int di_PressX = 0;                      /* 0x6310d4 */
int di_PressY = 0;                      /* 0x6310d8 */
int di_LClickTime = 0;                  /* 0x6310dc (.bss: 0 at start) */
union { int i; unsigned char b; } kb_RepeatKey = {0};     /* 0x6310e0 */
HWND di_Hwnd = 0;                           /* 0x146735c */
LPDIRECTINPUTDEVICE di_Mouse = {0};           /* 0x1467360 */
HINSTANCE di_Hinst = {0};                     /* 0x1467364 */
DIMOUSEST dm_L = {0};                         /* 0x1467380 */
LPDIRECTINPUTDEVICE di_Keyboard = {0};        /* 0x14673cc */
DIMOUSEST dm_S = {0};                         /* 0x14673e0 */
DIMOUSEST dm_Q[32] = {0};                     /* 0x1467440 */
unsigned short kb_Map[256] = {0};             /* 0x1467dc0 */
unsigned char kb_Down[256] = {0};             /* 0x14681c0 */
DIMOUSEST dm_P = {0};                         /* 0x14683c0 */
HANDLE di_Event = 0;                        /* 0x146840c */
unsigned char kb_Last = 0;                  /* 0x1468410 */
LPDIRECTINPUT di_Main = {0};                  /* 0x1468414 */
unsigned char kb_Pressed[256] = {0};          /* 0x1468420 */

/* 0x4103a0 */
HKEY reg_OpenKey(char *name, int mode)
{
    char path[256];
    HKEY key;
    DWORD disp;
    int r;

    sprintf(path, "%s%s", "Software\\Ahead Entertainment\\", name);
    switch (mode) {
    case 1:
        r = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, KEY_READ, &key);
        break;
    case 2:
        r = RegCreateKeyEx(HKEY_CURRENT_USER, path, 0, NULL, 0, KEY_WRITE, NULL, &key, &disp);
        break;
    default:
        return (HKEY)-1;
    }
    if (r == 0)
        return key;
    return (HKEY)-1;
}
/* 0x410430, close a registry key opened by reg_OpenKey */

void reg_CloseKey(HKEY key)
{
    RegCloseKey(key);
}
/* 0x410440 */

int reg_GetString(HKEY key, char *name, char *out)
{
    BYTE buf[128];
    DWORD size = 128;
    BYTE *p;
    BYTE c;

    if (RegQueryValueEx(key, name, NULL, NULL, buf, &size) == 0) {
        p = buf;
        do {
            c = *p;
            p[(BYTE *)out - buf] = c;
            p++;
        } while (c);
        return 1;
    }
    return 0;
}
/* 0x4104b0, store value as a REG_SZ string under name */

void reg_SetString(HKEY key, char *name, char *value)
{
    RegSetValueEx(key, name, 0, REG_SZ, (BYTE *)value, strlen(value) + 1);
}
/* 0x4104e0, fill kb_Map[256] with the VkKeyScan / MapVirtualKey code of every character, 0xffff when no key; always returns 1 */

int kb_BuildMap(void)
{
    int i;
    unsigned int v;
    SHORT s;

    for (i = 0; i < 256; i++) {
        s = VkKeyScan((CHAR)i);
        v = s;
        if (v != 0xffffffff) {
            if ((v & 0xff00) == 0x600)
                v = 0x400 | (v & 0xff);
            kb_Map[i] = ((MapVirtualKey(v & 0xff, 0) ^ v) & 0xff) ^ v;
        } else
            kb_Map[i] = 0xffff;
    }
    return 1;
}
/* 0x410560, keyboard scan code of a character with the shift / ctrl / alt modifiers; the numpad scan codes map to the main row ('0'..'9', '-' 0x2d, '+' 0x2b), 0 when no key matches */

unsigned char kb_FindKey(int shift, int ctrl, int alt, char code)
{
    unsigned int v = code;
    int i;

    if (v == 0x52)
        return 0x30;
    if (v == 0x4f)
        return 0x31;
    if (v == 0x50)
        return 0x32;
    if (v == 0x51)
        return 0x33;
    if (v == 0x4b)
        return 0x34;
    if (v == 0x4c)
        return 0x35;
    if (v == 0x4d)
        return 0x36;
    if (v == 0x47)
        return 0x37;
    if (v == 0x48)
        return 0x38;
    if (v == 0x49)
        return 0x39;
    if (v == 0x4a)
        return 0x2d;
    if (v == 0x4e)
        return 0x2b;
    if (shift)
        v |= 0x100;
    if (ctrl)
        v |= 0x200;
    if (alt)
        v |= 0x400;
    for (i = 0; i < 256; i++) {
        if (kb_Map[i] == v)
            return i;
    }
    return 0;
}
/* 0x410610, set the mouse speed base (kb_Base = v + 0x100) */

void kb_SetBase(int v)
{
    kb_Base = v + 0x100;
}

/* 0x410620 */
#define di_A (&dm_L.ldown)              /* 0x1467390 */
#define di_B (&dm_P.ldown)              /* 0x14683d0 */
#define di_C (&dm_S.ldown)              /* 0x14673f0 */

/* 0x410620 */
void di_ClearButtons(void)
{
    di_A[2] = 0;
    di_B[2] = 0;
    di_C[2] = 0;
    di_A[0] = 0;
    di_B[0] = 0;
    di_C[0] = 0;
    di_A[1] = 0;
    di_B[1] = 0;
    di_C[1] = 0;
    di_A[7] = 0;
    di_B[7] = 0;
    di_C[7] = 0;
    di_A[4] = 0;
    di_B[4] = 0;
    di_C[4] = 0;
    di_A[5] = 0;
    di_B[5] = 0;
    di_C[5] = 0;
    di_A[6] = 0;
    di_B[6] = 0;
    di_C[6] = 0;
}
/* 0x410690, clear the right-button event fields (rup / rdown / rhold / rclick) of dm_L, dm_P and dm_S */

void di_ClearAxes(void)
{
    di_A[7] = 0;
    di_B[7] = 0;
    di_C[7] = 0;
    di_A[4] = 0;
    di_B[4] = 0;
    di_C[4] = 0;
    di_A[5] = 0;
    di_B[5] = 0;
    di_C[5] = 0;
    di_A[6] = 0;
    di_B[6] = 0;
    di_C[6] = 0;
}
/* 0x4106d0, format fmt and the HRESULT ("Error = %08x") into a local buffer; the original has no output call */

void di_Error(int unk, int hr, char *fmt, ...)
{
    char buf[1024];
    char *p;
    va_list ap;

    va_start(ap, fmt);
    p = buf + wsprintf(buf, fmt, ap);
    wsprintf(p, "\n\nError = %08x", hr);
}
/* 0x410710, acquire (on) or unacquire the mouse device; 0 when there is no device or Acquire fails */

int di_AcquireMouse(int on)
{
    if (!di_Mouse)
        return 0;
    if (on) {
        if (di_Mouse->lpVtbl->Acquire(di_Mouse) < 0)
            return 0;
    } else
        di_Mouse->lpVtbl->Unacquire(di_Mouse);
    return 1;
}
/* 0x410740, acquire (on) or unacquire the keyboard device; 0 when there is no device or Acquire fails */

int di_AcquireKeyboard(int on)
{
    if (!di_Keyboard)
        return 0;
    if (on) {
        if (di_Keyboard->lpVtbl->Acquire(di_Keyboard) < 0)
            return 0;
    } else
        di_Keyboard->lpVtbl->Unacquire(di_Keyboard);
    return 1;
}
/* 0x410770 */

int di_Init(HWND hwnd, HINSTANCE hinst, unsigned int flags)
{
    HRESULT hr;
    DIPROPDWORD dp;

    di_Hinst = hinst;
    di_Flags = flags;
    di_Hwnd = hwnd;
    hr = DirectInputCreate(hinst, 0x700, &di_Main, NULL);
    if (hr < 0) {
        if (hr == 0x80070481) {
            di_Error(0, hr, "DirectInputCreate-BetadiVersion");
            return 0;
        }
        if (hr == 0x80070057) {
            di_Error(0, hr, "DirectInputCreate-Invalidparam");
            return 0;
        }
        if (hr == 0x8007047e) {
            di_Error(0, hr, "DirectInputCreate-OldVersion");
            return 0;
        }
        if (hr == 0x8007000e)
            di_Error(0, hr, "DirectInputCreate-OutOfMemory");
        return 0;
    }
    hr = di_Main->lpVtbl->CreateDevice(di_Main, &GUID_SysMouse, &di_Mouse, NULL);
    if (hr < 0) {
        di_Error(0, hr, "CreateDevice(SysMouse)");
        return 0;
    }
    hr = di_Mouse->lpVtbl->SetDataFormat(di_Mouse, &c_dfDIMouse);
    if (hr < 0) {
        di_Error(0, hr, "SetDataFormat(SysMouse, dfDIMouse)");
        return 0;
    }
    if (flags & 2)
        hr = di_Mouse->lpVtbl->SetCooperativeLevel(di_Mouse, hwnd, 0xa);
    else
        hr = di_Mouse->lpVtbl->SetCooperativeLevel(di_Mouse, hwnd, 5);
    if (hr != 0) {
        if (hr == 0x80070057)
            di_Error(0, hr, "SetCooperativeLevel(SysMouse):InvalidParam");
        else if (hr == 0x80070015)
            di_Error(0, hr, "SetCooperativeLevel(SysMouse):DIERR_NOTINITIALIZED ");
        else
            di_Error(0, hr, "SetCooperativeLevel(SysMouse):Unknown-error");
    }
    hr = di_Mouse->lpVtbl->SetEventNotification(di_Mouse, NULL);
    if (hr != 0) {
        di_Error(0, hr, "SetEventNotification(SysMouse)");
        return 0;
    }
    dp.diph.dwSize = sizeof(DIPROPDWORD);
    dp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dp.diph.dwObj = 0;
    dp.diph.dwHow = DIPH_DEVICE;
    dp.dwData = 0x80;
    hr = di_Mouse->lpVtbl->SetProperty(di_Mouse, DIPROP_BUFFERSIZE, &dp.diph);
    if (hr < 0) {
        di_Error(0, hr, "Set buffer size(SysMouse)");
        return 0;
    }
    di_AcquireMouse(1);
    dm_S.x = 200;
    dm_S.y = 200;
    dm_S.z = 0;
    hr = di_Main->lpVtbl->CreateDevice(di_Main, &GUID_SysKeyboard, &di_Keyboard, NULL);
    if (hr < 0) {
        di_Error(0, hr, "CreateDevice(GUID_SysKeyboard)");
        return 0;
    }
    hr = di_Keyboard->lpVtbl->SetDataFormat(di_Keyboard, &c_dfDIKeyboard);
    if (hr < 0) {
        di_Error(0, hr, "SetDataFormat(&c_dfDIKeyboard)");
        return 0;
    }
    if (flags & 1)
        hr = di_Keyboard->lpVtbl->SetCooperativeLevel(di_Keyboard, hwnd, 0x15);
    else
        hr = di_Keyboard->lpVtbl->SetCooperativeLevel(di_Keyboard, hwnd, 0xa);
    if (hr != 0)
        return 0;
    dp.diph.dwSize = sizeof(DIPROPDWORD);
    dp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dp.diph.dwObj = 0;
    dp.diph.dwHow = DIPH_DEVICE;
    dp.dwData = 0x80;
    hr = di_Keyboard->lpVtbl->SetProperty(di_Keyboard, DIPROP_BUFFERSIZE, &dp.diph);
    if (hr < 0)
        return 0;
    di_AcquireKeyboard(1);
    kb_BuildMap();
    return 1;
}
/* 0x410a50, unacquire and release the keyboard, mouse and DirectInput objects, close di_Event, clear di_Hwnd */

void di_Exit(void)
{
    di_Hwnd = 0;
    di_AcquireKeyboard(0);
    di_AcquireMouse(0);
    if (di_Keyboard) {
        di_Keyboard->lpVtbl->Release(di_Keyboard);
        di_Mouse = 0;
    } else if (di_Mouse) {
        di_Mouse->lpVtbl->Release(di_Mouse);
        di_Mouse = 0;
    }
    if (di_Main) {
        di_Main->lpVtbl->Release(di_Main);
        di_Main = 0;
    }
    if (di_Event) {
        CloseHandle(di_Event);
        di_Event = 0;
    }
}
/* 0x410ac0: button state machine (press timing, hold, double click) and event queue @deep behaviour draft, one code hunk left (7 differing instructions): the queue scan's tail. Ours sinks i=31 into the loop preheader, so the guard load dm_Q[31].valid takes eax (mov eax,ds:X; test eax,eax; jne) and the constant follows the branch; the original materialises i (mov eax,0x1f) between the load and its test, so the load takes ecx (mov ecx,ds:X; mov eax,0x1f; test ecx,ecx; jne). The 1-byte fill nop is an alignment artifact of the 1-byte-shorter body. Meaning is right (hunks read against the disassembly 410e58..410ec2). spell.py: no menu rule for either hunk. Last-bytes leads tried, each one edit + one build, all reverted as WORSE: (a) for-init form 'for (i = 31, q = &dm_Q[31].valid; *q == 0;)' with the && dm_Q[31].valid == 0 guard dropped -> 458 bytes / 180 asm lines; (b) comma form '(i = 31, q = &dm_Q[31].valid, *q == 0)' inside the guard -> identical 458 / 180; (c) 'changed && (i = 31, dm_Q[31].valid == 0)' with q initialised in the body -> 458 / 176, the same collapse: every form that materialises i before the guard test merges the loop head. The cost is the register-priority shift the note measured with c2prio (2026-10-23): the const#0 web lifts from -58 to -55 past _di_PressX (-57), the zero takes ebp and PressX loses its promotion -> allocation/scheduling work (match-deep). Readability done: Q4's 4 raw volatile-short casts replaced by the DIMOUSEST anonymous-union volatile view (dm_S.vx / dm_S.vy) with identical bytes; Q6 already described. */

void di_UpdateButtons(void)
{
    int i;
    int *q;      /* walks the valid flags of dm_Q downwards (stride 19 ints = one DIMOUSEST) */
    int now = tb_Calls;

    if (di_Disabled)
        return;
    if (di_LDownTime != 0
        && (now - di_LDownTime > 15 || dm_S.x < di_PressX - 3 || dm_S.x > di_PressX + 3
            || dm_S.y > di_PressY + 3 || dm_S.y < di_PressY - 3)) {
        dm_S.lhold = 1;
        di_LDownTime = 0;
        di_LClickTime = -1;
    }
    if (dm_S.lup == 0 || di_GuiClick == 0) {
        if (dm_S.ldown != 0 && di_LDownTime == 0) {
            di_LDownTime = now;
            di_PressX = dm_S.vx;
            di_PressY = dm_S.vy;
        }
        if (di_LClickTime != -1 && now - di_LClickTime > 40) {
            di_LDownTime = 0;
            di_LClickTime = -1;
        }
        if (dm_S.lup != 0) {
            if (di_LClickTime == -1 || now - di_LClickTime >= 40) {
                di_LDownTime = 0;
                di_LClickTime = now;
                if (dm_S.lhold != 0) {
                    dm_S.lup = 0;
                    di_LClickTime = -1;
                }
                dm_S.lhold = 0;
            } else {
                dm_S.lextra = 1;
                dm_S.lup = 0;
                di_LClickTime = -1;
                di_LDownTime = 0;
            }
        }
    } else {
        di_LDownTime = 0;
        di_GuiClick = 0;
        dm_S.ldown = 0;
        dm_S.lup = 0;
        di_LClickTime = -1;
        dm_S.lhold = 0;
    }
    if (dm_S.rdown != 0 && di_RDownTime == 0) {
        di_RDownTime = now;
        di_PressX = dm_S.vx;
        di_PressY = dm_S.vy;
    }
    if (di_RDownTime != 0) {
        if (dm_S.rhold == 0 && dm_S.rdown != 0) {
            if (now - di_RDownTime > 15) {
                di_RDownTime = 0;
                dm_S.rhold = 1;
            }
            if (dm_S.x < di_PressX - 3 || dm_S.x > di_PressX + 3 || dm_S.y > di_PressY + 3
                || dm_S.y < di_PressY - 3) {
                di_RDownTime = 0;
                dm_S.rhold = 1;
            }
        }
    }
    if (dm_S.rup != 0) {
        if (dm_S.rhold != 0) {
            dm_S.rup = 0;
            dm_S.rhold = 0;
            dm_S.ldown = 0;
            dm_S.lhold = 0;
        }
        di_RDownTime = 0;
    }
    if (dm_S.mdown != 0 && di_MDownTime == 0) {
        di_MDownTime = now;
        di_PressX = dm_S.x;
        di_PressY = dm_S.y;
    }
    if (di_MDownTime != 0) {
        if (dm_S.mhold == 0 && dm_S.mdown != 0) {
            if (now - di_MDownTime > 15) {
                dm_S.mhold = 1;
                di_MDownTime = 0;
            }
            if (dm_S.x < di_PressX - 3 || dm_S.x > di_PressX + 3 || dm_S.y > di_PressY + 3
                || dm_S.y < di_PressY - 3) {
                di_MDownTime = 0;
                dm_S.mhold = 1;
            }
        }
    }
    if (dm_S.mup != 0) {
        dm_S.mup = 0;
        if (dm_S.mhold != 0)
            dm_S.mhold = 0;
        di_MDownTime = 0;
    }
    if ((dm_S.lup != dm_P.lup || dm_S.ldown != dm_P.ldown || dm_S.lhold != dm_P.lhold
         || dm_S.lextra != dm_P.lextra || dm_S.rup != dm_P.rup || dm_S.rdown != dm_P.rdown
         || dm_S.rhold != dm_P.rhold || dm_S.mup != dm_P.mup || dm_S.mdown != dm_P.mdown
         || dm_S.mhold != dm_P.mhold || dm_S.wup != dm_P.wup || dm_S.wdown != dm_P.wdown)
        && dm_Q[31].valid == 0) {
        /* find the last used queue slot, append the state behind it */
        i = 31;
        q = &dm_Q[31].valid;
        do {
            q -= 19;
            i--;
            if ((int)q < (int)&dm_Q[0].valid) {   /* signed compare, needed for VC6's code */
                i = 0;
                goto put;
            }
        } while (*q == 0);
        if (i < 31) {
put:
            i++;
            dm_Q[i] = dm_S;
            dm_Q[i].valid = 1;
        }
    }
    dm_P = dm_S;
}
/* 0x410ed0, with di_SwapLR set, exchange the left / right click and release flags of dm_S */

void di_SwapButtons(void)
{
    DIMOUSEST t;

    if (di_SwapLR != 0) {
        t = dm_S;
        dm_S.lup = dm_S.rup;
        dm_S.lclick = dm_S.rclick;
        dm_S.rclick = t.lclick;
        dm_S.rup = t.lup;
    }
}
/* 0x410f20, drain the buffered mouse events into dm_S (axes, wheel, buttons), clamp to the mouse limits, follow the cursor when di_Flags has bit 2, then di_UpdateButtons */

void di_PollMouse(void)
{
    int count = 1;
    DIDEVICEOBJECTDATA od;
    POINT pt;
    int v;

    dm_S.dx = 0;
    dm_S.dy = 0;
    dm_S.dz = 0;
    if (di_Mouse == 0)
        return;
    while (1) {
        dm_S.lclick = 0;
        dm_S.lup = 0;
        dm_S.lextra = 0;
        dm_S.rclick = 0;
        dm_S.rup = 0;
        dm_S.mclick = 0;
        dm_S.mup = 0;
        dm_S.wdown = 0;
        dm_S.wup = 0;
        v = di_Mouse->lpVtbl->GetDeviceData(di_Mouse, sizeof(od), &od, &count, 0);
        if (v < 0 || count == 0)
            break;
        switch (od.dwOfs) {
        case 0:
            dm_S.dx = (int)((float)(kb_Base * 0.00390625f) * (int)od.dwData);
            dm_S.x += dm_S.dx;
            break;
        case 4:
            dm_S.dy = (int)((float)(kb_Base * 0.00390625f) * (int)od.dwData);
            dm_S.y += dm_S.dy;
            break;
        case 8:
            v = (kb_Base * (int)od.dwData) >> 8;
            if (v > 0)
                dm_S.wup = 1;
            else if (v < 0)
                dm_S.wdown = 1;
            dm_S.z += v;
            dm_S.dz = v;
            di_UpdateButtons();
            break;
        case 0xc:

            if (od.dwData & 0x80) {
                dm_S.ldown = 1;
                dm_S.lup = 0;
                if (dm_L.rhold == 0)
                    dm_S.lclick = 1;
                di_SwapButtons();
                di_UpdateButtons();
            }
            if ((od.dwData & 0x80) == 0) {
                dm_S.lup = 1;
                dm_S.ldown = 0;
                di_SwapButtons();
                di_UpdateButtons();
            }
            break;
        case 0xd:

            if (od.dwData & 0x80) {
                dm_S.rdown = 1;
                dm_S.rup = 0;
                dm_S.rclick = 1;
                di_SwapButtons();
                di_UpdateButtons();
            }
            if ((od.dwData & 0x80) == 0) {
                dm_S.rup = 1;
                dm_S.rdown = 0;
                di_SwapButtons();
                di_UpdateButtons();
            }
            break;
        case 0xe:

            if (od.dwData & 0x80) {
                dm_S.mdown = 1;
                dm_S.mup = 0;
                dm_S.mclick = 1;
                di_SwapButtons();
                di_UpdateButtons();
            }
            if ((od.dwData & 0x80) == 0) {
                dm_S.mup = 1;
                dm_S.mdown = 0;
                di_SwapButtons();
                di_UpdateButtons();
            }
            break;
        }
    }
    if (dm_S.x < di_MinX)
        dm_S.x = di_MinX;
    if (dm_S.x > di_MaxX)
        dm_S.x = di_MaxX;
    if (dm_S.y < di_MinY)
        dm_S.y = di_MinY;
    if (dm_S.y > di_MaxY)
        dm_S.y = di_MaxY;
    if (di_Flags & 4) {
        GetCursorPos(&pt);
        ScreenToClient(di_Hwnd, &pt);
        if (di_Absolute == 0) {
            dm_S.x += pt.x - dm_L.x;
            dm_S.y += pt.y - dm_L.y;
            pt.x = dm_L.x;
            pt.y = dm_L.y;
            ClientToScreen(di_Hwnd, &pt);
            SetCursorPos(pt.x, pt.y);
            di_UpdateButtons();
            return;
        }
        dm_S.x = max(di_MinX, min(pt.x, di_MaxX));
        dm_S.y = max(di_MinY, min(pt.y, di_MaxY));
    }
    di_UpdateButtons();
}
/* 0x4112e0, drain the buffered keyboard events into kb_Down / kb_Pressed, remember the last key and repeat it while it is held */

void di_PollKeyboard(void)
{
    int count;
    DIDEVICEOBJECTDATA od;
    int hr;

    kb_Last = 0;
    while (1) {
        count = 1;
        hr = di_Keyboard->lpVtbl->GetDeviceData(di_Keyboard, sizeof(od), &od, &count, 0);
        if (hr < 0 || count == 0)
            break;
        if (od.dwData & 0x80) {
            if (di_Disabled == 0) {
                kb_Last = (unsigned char)od.dwOfs;
                kb_Down[kb_Last] = 1;
            }
        } else if (di_Disabled == 0) {
            kb_Down[od.dwOfs] = 0;
            kb_Pressed[od.dwOfs] = 1;
        }
    }
    if (kb_Last != 0) {
        kb_RepeatTime = tb_Calls + 9;
        kb_RepeatKey.b = kb_Last;
    }
    if (kb_RepeatKey.b != 0) {
        if (kb_Last == 0 && kb_Down[kb_RepeatKey.i & 0xff] != 0 && tb_Calls > kb_RepeatTime + 5) {
            kb_RepeatTime = tb_Calls;
            kb_Last = kb_RepeatKey.b;
        }
        if (kb_Down[kb_RepeatKey.i & 0xff] == 0) {
            kb_RepeatKey.b = 0;
            kb_RepeatTime = 0x4e729fff;
        }
    }
}
/* 0x4113f0, set the mouse position in dm_P, dm_S and dm_L and on the cursor when not absolute */

void di_SetMousePos(int x, int y)
{
    dm_P.x = (short)x;
    dm_P.y = (short)y;
    dm_S.x = dm_P.x;
    dm_S.y = dm_P.y;
    dm_L.x = dm_P.x;
    dm_L.y = dm_P.y;
    if (di_Absolute == 0)
        SetCursorPos(x, y);
}
/* 0x411440: latch the next queued mouse event into dm_L */

void di_GetMouse(void)
{
    int i;

    if (di_Absolute != 0) {
        dm_L.x = dm_P.x;
        dm_L.y = dm_P.y;
        dm_L.z = dm_P.z;
    }
    i = 0;
    dm_L.lup = 0;
    dm_L.lclick = 0;
    dm_L.rup = 0;
    dm_L.rclick = 0;
    dm_L.mup = 0;
    dm_L.mclick = 0;
    for (; i < 32; i++) {
        if (dm_Q[i].valid != 0) {
            dm_L.lup = dm_Q[i].lup;
            dm_L.ldown = dm_Q[i].ldown;
            dm_L.lhold = dm_Q[i].lhold;
            dm_L.lclick = dm_Q[i].lclick;
            dm_L.lextra = dm_Q[i].lextra;
            dm_L.rup = dm_Q[i].rup;
            dm_L.rdown = dm_Q[i].rdown;
            dm_L.rhold = dm_Q[i].rhold;
            dm_L.rclick = dm_Q[i].rclick;
            dm_L.mup = dm_Q[i].mup;
            dm_L.mdown = dm_Q[i].mdown;
            dm_L.mhold = dm_Q[i].mhold;
            dm_L.mclick = dm_Q[i].mclick;
            dm_L.wup = dm_Q[i].wup;
            dm_L.wdown = dm_Q[i].wdown;
            dm_L.dx = dm_Q[i].dx;
            dm_L.dy = dm_Q[i].dy;
            dm_L.dz = dm_Q[i].dz;
            dm_Q[i].valid = 0;
            di_PollKeyboard();
            return;
        }
    }
    di_PollKeyboard();
}
/* 0x4115b0, clear the click / button events selected by mask in dm_L and dm_P (left and right swapped when di_SwapLR) */

void di_ClearEvents(unsigned char mask)
{
    if (di_SwapLR != 0) {
        if (mask & 1) {
            dm_L.rclick = 0;
            dm_P.rclick = 0;
        }
        if (mask & 2) {
            dm_L.rup = 0;
            dm_P.rup = 0;
        }
        if (mask & 4) {
            dm_L.rdown = 0;
            dm_P.rdown = 0;
        }
        if (mask & 8) {
            dm_L.lclick = 0;
            dm_P.lclick = 0;
        }
        if (mask & 0x10) {
            dm_L.lup = 0;
            dm_P.lup = 0;
        }
        if (mask & 0x20) {
            dm_L.ldown = 0;
            dm_P.ldown = 0;
        }
    } else {
        if (mask & 1) {
            dm_L.lclick = 0;
            dm_P.lclick = 0;
        }
        if (mask & 2) {
            dm_L.lup = 0;
            dm_P.lup = 0;
        }
        if (mask & 4) {
            dm_L.ldown = 0;
            dm_P.ldown = 0;
        }
        if (mask & 8) {
            dm_L.rclick = 0;
            dm_P.rclick = 0;
        }
        if (mask & 0x10) {
            dm_L.rup = 0;
            dm_P.rup = 0;
        }
        if (mask & 0x20) {
            dm_L.rdown = 0;
            dm_P.rdown = 0;
        }
    }
}
