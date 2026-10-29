/* 0x4d3980, an explosion event: the fuse sound "ZuendschnurBrennt_l_01", the attacker's worker count synced, 0x4ce290 */

void ai_Unk4d3980(char *ev)
{
    char *att = *(char **)(ev + 0x180);
    char *k, *r;
    int vv;

    k = ai_Unk4cda50(att);
    ai_Unk4ce290(*(char **)(*(char **)(att + 0x184) + 0x34) + 0x4c);
    sb_PlaySample(ai_Unk6f0d14, "ZuendschnurBrennt_l_01", 3, 0x3f, 0x7f, 0, 0);
    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && (*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2)) {
        G19_FIGHTER_SYNC(att);
    }
}
