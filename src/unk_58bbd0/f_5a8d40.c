/* 0x5a8d40, a production message (31 B): the text 0x20be and 0x53dda0 (the +4 id, 0x5fb) for a character of
   the kind 6 / 7 (+2) */

void ai_Unk5a8d40(int param_1)
{
    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20be);
        he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fb, (char *)(unsigned)&ch_EmptyName);
    }
}
