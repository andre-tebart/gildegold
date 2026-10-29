/* 0x4e2e00, waits (frame wait 0x79706) until no used slot of 0x6332b4 has +0x104 == -2: -1 */

int ai_Unk4e2e00(void)
{
    int i, none;
    char *e;

    none = -1;
    if (sv_RunFrame(0, 0x79706) != 0) {
        do {
            e = ai_Unk6332b4 + 0x104;
            for (i = 0; i < 0x80; i++, e += 0xc98) {
                if (*(int *)(e - 4) != none && *(int *)e == -2)
                    goto wait;
            }
            return none;
        wait:;
        } while (sv_RunFrame(0, 0x79706) != 0);
    }
}
