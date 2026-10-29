/* 0x5064d0, swaps the two characters a and b (0x505df0): runs 0x4b3a90 for a, clears the scene links (0x502b60 for the buildings of class 2 owned by a), swaps the 0x21c-byte records (a keeping its slot number, the byte at +9 of the first, the job bytes fixed up with the job tables 0x504ad0 / 0x5044b0 / 0x504ce0, the name copied or the icon looked up 0x4fb330), re-points the object lists (+0x178) at the new ids, swaps all references to a / b in the relation ids (+0x5c .. +0x7c of every character), adds the kind 9 objects of a's list to a's id (0x500970), restores the money (+0x1a8) and the state words and re-registers (0x503c60 (id, 0)). Returns a's slot or -1 / -2 when a / b is unknown. ROUGH */

unsigned gm_Unk5064d0(int a, int b)
{
    unsigned short *ca, *cb;
    char tmpa[0x21c];
    char tmpb[0x21c];
    unsigned sa, sb;
    int money, i, k, off;
    int *q;
    unsigned short *o;

    ca = (unsigned short *)gm_FindChar(a);
    if (ca == 0)
        return -1;
    ai_RestartMission(ca);
    for (i = 0; i < 0x100; i++) {
        char *bl = ai_Blds + i * 0xa9;
        if (*bl != 0 && gm_BuildingClass((unsigned char)*bl) == 2 && *(unsigned short *)(bl + 0x27) == *ca)
            gm_AssignBuilding((int)bl, *(unsigned short *)ai_PlayerChar, *(unsigned short *)ai_PlayerChar);
    }
    sa = *ca;
    money = (int)gm_GetWorth(sa);
    cb = (unsigned short *)gm_FindChar(b);
    if (cb == 0)
        return -2;
    he_Unk53ed40(ca);
    he_Unk5538b0(ca);
    he_Unk5538b0(cb);
    sb = *cb;
    if (ca[0x28] != cb[0x28])
        cb[0x28] = ca[0x28];
    amt_ClearOffices(ca);
    memcpy(tmpb, cb, 0x21c);
    *(unsigned short *)tmpb = (unsigned short)sa;
    *(int *)(tmpb + 4) = *(int *)(ca + 2);
    *(int *)(tmpb + 0x170) = *(int *)(ca + 0xb8);
    memcpy(tmpa, ca, 0x21c);
    *(unsigned short *)tmpa = *cb;
    *(int *)(tmpa + 4) = *(int *)(cb + 2);
    ai_Chars[sa][0] = tmpb[0];
    memcpy(ai_Chars[sa], tmpb, 0x21c);
    memcpy(ai_Chars[sb], tmpa, 0x21c);
    for (o = *(unsigned short **)(ai_Chars[sa] + 0x178); o != 0; o = *(unsigned short **)((char *)o + 0x3f))
        *(int *)((char *)o + 6) = *(int *)(ai_Chars[sa] + 4);
    for (o = *(unsigned short **)(ai_Chars[sb] + 0x178); o != 0; o = *(unsigned short **)((char *)o + 0x3f))
        *(int *)((char *)o + 6) = *(int *)(ai_Chars[sb] + 4);
    for (k = 0; k < 0x300; k++) {
        q = (int *)(ai_Chars[k] + 0x5c);
        for (i = 0; i < 8; i++) {
            if (q[i] == a)
                q[i] = b;
            else if (q[i] == b)
                q[i] = a;
        }
    }
    for (o = *(unsigned short **)(ai_Chars[sa] + 0x178); o != 0; o = *(unsigned short **)((char *)o + 0x3f)) {
        if (*o != 0 && ai_Goods[*o * 0x41] == 9)
            gm_AddObjekt(*(int *)(ai_Chars[sa] + 4), *o, *(int *)((char *)o + 0xe));
    }
    off = sa * 0x21c;
    *(int *)(ai_Chars[0] + off + 0x1a8) = money;
    *(int *)(ai_Chars[0] + off + 0x194) = 4;
    *(int *)(ai_Chars[0] + off + 0x1c8) = 0;
    unk_Unk503c60(sa, 0);
    return sa;
}
