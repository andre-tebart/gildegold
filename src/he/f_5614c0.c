/* 0x5614c0, the run handler of the ransom demand ("locations\diebesgilde\Diebesgilde_Loesegeld_Msg" window; ROUGH skeleton, 1376 bytes): opens the thieves guild message (0x53d6f0, d2_SetForm), shows the demand (0x515960, the news 0x53dda0), the answer pays (0x4c4830 / 0x4c5fa0, 0x4c3ba0 / 0x4c3c00 / 0x4c49a0, the player record) or refuses (a random reaction 0x53dee0), the thief figure leaves (0x55eba0, 0x4c52e0); the window is closed and the record freed */

void he_Unk5614c0(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st < 0 || st > 1) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        he_Unk53d6f0(ev, 0, 0, (int)"locations\\diebesgilde\\Diebesgilde_Loesegeld_Msg", 0x5fd);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (U_I((unsigned)&d2_ClickedId) == -1)
        return;
    if (U_I((unsigned)&d2_ClickedId) == 0)
        cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xac), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, *(int *)(ev + 0xb0));
    ((void (*)(int))ai_Unk4c5fa0)(*(int *)(ev + 0xac));
    he_Unk53d800(ev);
    he_Unk53e1f0((int *)ev);
}
