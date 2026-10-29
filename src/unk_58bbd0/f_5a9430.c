/* 0x5a9430, a message (61 B): the text 0x1747 with the word of param_2 and the +4 id of param_1 (0x5fa) */

void ai_Unk5a9430(int param_1, unsigned short *param_2)
{
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x1747, *param_2);
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fa, (char *)(unsigned)&ch_EmptyName);
}
