/* 0x5a8cb0, a production message without a value (74 B): the text 0x20bb and 0x53dda0 (the +4 id, 0x5fd) */

void ai_Unk5a8cb0(unsigned short *param_1, char *param_2)
{
    if (*(char *)(param_2 + 2) == 6 || *(char *)(param_2 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20bb, *param_1);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fd, (char *)(unsigned)&ch_EmptyName);
    }
}
