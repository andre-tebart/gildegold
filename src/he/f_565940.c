/* 0x565940, a Ghidra miss: the run handler of the event 0x6e (ROUGH): when the sound handle +0xac is done (0x4c3ae0) the result 0x4c3b10 and the object +0xb4 (0x502020) decide: 0x4c54a0 (obj + 1, the owner (+0x25) player record 0x770b84 or the result), the reward
   0x4c47e0 (result +4, -1, 0x6f1bc5, 0x5085f0 (0xbb8, 0x6f1bc5)); with a value +0xbc the owner (kinds 6 / 7: the news 0x17e6) gets the share 0x4c47e0 and 0x55ea60 yields a target for 0x4c52e0 (id, target, 0, 0, 0x627b20); always freed; -1 / -2 free */

void he_Unk565940(int ev)
{
    int st, r, o, own;
    int v[2];
    char text[0x400];

    st = *(int *)(ev + 0x70);
    if (st < -2)
        return;
    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    if (cm_GetSlotState(*(int *)(ev + 0xac)) == 0)
        return;
    r = cm_GetSlotValue(*(int *)(ev + 0xac));
    o = gm_FindBuildingById(*(int *)(ev + 0xb4));
    if (r != 0 && o != 0) {
        if (*(unsigned short *)(o + 0x25) != 0xffff)
            cm_SendOwnership(*(int *)(o + 1), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(o + 0x25) * 0x21c), *(int *)(r + 4));
        else
            cm_SendOwnership(*(int *)(o + 1), *(int *)(r + 4), *(int *)(r + 4));
        cm_SendMoveGoods(*(int *)(r + 4), -1, U_I((unsigned)&ai_OwnPlayer), gm_PlayerFactor(0xbb8, *(unsigned char *)(unsigned)&ai_OwnPlayer));
        if (*(int *)(ev + 0xbc) != 0) {
            own = (unsigned)&ai_Chars + *(unsigned short *)(o + 0x25) * 0x21c;
            if (own != 0) {
                if (*(char *)(own + 2) == 6 || *(char *)(own + 2) == 7) {
                    txt_Format(text, 0x17e6, o, *(unsigned short *)r);
                    he_MessageBoxQuickjump(*(int *)(own + 4), -1, text, 0, 0x5ec, *(int *)(o + 1), -1, 0, ((unsigned)&ai_Unk61aa38 + 0x12c));
                }
                cm_SendMoveGoods(*(int *)(own + 4), -1, U_I((unsigned)&ai_OwnPlayer), *(int *)(ev + 0xbc));
            }
            cm_SendOwnership(*(int *)(o + 1), *(int *)(r + 4), *(int *)(r + 4));
            /* the original caller pushes 3 arguments (0x565ab5 push ecx, 0x565ab6 push edx, 0x565ab7 push edi) to the
               1-argument 0x55ea60, which reads only [esp+8] (0x55ea61); the extra pushes are the original bytes */
            if (he_Unk55ea60(r, &v[0], &v[1]) != 0)    /* @original: 3 args to the 1-arg 0x55ea60 (see above) */
                ai_Unk4c52e0(*(int *)(r + 4), v[0], v[1], 0, 0, (unsigned)&full_data_627b20);
        }
    }
    he_Unk53e1f0((int *)ev);
}
