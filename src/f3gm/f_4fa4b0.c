/* 0x4fa4b0, collects the incomes of the character id into the 0x38-byte record out (cleared first): 0 when the slot is free / not active (+8) / 0x4fd7e0 says so; the incomes 1 or 2 (by 0x4e9400 of the character's job at +0x164), 5, 3 (only when +0x1a8 > 0) and 4. Returns 1 */

int unk_Unk4fa4b0(unsigned id, int *out, unsigned char flags)
{
    char *c;
    int i;

    c = CH(id);
    for (i = 0; i < 0xe; i++)
        out[i] = 0;
    if (*(short *)c == -1)
        return 0;
    if (c[8] == 0)
        return 0;
    if (unk_Unk4fd7e0(id) != 0)
        return 0;
    if (amt_HasTownObject(gm_TitleGroup(*(int *)(c + 0x164))) == 0)
        unk_Unk4f9f60(id, (char *)out, flags);
    else
        unk_Unk4fa060(id, (char *)out, flags);
    unk_Unk4fa3d0(id, (char *)out, flags);
    if (*(int *)(c + 0x1a8) > 0)
        unk_Unk4fa160(id, (char *)out, flags);
    unk_Unk4fa2a0(id, (char *)out, flags);
    return 1;
}
