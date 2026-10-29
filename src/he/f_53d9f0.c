/* 0x53d9f0, shows / hides the event bar for the player slot `pl` (0xffff = hide, 0x62571c = the panel or -1) */

void he_Unk53d9f0(int pl)
{
    int *p;

    if (U_I((unsigned)&full_data_62571c) != -1) {
        p = (int *)(unsigned)&full_data_6faa88;
        do {
            if (p[-1] != -1 && *p != -1)
                d2_SetViewEnabled(*p, 0);
            p += 3;
        } while ((int)p < ((unsigned)((unsigned)full_data_6faa88 + 0xb8) + 0x8));
        d2_SetForm(U_I((unsigned)&full_data_6faa60), 0);
        d2_MoveObj((((U_I((unsigned)&d2_LastWindow) * 0xf1) & 0xffff0000) | (unsigned short)(*(short *)(unsigned)&d2_ScreenW - 0x7d)), *(short *)(unsigned)&d2_ScreenH - 0x21,
                     U_I(((unsigned)&ai_Unkf8f13c + 0x250) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
        d2_SetViewEnabled(U_I((unsigned)&full_data_6faa60), 0);
    }
    if (pl != 0xffff) {
        d2_SetViewEnabled(U_I((unsigned)&full_data_6faa60), 1);
        U_I((unsigned)&full_data_625718) = 0;
        p = (int *)(unsigned)&full_data_6faa84;
        do {
            if (*p == -1) {
                U_I((unsigned)&full_data_62571c) = U_I((unsigned)&full_data_6faa60);
                return;
            }
            p += 3;
            U_I((unsigned)&full_data_625718)++;
        } while ((int)p < ((unsigned)((unsigned)full_data_6faa84 + 0xbc) + 0x4));
        U_I((unsigned)&full_data_62571c) = U_I((unsigned)&full_data_6faa60);
        return;
    }
    U_I((unsigned)&full_data_625718) = -1;
    U_I((unsigned)&full_data_62571c) = -1;
}
