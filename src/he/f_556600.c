/* 0x556600, the run handler "he_RunTransport" of the transport events (ROUGH, a skeleton: the jump table 0x556d94 has 8 states): a flagged (2) record with a pending handle +0x84 waits for it (0x4c3ae0); the buildings +0xb0 / +0xb4 (0x501190 x4), the cart +0xac (0x4fe980) are looked up (missing cart: the event is freed, buildings: nothing). State 0 posts the record 3 later with the handle 4, state 1 waits for the due time and the two handles, then runs the leg by the mode +0xc8 (1: 0x555690, 2: 0x555350, 3: 0x555b60, 4: 0x556080; else the debug text "he_RunTransport: invalid type!" 0x4222b0), a random chance (0x42ec00) lets the cart play the cart noise "Karrenrumpeln_r" (0x4c5a70 / 0x53dda0 with the news 0x1c2f / .., 0x4c5a70), the later states hand over goods (0x4c4db0, sb_PlaySample 0x426fa0 "Karrenrumpeln_r", 0x509 .. 0x4bfa40 / 0x4bfc60 / 0x4bda80 / 0x59e170 / 0x541250 / 0x50ca50 / 0x472ce0) and end the transport (state -1 / 2: the cart's flag 0x40 / 0x20 cleared, +0x24 = 0, free). The jump-table cases are only outlined by the helpers 0x556b50, 0x556be0 .. above them */

void he_Unk556600(int ev)
{
    int b0, b1, a44, a45, cart, mode, st;
    int h;

    if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && *(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    cart = gm_FindObjectInst(*(int *)(ev + 0xac));
    if (a44 == 0 || a45 == 0 || b0 == 0 || b1 == 0)
        return;
    if (cart == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    st = *(int *)(ev + 0x70);
    if (st > 7)
        return;
    switch (st) {
    case 0:
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 3, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) != 0)
            return;
        h = *(int *)(ev + 0xec);
        if (h != -1 && cm_GetSlotState(h) == 0)
            return;
        h = *(int *)(ev + 0xe8);
        if (h != -1 && cm_GetSlotState(h) == 2) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        mode = *(int *)(ev + 0xc8);
        if (mode == 2)
            he_Unk555350(ev);
        else if (mode == 1)
            he_Unk555690(ev);
        else if (mode == 3)
            he_Unk555b60(ev);
        else if (mode == 4)
            he_Unk556080(ev);
        else
            err_Message("he_RunTransport: invalid type!");
        return;
    default:
        he_Unk53e1f0((int *)ev);
        return;
    }
}
