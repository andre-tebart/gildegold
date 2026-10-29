/* 0x42e9d0, unregisters proc from the timer; 1 when it was registered, else 0. */

int tb_CloseFunction(TBPROC proc)
{
    int i;

    for (i = 0; i < 32; i++)
        if (tb_Funcs[i].proc == proc) {
            tb_Funcs[i].proc = NULL;
            return 1;
        }
    return 0;
}
