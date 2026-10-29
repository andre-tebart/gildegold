/* 0x4fad60, finds the character with the highest 0x50a640 value (of the active persons of kind < 10, +8 set), caches it in 0x6f1bac / 0x617730 until the clock 0x770ae0 passes 0x61772c or the cached record is gone; out: its id (-1 when none) and the value (0xc80 when none). Returns 1 when found */

int gm_FindRichest(short *id, int *rate)
{
    int best, bi, i, v;
    unsigned char *p;

    best = -100000;
    bi = 0xffff;
    if (ai_Unk6f1bac == 0 || ai_Unk61772c < *(int *)ai_Time || *ai_Unk6f1bac == -1 || (char)ai_Unk6f1bac[4] == 0) {
        ai_Unk61772c = *(int *)ai_Time;
        i = 0;
        for (p = (unsigned char *)ai_Chars[0] + 2; (int)p < (unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c) {
            if (*(short *)(p - 2) != -1 && *p < 10 && p[6] != 0) {
                v = I50A640(i);
                if (best < v) {
                    best = v;
                    bi = i;
                }
            }
            i++;
        }
        if (bi == 0xffff) {
            ai_Unk6f1bac = 0;
            ai_Unk617730 = 0xc80;
            goto none;
        }
        ai_Unk6f1bac = (short *)ai_Chars[bi];
        ai_Unk617730 = best;
    }
    if (ai_Unk6f1bac != 0) {
        *id = *ai_Unk6f1bac;
        *rate = ai_Unk617730;
        return 1;
    }
none:
    *id = -1;
    *rate = ai_Unk617730;
    return 0;
}
