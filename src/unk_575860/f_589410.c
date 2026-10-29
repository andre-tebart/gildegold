/* 0x589410, the init of the 12-byte slot table 0x6f5ec0 (63 B, 0xb slots up to 0x6f5f43): cleared, then each
   slot's byte 0 = i + 0x3b (0x57 for the last one, 0x6f5f38) and 0x589400 (the slot) */

void ai_Unk589410(void)
{
    char *p;
    int i;

    memset((char *)(unsigned)&full_data_6f5ec0, 0, 0x84);
    for (i = 0; i < 0xb; i++) {
        p = (char *)((unsigned)&full_data_6f5ec0 + i * 0xc);
        *(unsigned char *)p = (unsigned char)(i + 0x3b);
        if (i == 0xa)
            *(unsigned char *)(unsigned)&full_data_6f5f38 = 0x57;
        ai_Unk589400((int *)p);
    }
}
