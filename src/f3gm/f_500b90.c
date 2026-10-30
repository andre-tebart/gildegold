/* 0x500b90, looks the building kind of the building b up in the table 0x61a348 (0x1e records of 0x15 bytes: a kind byte and ten object words) and collects the non-zero words into 0x889400 (count 0x76fdc4); 1 when the kind is not there (or no tables), else 0 */

int gm_BuildKindObjects(char *b)
{
    char *t;
    int i, k;
    unsigned short *w;

    if (ai_Goods == 0)
        return 1;
    i = 0;
    t = (char *)ai_Unk61a348;
    do {
        if (*t == *b) {
            if (i * 0x15 == -(unsigned)&ai_Unk61a348)
                return 1;
            ai_Unk76fdc4 = 0;
            w = ai_Unk61a348[i].items;
            for (k = 10; k != 0; k--, w++) {
                if (*w != 0)
                    ai_Unk889400[ai_Unk76fdc4++] = *w;
            }
            return 0;
        }
        t += 0x15;
        i++;
    } while ((int)t < ((unsigned)&ai_Unk61a348 + 0x309));
    return 1;
}
