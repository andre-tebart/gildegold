/* 0x4f0dd0, graveyard order type 6 (p: +8 the object id, +0x13 the number): makes that many new dead (0x4ef9c0); 0 when some were made, else -4 */

int unk_Unk4f0dd0(unsigned char *p)
{
    unsigned char *o;
    int n;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerChar)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    n = unk_Unk4ef9c0(o, *(int *)(p + 0x13));
    return (n > 0) - 1 & 0xfffffffc;
}
