/* 0x55bfd0, the run handler of the building purchase event (a Ghidra function with the jump table 0x55c52c, 4 states and the case code 0x55c370 .. 0x55c52b; ROUGH skeleton): the buyer +0xb4 (0x505df0) and the building +0xac (0x502020) are looked up; state 0 opens the offer for the player (0x17e9 / 0x17ea news, 0x53dee0, the building flag 0x4c4a60 0x5a / 2 / 0x200), a computer buyer decides at once by a price factor (0x41700000 / 0x40a00000 / 0x3f99999a / 0x41000000 / 0x3fcccccd from the relation, the owner kind 5 / 6 / 7 and 0x50c550) against the value (0x509b10 (building, +0x5c)); the answer (0xf8a668: 0 = buy, 1 = decline) pays (0x4c4830), hands the building over (0x4c54a0) and closes the window (0x53d800); the end frees the record */

void he_Unk55bfd0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int buyer, b, price;
    float f;
    char text[0x200];

    if (st < 0 || st > 3) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    buyer = gm_FindChar(*(int *)(ev + 0xb4));
    b = gm_FindBuildingById(*(int *)(ev + 0xac));
    if (buyer == 0 || b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        f = 1.0f;
        price = gm_EquipmentValue(b, *(char *)(b + 0x5c));
        (void)price;
        gm_Liking(*(unsigned short *)(b + 0x25));
        txt_Format(text, 0x17e9, *(unsigned short *)(b + 0x27));
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(int *)(ev + 0xb0) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
        cm_SendSetField(*(int *)(buyer + 1), 0x5a, 2, 0x200, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (U_I((unsigned)&d2_ClickedId) == -1)
            return;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            cm_SendMoveGoodsUnchecked(*(int *)(buyer + 1), *(int *)((unsigned)&ai_CharInts + *(int *)(ev + 0xb0) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            ((void (*)(int))cm_SendOwnership)(*(int *)(buyer + 1));
        } else {
            txt_Format(text, 0x17ea, *(unsigned short *)(b + 0x27));
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(int *)(ev + 0xb0) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
        }
        cm_SendSetField(*(int *)(buyer + 1), 0x5a, 2, 0x200, 0);
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    default:
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
}
