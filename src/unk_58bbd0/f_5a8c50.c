/* 0x5a8c50, a production message with two values (84 B): the text 0x20bc and 0x53dda0 (the +4 id, 0x5ff) */

void ai_Unk5a8c50(unsigned short *param_1, char *param_2, int param_3, int param_4)
{
    if (*(char *)(param_2 + 2) == 6 || *(char *)(param_2 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20bc, *param_1, param_3, param_4);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ff, (char *)(unsigned)&ch_EmptyName);
    }
}
