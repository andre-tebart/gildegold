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
