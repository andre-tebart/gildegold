/* 0x5bea00, the special-amount list window over a caller record (79 B): the local 0x50-byte config is
   zeroed and gets +4 = 0x204, +8 = 0x5eed68, +0xc = 0x5eed6c and +0x10 = 6, then 0x5c0970 (the config, the
   second argument, 0, 0) runs the tabbed list; its result is returned */

int ai_Unk5bea00(int param_1, int param_2)
{
    int cfg[0x14];
    int i;

    (void)param_1;
    for (i = 0; i < 0x14; i++)
        cfg[i] = 0;
    cfg[1] = 0x204;
    cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
    cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
    cfg[4] = 6;
    return ai_Unk5c0970((int)cfg, param_2, 0, 0);
}
