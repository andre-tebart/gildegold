/* 0x4de640, the pistol shot of a duel (callback of the object made by 0x4de900: o + 0x180 shoots at o + 0x17c): the hit chance
   0x504ec0(shooter, 3) * 0.8 (* 1.5 with the skill 0x16f, * 0.66 for a player who lost the insult round), the damage 5 + (0..15) or
   15 + (0..30) (* 0.75 with the skill 0x2b5) added to the counters 0x6f0da8 / 0x6f0da9, the life 0x4cedc0, the texts 0x1102 / 0x1109 */

void ai_Unk4de640(char *o)
{
    char *sh = *(char **)(o + 0x180);
    char *tg = *(char **)(o + 0x17c);
    float acc, b;
    int hit, dmg, n;

    d2_SetForm(ai_Unk60d368, 0);
    acc = (float)gm_GetSkill(sh, 3);
    gm_GetSkill(tg, 3);
    acc = (float)(acc * 0.8);
    if (ai_InventoryHas(sh, 0x16f) != 0)
        acc = (float)(acc * 1.5);
    if (((int)sh == ai_Unk92bca4 && ai_Unk6f0da8[3] != 0) || ((int)sh == ai_Unk92bca0 && ai_Unk6f0daa != 0))
        acc = (float)(acc * 0.66);
    if (ai_Unk4e5360() < acc) {
        hit = 1;
        if (((int)sh == ai_Unk92bca4 && ai_Unk6f0da8[5] != 0) || ((int)sh == ai_Unk92bca0 && ai_Unk6f0da8[4] != 0))
            dmg = ai_Unk4e52f0(0xf) + 5;
        else
            dmg = ai_Unk4e52f0(0x1e) + 0xf;
        if (ai_InventoryHas(tg, 0x2b5) != 0)
            dmg = (int)(dmg * 0.75f);
        if ((int)tg == ai_Unk92bca4)
            ai_Unk6f0da8[0] += dmg;
        else
            ai_Unk6f0da8[1] += dmg;
        ai_Unk4cedc0(tg, (int)(*(float *)(tg + 0x1c) * 0.01 * dmg), 0);
    } else {
        hit = 0;
        dmg = 0;
    }
    sb_PlaySampleI(0, "Schuss", 1, 0x3f, 0x7f, 0, 0);
    if (ai_Unk4de530(tg, dmg) != 0) {
        ai_Unk6f0d94 = 1;
        return;
    }
    if (hit != 0) {
        d3t_CreateBlood(*(int *)(*(char **)(tg + 0x184) + 0x34), 0x40, 0x3e800000, 0x3dcccccd, 0x3e0f5c29, 0x425c0000);
        if (*(int *)(*(char **)(tg + 0x184) + 0x128) == 0)
            ch_QueueAnim(*(char **)(tg + 0x184), "kampf/taumeln", 1);
        d2_SetForm(ai_Unk60d368, 0);
        n = ai_Unk4de610(6);
        io_PrintText(n + 0x1102, *(unsigned short *)tg);
        snd_PlayVoice(*(unsigned short *)tg, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_TREFFE", n);
        b = (float)(*(float *)(tg + 0x1c) * dmg * 0.01);
        *(int *)(tg + 0x24) = (int)(*(int *)(tg + 0x24) - b);
        gm_AddHealth(*(unsigned short *)tg, (int)-b);
    } else {
        if (*(int *)(*(char **)(tg + 0x184) + 0x128) == 0)
            ch_QueueAnim(*(char **)(tg + 0x184), "kampf/stich_ausweichen", 1);
        n = ai_Unk4de610(9);
        io_PrintText(n + 0x1109, *(unsigned short *)tg);
        snd_PlayVoice(*(unsigned short *)tg, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_DANEBE", n);
    }
}
