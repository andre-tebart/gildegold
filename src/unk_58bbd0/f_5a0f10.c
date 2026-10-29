/* 0x5a0f10, the office application entry (240 B): 0x4e94e0 (the player) == 1 opens the request 0x5b2180
   (1, the 0x50-byte record: +0 = 1, +4 = -1, +0x10 = 6, +0xc = 0x7d4190, +0x4 = 1, +0x18 = 0x727, the text
   0x143fb3c, the callback 0x5a0ca0); == -1 shows the text 0x143fbd8 (0x521150); else 0x520f90 (the player, 5) */

void ai_Unk5a0f10(int param_1)
{
    int req[20];
    int r;
    int i;

    r = amt_CanUseMayorPower2((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    if (r == 1) {
        for (i = 0; i < 20; i++)
            req[i] = 0;
        req[0] = 1;
        req[1] = -1;
        req[2] = ((unsigned)&ai_Unk770d8c + 0x63404);
        req[3] = 1;
        req[4] = 6;
        req[19] = 0x727;
        ai_Unk5b2180(1, req, (void *)U_I((unsigned)&full_data_143fb3c), (void *)(unsigned)&ai_Unk5a0ca0, 0);
        return;
    }
    if (r != -1) {
        sv_CheckMoney((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, 5);
        return;
    }
    sv_MessageBox((char *)U_I((unsigned)&full_data_143fbd8), 0);
}
