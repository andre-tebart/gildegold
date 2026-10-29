/* 0x55e3d0, the run handler of a news event (ROUGH skeleton, 496 bytes): a figure (0x505df0) is looked up, a random building of the query list (gm_Query / gm_QueryNext, 0x42ec00) is picked, the amount 0x5089c0 / _ftol is booked (0x4c4880) and the news "_NACHRICHTEN_HS_09" shown to the owner (0x53dee0); the record is freed */

void he_Unk55e3d0(int ev)
{
    int p;
    char text[0x100];

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0) {
        cm_SendExSellObject(*(int *)(ev + 0xac), -1, 0, 0, 0, 0);
        txt_Format(text, 0x1bba, p);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(p + 4), -1, "_NACHRICHTEN_HS_09", 0);
    }
    he_Unk53e1f0((int *)ev);
}
