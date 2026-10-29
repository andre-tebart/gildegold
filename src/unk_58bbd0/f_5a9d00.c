/* 0x5a9d00, a Domizil message (94 B): as 0x5a9ca0 with the text 0x407b (four text arguments) */

void ai_Unk5a9d00(int param_1, int param_2, int param_3)
{
    if (*(unsigned char *)(param_1 + 2) != 6 && *(unsigned char *)(param_1 + 2) != 7)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x407b, param_2 + 0x30, param_2 + 0x40);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, *(int *)(param_3 + 1), -1, 0,
                 (char *)(unsigned)&full_data_62c3e8);
}
