/* 0x4d3a30, a bomb event: the attacker's worker count synced, 0x4ce5d0 at the target */

void ai_Unk4d3a30(char *ev)
{
    char *att = *(char **)(ev + 0x17c);
    char *r;
    int vv;

    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c) {
        G19_FIGHTER_SYNC(att);
    }
    ai_Unk4ce5d0(*(float **)(*(char **)(att + 0x184) + 0x34) + 0x13, *(int *)(ev + 0x180), *(int *)(ev + 0x184));
}
