/* 0x5a9110, a production message to the player (95 B): when the word of param_1 is not the player 0x6f25b8,
   the text 0x20c5 and the id +4 of the player's character record 0x770b80 + 0x21c * the player (0x605) */

void ai_Unk5a9110(unsigned short *param_1, int param_2)
{
    if (*param_1 != *(unsigned short *)((unsigned)&ai_PlayerId)) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20c5, *param_1, param_2);
        he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)((unsigned)&ai_PlayerId) * 0x21c), -1,
                     (char *)(unsigned)&full_data_6f26c8, 0, 0x605, (char *)(unsigned)&ch_EmptyName);
    }
}
