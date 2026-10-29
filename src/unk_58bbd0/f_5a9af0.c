/* 0x5a9af0, a graveyard message (83 B): the text 0x40af (0x40a3, the word of param_2, its +0x16c, the word of
   param_3), the +4 id (0, 0x5fa, the key 0x62c39c) */

void ai_Unk5a9af0(int param_1, unsigned short *param_2, unsigned short *param_3)
{
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x40af, 0x40a3, *param_2, *(int *)((char *)param_2 + 0x16c),
                 *param_3);
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fa, (char *)(unsigned)&full_data_62c39c);
}
