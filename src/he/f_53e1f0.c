/* 0x53e1f0, frees the event record `r` (the memo block at +0x7c, the slot of the quick list 0x74db40) and shrinks the used counters 0x6f261c / 0x6f2620 */

void he_Unk53e1f0(int *r)
{
    unsigned i;
    int *q;
    int k;
    char *pc;
    unsigned u;

    if (r != 0) {
        if (r[0x1f] != 0) {
            mem_Free(r[0x1f]);
            r[0x1f] = 0;
        }
        if ((*(unsigned char *)(r + 0x1e) & 8) != 0) {
            i = 0;
            do {
                if (U_I((unsigned)&he_QuickList + i * 4) == (int)r)
                    break;
                i++;
            } while ((int)i < 0x100);
            q = (int *)U_I((unsigned)&he_QuickList + i * 4);
            if (q != r)
                return;
            if (q[0x1d] != 0 && *(int *)(q[0x1d] + 8) != -1)
                he_Unk53d800((int)q);
            u = U_I((unsigned)&ai_Unk6f2620);
            U_I((unsigned)&he_QuickList + i * 4) = 0;
            if (i == u) {
                for (; (int)u >= 0 && U_I((unsigned)&he_QuickList + u * 4) == 0; u--)
                    ;
                U_I((unsigned)&ai_Unk6f2620) = ((int)u < 0) - 1 & u;
            }
        }
        U_I((unsigned)&ai_Unk7585e4)--;
        q = r;
        for (k = 0; k < 0x53; k++)
            *q++ = 0;
        if (r == (int *)((unsigned)&he_Events + U_I((unsigned)&ai_Unk6f261c) * 0x14c)) {
            if (U_I((unsigned)&ai_Unk6f261c) >= 0) {
                pc = (char *)((unsigned)&he_Events + U_I((unsigned)&ai_Unk6f261c) * 0x14c);
                do {
                    if (*pc != 0)
                        break;
                    U_I((unsigned)&ai_Unk6f261c)--;
                    pc -= 0x14c;
                } while (U_I((unsigned)&ai_Unk6f261c) >= 0);
            }
            U_I((unsigned)&ai_Unk6f261c) = (U_I((unsigned)&ai_Unk6f261c) < 0) - 1 & U_I((unsigned)&ai_Unk6f261c);
        }
    }
}
