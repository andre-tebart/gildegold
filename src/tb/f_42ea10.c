/* 0x42ea10, switches the timer callback of proc off (off != 0) or on; 1 when proc is registered. */

int tb_SetFunctionOff(TBPROC proc, int off)
{
    int i;

    for (i = 0; i < 32; i++)
        if (tb_Funcs[i].proc == proc) {
            tb_Funcs[i].off = off;
            return 1;
        }
    return 0;
}
