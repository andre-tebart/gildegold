/* 0x5a9240, a plague message (112 B): 0x42ec00 (2) picks the text (0x20ca + the value) and the key 0x62c2c0
   formatted with the value + 3; for a character of the kind 6 / 7 (+2) the +4 id gets it (0x600) */

void ai_Unk5a9240(int param_1)
{
    char local[64];
    unsigned short v;

    v = (unsigned short)util_Random(2);
    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
        crt_Sprintf(local, (char *)(unsigned)&full_data_62c2c0, v + 3);
        txt_Format((char *)(unsigned)&full_data_6f26c8, v + 0x20ca);
        he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x600, local);
    }
}
