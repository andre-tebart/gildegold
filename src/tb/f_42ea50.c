/* 0x42ea50, tells whether proc is switched off in the timer; 0 for an unregistered proc. */

int tb_IsFunctionOff(TBPROC proc)
{
    int i;

    for (i = 0; i < 32; i++)
        if (tb_Funcs[i].proc == proc)
            return tb_Funcs[i].off != 0;
    return 0;
}
