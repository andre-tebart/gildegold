/* 0x5a92b0, a plague message to every kind 6 character (119 B): 0x42ec00 (2) picks the text (0x20cd + the
   value) and the key 0x62c2dc formatted with the value + 0x12; every character of the 0x300 records with the
   kind 6 (+2) gets it with the +4 id (0x600) */

void ai_Unk5a92b0(void)
{
    char local[64];
    int *p;
    unsigned short v;

    v = (unsigned short)util_Random(2);
    sprintf(local, (char *)(unsigned)&full_data_62c2dc, v + 0x12);
    txt_Format((char *)(unsigned)&full_data_6f26c8, v + 0x20cd);
    p = (int *)(unsigned)&ai_CharInts;
    do {
        if (*(char *)((char *)p - 2) == 6)
            he_ShowNews(*p, -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x600, local);
        p += 0x87;
    } while ((int)p < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
}
