/* 0x53ee40, runs the run handler of the records with bit 0x10 at +0x78 */

void he_Unk53ee40(void)
{
    unsigned char *pb;
    int i = 0;

    if (U_I((unsigned)&ai_Unk6f261c) >= 0) {
        pb = (unsigned char *)(unsigned)&he_Events;
        do {
            if ((int)pb >= (int)((unsigned)&he_Events + 0x53000))
                return;
            if (*pb != 0 && (pb[0x78] & 0x10) != 0 && *pb < 0xa7) {
                he_Unk53ec40(pb);
                ((void (*)(unsigned char *))U_I((unsigned)&he_RunHandlers + *pb * 4))(pb);
            }
            i++;
            pb += 0x14c;
        } while (i <= U_I((unsigned)&ai_Unk6f261c));
    }
}
