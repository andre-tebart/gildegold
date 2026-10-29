/* 0x5a9bc0, a Domizil message (108 B): as 0x5a9b50 with the text 0x4075 and the kind text 2 * param_4 + 0x9a8 in
   place of param_4 + 0x4172 (the key 0x62c3b8) */

void ai_Unk5a9bc0(int param_1, int param_2, int param_3, int param_4)
{
    if (*(unsigned char *)(param_1 + 2) != 6 && *(unsigned char *)(param_1 + 2) != 7)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4075, param_2 + 0x30, param_2 + 0x40, param_4 * 2 + 0x9a8);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ec,
                 *(int *)(*(int *)(param_2 + 0x16c) + 1), *(int *)(param_3 + 2), 0, (char *)(unsigned)&full_data_62c3b8);
}
