/* 0x573e30, the second step of the event 0x9d for the figure pl and its argument a (the approach): a random count n by the
   approach (a < 2: 2 + random (2); a == 2: 3 + random (2); a == 3: 3 + random (3); else 4 + random (4), 3 when a == 4); while
   pl's +0x194 is below n the approach 0x5736d0 (pl, pl, a) runs; else pl +4 is charged -n (0x50cc50) and the text 0x41db /
   the message 0x5fa (0x53dda0) with 0x629054 are sent to pl +4 */

void gm_Unk50cc50(int a, int b);                 /* 0x50cc50 */

void he_Unk573e30(char *pl, int a)
{
    char text[0x200];
    int n;

    if (a < 2)
        n = (util_Random(2) & 0xffff) + 2;
    else if (a == 2)
        n = (util_Random(2) & 0xffff) + 3;
    else if (a == 3)
        n = (util_Random(3) & 0xffff) + 3;
    else
        n = (util_Random(4) & 0xffff) + (a == 4 ? 3 : 4);
    if (HM_I(pl, 0x194) < n) {
        he_Unk5736d0(pl, pl, a);
        return;
    }
    gm_Unk50cc50(HM_I(pl, 4), -n);
    txt_Format(text, 0x41db, n);
    he_ShowNews(HM_I(pl, 4), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_629054);
}
