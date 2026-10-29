/* 0x5a9330, the swap text of a delivery (253 B; the only caller is amt.c amt_ExecuteSwapMessage 0x4e6c50, message 0x5c): the text 0x1dcc with the first and the third word for a
   kind 6 (+2) second record (sent with its +4 id, 0x5fd), the same with the first and the second word for a
   kind 6 third record, else the text 0x1dcd with the three words to the player's character (0x770b84 + the
   player * 0x21c, 0x5fe) */

void ai_SendSwapText(unsigned short *param_1, unsigned short *param_2, unsigned short *param_3)
{
    if (*(char *)(param_2 + 1) == 6) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x1dcc, *param_1, *param_3);
        he_ShowNews(*(int *)(param_2 + 2), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fd, (char *)(unsigned)&ch_EmptyName);
        return;
    }
    if (*(char *)(param_3 + 1) == 6) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x1dcc, *param_1, *param_2);
        he_ShowNews(*(int *)(param_3 + 2), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fd, (char *)(unsigned)&ch_EmptyName);
        return;
    }
    if (*(char *)(param_1 + 1) != 6) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x1dcd, *param_1, *param_2, *param_3);
        he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)((unsigned)&ai_PlayerId) * 0x21c), -1,
                     (char *)(unsigned)&full_data_6f26c8, 0, 0x5fe, (char *)(unsigned)&ch_EmptyName);
    }
}
