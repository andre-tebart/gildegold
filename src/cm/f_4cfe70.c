/* 0x4cfe70, removes the portrait boxes ai_Unk930400[6] again */

void ai_Unk4cfe70()
{
    int *p = (int *)(ai_Unk930400 + 8);
    int t;

    do {
        if (p[0] != -1) {
            d2_RemoveWindow(p[0]);
            t = p[-1];
            p[0] = -1;
            d2_RemoveObj(t);
        }
        p += 8;
    } while ((int)p < (int)((unsigned)&ai_Unk930400 + 0xc8));
}
