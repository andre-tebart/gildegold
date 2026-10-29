/* 0x50b2f0, the generation of the family record p: 0 without parents, else one more than the older parent's (cached in the word at +0xc) */

unsigned gm_Unk50b2f0(int p)
{
    unsigned a, b, i;
    int n, j;
    int *q;

    n = ai_Unk7d6a40;
    a = 0;
    b = 0;
    if (*(unsigned short *)(ai_Unk76fde0 + p * 0x34 + 0xc) != 0xffff)
        return *(unsigned short *)(ai_Unk76fde0 + p * 0x34 + 0xc);
    i = 0;
    if (*(int *)(ai_Unk76fde0 + p * 0x34 + 4) != 0) {
        if (0 < ai_Unk7d6a40) {
            q = (int *)ai_Unk76fde0;
            do {
                if (*(int *)(ai_Unk76fde0 + p * 0x34 + 4) == *q) {
                    a = gm_Unk50b2f0(i) + 1;
                    break;
                }
                i++;
                q += 0xd;
            } while ((int)i < ai_Unk7d6a40);
        }
        if (*(int *)(ai_Unk76fde0 + p * 0x34 + 8) == 0)
            return a;
        j = 0;
        if (0 < n) {
            q = (int *)ai_Unk76fde0;
            do {
                if (*(int *)(ai_Unk76fde0 + p * 0x34 + 8) == *q) {
                    b = gm_Unk50b2f0(j) + 1;
                    break;
                }
                j++;
                q += 0xd;
            } while (j < n);
        }
        i = a;
        if ((int)a <= (int)b)
            i = b;
    }
    return i;
}
