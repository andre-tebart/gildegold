/* 0x5512c0, the allocation handler of the ranking-like event (ROUGH): state 0; unless flag 4 of +0x78 is set it clears the ten 16-byte slots from +0xac (two ids = -1, four bytes 0), sets the due clock to now with the minute word 0xb and posts the event (0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk5512c0(int ev)
{
    int i;
    unsigned char *s;

    *(int *)(ev + 0x70) = 0;
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        s = (unsigned char *)(ev + 0xac);
        for (i = 0; i < 10; i++) {
            *(int *)s = -1;
            *(int *)(s + 4) = -1;
            s[8] = 0;
            s[9] = 0;
            s[10] = 0;
            s[11] = 0;
            s += 0x10;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(unsigned short *)(ev + 0x56) = 0xb;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
