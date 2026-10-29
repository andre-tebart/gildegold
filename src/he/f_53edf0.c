/* 0x53edf0, runs the run handler of the quick list records (0x74db40) */

void he_Unk53edf0(void)
{
    unsigned char *pb;
    int i = 0;

    if (U_I((unsigned)&ai_Unk6f2620) >= 0) {
        do {
            if (i > 0xff)
                return;
            pb = (unsigned char *)U_I((unsigned)&he_QuickList + i * 4);
            if (pb != 0 && *pb < 0xa7) {
                he_Unk53ec40(pb);
                ((void (*)(unsigned char *))U_I((unsigned)&he_RunHandlers + *pb * 4))(pb);
            }
            i++;
        } while (i <= U_I((unsigned)&ai_Unk6f2620));
    }
}
