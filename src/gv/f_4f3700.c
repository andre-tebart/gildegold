/* 0x4f3700, for each of the 512 records of 0x91f700 by its state (int +0x25): state 1 sends 0x4c4e90 (id at +0, 0); a state above 1 whose 0x53e400 (1, 1, state) lookup fails sends 0x4c4e90 (id, 1); after 32 sends it waits for the last one (0x4c3ae0 / 0x52fe00). ROUGH */

void unk_Unk4f3700(void)
{
    int h, n, k;
    unsigned s;
    unsigned *p;
    int id, flag;

    h = -1;
    n = 0;
    p = (unsigned *)(gv_Graveyard + 0x25);
    do {
        s = *p;
        if (s == 1) {
            id = *(int *)((char *)p - 0x25);
            flag = 0;
            h = ((int (*)(int, int))ai_Unk4c4e90)(id, flag);
            n++;
        } else if (s > 1) {
            k = (int)he_FindEvent(1, 1, s);
            if (k == 0) {
                id = *(int *)((char *)p - 0x25);
                flag = 1;
                h = ((int (*)(int, int))ai_Unk4c4e90)(id, flag);
                n++;
            }
        }
        if (n > 0x1f) {
            if (h != -1) {
                while (cm_GetSlotState(h) == 0)
                    sv_PumpNet();
            }
            h = -1;
            n = 0;
        }
        p = (unsigned *)((char *)p + 0x2d);
    } while ((int)p < ((unsigned)&amt_Table + 0x5));
}
