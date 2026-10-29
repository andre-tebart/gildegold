/* 0x53ed70, the per-frame run of the event records: updates the gfx infos, then calls the run handler (0x758600) of every record (not 0x18 flagged) that is due */

void he_Unk53ed70(void)
{
    int iv, i;
    unsigned char *pb;

    he_Unk53eba0();
    i = 0;
    if (U_I((unsigned)&ai_Unk6f261c) >= 0) {
        pb = (unsigned char *)(unsigned)&he_Events;
        do {
            if ((int)pb >= (int)((unsigned)&he_Events + 0x53000))
                return;
            if (*pb != 0 && (pb[0x78] & 0x18) == 0) {
                he_Unk53e8b0(pb);
                iv = gm_CompareTime((unsigned)&ai_Time, pb + 0x52);
                if (iv > 0 && *pb < 0xa7) {
                    he_Unk53ec40(pb);
                    ((void (*)(unsigned char *))U_I((unsigned)&he_RunHandlers + *pb * 4))(pb);
                }
            }
            i++;
            pb += 0x14c;
        } while (i <= U_I((unsigned)&ai_Unk6f261c));
    }
}
