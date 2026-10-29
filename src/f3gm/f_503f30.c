/* 0x503f30, 0 when the kind number of 0x504ad0 (x) is 10 .. 14, else 2 */

int gm_Unk503f30(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k >= 10 && k <= 14)
        return 0;
    return 2;
}
