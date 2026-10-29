/* 0x5778b0, the run window's text tick: -4 without the run record / when 0x577850 fails, else the controls 1 / 2
   with the texts of the record +0xc / +0xc + 1 (or 0x3fd3 / 0x3fd4 in the states 0xa / 0xb of the modes 1 / 2)
   and, when the record word +0x10 is set, the window S+0x14 recreated by 0x4c2760 (0xfffffff9, 0, +0x10, -1)
   after releasing the previous one (0x426f20 / 0x427290); 0 */

int ai_Unk5778b0(void)
{
    char *st;
    char *r;

    if (!*(int *)(S + 0x18))
        return -4;
    if (ai_Unk577850() == -4)
        return -4;
    st = S;
    r = *(char **)(st + 0x18);
    if ((*(int *)(r + 8) == 1 || *(int *)(r + 8) == 2)
            && (*(int *)(st + 4) == 0xa || *(int *)(st + 4) == 0xb)) {
        d2_SetForm(R8, 1);
        io_PrintText(0x3fd3);
        d2_SetForm(R8, 2);
        io_PrintText(0x3fd4);
        return 0;
    }
    d2_SetForm(R8, 1);
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0xc));
    d2_SetForm(R8, 2);
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0xc) + 1);
    r = *(char **)(S + 0x18);
    if (*(int *)(r + 0x10)) {
        if (U_I(S + 0x14)) {
            if (sb_IsPlaying(U_I(S + 0x14)))
                sb_Stop(U_I(S + 0x14), 0);
            U_I(S + 0x14) = 0;
        }
        U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, *(int *)(r + 0x10), -1);
    }
    return 0;
}
