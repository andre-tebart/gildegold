/* 0x4de530, a fighter whose life (0x4fc8e0) is under 0.2 sinks to the ground ("bewegung/zu_boden_sinken"): 1 */

int ai_Unk4de530(char *c, int dmg)
{
    char *o;

    if (unk_Unk4fc8e0(c) < 0.2) {
        d3t_CreateBlood(*(int *)(*(char **)(c + 0x184) + 0x34), 0x40, 0x3e800000, 0x3dcccccd, 0x3e0f5c29, 0x425c0000);
        ch_StopActions(*(char **)(c + 0x184));
        o = (char *)ch_AddActionCBK(*(char **)(c + 0x184), ai_Unk4de4b0, 0x12, 0x36, 1);
        strcpy(o + 0xf0, "bewegung/zu_boden_sinken");
        *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x220) = 0;
        ai_Unk6f0d94 = 1;
        return 1;
    }
    return 0;
}
