/* 0x572b90, the alloc of the event 0x93: the due clock +0x52 = now + 6 hours (0x4fe1e0), the second clock +0x60 = the due clock */

void gm_SetTime(struct GCLOCK *c, unsigned char hour, unsigned min, unsigned sec); /* 0x4fe1e0 */

void he_Unk572b90(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_SetTime(ev + 0x52, 6, 0, 0);
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x52);
}
