/* 0x5a9ca0, a Domizil message (98 B): the text 0x407a with the +0x30 / +0x40 fields of param_2, and 0x53dee0
   (flag 1, the text 0x5ec, the +1 field of param_3, the key "_NACHRICHTEN_DOMIZIL_04" 0x62c3e8) */

void ai_Unk5a9ca0(int param_1, int param_2, int param_3)
{
    if (*(unsigned char *)(param_1 + 2) != 6 && *(unsigned char *)(param_1 + 2) != 7)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x407a, param_2 + 0x30, param_2 + 0x40);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, *(int *)(param_3 + 1), -1, 0,
                 (char *)(unsigned)&full_data_62c3e8);
}
