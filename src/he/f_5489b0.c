/* 0x5489b0, a Ghidra miss: the run handler of the monthly payment event (+0xac a counter in bytes, +0xb0 the amount): at 0 a random 0x100 against 0.5 * the byte at +0x80 of the player record + 64 decides between the news 0x10b5 (2 * the amount is paid, 0x4c47e0) and 0x10b6 (0.5 * the amount) (ROUGH) */

void he_Unk5489b0(int ev)
{
    char text[0x400];
    int st, r;
    double x;

    st = *(int *)(ev + 0x70);
    if (st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    if (*(unsigned char *)(ev + 0xac) > 0) {
        *(unsigned char *)(ev + 0xac) = *(unsigned char *)(ev + 0xac) - 1;
        return;
    }
    r = (unsigned short)util_Random(0x100);
    x = (double)*(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x77) + *(unsigned short *)(ev + 8) * 0x21c) * 0.5 + 64.0;
    if (x > (double)r) {
        txt_Format(text, 0x10b5, *(int *)(ev + 0xb0) * 2);
        he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5ff, (char *)0);
        cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, *(int *)(ev + 0xb0) * 2);
    } else {
        txt_Format(text, 0x10b6, (int)((double)*(int *)(ev + 0xb0) * 0.5));
        he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5ff, (char *)0);
        cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)((double)*(int *)(ev + 0xb0) * 0.5));
    }
    he_Unk53e1f0((int *)ev);
}
