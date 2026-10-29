/* 0x5c08b0, clears the tab table (96 B): the count and the control id of the eight 0x14-byte entries
   0x62d14c .. 0x62d1ec */

void ai_Unk5c08b0(void)
{
    int *p;

    for (p = (int *)(unsigned)&full_data_62d150; p < (int *)(unsigned)((unsigned)full_data_62d150 + 0xa0); p += 5) {
        p[-1] = 0;
        p[0] = 0;
    }
}
