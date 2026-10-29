/* 0x565410, the run handler of an event that hurts the stock of several buildings (ROUGH): state 0 sets the three days after today in the tables 0x760488 (1000) / 0x7604e8 (3), the next run 2 days ahead; state 1: when the flag +0x78 & 2 and +0xec are set every building of the 16 slots at +0xac gets a random change 0x50cad0 (-(random(5) + 5)); then freed */

void he_Unk565410(int ev)
{
    unsigned i, last, base;
    int *p;
    int n, b;

    switch (*(int *)(ev + 0x70)) {
    case 0:
        base = U_I((unsigned)&ai_TimeHour);
        i = (base & 0xffff) + 1;
        last = (base & 0xffff) + 4;
        if (i < last) {
            do {
                ((int *)(unsigned)&ai_Unk760488)[i] = 1000;
                ((int *)(unsigned)&full_data_7604e8)[i] = 3;
                i++;
            } while ((int)i < (int)last);
        }
        *(short *)(ev + 0x56) = (short)base + 2;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && *(char *)(ev + 0xec) != 0) {
            p = (int *)(ev + 0xac);
            n = 0x10;
            do {
                if (*p != -1 && (b = gm_BuildQuery(1, 1, *p)) != 0)
                    gm_Unk50cad0(b, -((unsigned short)util_Random(5) + 5));
                p++;
                n--;
            } while (n != 0);
            *(char *)(ev + 0xec) = 0;
        }
        he_Unk53e1f0((int *)ev);
        return;
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
}
