/* 0x4e4f80, runs all pending parties (0x4e4f40) of the player pl that he is a member of: his field +0x208 holds the id of the party while it runs
   (0x4e4060), every party is dropped (0x4e5180) and the field is sent back (0x4c3d90); 1 when one ran. ROUGH */

char ai_Unk4e4f80(int pl)
{
    char ret;
    int save;
    int p;

    ret = 0;
    save = ai_CharInts[pl][0x208 / 4];
    p = ai_Unk4e4f40();
    while (p != 0) {
        if (ai_Unk4e4870(ai_CharInts[pl][0], (char *)p) != 0) {
            ai_CharInts[pl][0x208 / 4] = *(int *)p;
            ai_Unk4e4060(p);
            ai_CharInts[pl][0x208 / 4] = -1;
            ret = 1;
        }
        ai_Unk4e5180(*(int *)p);
        p = ai_Unk4e4f40();
    }
    cm_BeginFieldBlock((char *)ai_CharInts[pl] - 4, ai_CharInts[pl][0]);
    cm_AddFieldValue(4, 1, (unsigned short)(pl * 0x21c - (int)cm_FieldObject + (int)ai_Unk770d8c), &save);
    cm_SendFieldValues();
    return ret;
}
