/* 0x574fe0, the meeting room of the event 0xa1: up to three of the words +0xc0 .. (+0xc0 + n * 2, n stepping by n % 4 + 1 from
   +0xbc) are looked up as buildings (0x501190 (1, 5, word)); returns the first hit, 0 when none */

int ai_Unk574fe0(int ev)
{
    int i, n, r;

    n = HM_I(ev, 0xbc);
    r = 0;
    i = 3;
    while (i != 0) {
        r = gm_BuildQuery(1, 5, *(unsigned short *)(ev + 0xc0 + n * 2));
        i--;
        n = n % 4 + 1;
        if (r != 0)
            break;
    }
    return r;
}
