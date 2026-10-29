/* 0x50c340, opens the family tree window for the character (the form name 0x4741f0), prints the title ("$[%s$]", 0x19: the name text 0x48e + 0xe * kind or 0x9a8 + 2 * id), draws the boxes (0x50b3a0) and the lines between the records (0x50b9f0 positions them), returns the window or -1. ROUGH */

int gm_Unk50c340(int form, unsigned char *p, unsigned short *q)
{
    int i, j, w, t, n;
    int *r;
    int a, b, c, d;

    ai_Unk770af0 = d2_OpenForm(0, 0, form);
    d2_CentreViewWindows(ai_Unk770af0);
    ai_Unk76fdc8 = ai_Unk6333cc;
    w = *(int *)(ai_Unkfb1030 + ai_Unk770af0 * 0x2ac);
    d2_SetForm(0, 1);
    if (*q == 0xfd)
        t = *p * 0xe + 0x48e;
    else
        t = *q * 2 + 0x9a8;
    io_PrintText("$[%s$]", 0x19, t);
    d2_SetForm(0, 3);
    n = gm_Unk50b3a0(p, q, (int)*(short *)(ai_Unkf8f128 + w * 0x3c4), (int)*(short *)(ai_Unkf8f128 + 2 + w * 0x3c4));
    if (n != 0) {
        d2_CloseView(ai_Unk770af0);
        return -1;
    }
    gm_Unk50b9f0(p, q);
    i = 0;
    if (0 < ai_Unk7d6a40) {
        j = 0;
        do {
            if (0 < j) {
                r = (int *)(ai_Unk76fde0 + 0x14);
                t = i;
                do {
                    if (r[-5] == *(int *)(ai_Unk76fde0 + 4 + j)) {
                        a = *(int *)(ai_Unk76fde0 + 0x14 + j);
                        b = *(int *)(ai_Unk76fde0 + 0x10 + j);
                        c = *r;
                        d = r[-1];
                        gm_Unk50c310(d + 0x18, c + 0x30, b + 0x18, a + 0x18);
                    } else if (r[-5] == *(int *)(ai_Unk76fde0 + 8 + j)) {
                        a = *(int *)(ai_Unk76fde0 + 0x14 + j);
                        b = *(int *)(ai_Unk76fde0 + 0x10 + j);
                        c = *r;
                        d = r[-1];
                        gm_Unk50c310(d + 0x18, c + 0x30, b + 0x18, a + 0x18);
                    }
                    r += 0xd;
                    t--;
                } while (t != 0);
            }
            i++;
            j += 0x34;
        } while (i < ai_Unk7d6a40);
    }
    return ai_Unk770af0;
}
