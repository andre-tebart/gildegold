/* 0x4f3640, stores the pair (a, b) in the free slot (0x4f3780 (a, b)) of the table 0x91b6e0 ("gs_AddBeweis: Executed .." is printed); 1 when stored */

int unk_Unk4f3640(int b, int a)
{
    int k;
    char buf[512];

    k = unk_Unk4f3780(a, b);
    if (k >= 0) {
        ((int *)unk_Tab91b6e0)[k * 2] = a;
        ((int *)unk_Tab91b6e0)[k * 2 + 1] = b;
        sprintf(buf, "gs_AddBeweis: Executed .. own", a, b, k);
        return 1;
    }
    return 0;
}
