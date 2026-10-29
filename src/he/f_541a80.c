/* 0x541a80, starts the event record `r` (when bit 4 of +0x78 is clear): due now + 1, a second clock (+0xb0) due in 0x18 units, a random 0x13 .. 0x16 at +0xb4, state 1 at +0xc0, posts it (0x4c4c60) and keeps the result at +0x84 */

void he_Unk541a80(int r)
{
    short s;

    if ((*(unsigned char *)(r + 0x78) & 4) == 0) {
        *(HETIME *)(r + 0x52) = ai_Time;
        gm_AddTime(r + 0x52, 1, 0, 0);
        *(HETIME *)(r + 0xb0) = ai_Time;
        gm_AddTime(r + 0xb0, 0x18, 0, 0);
        *(int *)(r + 0xb6) = 0;
        s = util_Random(4);
        *(short *)(r + 0xb4) = s + 0x13;
        *(int *)(r + 0xc0) = 1;
        *(int *)(r + 0x84) = cm_SendExSetHE(0, r);
    }
}
