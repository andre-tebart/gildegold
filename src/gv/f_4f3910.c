/* 0x4f3910, changes the state (int +0x25) of the 0x2d-byte records of 0x91f700: without mode m every record in state s is set to 1 (count returned); with m the records of the pairs (a, ..) of 0x91b6e0 that are in state 1 and belong
   to the character b (+0x16) are set to s */

int unk_Unk4f3910(int a, int b, int s, int m)
{
    int cnt, i;
    int *p, *q;

    cnt = 0;
    if (m == 0) {
        p = (int *)(gv_Graveyard + 0x25);
        do {
            if (*p == s) {
                *p = 1;
                cnt++;
            }
            p = (int *)((char *)p + 0x2d);
        } while ((int)p < ((unsigned)&amt_Table + 0x5));
        return cnt;
    }
    p = (int *)(unk_Tab91b6e0 + 4);
    do {
        if (p[-1] == a) {
            i = 0;
            q = (int *)gv_Graveyard;
            do {
                if (*q == *p) {
                    if (*(int *)(gv_Graveyard + 0x25 + i * 0x2d) == 1 && *(int *)(gv_Graveyard + 0x16 + i * 0x2d) == b) {
                        *(int *)(gv_Graveyard + 0x25 + i * 0x2d) = s;
                        cnt++;
                    }
                    break;
                }
                q = (int *)((char *)q + 0x2d);
                i++;
            } while ((int)q < (unsigned)&ai_Unk925100);
        }
        p += 2;
        if ((int)p > ((unsigned)unk_Tab91b6e0 + 0x4003))
            return cnt;
    } while (1);
}
