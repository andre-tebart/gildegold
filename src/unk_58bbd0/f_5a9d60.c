/* 0x5a9d60, a Domizil message (114 B): the text 0x407c with the +0x30 / +0x40 fields of param_2, param_3 and the
   kind text 2 * the word of param_4 + 0x9a8; 0x53dee0 with the +2 field of param_4, the +1 field of param_3 and
   the key 0x62c3e8 */

void ai_Unk5a9d60(int param_1, int param_2, int param_3, unsigned short *param_4)
{
    if (*(unsigned char *)(param_1 + 2) != 6 && *(unsigned char *)(param_1 + 2) != 7)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x407c, param_2 + 0x30, param_2 + 0x40, param_3, *param_4 * 2 + 0x9a8);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, *(int *)(param_3 + 1),
                 *(int *)((char *)param_4 + 2), 0, (char *)(unsigned)&full_data_62c3e8);
}
