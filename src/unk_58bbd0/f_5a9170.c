/* 0x5a9170, a production message (95 B): for a character of the kind 6 / 7 (+2) the text 0x20c6 with the word
   of param_1 and the +4 id (0x5fa, the key 0x62c2ac) */

void ai_Unk5a9170(unsigned short *param_1, int param_2)
{
    if (*(char *)(param_2 + 2) == 6 || *(char *)(param_2 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20c6, *param_1);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fa, (char *)(unsigned)&full_data_62c2ac);
    }
}
