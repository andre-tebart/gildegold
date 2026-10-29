/* 0x41e050 */

void dd_Unlock(void)
{
    if (dd_Surface) {
        switch (dd_Mode) {
        case DD_WINDOW:
        case DD_FULLSCREEN:
            dd_BackBuffer->lpVtbl->Unlock(dd_BackBuffer, NULL);
        case DD_GDI:
        case DD_WINDOWMEM:
        case DD_FULLMEM:
            dd_Surface = NULL;
        }
    }
}
