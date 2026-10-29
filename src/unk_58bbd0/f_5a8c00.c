/* 0x5a8c00, a production message with one value (79 B): the text 0x20ba and 0x53dda0 (the +4 id, 0x5ec) */

void ai_Unk5a8c00(unsigned short *param_1, char *param_2, int param_3)
{
    if (*(char *)(param_2 + 2) == 6 || *(char *)(param_2 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20ba, *param_1, param_3);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ec, (char *)(unsigned)&ch_EmptyName);
    }
}
