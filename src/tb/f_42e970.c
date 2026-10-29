/* 0x42e970, registers a function called every `period` ticks. */

int tb_OpenFunction(TBPROC proc, unsigned long period)
{
    int i = 0;

    while (tb_Funcs[i++].proc && i < 32)
        ;
    if (i > 32) {
        err_Message("tb_OpenFunction: Too many TimeBaseProcs!");
        return 0;
    }
    i--;
    tb_Funcs[i].proc = proc;
    tb_Funcs[i].period = period;
    tb_Funcs[i].off = 0;
    return 1;
}
