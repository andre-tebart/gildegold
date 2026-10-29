/* 0x541250, sets the player (word +8) and its id (+0xc) of the record */

void he_Unk541250(int r, int pl)
{
    *(short *)(r + 8) = (short)pl;
    if (pl != 0xffff) {
        *(int *)(r + 0xc) = U_I((unsigned)&ai_CharInts + pl * 0x21c);
        return;
    }
    *(int *)(r + 0xc) = -1;
}
