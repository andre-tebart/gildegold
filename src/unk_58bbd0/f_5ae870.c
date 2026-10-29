/* 0x5ae870, the transport order of one entry (100 B): walks the carriers (0x53e400 (1, 0, 0xf)); the carrier
   whose +0xac matches the word +2 of param_1 gets the clock (+0x52 .. +0x5e) and is marked 0x4c4c60 (-1, the
   carrier) */

void ai_Unk5ae870(int param_1)
{
    int carrier;

    carrier = he_FindEvent(1, 0, 0xf);
    while (carrier != 0) {
        if (*(int *)(carrier + 0xac) == *(int *)(param_1 + 2)) {
            *(HETIME *)(carrier + 0x52) = ai_Time;
            cm_SendExSetHE(-1, carrier);
            return;
        }
        carrier = he_NextEvent();
    }
}
