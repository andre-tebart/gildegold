/* 0x570bf0, a plunder message: 0x4c4830 (a[0], b + 4, the byte 0x6f1bc5, a[3]), 0x4c4a60 (b + 4, 0x1cc, 4, 1, 0), the text 0x40fe to a[0] (flag 1, 0x5f4) */

void he_Unk570bf0(int *a, int b)
{
    char text[0x400];

    ((int (*)(int, int, unsigned char, int))cm_SendMoveGoodsUnchecked)(a[0], *(int *)(b + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer, a[3]);
    cm_SendSetField(*(int *)(b + 4), 0x1cc, 4, 1, 0);
    txt_Format(text, 0x40fe, *(unsigned short *)b, a[3]);
    he_ShowNews(a[0], -1, text, 1, 0x5f4, 0);
}
