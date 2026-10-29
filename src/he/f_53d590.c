/* 0x53d590, frees every used record (0x14c bytes each from 0x6fab40) up to 0x6f261c */

void he_Unk53d590(void)
{
    char *p;
    int i = 0;

    if (U_I((unsigned)&ai_Unk6f261c) >= 0) {
        p = (char *)(unsigned)&he_Events;
        do {
            if ((int)p >= (int)((unsigned)&he_Events + 0x53000))
                return;
            if (*p != 0)
                he_Unk53e1f0(p);
            i++;
            p += 0x14c;
        } while (i <= U_I((unsigned)&ai_Unk6f261c));
    }
}
