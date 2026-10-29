/* 0x4e1220, 1 when the life (0x4fc820) of the first of m is not above 0 */

int ai_Unk4e1220(char *m)
{
    if (gm_CharacterHealth(gm_FindChar(*(int *)(m + 0x34))) <= 0.0f)
        return 1;
    return 0;
}
