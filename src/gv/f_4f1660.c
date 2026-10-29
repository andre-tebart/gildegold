/* 0x4f1660, 1 while the character c (kind 0x14 at +2, +8 set) still owes its grave payment: with an object (+0x16c, not the player's) the owed amount (+0x14 - 50 * 0x4f16f0 (object)) is set (0x4c4ac0 (id, 0x1c, -owed))
   and the character is done when +0x1c does not exceed it; without one the amount is -1000.0; 0 for other characters */

int unk_Unk4f1660(char *c)
{
    char *o;
    float owed, rest;

    if (c != 0 && c[2] == 0x14 && c[8] != 0) {
        o = *(char **)(c + 0x16c);
        if (o == 0 || *(short *)(o + 0x27) == -1 || *(unsigned short *)(o + 0x27) == *(unsigned short *)ai_PlayerChar) {
            ai_Unk4c4ac0(*(int *)(c + 4), 0x1c, 0xc47a0000);
        } else {
            owed = *(float *)(c + 0x14) - unk_Unk4f16f0(o) * 50.0f;
            rest = *(float *)(c + 0x1c) - owed;
            ai_Unk4c4ac0(*(int *)(c + 4), 0x1c, -owed);
            if (rest > 0.0f)
                return 0;
        }
        return 1;
    }
    return 0;
}
