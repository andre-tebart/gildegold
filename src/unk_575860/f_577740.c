/* 0x577740, the start of the tutorial ("tutorial\main" 0x629304): -4 without a player / with a running run
   record, else the tutorial message 0x5ec (0x53d6f0), the controls of the run's window (0x475850, 0x475600,
   0x475f70 (id, 0) / (id, 3)), 0x52ea30 (0x6ed938), the text 0x624204 (0x515960, 0x7d) and the window state
   S+0x20 = 0x475b80 (id, 3, text); -1 without the run record, else 0. ROUGH: the chain 0x5eec60 -> +0x1c -> +0x74
   is reloaded before every call as in the original */

#define R8 (*(int *)(*(char **)(*(char **)(S + 0x1c) + 0x74) + 8))

int ai_Unk577740(void)
{
    char *per;

    per = *(char **)(S + 0x1c);
    if (!per)
        return -4;
    if (*(int *)(per + 0x74))
        return -4;
    he_Unk53d6f0(per, 0, 0, (char *)(unsigned)&full_data_629304, 0x5ec);
    if (!*(int *)(*(char **)(S + 0x1c) + 0x74))
        return -1;
    d2_PlaceMainWindow(R8, 3);
    d2_CentreViewWindows(R8);
    d2_SetForm(R8, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(R8, 3);
    U_I(S + 0x20) = d2_GetWinObj(R8, 3, io_PrintText((char *)(unsigned)"$Z%s", 0x7d));
    return 0;
}
