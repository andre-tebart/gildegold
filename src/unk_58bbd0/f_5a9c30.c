/* 0x5a9c30, a Domizil message (108 B): as 0x5a9bc0 with the text 0x4077, the key "_NACHRICHTEN_DOMIZIL_01"
   0x62c3d0, the flag 1 and param_3 (a string) as the last text argument; 0x53dee0 gets the +1 field of param_3
   and -1 */

void ai_Unk5a9c30(int param_1, int param_2, int param_3)
{
    if (*(unsigned char *)(param_1 + 2) != 6 && *(unsigned char *)(param_1 + 2) != 7)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4077, param_2 + 0x30, param_2 + 0x40, param_3);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, *(int *)(param_3 + 1), -1, 0,
                 (char *)(unsigned)&full_data_62c3e8);
}
