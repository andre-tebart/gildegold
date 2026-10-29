/* 0x541200, sets the due clock (+0x52) of the event record to now and posts it when flag 2 of +0x78 is set */

void he_Unk541200(int r)
{
    *(HETIME *)(r + 0x52) = ai_Time;
    if ((*(unsigned char *)(r + 0x78) & 2) != 0)
        cm_SendExSetHE(-1, r);
}
