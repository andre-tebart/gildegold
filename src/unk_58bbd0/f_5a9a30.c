/* 0x5a9a30, a graveyard message with an optional second part (188 B): the text 0x40ad (0x40a3, the word of
   param_2, its +0x16c, the word of param_3); with param_4 the text 0x40ae (0x20d + the flag +9 of param_2 (0x23)
   + the byte +0x165) into 0x6f2ac8 is appended; the +4 id sends it (0, 0x5fa, the key 0x62c39c) */

void ai_Unk5a9a30(int param_1, unsigned short *param_2, unsigned short *param_3, int param_4)
{
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x40ad, 0x40a3, *param_2, *(int *)((char *)param_2 + 0x16c),
                 *param_3);
    if (param_4 != 0) {
        txt_Format((char *)(unsigned)&full_data_6f2ac8, 0x40ae,
                     (*(char *)((char *)param_2 + 9) != 0 ? 0x23 : 0) + 0x20d +
                     *(unsigned char *)((char *)param_2 + 0x165));
        strcat((char *)(unsigned)&full_data_6f26c8, (char *)(unsigned)&full_data_6f2ac8);
    }
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fa, (char *)(unsigned)&full_data_62c39c);
}
