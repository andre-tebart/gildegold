/* 0x573ae0, the approach 3 of the event 0x9d: over the five values per +0x80 .. +0x84 the highest and the lowest are sought;
   with two different ones their differences are sent (0x4c6020 (per +4, index, difference), highest - lowest and lowest -
   highest), the name and the texts 0x41e1 (the indices + 0x16b7) / 0x41dd (else 0x41dc) are built and the message 0x5fb
   (0x53dda0) sent to per +4 */

void he_Unk573ae0(char *pl, char *per)
{
    char name[0x19];
    char t1[0x400];
    char t2[0x400];
    int hi = -1, lo = 0xff, hi_i = -1, lo_i = -1, i, v;

    for (i = 0; i < 5; i++) {
        v = HM_B(per, 0x80 + i);
        if (v > hi) {
            hi = v;
            hi_i = i;
        }
        if (v < lo) {
            lo = v;
            lo_i = i;
        }
    }
    if (lo_i == hi_i || lo_i < 0 || hi_i < 0)
        return;
    ai_Unk4c6020(HM_I(per, 4), lo_i, hi - lo);
    ai_Unk4c6020(HM_I(per, 4), hi_i, lo - hi);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e1, lo_i + 0x16b7, hi_i + 0x16b7);
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}
