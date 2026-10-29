/* 0x55c540, the allocation handler of the purchase event (ROUGH): the due clock is now, the end clock +0xc4 = now + the delay +0xb8, +0xc8 = 8, +0xca / +0xce = 0, +0xbc = +0xb8; the building +0xac is looked up (none: free), the buyer +0xb0 (0x505df0); for a player buyer (kind 6 / 7) the offer window is prepared (0x4ffe90, the news 0x18d7 with 0x5d, 0x53dee0 (.., 0x5ec)) */

void he_Unk55c540(int ev)
{
    int b, buyer, o;
    char text[0x200];

    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0xc4) = ai_Time;
    gm_AddTime(ev + 0xc4, 0, *(int *)(ev + 0xb8), 0);
    *(unsigned short *)(ev + 0xc8) = 8;
    *(int *)(ev + 0xca) = 0;
    *(int *)(ev + 0xce) = 0;
    *(int *)(ev + 0xbc) = *(int *)(ev + 0xb8);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
        return;
    buyer = gm_FindChar(*(int *)(ev + 0xb0));
    if (buyer != 0 && (*(unsigned char *)(buyer + 2) == 6 || *(unsigned char *)(buyer + 2) == 7)) {
        o = gm_Query(*(int *)(b + 0x5d), 2, 6, 1, *(unsigned short *)(ev + 0xb0));
        if (o != 0) {
            txt_Format(text, 0x18d7, 0x5d);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
        }
    }
}
