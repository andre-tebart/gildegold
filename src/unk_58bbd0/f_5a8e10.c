/* 0x5a8e10, three production messages (201 B): for a character of the kind 6 / 7 (+2) the text 0x20c1 with the
   word of param_1 and the +4 id (0x5fb); then the text 0x20c2 (param_3, the word of param_1, the flag +9 of
   param_1, the word of param_2) for every OTHER character of the 0x300 records 0x770b80 (the kind +2 6 / 7,
   the +4 id, 0x5ec) */

void ai_Unk5a8e10(char *param_1, char *param_2, int param_3)
{
    char *p;

    if (param_2[2] == 6 || param_2[2] == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20c1, *(unsigned short *)param_1, param_3);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fb, (char *)(unsigned)&ch_EmptyName);
    }
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20c2, param_3, *(unsigned short *)param_1,
                 (param_1[9] != 0) + 0x20c3, *(unsigned short *)param_2);
    p = (char *)((unsigned)&ai_Chars + 0x2);
    do {
        if (p - 2 != param_1 && p - 2 != param_2 && (*p == 6 || *p == 7))
            he_ShowNews(*(int *)(p + 2), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ec, (char *)(unsigned)&ch_EmptyName);
        p += 0x21c;
    } while ((int)p < (int)((unsigned)&ai_Chars + 0x65402));
}
