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
