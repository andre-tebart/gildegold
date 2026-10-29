/* 0x5a17b0, the guild level 2 / 3 entry (288 B): 0x4e95a0 (the player) == 1 switches on the office byte
   0x770ce9 + slot * 0x21c: 0x1e opens the request 0x5c1130 (the record {0, 0xc00, +0x10 = 6, +0x18 = 1}, the
   text 0x143fbb8, the callback 0x5a13f0); 0x1f / 0x21 the window 0x5c1b40 (0x5a1530, the text 0x143fb70, 0);
   0x20 the window 0x5c1b40 (0x5a1670, the text 0x143fb98, 0); else 0x520f90 (the player, 8) or the text
   0x143fbd8 when the player does not qualify (0x52d7a0 on every exit) */

void ai_Unk5a17b0(void)
{
    int req[20];
    char *per;
    int r;
    int i;

    per = (char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c;
    r = amt_CanUseMayorPower3(per);
    if (r == 1) {
        if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x1e) {
            for (i = 0; i < 20; i++)
                req[i] = 0;
            req[1] = 0xc00;
            req[4] = 6;
            req[6] = 1;
            ai_AmountMenu(req, (char *)U_I((unsigned)&full_data_143fbb8), 0, (void *)(unsigned)&ai_Unk5a13f0);
            sv_CameraFollow();
            return;
        }
        if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x1f ||
            *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x21) {
            ai_Unk5c1b40((void *)(unsigned)&ai_Unk5a1530, (char *)U_I((unsigned)&full_data_143fb70), 0);
        } else if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c) == 0x20) {
            ai_Unk5c1b40((void *)(unsigned)&ai_Unk5a1670, (char *)U_I((unsigned)&full_data_143fb98), 0);
            sv_CameraFollow();
            return;
        }
        sv_CameraFollow();
        return;
    }
    if (r == -1) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_143fbd8), 0);
        return;
    }
    sv_CheckMoney(per, 8);
}
