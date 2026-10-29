/* 0x4d0740, the per-tick command handling for the player record rec: the click orders of the selected characters
   (0x4d0570, then the comment "_K_C2C_BEFEHL_ERHALTEN" and the mark +0x188 is cleared), the highlight of the selected
   characters' models (bit 2 of +0x212 as the mark). Rough draft. */

void ai_Unk4d0740(char *rec)
{
    char *c, *m, *mdl, **pp;

    if (dm_L.lhold == 0 && ai_Unk760440 != 0)
        sv_Unk52eed0();
    if (dm_L.lhold != 0 && dm_L.rdown == 0) {
        if (ai_Unk760440 == 0 && di_Absolute != 0 && ai_Unk5fe710 == -1) {
            ai_Unk4d08c0();
            sv_Unk52ee50();
        }
    } else if (dm_L.lup != 0) {
        for (pp = ai_Unk7603a0; (int)pp < (int)ai_Unk760420; pp++) {
            c = *pp;
            if (c != 0 && c[0x188] != 0 && (ai_Unk4ce170(*(unsigned short *)ai_Unk6f0d74, c) & 0x800) &&
                *(int *)(c + 0x184) != 0 && c[8] != 0) {
                if (ai_Unk4d0570(rec, c)) {
                    if (ai_Unk6f0d74 == (char *)ai_Unk6f0cec)
                        snd_PlayVoice(*(unsigned short *)c, (int)ai_Unk6f0d34, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    else
                        snd_PlayVoice(*(unsigned short *)c, (int)ai_Unk6f0d38, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    c[0x188] = 0;
                }
            }
        }
    }
    for (pp = ai_Unk7603a0; (int)pp < (int)ai_Unk760420; pp++) {
        c = *pp;
        if (c != 0) {
            m = *(char **)(c + 0x184);
            if (!(m[0x8c] & 0x40)) {
                mdl = *(char **)(m + 0x34);
                if (c[0x188]) {
                    if (!(mdl[0x212] & 2))
                        d3t_SetGrey(mdl, 1);
                } else if ((mdl[0x212] & 2) == 2)
                    d3t_SetGrey(mdl, 0);
            }
        }
    }
    if (dm_L.rup != 0)
        ai_Unk4d08c0();
}
