/* 0x52f600, per frame update of an open book: hides the page widgets outside the shown pair (+0x270 .. +0x270 + 1), turns the page when a page button (+0x268 / +0x26c) is the focus, and shows the shown
   pair (blink 0x6f23f0) */

void sv_Unk52f600(int bk)
{
    int *p, i;

    if (bk != 0) {
        i = 0;
        if (*(char *)(bk + 0x271) > 0) {
            p = (int *)(bk + 0x228);
            do {
                if (i < *(char *)(bk + 0x270) || *(char *)(bk + 0x270) + 1 < i)
                    d2_EnableViewObjects(*p, 0);
                i++;
                p++;
            } while (i < *(char *)(bk + 0x271));
        }
        if (*(int *)(bk + 0x26c) != -1 && ai_Unk5fe710 == *(int *)(bk + 0x26c) && SV_I(((unsigned)&dm_L + 0xc)) != 0)
            sv_Unk52fa00((int *)bk);
        if (*(int *)(bk + 0x268) != -1 && ai_Unk5fe710 == *(int *)(bk + 0x268) && SV_I(((unsigned)&dm_L + 0xc)) != 0)
            sv_Unk52f9f0((int *)bk);
        i = (int)*(char *)(bk + 0x270) + SV_B((unsigned)&full_data_6f23f0);
        if (i < *(char *)(bk + 0x271)) {
            p = (int *)(bk + 0x228 + i * 4);
            do {
                if ((int)*(char *)(bk + 0x270) + SV_B((unsigned)&full_data_6f23f0) < i)
                    break;
                d2_RedrawView(*p);
                i++;
                p++;
            } while (i < *(char *)(bk + 0x271));
        }
        SV_I((unsigned)&full_data_6f23f0) ^= 1;
    }
}
