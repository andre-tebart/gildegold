/* 0x55d390, the run handler of a payment event (ROUGH): states -2 / -1 free; the building +0xac (none: free), the news 0x190b (+0xb4, +0xb0) to the player (0x53dda0 .., 0x5ff) and the payment 0x4c4880 (+0xb4, .., +0xb0, ..); then the record is freed */

void he_Unk55d390(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b;
    char text[0x200];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac))) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    txt_Format(text, 0x190b, *(int *)(ev + 0xb4), *(unsigned char *)(ev + 0xb0));
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ff, 0);
    cm_SendExSellObject(*(int *)(ev + 0xb4), -1, *(unsigned char *)(ev + 0xb0), 0, 0, 0);
    he_Unk53e1f0((int *)ev);
}
