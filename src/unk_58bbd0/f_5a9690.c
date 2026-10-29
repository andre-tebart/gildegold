/* 0x5a9690, a message (69 B): the text 0x4154 with param_2 to a character of the kind 6 / 7 (+2), the +4 id
   (0x5fc) */

void ai_Unk5a9690(int param_1, int param_2)
{
    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4154, param_2);
        he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fc, (char *)(unsigned)&ch_EmptyName);
    }
}
