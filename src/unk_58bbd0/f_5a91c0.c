/* 0x5a91c0, the plague message (91 B): 0x42ec00 (3) picks the text (0x20c7 + the value) into 0x6f26c8 and the
   key 0x62c2c0 is formatted by 0x5de652 into the 256-byte buffer; every character of the kind 6 / 7 (0x770b82,
   0x300 records) gets it with the +4 id (0x600) */

void ai_Unk5a91c0(void)
{
    char local[256];
    char *p;
    unsigned short v;

    v = (unsigned short)util_Random(3);
    crt_Sprintf(local, (char *)(unsigned)&full_data_62c2c0, (unsigned int)v);
    txt_Format((char *)(unsigned)&full_data_6f26c8, v + 0x20c7);
    p = (char *)((unsigned)&ai_Chars + 0x2);
    do {
        if (*p == 6 || *p == 7)
            he_ShowNews(*(int *)(p + 2), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x600, local);
        p += 0x21c;
    } while ((int)p < (int)((unsigned)&ai_Chars + 0x65402));
}
