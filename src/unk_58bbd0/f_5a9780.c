/* 0x5a9780, a graveyard message (94 B): for a character of the kind 6 / 7 (+2) the text 0x4151 (0x4152 when
   param_3 != 1) with param_2, param_3, the text and param_4, the +4 id (0x5fc) */

void ai_Unk5a9780(int param_1, int param_2, int param_3, int param_4)
{
    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4151, param_2, param_3, (param_3 != 1) + 0x4152, param_4);
        he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fc, (char *)(unsigned)&ch_EmptyName);
    }
}
